bool PatternMatch(const char *p, const char *s);
bool PatternMatchMulti(const char *p, const char *s);

const char  *GetFileNamePos(const char *fileName);
const char  *GetFileExtPos(const char *fileName);

bool    HasFileName(const char *fp);
bool    HasFileExt(const char *fp);
bool    HasWildcards(const char *fp);
bool    IsFullPath(const char *fp);

String  GetFileDirectory(const char *fp); // with DIR_SEP at the end
String  GetFileFolder(const char *fileName); // without DIR_SEP at the end, if not Win32 root
String  GetFileTitle(const char *fp);
String  GetFileExt(const char *fp);
String  GetFileName(const char *fp);

String  AppendFileName(const String& path, const char *fp);

bool PathIsEqual(const char *p1, const char *p2);

int     ComparePath(const char *a, const char *b, int length);
int     ComparePath(String fa, String fb);

inline bool LessPath(String fa, String fb) { return ComparePath(fa, fb) < 0; }

String WinPath(const char *path);
String UnixPath(const char *path);

#ifdef  PLATFORM_WIN32
inline String  NativePath(const char *path) { return WinPath(path); }
#endif

#ifdef  PLATFORM_POSIX
inline String  NativePath(const char *path) { return UnixPath(path); }
#endif

String  AppendExt(const char *fn, const char *ext);
String  ForceExt(const char *fn, const char *ext);

String  GetFileOnPath(const char *file, const char *paths, bool current = true, const char *curdir = NULL);

String  GetFullPath(const char *path);
String  GetCurrentDirectory();

struct FileTime;

int Compare_FileTime(const FileTime& fa, const FileTime& fb);

#ifdef PLATFORM_WIN32

struct FileTime : FILETIME, CompareRelOps<const FileTime&, &Compare_FileTime> {
	FileTime()                          {}
	FileTime(const FILETIME& ft)        { dwLowDateTime = ft.dwLowDateTime;
	                                      dwHighDateTime = ft.dwHighDateTime; }
};

class  FindFile : public WIN32_FIND_DATA {
protected:
	HANDLE      handle;

public:
	bool        Search(const char *name);
	bool        Next();
	void        Close();

	dword       GetAttributes() const    { return dwFileAttributes; }
	String      GetName() const;
	int64       GetLength() const        { return (int64)nFileSizeLow | ((int64)nFileSizeHigh << 32); }
	FileTime    GetCreationTime() const  { return ftCreationTime; }
	FileTime    GetLastAccessTime() const{ return ftLastAccessTime; }
	FileTime    GetLastWriteTime() const { return ftLastWriteTime; }
	const char *GetMSDOSName() const     { return cAlternateFileName; }

	bool        IsDirectory() const      { return dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY; }
	bool        IsFolder() const;
	bool        IsFile() const           { return !IsDirectory(); }
	bool        IsArchive() const        { return dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE; }
	bool        IsCompressed() const     { return dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED; }
	bool        IsHidden() const         { return dwFileAttributes & FILE_ATTRIBUTE_HIDDEN; }
	bool        IsReadOnly() const       { return dwFileAttributes & FILE_ATTRIBUTE_READONLY; }
	bool        IsSystem() const         { return dwFileAttributes & FILE_ATTRIBUTE_SYSTEM; }
	bool        IsTemporary() const      { return dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY; }

	operator    bool() const             { return handle != INVALID_HANDLE_VALUE; }

	FindFile()                           { handle = INVALID_HANDLE_VALUE; }
	FindFile(const char *name);
	~FindFile()                          { Close(); }
};

#endif

#ifdef PLATFORM_POSIX

struct FileTime : CompareRelOps<const FileTime&, &Compare_FileTime>
{
	FileTime() {}
	FileTime(time_t ft) : ft(ft) {}

	operator time_t () const { return ft; }

	time_t ft;
};

inline int Compare_FileTime(const FileTime& f, const FileTime& g) { return f.ft < g.ft ? -1 : f.ft > g.ft ? 1 : 0; }

class FindFile {
	bool           file;
	DIR           *dir;
	mutable bool   statis;
	mutable struct stat statf;
	String         path;
	String         name;
	String         pattern;

	struct stat &Stat() const;

public:
	bool        Search(const char *name);
	bool        Next();
	void        Close();

