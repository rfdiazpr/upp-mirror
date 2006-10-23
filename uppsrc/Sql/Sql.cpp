#include "Sql.h"

bool SqlToBool(const String& s) {
	return !(IsNull(s) || *s == '0' && s[1] == '\0');
}

bool SqlToBool(const Value& v) {
	if(IsNull(v)) return false;
	if(IsString(v)) return SqlToBool(String(v));
	if(IsNumber(v)) return (int) v;
	return true;
}

const String& BoolToSql(bool b) {
	static String T("1"), F("0");
	return b ? T : F;
}

void FieldOperator::Field(Ref f) {}

void FieldOperator::Field(const char *name, Ref f) { Field(f); }

FieldOperator& FieldOperator::operator()(const char *name, bool& b) {
	String x = BoolToSql(b);
	Field(name, x);
	b = SqlToBool(x);
	return *this;
}

static char sql_error[] = "Datab??zov?? chyba";

#ifndef NOAPPSQL
SqlExc::SqlExc() : Exc(sql_error) {
	SetSessionError(SQL.GetSession());
}
#endif

SqlExc::SqlExc(const SqlSession& session) : Exc(sql_error) {
	SetSessionError(session);
}

SqlExc::SqlExc(const Sql& sql) : Exc(sql_error) {
	SetSessionError(sql.GetSession());
}

void SqlExc::SetSessionError(const SqlSession& session) {
	if(session.WasError())
		*this = session.GetLastError();
	else
		*this = String(sql_error);
	*this << "\nSQL p?¸?­kaz: " << session.GetErrorStatement();
}

SqlConnection::SqlConnection()  { parse = true; fetchrows = 32; longsize = 16384; }
SqlConnection::~SqlConnection() {}

void SqlConnection::Cancel() {}

int  SqlConnection::GetRowsProcessed() const {
	NEVER();
	return 0;
}

String SqlConnection::GetUser() const {
	NEVER();
	return Null;
}

Value SqlConnection::GetInsertedId() const
{
	NEVER();
	return Null;
}

String Sql::Compile(const SqlStatement& s)
{
	byte dialect = GetDialect();
	ASSERT(dialect);
	return s.Get(dialect);
}

void Sql::Clear() {
	if(cn) {
		cn->Cancel();
		cn->parse = true;
	}
}

void Sql::SetParam(int i, const Value& val) {
	cn->SetParam(i, val);
}

void Sql::SetStatement(const String& s) {
	cn->statement = s;
	cn->parse = true;
}

bool Sql::Execute() {
	if(GetSession().traceslow < INT_MAX)
		cn->starttime = GetTickCount();
	bool b = cn->Execute();
	if(GetSession().usrlog)
		UsrLogT(9, cn->statement);
	if(Stream *s = GetSession().GetTrace()) {
		*s << cn->statement << '\n';
		if(!b)
			*s << "## ERROR: " << GetSession().GetLastError() << '\n';
	}
	return b;
}

void Sql::ExecuteX() {
	if(!Execute())
		throw SqlExc(GetSession());
}

bool Sql::Execute(const String& s) {
	SetStatement(s);
	return Execute();
}

void Sql::ExecuteX(const String& s) {
	SetStatement(s);
	ExecuteX();
}

#define E__SetParam(I)    SetParam(I - 1, p##I)

#define E__RunF(I) \
bool Sql::Run(__List##I(E__Value)) { \
	__List##I(E__SetParam); \
	return Run(); \
}
__Expand(E__RunF)

#define E__RunFX(I) \
void Sql::RunX(__List##I(E__Value)) { \
	__List##I(E__SetParam); \
	RunX(); \
}
__Expand(E__RunFX)

#define E__ExecuteF(I) \
bool Sql::Execute(const String& s, __List##I(E__Value)) { \
	SetStatement(s); \
	__List##I(E__SetParam); \
	return Execute(); \
}
__Expand(E__ExecuteF)

