#ifndef __OLEDB_H__
#define __OLEDB_H__

#include <Sql/Sql.h>

#if defined(PLATFORM_WIN32) && defined(COMPILER_MSC)

#define Font OleFont
#define Picture OlePicture
#include <Ole/Ole.h>
#include <winnls.h>
#include <oleauto.h>
#include <oledb.h>
#include <msdasc.h>
#include <msdaguid.h>
#undef Font
#undef Picture

bool   OleDBPerformScript(const String& text, StatementExecutor& executor, Gate2<int, int> progress_canceled = false);
String OleDBTextType(int width);

class OleDBSession : public SqlSession
{
public:
	OleDBSession();
	virtual ~OleDBSession();

	bool                   Open(String connect);
	bool                   Open(String user, String password, String datasource, String provider);
	bool                   OpenProp(String propset);
	void                   Close();

	virtual void           Begin();
	virtual void           Commit();
	virtual void           Rollback();

	virtual String         Savepoint();
	virtual void           RollbackTo(const String& savepoint);

	virtual bool           IsOpen() const;
	virtual RunScript      GetRunScript() const { return &OleDBPerformScript; }

	virtual Vector<String> EnumUsers();
	virtual Vector<String> EnumDatabases();
	virtual Vector<String> EnumTables(String database);
	virtual Vector<String> EnumViews(String database);
	virtual Vector<String> EnumSequences(String database);
	virtual Vector<String> EnumPrimaryKeys(String database, String table);
	virtual String         EnumRowID(String database, String table);

	struct Provider
	{
		String name;
		String description;
		String guid;
	};
	static Array<Provider> EnumProviders();

protected:
	virtual SqlConnection *CreateConnection();

protected:
	friend class OleDBConnection;

	IRef<IDBInitialize>    dbinit;
	IRef<IDBCreateCommand> dbsession;
	IRef<ITransactionLocal> transaction;
	IRef<ITransactionObject> transaction_object;
	Link<OleDBConnection>  clink;
	Guid                   dialect;
	String                 user;
	int                    level;
};

#endif

#endif//__OLEDB_H__
