#include "SqlCtrl.h"
#include <TCtrlLib/TCtrlLib.h>
#include <TCtrlLib/OldTreeCtrl.h>

static bool StringLessNoCase(String a, String b) { return CompareNoCase(a, b) < 0; }

class DlgSqlExport : public WithSqlExportLayout<TopWindow>
{
public:
	typedef DlgSqlExport CLASSNAME;
	DlgSqlExport(String table_name);

	Vector<int> Run(const Vector<SqlColumnInfo>& cols);
	void        Serialize(Stream& stream);

private:
	ArrayOption exp;
};

DlgSqlExport::DlgSqlExport(String table_name)
{
	CtrlLayoutOKCancel(*this, table_name + " - data export");
	columns.AddColumn("Column name");
	columns.AddColumn("Type");
	columns.AddColumn("Width");
	exp.AddColumn(columns, "Export");
	dialect.Add(SQLD_ORACLE, "Oracle");
	dialect.Add(SQLD_MYSQL, "MySQL");
	dialect.Add(SQLD_SQLITE3, "SQLite3");
	dialect.Add(SQLD_MSSQL, "MS-SQL Server");
	commit_each <<= 100;
}

Vector<int> DlgSqlExport::Run(const Vector<SqlColumnInfo>& info)
{
	for(int i = 0; i < info.GetCount(); i++)
		columns.Add(info[i].name, info[i].type, info[i].width, 1);
	static String cfg;
	StringStream strm(cfg);
	if(!strm.IsEof())
		Serialize(strm);
	while(TopWindow::Run() == IDOK)
	{
		Vector<int> out;
		for(int i = 0; i < columns.GetCount(); i++)
			if((int)columns.Get(i, 3))
				out.Add(i);
		if(out.IsEmpty())
		{
			Exclamation("No columns selected!");
			continue;
		}
		strm.Create();
		Serialize(strm);
		cfg = strm;
		return out;
	}
	return Vector<int>();
}

void DlgSqlExport::Serialize(Stream& stream)
{
	int version = 1;
	stream / version
	% commit_each
	% dialect
	% omit_nulls % omit_schema
	% identity_insert;
}

class SqlObjectTree : public TopWindow {
public:
	typedef SqlObjectTree CLASSNAME;
	SqlObjectTree(SqlSession& session);

	void        Run();

private:
	void        OnSchemas(OldTreeItem& root);
	void        OnSchemaObjects(OldTreeItem& root);
	void        OnTables(OldTreeItem& root);
	void        OnViews(OldTreeItem& root);
	void        OnSequences(OldTreeItem& root);
	void        OnTableColumns(OldTreeItem& root);

	void        ToolLocal(Bar& bar);
	void        OnTableExport(String table_name);

private:
	OldTreeCtrl    schema;
	SqlSession& session;
	enum { OBJ_ROOT, OBJ_DATABASE, OBJ_TABLE, OBJ_VIEW, OBJ_SEQUENCE, OBJ_COLUMN, OBJ_PRIMARY_KEY, OBJ_ROWID };
};

void SQLObjectTree(SqlSession& session) { SqlObjectTree(session).Run(); }

SqlObjectTree::SqlObjectTree(SqlSession& sess)
: session(sess)
{
	Title("SQL object tree");
	Sizeable().MaximizeBox();
	Add(schema.SizePos());
	schema.SetImage(CtrlImg::Computer()).Text("Schemas").Subtree(THISBACK(OnSchemas));
	schema.AutoSize().TouchLayout();
	schema.WhenBar = THISBACK(ToolLocal);
}

void SqlObjectTree::Run() {
	TopWindow::Run();
}

void SqlObjectTree::OnSchemas(OldTreeItem& root) {
	try {
		root.Clear();
		Vector<String> schemas = session.EnumDatabases();
		Sort(schemas);
		for(int i = 0; i < schemas.GetCount(); i++)
			root.Add().SetImage(CtrlImg::Dir()).Text(schemas[i]).Subtree(THISBACK(OnSchemaObjects));
	} catch(Exc e) { ShowExc(e); }
}

void SqlObjectTree::OnSchemaObjects(OldTreeItem& root) {
	root.Clear();
	root.Add().SetImage(CtrlImg::Dir()).Text("Tables").Subtree(THISBACK(OnTables)).Class(OBJ_DATABASE);
	root.Add().SetImage(CtrlImg::Dir()).Text("Views").Subtree(THISBACK(OnViews)).Class(OBJ_DATABASE);
	root.Add().SetImage(CtrlImg::Dir()).Text("Sequences").Subtree(THISBACK(OnSequences)).Class(OBJ_DATABASE);
}

void SqlObjectTree::OnTables(OldTreeItem& root) {
	try {
		root.Clear();
		String schema = root.GetParent()->GetText();
		Vector<String> tables = session.EnumTables(schema);
		Sort(tables);
		for(int i = 0; i < tables.GetCount(); i++)
			root.Add().SetImage(CtrlImg::File()).Text(tables[i]).Subtree(THISBACK(OnTableColumns))
			.Class(OBJ_TABLE)
			.SetData(schema + "." + tables[i]);
	} catch(Exc e) { ShowExc(e); }
}

