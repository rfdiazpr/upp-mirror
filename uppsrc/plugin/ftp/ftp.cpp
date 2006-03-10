#include "ftp.h"
#pragma hdrstop

#include "lib/ftplib.h"

#define LLOGBLOCK(x)  LOGBLOCK(x)
#define LLOG(x)       LOG(x)

static bool ftpinit = false;

FtpClient::FtpClient()
{
	ftpconn = NULL;
	if(!ftpinit) {
		INTERLOCKED {
			if(!ftpinit) {
				FtpInit();
				ftpinit = true;
			}
		}
	}
}

FtpClient::~FtpClient()
{
	Close();
}

bool FtpClient::IsOpen() const
{
	return ftpconn;
}

bool FtpClient::Connect(const char *host, const char *user, const char *password, bool pasv)
{
	LOGBLOCK("FtpClient::Connect");
	Close();
	char perror[512];
	LLOG("FtpConnect(" << host << ")");
	if(WhenProgress()) {
		error = "connect aborted";
		return false;
	}
	if(!FtpConnect(host, &ftpconn, perror, &FtpClient::Callback, this, 200)) {
		error = perror;
		return false;
	}
	LLOG("FtpLogin(" << user << ", " << password << ")");
	if(!FtpLogin(user, password, ftpconn)) {
		error = FtpError(ftpconn);
		Close();
		return false;
	}
	LLOG("FtpOptions(pasv = " << pasv << ")");
	if(!FtpOptions(FTPLIB_CONNMODE, pasv ? FTPLIB_PASSIVE : FTPLIB_PORT, ftpconn)) {
		error = FtpError(ftpconn);
		Close();
		return false;
	}
	return true;
}

int FtpClient::Callback(netbuf *nControl, int xfered, void *arg)
{
	return !((FtpClient *)arg)->WhenProgress();
}

void FtpClient::Close()
{
	if(ftpconn) {
		FtpQuit(ftpconn);
		ftpconn = NULL;
	}
}

bool FtpClient::CheckOpen()
{
	if(!IsOpen()) {
		error = t_("not connected");
		return false;
	}
	return true;
}

String FtpClient::Load(const char *path, Gate1<String> progress)
{
	LLOGBLOCK("FtpClient::Load");
	if(!CheckOpen())
		return String::GetVoid();
	netbuf *ftpdata;
	LLOG("FtpAccess(" << path << ")");
	if(progress(NFormat(t_("Reading file '%s'"), path))) {
		error = t_("aborted");
		return String::GetVoid();
	}
	if(!FtpAccess(path, FTPLIB_FILE_READ, FTPLIB_IMAGE, ftpconn, &ftpdata)) {
		error = FtpError(ftpconn);
		return String::GetVoid();
	}
	String result;
	int p = 0;
	for(;;) {
		if(progress(result))
			break;
		byte buffer[1024];
		int ndata = FtpRead(buffer, sizeof(buffer), ftpdata);
		LLOG("FtpRead -> " << ndata);
		if(ndata < 0) {
			error = FtpError(ftpdata);
			FtpClose(ftpdata);
			return String::GetVoid();
		}
		if(ndata == 0) {
			result.Shrink();
			break;
		}
		result.Cat(buffer, ndata);
#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; Sleep(d))
			;
#endif
	}
	FtpClose(ftpdata);
	return result;
}

bool FtpClient::Save(const char *path, String data, Gate2<int, int> progress)
{
	LLOGBLOCK("FtpClient::Save");
	netbuf *ftpdata;
	if(!CheckOpen())
		return false;
	LLOG("FtpAccess(" << path << ")");
	if(progress(0, data.GetLength()))
		return false;
	if(!FtpAccess(path, FTPLIB_FILE_WRITE, FTPLIB_IMAGE, ftpconn, &ftpdata)) {
		error = FtpError(ftpconn);
		return false;
	}
	for(int done = 0; done < data.GetLength();) {
		if(progress(done, data.GetLength())) {
			error = NFormat(t_("write aborted after %d bytes(s)"), done);
			FtpClose(ftpdata);
			return false;
		}
		int chunk = min(data.GetLength() - done, 1024);
		int ndata = FtpWrite((void *)data.GetIter(done), chunk, ftpdata);
		LLOG("FtpWrite(" << chunk << ") -> " << ndata);
		if(ndata <= 0 || ndata > chunk) {
			error = FtpError(ftpdata);
			FtpClose(ftpdata);
			return false;
		}
		done += ndata;
#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; Sleep(d))
			;
#endif
	}
	progress(data.GetLength(), data.GetLength());
	LLOG("FtpClose");
	FtpClose(ftpdata);
	return true;
}

bool  FtpClient::Exists(const char *path) {
	LLOGBLOCK("FtpClient::Exists");
	if(!CheckOpen())
		return false;
	netbuf *data;
	LLOG("FtpAccess(" << path << ")");
	if(!FtpAccess(path, FTPLIB_FILE_READ, FTPLIB_IMAGE, ftpconn, &data)) {
		error = FtpError(ftpconn);
		return false;
	}
	FtpClose(data);
	return true;
}

bool FtpClient::Rename(const char *oldpath, const char *newpath) {
	LLOGBLOCK("FtpClient::Rename");
	LLOG("FtpRename(oldname = " << oldpath << ", newname = " << newpath << ")");
	return CheckOpen() && !!FtpRename(oldpath, newpath, ftpconn);
}

bool FtpClient::Cd(const char *path) {
	LLOGBLOCK("FtpClient::Cd");
	LLOG("FtpChdir(" << path << ")");
	return CheckOpen() && !!FtpChdir(path, ftpconn);
}

bool FtpClient::MkDir(const char *path) {
	LLOGBLOCK("FtpClient::MkDir");
	LLOG("FtpMkdir(" << path << ")");
	return CheckOpen() && !!FtpMkdir(path, ftpconn);
}

bool FtpClient::RmDir(const char *path) {
	LLOGBLOCK("FtpClient::RmDir");
	LLOG("FtpRmdir(" << path << ")");
	return CheckOpen() && !!FtpRmdir(path, ftpconn);
}

bool FtpClient::Delete(const char *path) {
	LLOGBLOCK("FtpClient::Delete");
	LLOG("FtpDelete(" << path << ")");
	return CheckOpen() && !!FtpDelete(path, ftpconn);
}

void FtpClient::RealizePath(const char *path)
{
	LLOGBLOCK("FtpClient::RealizePath");
	const char *s = path;
	if(*s == '\0') return;
	for(;;) {
		s = strchr(s + 1, '/');
		if(!s) break;
		MkDir(String(path, s));
	}
}

String FtpClientGet(String url, String *error)
{
	const char *u = url, *t = u;
	while(*t && *t != '?')
		if(*t++ == '/') {
			if(*t == '/')
				u = ++t;
			break;
		}
	t = u;
	while(*u && *u != '@' && *u != '/')
		u++;
	String host = String(t, u);
	String user, pwd;
	if(*u == '@') {
		t = ++u;
		while(*u && *u != '/' && *u != ':')
			u++;
		user = String(t, u);
		if(*u == ':') {
			t = ++u;
			while(*u && *u != '/')
				u++;
			pwd = String(t, u);
		}
	}
	FtpClient ftp;
	if(!ftp.Connect(host, user, pwd)) {
		if(error)
			*error = ftp.GetError();
		return String::GetVoid();
	}
	String data = ftp.Load(u);
	if(data.IsVoid()) {
		if(error)
			*error = ftp.GetError();
	}
	return data;
}