#define E__ExecuteFX(I) \
void Sql::ExecuteX(const String& s, __List##I(E__Value)) { \
	SetStatement(s); \
	__List##I(E__SetParam); \
	ExecuteX(); \
}
__Expand(E__ExecuteFX)

bool Sql::Fetch() {
	int t0 = GetTickCount();
	bool b = cn->Fetch();
	int t = GetTickCount();
	if(t - GetSession().traceslow > cn->starttime)
		BugLog() << t - cn->starttime << " ms: " << cn->statement << '\n';
	else
	if(t - t0 > GetSession().traceslow)
		BugLog() << t - t0 << " ms further fetch: " << cn->statement << '\n';
	cn->starttime = INT_MAX;
	return b;
}

#define E__GetColumn(I)    cn->GetColumn(I - 1, p##I)

#define E__FetchF(I) \
bool Sql::Fetch(__List##I(E__Ref)) { \
	if(!Fetch()) return false; \
	__List##I(E__GetColumn); \
	return true; \
}
__Expand(E__FetchF)

bool Sql::Fetch(Vector<Value>& row) {
	if(!Fetch()) return false;
	int n = GetColumns();
	row.SetCount(n);
	for(int i = 0; i < n; i++)
		GetColumn(i, row[i]);
	return true;
}

struct sFetchFields : public FieldOperator {
	int  pi;
	Sql *sql;

	void Field(Ref r)       { sql->GetColumn(pi++, r); }
};

bool Sql::Fetch(Fields fo) {
	if(!Fetch()) return false;
	sFetchFields ff;
	ff.sql = this;
	ff.pi = 0;
	fo(ff);
	return true;
}

int  Sql::GetColumns() const {
	return cn->info.GetCount();
}

void Sql::GetColumn(int i, Ref r) const {
	cn->GetColumn(i, r);
}

Value Sql::operator[](int i) const {
	Value v;
	cn->GetColumn(i, v);
	return v;
}

Value Sql::operator[](SqlId id) const {
	String s = ~id;
	for(int i = 0; i < cn->info.GetCount(); i++)
		if(cn->info[i].name == s)
			return operator[](i);
	return Value();
}

Vector<Value> Sql::GetRow() const {
	Vector<Value> row;
	int cn = GetColumns();
	for(int i = 0; i < cn; i++)
		row.Add((*this)[i]);
	return row;
}

Value Sql::Select0(const String& s) {
	SetStatement(s);
	if(!Run())
		return ErrorValue(GetLastError());
	if(!Fetch())
		return Null;
	Value v;
	cn->GetColumn(0, v);
	return v;
}

Value Sql::Select(const String& s) {
	return Select0("select " + s);
}

#define E__SelectF(I) \
Value Sql::Select(const String& s, __List##I(E__Value)) { \
	__List##I(E__SetParam); \
	return Select(s); \
}
__Expand(E__SelectF)

#define E__Inserter(I)  clist += ", ", clist += c##I, qlist += ", ?", SetParam(I, v##I)

#define E__InsertF(I) \
bool Sql::Insert(const char *table, const char *c0, const Value& v0, __List##I(E__ColVal)) { \
	String  clist = c0; \
	String  qlist = "?"; \
	SetParam(0, v0); \
	__List##I(E__Inserter); \
	return Execute(String("insert into ") + table + '(' + clist + ") values(" + qlist + ')'); \
}
__Expand(E__InsertF)

#define E__InserterId(I)  clist += ", ", clist += c##I.ToString(), qlist += ", ?", SetParam(I, v##I)

#define E__InsertIdF(I) \
bool Sql::Insert(SqlId table, SqlId c0, const Value& v0, __List##I(E__IdVal)) { \
	String  clist = c0.ToString(); \
	String  qlist = "?"; \
	SetParam(0, v0); \
	__List##I(E__InserterId); \
	return Execute( \
			String("insert into ") + table.ToString() + '(' + clist + ") values(" + qlist + ')'); \
}
__Expand(E__InsertIdF)


