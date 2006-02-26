#ifdef PLATFORM_WIN32
String  GetExeDirFile(const char *fp);
String  GetExeFilePath();
#endif

#ifdef PLATFORM_POSIX
String  GetHomeDirFile(const char *fp);
String  GetHomeDirectory();

void    SetExeTitle(const char *argv0);
void    SetHomeDirectory(const char *dir);

const char *GetExeTitleCharPtr();

#endif

String  GetExeTitle();
String  ConfigFile(const char *file);
String  ConfigFile();

const Vector<String>& CommandLine();
const VectorMap<String, String>& Environment();

void    SetExitCode(int code);
int     GetExitCode();

bool    IsMainRunning();

#ifndef flagSO
//void    Main(); // By console application
#endif

void AppExit__();

#ifdef PLATFORM_WIN32

void AppInit__(int argc, const char **argv);
void AppInitEnvironment__();

#define CONSOLE_APP_MAIN \
void ConsoleMainFn_(); \
 \
int main(int argc, char *argv[]) { \
	AppInit__(argc, (const char **)argv); \
	ConsoleMainFn_(); \
	DeleteUsrLog(); \
	AppExit__(); \
	return GetExitCode(); \
} \
 \
void ConsoleMainFn_()

#endif

#ifdef PLATFORM_POSIX

void AppInit__(int argc, const char **argv, const char **envptr);

#define CONSOLE_APP_MAIN \
void ConsoleMainFn_(); \
 \
int main(int argc, const char **argv, const char **envptr) { \
	AppInit__(argc, argv, envptr); \
	ConsoleMainFn_(); \
	DeleteUsrLog(); \
	AppExit__(); \
	return GetExitCode(); \
} \
 \
void ConsoleMainFn_()

#endif

String  GetDataFile(const char *filename);

void    LaunchWebBrowser(const String& url);
