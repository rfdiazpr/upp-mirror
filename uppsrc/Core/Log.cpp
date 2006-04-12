#include "Core.h"

LogStream::LogStream()
{
#ifdef PLATFORM_POSIX
	hfile = -1;
#else
	hfile = INVALID_HANDLE_VALUE;
#endif
	part = 0;
	sizelimit = 0;
	*filename = 0;
}

LogStream::~LogStream() {}

void LogStream::Close()
{
#ifdef PLATFORM_POSIX
	if(hfile >= 0)
		close(hfile);
	hfile = -1;
#else
	if(hfile != INVALID_HANDLE_VALUE)
		CloseHandle(hfile);
	hfile = INVALID_HANDLE_VALUE;
#endif
}

bool LogStream::Delete()
{
	Close();
	if(*filename) {
		if(!DeleteFile(filename)) {
			BugLog() << "Error deleting " << filename << ": " << GetLastErrorMessage();
			return false;
		}
		*filename = 0;
	}
	return true;
}

void LogStream::Create(const char *path, bool append)
{
	Close();

	strcpy(filename, path);
	strcpy(backup, filename);
	strcat(backup, ".old");

#if defined(PLATFORM_WIN32)
	::DeleteFile(backup);
#elif defined(PLATFORM_POSIX)
	unlink(backup);
#else
	#error
#endif

#if defined(PLATFORM_WIN32)
	::MoveFile(filename, backup);
#elif defined(PLATFORM_POSIX)
	!rename(filename, backup);
#else
	#error
#endif

	filesize = 0;

#ifdef PLATFORM_WIN32
	hfile = CreateFile(filename,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		append ? OPEN_ALWAYS : CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if(append)
		filesize = (int)SetFilePointer(hfile, 0, NULL, FILE_END);
#else
	hfile = open(filename, append ? O_CREAT|O_RDWR : O_CREAT|O_RDWR|O_TRUNC, 0644);
	if(append)
		filesize = (int)lseek(hfile, 0, SEEK_END);
#endif
	wrlim = ptr = (byte *)this;
	p = buffer;

	Time t = GetSysTime();
	char exe[512];
	char user[500];
	*user = 0;

#ifdef PLATFORM_WIN32
	GetModuleFileName(AppGetHandle(), exe, 512);
	dword w = 2048;
	GetUserName(user, &w);
#else //#
	const char *GetExeTitleCharPtr();
	strcpy(exe, GetExeTitleCharPtr());
	strcpy(user, getenv("USER"));
#endif

	char h[1000];
	sprintf(h, "* %s %02d.%02d.%04d %02d:%02d:%02d, user: %s\n",
	           exe, t.day, t.month, t.year, t.hour, t.minute, t.second, user, part);
	dword n;
#ifdef PLATFORM_WIN32
	WriteFile(hfile, h, strlen(h), &n, NULL);
	if(part) {
		sprintf(h, ", #%d", part);
		WriteFile(hfile, h, strlen(h), &n, NULL);
	}
	WriteFile(hfile, "\r\n", 2, &n, NULL);
#else
	write(hfile, h, strlen(h));
	if(part) {
		sprintf(h, ", #%d", part);
		write(hfile, h, strlen(h));
	}
	write(hfile, "\r\n", strlen(h));
#endif
}

void LogStream::Flush()
{
	int count = p - buffer;
	if(count == 0) return;
#ifdef PLATFORM_WIN32
	if(hfile != INVALID_HANDLE_VALUE) {
		dword n;
		WriteFile(hfile, buffer, count, &n, NULL);
	}
#else
	if(hfile >= 0)
		write(hfile, buffer, count);
#endif
	filesize += count;
	p = buffer;
	if(sizelimit > 0 && filesize > sizelimit)
		Create(filename, false);
}

void LogStream::Put0(int w)
{
	*p++ = w;
	if(w == '\n' || p == buffer + 512)
		Flush();
}

void LogStream::_Put(int w)
{
	CriticalSection::Lock __(cs);
	Put0(w);
}

void  LogStream::_Put(const void *data, dword size)
{
	CriticalSection::Lock __(cs);
	const byte *q = (byte *)data;
	while(size--)
		Put0(*q++);
}

bool LogStream::IsOpen() const
{
#ifdef PLATFORM_POSIX
	return hfile >= 0;
#else
	return hfile != INVALID_HANDLE_VALUE;
#endif
}