static inline void sComma(int I, String& s) {
	if(I > 1) s.Cat(", ");
}

#define E__Updater(I)  sComma(I, list), list.Cat(c##I), list.Cat(" = ?"), SetParam(I - 1, v##I)

#define  E__UpdateF(I) \
bool Sql::Update(const char *table, const char *key, const Value& keyval, __List##I(E__ColVal)) { \
	String list; \
	__List##I(E__Updater); \
	SetParam(I, keyval); \
	return Execute(String ("update ") + table + " set " + list + " where " + key + " = ?"); \
}
__Expand(E__UpdateF)

#define E__UpdaterId(I)  sComma(I, list), list.Cat(c##I.ToString()), list.Cat(" = ?"), SetParam(I - 1, v##I)

#define  E__UpdateIdF(I) \
bool Sql::Update(SqlId table, SqlId key, const Value& keyval, __List##I(E__IdVal)) { \
	String list; \
	__List##I(E__UpdaterId); \
	SetParam(I, keyval); \
	return Execute(String ("update ") + table.ToString() + \
	               " set " + list + " where " + key.ToString() + " = ?"); \
}
__Expand(E__UpdateIdF)

bool Sql::Delete(const char *table, const char *key, const Value& keyval) {
	return Execute("delete from " + String(table) + " where " + key + " = ?", keyval);
}

bool Sql::Delete(SqlId table, SqlId key, const Value& keyval) {
	return Delete(~table.ToString(), ~key.ToString(), keyval);
}

int Sql::GetDialect() const {
	return GetSession().GetDialect();
}

struct NfInsert : public FieldOperator {
	int    i;
	Sql   *sql;
	String clist;
	String qlist;

	virtual void Field(const char *name, Ref f) {
		if(i) {
			clist += ", ";
			qlist += ", ";
		}
		clist += name;
		qlist += "? ";
		sql->SetParam(i++, f);
	}
};

bool Sql::Insert(Fields nf) {
	NfInsert w;
	w.i = 0;
	w.sql = this;
	nf(w);
	return Execute(String("insert into ") + w.table + '(' + w.clist + ") values(" + w.qlist + ')');
}

#define E__Updater(I)  sComma(I, list), list.Cat(c##I), list.Cat(" = ?"), SetParam(I - 1, v##I)

struct NfUpdate : public FieldOperator {
	int    i;
	Sql   *sql;
	String list;
	String key;
	Value  keyval;

	virtual void Field(const char *name, Ref f) {
		if(i == 0) {
			key = name;
			keyval = f;
		}
		else {
			if(i > 1)
				list += ", ";
			list << name << " = ?";
			sql->SetParam(i - 1, f);
		}
		i++;
	}
};

bool Sql::Update(Fields nf) {
	NfUpdate w;
	w.i = 0;
	w.sql = this;
	nf(w);
	SetParam(w.i - 1, w.keyval);
	return Execute(String ("update ") + w.table + " set " + w.list + " where " + w.key + " = ?");
}

void Sql::Assign(SqlSource& s) {
	if(cn) delete cn;
	cn = s.CreateConnection();
}

void   Sql::SetError(const String& err, const String& stmt) { GetSession().SetError(err, stmt); }
void   Sql::ClearError()                          { GetSession().ClearError(); }

String Sql::GetLastError() const                  { return GetSession().GetLastError(); }
String Sql::GetErrorStatement() const             { return GetSession().GetErrorStatement(); }
bool   Sql::WasError() const                      { return GetSession().WasError(); }

void   Sql::Begin()                               { GetSession().Begin(); }
void   Sql::Commit()                              { GetSession().Commit(); }
void   Sql::Rollback()                            { GetSession().Rollback(); }

String Sql::Savepoint()                           { return GetSession().Savepoint(); }
void   Sql::RollbackTo(const String& savepoint)   { GetSession().RollbackTo(savepoint); }

bool   Sql::IsOpen()                              { return cn && GetSession().IsOpen(); }