	dword       GetMode() const           { return Stat().st_mode; }
	String      GetName() const           { return name; }
	int64       GetLength() const         { return Stat().st_size; } // TRC: todo: 64-bit file size in Linux
	FileTime    GetLastChangeTime() const { return Stat().st_ctime; }
	FileTime    GetLastAccessTime() const { return Stat().st_atime; }
	FileTime    GetLastWriteTime() const  { return Stat().st_mtime; }

	bool        IsReadOnly() const        { return !(GetMode() & (S_IWUSR|S_IWGRP|S_IWOTH)); }

	bool        IsDirectory() const       { return S_ISDIR(GetMode()); }
	bool        IsFolder() const;

	bool        IsFile() const            { return S_ISREG(GetMode()); }
	bool        IsSymLink() const         { return S_ISLNK(GetMode()); }

	operator    bool() const              { return file; }

	FindFile()                            { file = false; dir = NULL; }
	FindFile(const char *name);
	~FindFile()                           { Close(); }
};

// POSIX FileTime is unfortunately long int and clashes with Date::operator int()
inline bool operator==(Time a, FileTime b) { return a == Time(b); }
inline bool operator!=(Time a, FileTime b) { return a != Time(b); }

inline bool operator==(FileTime a, Time b) { return Time(a) == b; }
inline bool operator!=(FileTime a, Time b) { return Time(a) != b; }

#endif

int64       GetFileLength(const char *name);
bool        FileExists(const char *name);
bool        IsFolder(String path);

String      NormalizePath(const char *path);
String      NormalizePath(const char *path, const char *currdir);

bool        FileCopy(const char *oldname, const char *newname);
bool        FileMove(const char *oldname, const char *newname);
bool        FileDelete(const char *filename);

bool        DirectoryExists(const char *name);
#ifdef PLATFORM_POSIX
bool        DirectoryCreate(const char *dirname, int mode = 0755);
#else
bool        DirectoryCreate(const char *dirname);
#endif
bool        DirectoryDelete(const char *dirname);
void        RealizeDirectory(String dir);
void        RealizePath(String file);

struct Time;
Time        FileGetTime(const char *filename);
bool        SetFileTime(const char *filename, FileTime ft);
bool        FileSetTime(const char *filename, Time time);
FileTime    TimeToFileTime(Time time);

#ifdef PLATFORM_POSIX
inline bool DeleteFile(const char *fn)      { return unlink(fn) == 0; }
#endif

bool    DeleteFolderDeep(const char *dir);

String  GetTempPath();
String  GetTempFileName(const char *prefix = NULL);

template <class T>
class Array;

class FileSystemInfo
{
public:
	enum
	{
		ROOT_UNKNOWN     = 0,
		ROOT_NO_ROOT_DIR = 1,
		ROOT_REMOVABLE   = 2,
		ROOT_FIXED       = 3,
		ROOT_REMOTE      = 4,
		ROOT_CDROM       = 5,
		ROOT_RAMDISK     = 6,
		ROOT_NETWORK     = 7,
		ROOT_COMPUTER    = 8,
	};

	enum
	{
		STYLE_WIN32      = 0x0001,
		STYLE_POSIX      = 0x0002,
	};

	struct FileInfo
	{
		FileInfo();

		operator bool () const { return !IsNull(filename); }

		String filename;
		String msdos_name;
		String root_desc;
		int64  length;
		Time   last_access_time;
		Time   last_write_time;
		Time   creation_time;
		bool   read_only;
		bool   is_directory;
		bool   is_folder;
		bool   is_file;
		bool   is_symlink;
		bool   is_archive;
		bool   is_compressed;
		bool   is_hidden;
		bool   is_read_only;
		bool   is_system;
		bool   is_temporary;
		char   root_style;
	};

	virtual int             GetStyle() const;
	bool                    IsWin32() const { return GetStyle() & STYLE_WIN32; }
	bool                    IsPosix() const { return GetStyle() & STYLE_POSIX; }

	virtual Array<FileInfo> Find(String mask, int max_count = 1000000) const; // mask = Null -> root
	virtual bool            CreateFolder(String path, String& error) const;

	bool                    FolderExists(String path) const;
};

FileSystemInfo& StdFileSystemInfo();
