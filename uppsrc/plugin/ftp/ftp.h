#ifndef _plugin_ftp_ftp_h_
#define _plugin_ftp_ftp_h_

#include <Core/Core.h>

typedef struct NetBuf netbuf;

NAMESPACE_UPP

class FtpClient {
public:
	FtpClient();
	~FtpClient();

	bool    Connect(const char *host, const char *user = NULL, const char *password = NULL, bool pasv = true);
	void    Close();
	bool    IsOpen() const;

	bool    Save(const char *path, String data, Gate2<int, int> progress = false);
	String  Load(const char *path, Gate1<String> progress = false);

	bool    Rename(const char *oldpath, const char *newpath);
	bool    Cd(const char *path);
	bool    MkDir(const char *path);
	bool    RmDir(const char *path);
	bool    Delete(const char *path);
	bool    Exists(const char *path);
	void    RealizePath(const char *path);

	void    SetError(String s) { error = s; }
	String  GetError() const   { return error; }
	void    ClearError()       { error = NULL; }

	bool    CheckOpen();

public:
	Gate    WhenProgress;

private:
	static int Callback(netbuf *nControl, int xfered, void *arg);

private:
	netbuf *ftpconn;
	String  error;
};

String FtpClientGet(String url, String *error = NULL);

END_UPP_NAMESPACE

#endif