void SqlObjectTree::OnViews(OldTreeItem& root) {
	try {
		root.Clear();
		String schema = root.GetParent()->GetText();
		Vector<String> views = session.EnumViews(schema);
		Sort(views);
		for(int i = 0; i < views.GetCount(); i++)
			root.Add().SetImage(CtrlImg::File()).Text(views[i])
			.Class(OBJ_VIEW)
			.Subtree(THISBACK(OnTableColumns))
			.SetData(schema + "." + views[i]);
	} catch(Exc e) { ShowExc(e); }
}

void SqlObjectTree::OnSequences(OldTreeItem& root) {
	try {
		root.Clear();
		String schema = root.GetParent()->GetText();
		Vector<String> sequences = session.EnumSequences(schema);
		Sort(sequences);
		for(int i = 0; i < sequences.GetCount(); i++)
			root.Add().SetImage(CtrlImg::File()).Text(sequences[i])
			.Class(OBJ_SEQUENCE)
			.SetData(schema + "." + sequences[i]);
	} catch(Exc e) { ShowExc(e); }
}

void SqlObjectTree::OnTableColumns(OldTreeItem& root) {
	try {
		root.Clear();
		String schema_table = ~root;
		int f = schema_table.Find('.');
		ASSERT(f >= 0);
		String schema = schema_table.Left(f);
		String table = schema_table.Mid(f + 1);
		Vector<SqlColumnInfo> columns = session.EnumColumns(schema, table);
		for(int i = 0; i < columns.GetCount(); i++)
			root.Add().SetImage(CtrlImg::Hd()).Text(columns[i].name) //.Subtree(THISBACK(OnSubObjects))
			.Class(OBJ_COLUMN);
//			.SetData("COLUMN:" + schema + "." + table + "." + columns[i]);
		Vector<String> pk = session.EnumPrimaryKey(schema, table);
		if(!pk.IsEmpty()) {
			String pklist;
			for(int i = 0; i < pk.GetCount(); i++)
				pklist << (i ? "; " : "Primary key: ") << pk[i];
			root.Add().Text(pklist)
				.Class(OBJ_PRIMARY_KEY);
		}
		String rowid = session.EnumRowID(schema, table);
		if(!IsNull(rowid)) {
			root.Add().Text("RowID: " + rowid).Class(OBJ_ROWID);
		}
	} catch(Exc e) { ShowExc(e); }
}

void SqlObjectTree::ToolLocal(Bar& bar)
{
	schema.StdBar(bar);
	if(!bar.IsEmpty())
		bar.MenuSeparator();
	OldTreeItem *item = schema.GetCursorItem();
	if(item)
		switch(item->GetClass())
		{
		case OBJ_TABLE:
		case OBJ_VIEW:
			bar.Add("Export", THISBACK1(OnTableExport, item->GetData()));
		}
}

void SqlObjectTree::OnTableExport(String table_name)
{
	String schema, table = table_name;
	int f = table.Find('.');
	if(f >= 0) {
		schema = table.Left(f);
		table = table.Mid(f + 1);
	}
	Vector<SqlColumnInfo> info = session.EnumColumns(schema, table);
	DlgSqlExport expdlg(table_name);
	Vector<int> cindex = expdlg.Run(info);
	if(cindex.IsEmpty())
		return;
	int dialect = ~expdlg.dialect;
	String out_table = (expdlg.omit_schema ? table : table_name);
	Vector<String> colnames;
	SqlSet column_list;
	for(int i = 0; i < cindex.GetCount(); i++)
	{
		String name = info[cindex[i]].name;
		colnames.Add(name);
		column_list |= SqlCol(name);
	}
	static String recent_file;
	FileSel fsel;
	fsel.Type("SQL scripts (*.sql)", "*.sql")
		.AllFilesType()
		.DefaultExt("sql");
	fsel <<= AppendFileName(recent_file, AppendExt(ToLower(table), ".sql"));
	if(!fsel.ExecuteSaveAs("Save export as"))
		return;
	recent_file = GetFileDirectory(~fsel);
	Sql cursor(session);
	if(!Select(column_list).From(SqlSet(SqlCol(table_name))).Execute(cursor))
	{
		ShowExc(SqlExc(cursor));
		return;
	}
	FileOut fo;
	if(!fo.Open(~fsel)) {
		Exclamation(NFormat("Error creating file [* \1%s\1].", ~fsel));
		return;
	}
	Progress progress("Exporting row %d");
	int autocommit = ~expdlg.commit_each;
	int left = autocommit;
	while(cursor.Fetch()) {
		String script;
		SqlSet columns, values;
		values.Dialect(dialect);
		for(int i = 0; i < cursor.GetColumns(); i++)
			if(!expdlg.omit_nulls || !IsNull(cursor[i])) {
				columns |= SqlCol(colnames[i]);
				values |= cursor[i];
			}
		if(expdlg.identity_insert)
			script << "set identity_insert " << out_table << " on ";
		script << "insert into " << out_table << columns() << "\n" "values " << values() << ";";
		fo.PutLine(script);
		if(autocommit && --left <= 0) {
			fo.PutLine("commit;");
			left = autocommit;
		}
		if(progress.StepCanceled()) {
			Exclamation("Export aborted!");
			return;
		}
	}
	fo.Close();
	if(fo.IsError())
		Exclamation(NFormat("Error writing file [* \1%s\1].", ~fsel));
	return;
}
