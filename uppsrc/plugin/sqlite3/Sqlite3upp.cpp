//////////////////////////////////////////////////////////////////////

#include <Core/Core.h>
#include <Sql/Sql.h>
#include "lib/sqlite3.h"
#include "sqlite3.h"

#define LLOG(x) // LOG(x)

//////////////////////////////////////////////////////////////////////

const char *Sqlite3ReadString(const char *s, String& stmt) {
	stmt.Cat(*s);
	int c = *s++;
	for(;;) {
		if(*s == '\0') break;
		else
		if(*s == '\'' && s[1] == '\'') {
			stmt.Cat('\'');
			s += 2;
		}
		else
		if(*s == c) {
			stmt.Cat(c);
			s++;
			break;
		}
		else
		if(*s == '\\') {
			stmt.Cat('\\');
			if(*++s)
				stmt.Cat(*s++);
		}
		else
			stmt.Cat(*s++);
	}
	return s;
}

bool Sqlite3PerformScript(const String& txt, StatementExecutor& se, Gate2<int, int> progress_canceled) {
	const char *text = txt;
	for(;;) {
		String stmt;
		while(*text <= 32 && *text > 0) text++;
		if(*text == '\0') break;
		for(;;) {
			if(*text == '\0')
				break;
			if(*text == ';')
				break;
			else
			if(*text == '\'')
				text = Sqlite3ReadString(text, stmt);
			else
			if(*text == '\"')
				text = Sqlite3ReadString(text, stmt);
			else
				stmt.Cat(*text++);
		}
		if(progress_canceled(text - txt.Begin(), txt.GetLength()))
			return false;
		if(!se.Execute(stmt))
			return false;
		if(*text) text++;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////

class Sqlite3Connection : public SqlConnection {
protected:
	virtual void        SetParam(int i, const Value& r);
	virtual bool        Execute();
	virtual int         GetRowsProcessed() const;
	virtual bool        Fetch();
	virtual void        GetColumn(int i, Ref f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual String      ToString() const;

	typedef Sqlite3Session::sqlite3 sqlite3;
	typedef Sqlite3Session::sqlite3_stmt sqlite3_stmt;

private:
	Sqlite3Session&  session;
	sqlite3*         db;
	Vector<Value>    param;

	sqlite3_stmt*    current_stmt;
	String           current_stmt_string;
	bool             got_first_row;
	bool             got_row_data;

	VectorMap<String,sqlite3_stmt*> prepared_statements;
	int              prep_stmt_size;

	friend class Sqlite3Session;
	sqlite3_stmt*    GetCurrentStatement() { return current_stmt; }
	void             BindParam(int i, const Value& r);
	void             FlushStatements();
	sqlite3_stmt*    Compile(String statement);

public:
	Sqlite3Connection(Sqlite3Session& the_session, sqlite3 *the_db);
	virtual ~Sqlite3Connection();
};

Sqlite3Connection::Sqlite3Connection(Sqlite3Session& the_session, sqlite3 *the_db) :
	session(the_session), db(the_db), current_stmt(NULL), got_first_row(false), got_row_data(false)
{
	prep_stmt_size = 0;
}
Sqlite3Connection::~Sqlite3Connection() {
	Cancel();
	FlushStatements();
}

void Sqlite3Connection::FlushStatements()
{
	LLOG("Sqlite3Connection::FlushStatements(" << prepared_statements.GetCount() << " total)");
	for (int i = 0; i < prepared_statements.GetCount(); ++i) {
		LLOG("Sqlite3Connection::FlushStatements(" << FormatIntHex(prepared_statements[i]) << ") - " << prepared_statements.GetKey(i));
		int retval = sqlite3_finalize(prepared_statements[i]);
		if (SQLITE_OK != retval)
			session.SetError(sqlite3_errmsg(db),String("Finalizing statement: ")+prepared_statements.GetKeys()[i]);
	}
	prepared_statements.Clear();
	prep_stmt_size = 0;
}

void Sqlite3Connection::SetParam(int i, const Value& r) {
	LLOG(Format("SetParam(%d,%s)",i,r.ToString()));
	param.At(i,r);
}
void Sqlite3Connection::BindParam(int i, const Value& r) {
	if (IsNull(r))
		sqlite3_bind_null(current_stmt,i);
	else switch (r.GetType()) {
		case WSTRING_V:
		case STRING_V: {
				String p = r;
				sqlite3_bind_text(current_stmt,i,p,p.GetLength(),SQLITE_TRANSIENT);
			}
			break;
		case INT_V:
			sqlite3_bind_int(current_stmt, i, int(r));
			break;
		case DOUBLE_V:
			sqlite3_bind_double(current_stmt, i, double(r));
			break;
		case DATE_V: {
				Date d = r;
				String p = Format("\'%04d-%02d-%02d\'", d.year, d.month, d.day);
				sqlite3_bind_text(current_stmt,i,p,p.GetLength(),SQLITE_TRANSIENT);
			}
			break;
		case TIME_V: {
				Time t = r;
				String p = Format("\'%04d-%02d-%02d %02d:%02d:%02d\'",
						   t.year, t.month, t.day, t.hour, t.minute, t.second);
				sqlite3_bind_text(current_stmt,i,p,p.GetLength(),SQLITE_TRANSIENT);
			}
			break;
		default:
			NEVER();
	}
}
int ParseForArgs(const char* sqlcmd) {
	int numargs = 0;
	const char* ptr = sqlcmd;
	while (*ptr)
		if(*ptr == '\'')
			while(*++ptr && (*ptr != '\'' || *++ptr && *ptr == '\''))
				;
		else if(*ptr++ == '?')
			++numargs;
	return numargs;
}
bool Sqlite3Connection::Execute() {
	Cancel();
	current_stmt_string = statement;
	current_stmt = Compile(current_stmt_string);
	// Error compiling statement
	if (NULL == current_stmt)
		return false;
	int nparams = ParseForArgs(current_stmt_string);
	ASSERT(nparams == param.GetCount());
	for (int i = 0; i < nparams; ++i)
		BindParam(i+1,param[i]);
	param.Clear();
	// Make sure that compiling the statement never fails.
	ASSERT(NULL != current_stmt);
	int retcode = sqlite3_step(current_stmt);
	if ((retcode != SQLITE_DONE) && (retcode != SQLITE_ROW)) {
		session.SetError(sqlite3_errmsg(db), current_stmt_string);
		return false;
	}
	got_first_row = got_row_data = (retcode==SQLITE_ROW);
	if (got_row_data) {
		int numfields = sqlite3_column_count(current_stmt);
		info.SetCount(numfields);
		for (int i = 0; i < numfields; ++i) {
			SqlColumnInfo& field = info[i];
			field.name = sqlite3_column_name(current_stmt,i);
			switch (sqlite3_column_type(current_stmt,i)) {
				case SQLITE_INTEGER:
					field.type = INT_V;
					break;
				case SQLITE_FLOAT:
					field.type = DOUBLE_V;
					break;
				case SQLITE_TEXT:
					field.type = STRING_V;
					break;
				case SQLITE_NULL:
					field.type = VOID_V;
					break;
				case SQLITE_BLOB:
					field.type = STRING_V;
					break;
				default:
					NEVER();
					break;
			}
		}
	}
	return true;
}
int Sqlite3Connection::GetRowsProcessed() const {
	LLOG("GetRowsProcessed()");
	return sqlite3_changes(db);
}

bool Sqlite3Connection::Fetch() {
	ASSERT(NULL != current_stmt);
	if (!got_row_data)
		return false;
	if (got_first_row) {
		got_first_row = false;
		return true;
	}
	ASSERT(got_row_data);
	int retcode = sqlite3_step(current_stmt);
	if ((retcode != SQLITE_DONE) && (retcode != SQLITE_ROW))
		session.SetError(sqlite3_errmsg(db), String("Fetching prepared statement: ")+current_stmt_string );
	got_row_data = (retcode==SQLITE_ROW);
	return got_row_data;
}
void Sqlite3Connection::GetColumn(int i, Ref f) const {
	ASSERT(NULL != current_stmt);
	if(i == -1) {
		f = Value(sqlite3_last_insert_rowid(db));
		return;
	}

	ASSERT(got_row_data);
	switch (sqlite3_column_type(current_stmt,i)) {
		case SQLITE_INTEGER:
			f = sqlite3_column_int(current_stmt,i);
			break;
		case SQLITE_FLOAT:
			f = sqlite3_column_double(current_stmt,i);
			break;
		case SQLITE_TEXT:
			f = Value(String((const char*)sqlite3_column_text(current_stmt,i)));
			break;
		case SQLITE_NULL:
			f = Null;
			break;
		case SQLITE_BLOB:
			f = Value(String( (const byte*)sqlite3_column_blob(current_stmt,i),
			                  sqlite3_column_bytes(current_stmt,i)                ));
			break;
		default:
			NEVER();
			break;
	}
	return;
}
void Sqlite3Connection::Cancel() {
	if (NULL != current_stmt) {
		int retcode = sqlite3_reset(current_stmt);
		if (retcode != SQLITE_OK)
			session.SetError(sqlite3_errmsg(db),String("Resetting statement: ")+current_stmt_string);
		//ASSERT(SQLITE_OK == retcode);
		current_stmt = NULL;
	}
}
SqlSession& Sqlite3Connection::GetSession() const { return session; }
String Sqlite3Connection::ToString() const {
	return statement;
}
sqlite3_stmt *Sqlite3Connection::Compile(String statement) {
	int index = prepared_statements.Find(statement);
	if(index >= 0)
		return prepared_statements[index];
	if(prep_stmt_size + statement.GetLength() >= 1000000)
		FlushStatements();
	sqlite3_stmt* stmt = NULL;
	if (SQLITE_OK != sqlite3_prepare(db,statement,statement.GetLength(),&stmt,NULL)) {
		LLOG("Sqlite3Connection::Compile(" << statement << ") -> error");
		session.SetError(sqlite3_errmsg(db), String("Preparing: ")+statement );
		return NULL;
	}
	prepared_statements.Add(statement,stmt);
	LLOG("Sqlite3Connection::Compile(" << statement << ") = " << FormatIntHex(stmt));
	prep_stmt_size += statement.GetLength();
	return stmt;
}


//////////////////////////////////////////////////////////////////////


bool Sqlite3Session::Open(const char* filename) {
	// Only open db once.
	ASSERT(NULL == db);
	current_filename = filename;
	// By default, sqlite3 associates the opened db with "main.*"
	// However, using the ATTACH sql command, it can connect to more databases.
	// I don't know how to get the list of attached databases from the API
	current_dbname = "main";
	bool opened = (SQLITE_OK == sqlite3_open(filename, &db));
	return opened;
}
void Sqlite3Session::Close() {
	if (NULL != db) {
		int retval;
		retval = sqlite3_close(db);
		// If this function fails, that means that some of the
		// prepared statements have not been finalized.
		// See lib/sqlite3.h:91
		ASSERT(SQLITE_OK == retval);
		db = NULL;
	}
}
SqlConnection *Sqlite3Session::CreateConnection() {
	return new Sqlite3Connection(*this, db);
}
void Sqlite3Session::Begin() {
	static const char begin[] = "BEGIN;";
	if(trace)
		*trace << begin << "\n";
	if(SQLITE_OK != sqlite3_exec(db,begin,NULL,NULL,NULL))
		SetError(sqlite3_errmsg(db), begin);
}
void Sqlite3Session::Commit() {
	static const char commit[] = "COMMIT;";
	if(trace)
		*trace << commit << "\n";
	if(SQLITE_OK != sqlite3_exec(db,commit,NULL,NULL,NULL))
		SetError(sqlite3_errmsg(db), commit);
}
void Sqlite3Session::Rollback() {
	static const char rollback[] = "ROLLBACK;";
	if(trace)
		*trace << rollback << "\n";
	if(SQLITE_OK != sqlite3_exec(db,rollback,NULL,NULL,NULL))
		SetError(sqlite3_errmsg(db), rollback);
}
Vector<String> Sqlite3Session::EnumDatabases() {
	// In theory, sqlite3 can "ATTACH" multiple databases (up to 10).
	// However, I don't know how to list them.
	Vector<String> out;
	out.Add(current_dbname);
	return out;
}
Vector<String> Sqlite3Session::EnumTables(String database) {
	// Ignores database
	Vector<String> out;
	Sql sql(*this);
	sql*Select(SqlId("tbl_name")).From(SqlId("sqlite_master")).Where(SqlId("type")=="table");
	while (sql.Fetch())
		out.Add(sql[0]);
	return out;
}
Vector<SqlColumnInfo> Sqlite3Session::EnumColumns(String database, String table) {
	Vector<SqlColumnInfo> out;
	Sqlite3Connection sql(*this,db);
	sqlite3_stmt* s = sql.Compile(Format("select * from %s limit 0;",table));
	int retcode = sqlite3_step(s);
	ASSERT(SQLITE_DONE == retcode);
	for (int i = 0; i < sqlite3_column_count(s); i++) {
		SqlColumnInfo info;
		info.name = sqlite3_column_name(s,i);
		switch (sqlite3_column_type(s, i)) {
	    case 1:
	        info.type = INT_V;
	        break;
	    case 2:
	        info.type = DOUBLE_V;
	        break;
	    default:
	        info.type = STRING_V;
		}
		info.width = info.decimals = info.scale = info.prec = 0;
		out.Add(info);
	}
	sqlite3_reset(s);

	/*
	String cmd = Format("select * from %s limit 0;",table);
	sqlite3_stmt* s = // sql.Compile(); // doesn't work
	int retcode = sqlite3_step(s);
	ASSERT(SQLITE_DONE == retcode);
	for (int i = 0; i < sqlite3_column_count(s); i++) {
		SqlColumnInfo info;
		info.name = sqlite3_column_name(s,i);
		info.type = STRING_V;
		info.width = info.decimals = info.scale = info.prec = 0;
		out.Add(info);
	}
	sqlite3_reset(s);
	*/
	return out;
}


//////////////////////////////////////////////////////////////////////