#ifndef NOAPPSQL
GLOBAL_VAR(AppSql, AppCursor)
#endif

#ifndef NOAPPSQL
Sql::Sql() {
	cn = NULL;
	if(SQL.cn)
		cn = SQL.GetSession().CreateConnection();
}
#endif

Sql::Sql(SqlSource& s) {
	cn = s.CreateConnection();
}

#ifndef NOAPPSQL
Sql::Sql(const char *stmt) {
	cn = SQL.GetSession().CreateConnection();
	SetStatement(stmt);
}
#endif

Sql::Sql(const char *stmt, SqlSource& s) {
	cn = s.CreateConnection();
	SetStatement(stmt);
}

#ifndef NOAPPSQL
Sql::Sql(const SqlStatement& stmt) {
	cn = SQL.GetSession().CreateConnection();
	SetStatement(stmt);
}
#endif

Sql::Sql(const SqlStatement& stmt, SqlSource& s) {
	cn = s.CreateConnection();
	SetStatement(stmt);
}

Sql::Sql(SqlConnection *connection)
: cn(connection)
{}

Sql::~Sql() {
	if(cn) delete cn;
}

SqlSession::SqlSession()
{
	trace = NULL;
	traceslow = INT_MAX / 4;
	logerrors = false;
	usrlog = false;
	tracetime = false;
	dialect = 255;
}

SqlSession::~SqlSession() {}

void           SqlSession::Begin()                                       { NEVER(); }
void           SqlSession::Commit()                                      { NEVER(); }
void           SqlSession::Rollback()                                    { NEVER(); }
String         SqlSession::Savepoint()                                   { NEVER(); return Null; }
void           SqlSession::RollbackTo(const String&)                     { NEVER(); }
bool           SqlSession::IsOpen() const                                { NEVER(); return false; }
RunScript      SqlSession::GetRunScript() const                          { return NULL; }
SqlConnection *SqlSession::CreateConnection()                            { return NULL; }
Vector<String> SqlSession::EnumUsers()                                   { return Vector<String>(); }
Vector<String> SqlSession::EnumDatabases()                               { return Vector<String>(); }
Vector<String> SqlSession::EnumTables(String database)                   { return Vector<String>(); }
Vector<String> SqlSession::EnumViews(String database)                    { return Vector<String>(); }
Vector<String> SqlSession::EnumSequences(String database)                { return Vector<String>(); }
Vector<String> SqlSession::EnumPrimaryKey(String database, String table) { return Vector<String>(); }
Vector<String> SqlSession::EnumReservedWords()                           { return Vector<String>(); }
String         SqlSession::EnumRowID(String database, String table)      { return Null; }

Vector<SqlColumnInfo> SqlSession::EnumColumns(String database, String table)
{
	Sql cursor(*this);
	Vector<SqlColumnInfo> info;
	SqlBool none;
	none.SetFalse();
	if(cursor.Execute(Select(SqlAll()).From(SqlSet(SqlCol(database + '.' + table))).Where(none))) {
		info.SetCount(cursor.GetColumns());
		for(int i = 0; i < info.GetCount(); i++)
			info[i] = cursor.GetColumnInfo(i);
	}
	return info;
}

void   SqlSession::SetError(const String& error, const String& stmt) {
	lasterror = error;
	errorstatement = stmt;
	String err;
	err << "ERROR " << error << ": " << stmt << '\n';
	if(logerrors)
		BugLog() << err;
	if(GetTrace())
		*GetTrace() << err;
}

void   SqlSession::ClearError()
{
	lasterror.Clear();
	errorstatement.Clear();
}

bool StdStatementExecutor::Execute(const String& stmt)
{
	cursor.Execute(stmt);
	return true;
}

#ifndef NOAPPSQL
struct SQLStatementExecutorClass : StatementExecutor {
	virtual bool Execute(const String& stmt) { SQL.Execute(stmt); return true; }
};

StatementExecutor& SQLStatementExecutor() {
	return Single<SQLStatementExecutorClass>();
}
#endif
