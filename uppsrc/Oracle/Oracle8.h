#ifndef ORACLE8_H
#define ORACLE8_H

#include <Sql/Sql.h>
#include "OraCommon.h"

#if defined(PLATFORM_WIN32) && defined(COMPILER_MSC)
#define __STDC__ 1
#endif

//#define dword _dword
//#include <oci.h>
//#undef dword

// selected definitions from OCI headers
struct OCIEnv;
struct OCIServer;
struct OCISession;
struct OCISvcCtx;
struct OCIError;
struct OCILobLocator;
//

class Oracle8 : public SqlSession {
public:
	virtual void                  Begin();
	virtual void                  Commit();
	virtual void                  Rollback();

	virtual String                Savepoint();
	virtual void                  RollbackTo(const String& savepoint);

	virtual bool                  IsOpen() const;
	virtual int                   GetDialect() const            { return ORACLE; }

	virtual RunScript             GetRunScript() const          { return &OraclePerformScript; }

	virtual Vector<String>        EnumUsers();
	virtual Vector<String>        EnumDatabases();
	virtual Vector<String>        EnumTables(String database);
	virtual Vector<String>        EnumViews(String database);
	virtual Vector<String>        EnumSequences(String database);
	virtual Vector<String>        EnumPrimaryKey(String database, String table);
	virtual String                EnumRowID(String database, String table);
	virtual Vector<String>        EnumReservedWords();

protected:
	virtual SqlConnection *CreateConnection();

private:
	bool                   AllocOciHandle(void *hp, int type);
	void                   FreeOciHandle(void *hp, int type);

private:
	friend class OCI8Connection;
	friend class OracleBlob;

	OCIEnv              *envhp;
	OCIError            *errhp;
	OCIServer           *srvhp;
	OCISession          *seshp;
	OCISvcCtx           *svchp;
	Link<OCI8Connection> clink;
	bool                 in_server;
	bool                 in_session;
	String               user;
	int                  level;
	int                  tmode;

	String         Spn();
	void           PostError();
	bool           StdMode()   { return  tmode == NORMAL; }

public:
	bool Login(const char *name, const char *password, const char *connect, String *warn = NULL);
	bool Open(const String& connect_string, String *warn = NULL);
	void Logoff();

	enum TransactionMode {
		NORMAL,              // autocommit at level 0, no Commit or Rollback allowed at level 0
		ORACLE               // Oracle-style Commit and Rollback at level 0
	};

	void    SetTransactionMode(int mode)            { tmode = mode; }
	Oracle8();
	~Oracle8();
};

class OracleBlob : public BlockStream {
protected:
	virtual  void  SetStreamSize(int64 size);
	virtual  dword Read(int64 at, void *ptr, dword size);
	virtual  void  Write(int64 at, const void *data, dword size);

public:
	virtual  bool  IsOpen() const;
	virtual  void  Close();

protected:
	OCILobLocator   *locp;
	Oracle8         *session;

	void      Init(dword size);

public:
	operator  bool() const                 { return IsOpen(); }

	void      Assign(const Sql& sql, int blob);
	void      Assign(Oracle8& session, int blob);

	OracleBlob();
	OracleBlob(const Sql& sql, int blob);
	OracleBlob(Oracle8& session, int blob);
	~OracleBlob();
};

#ifdef text
#undef text
#endif

#endif
