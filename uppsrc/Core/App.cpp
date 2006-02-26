#include "Core.h"

#ifdef PLATFORM_WIN32
#include <shellapi.h>
#endif

#ifdef PLATFORM_WIN32

String GetExeFilePath()
{
	return FromSystemCharset(GetModuleFileName());
}

String GetExeDirFile(const char *filename)
{
	return AppendFileName(GetFileFolder(GetExeFilePath()), filename);
}

String GetExeTitle()
{
	return GetFileTitle(GetExeFilePath());
}

#endif

#ifdef PLATFORM_POSIX

static char sExeTitle[60] = "upp";

void    SetExeTitle(const char *title) {
	strcpy(sExeTitle, GetFileName(title));
}

String  GetExeTitle() {
	return sExeTitle;
}

const char *GetExeTitleCharPtr()
{
	return sExeTitle;
}

#endif

#ifdef PLATFORM_POSIX

String& sHomeDir() {//!!! Add MT locks
	static String s;
	return s;
}

String  GetHomeDirectory() {
	String& s = sHomeDir();
	if(s.IsEmpty())
		s = FromSystemCharset(getenv("HOME"));
	return s;
}

String  GetHomeDirFile(const char *fp) {
	return AppendFileName(GetHomeDirectory(), fp);
}

void    SetHomeDirectory(const char *dir)
{
	sHomeDir() = dir;
}

#endif//PLATFORM_POSIX


String  ConfigFile(const char *file) {
#if defined(PLATFORM_WIN32)
	return GetExeDirFile(file);
#elif defined(PLATFORM_POSIX)
	String p = GetHomeDirFile("." + GetExeTitle());
	static bool b;
	if(!b) {
		RealizeDirectory(p);
		b = true;
	}
	return AppendFileName(p, file);
#else
#error ConfigFile not implemented for this platform, comment this line to get input string back
	return file;
#endif//PLATFORM
}

String  ConfigFile() {
	return ConfigFile(GetExeTitle() + ".cfg");
}

GLOBAL_VAR(Vector<String>, coreCmdLine__)
const Vector<String>& CommandLine()
{
	return coreCmdLine__();
}

typedef VectorMap<String, String> StringMap;
GLOBAL_VAR(StringMap, coreEnvPtr__)
const VectorMap<String, String>& Environment()
{
	return coreEnvPtr__();
}

static int exitcode;
static bool sMainRunning;

void  SetExitCode(int code) { exitcode = code; }
int   GetExitCode()         { return exitcode; }

bool  IsMainRunning()
{
	return sMainRunning;
}

void LoadLangFiles(const char *dir)
{
	FindFile ff(AppendFileName(dir, "*.tr"));
	while(ff) {
		LoadLngFile(AppendFileName(dir, ff.GetName()));
		ff.Next();
	}
}

void CommonInit()
{
#ifdef PLATFORM_WIN32
	LoadLangFiles(GetFileFolder(GetExeFilePath()));
#else
	LoadLangFiles(GetHomeDirectory());
#endif

	Vector<String>& cmd = coreCmdLine__();
	for(int i = 0; i < cmd.GetCount(); i++)
		if(cmd[i] == "--export-tr") {
			{
				i++;
				int lang = 0;
				byte charset = CHARSET_UTF8;
				String fn = "all";
				if(i < cmd.GetCount())
					if(cmd[i].GetLength() != 4 && cmd[i].GetLength() != 5)
						lang = 0;
					else {
						lang = LNGFromText(cmd[i]);
						fn = cmd[i];
						int c = cmd[i][4];
						if(c >= '0' && c <= '7')
							charset = c - '0' + CHARSET_WIN1250;
						if(c >= 'A' && c <= 'J')
							charset = c - 'A' + CHARSET_ISO8859_1;
					}
				fn << ".tr";
			#ifdef PLATFORM_WIN32
				FileOut out(GetExeDirFile(fn));
			#else
				FileOut out(GetHomeDirFile(fn));
			#endif
				if(lang)
					SaveLngFile(out, SetLNGCharset(lang, charset));
				else {
					Index<int> l = GetLngSet();
					for(int i = 0; i < l.GetCount(); i++)
						SaveLngFile(out, SetLNGCharset(l[i], charset));
				}
			}
			exit(0);
		}
	sMainRunning = true;
}

CriticalSection& sCriticalSectionLock();

#ifdef PLATFORM_POSIX
void AppInit__(int argc, const char **argv, const char **envptr)
{
	SetLanguage(LNG_ENGLISH);
	SetExeTitle(argv[0]);
	for(const char *var; var = *envptr; envptr++)
	{
		const char *b = var;
		while(*var && *var != '=')
			var++;
		String varname(b, var);
		if(*var == '=')
			var++;
		coreEnvPtr__().Add(varname, var);
	}
	Vector<String>& cmd = coreCmdLine__();
	for(int i = 1; i < argc; i++)
		cmd.Add(argv[i]);
	sCriticalSectionLock();
	CommonInit();
}
#endif

#if defined(PLATFORM_WIN32)

void AppInitEnvironment__()
{
	char *env = GetEnvironmentStrings();
	for(char *ptr = env; *ptr; ptr++)
	{
		const char *b = ptr;
		if(*ptr)
			ptr++;
		while(*ptr && *ptr != '=')
			ptr++;
		String varname(b, ptr);
		if(*ptr)
			ptr++;
		b = ptr;
		while(*ptr)
			ptr++;
		coreEnvPtr__().GetAdd(ToUpper(varname)) = String(b, ptr);
	}
	FreeEnvironmentStrings(env);
	sCriticalSectionLock();
	CommonInit();
}

void AppInit__(int argc, const char **argv)
{
	SetLanguage(LNG_ENGLISH);
	Vector<String>& cmd = coreCmdLine__();
	for(int i = 1; i < argc; i++)
		cmd.Add(argv[i]);
	AppInitEnvironment__();
}
#endif

void AppExit__()
{
	sMainRunning = false;
}

void    LaunchWebBrowser(const String& url)
{
#ifdef PLATFORM_WIN32
	ShellExecute(NULL, "open", url, NULL, ".", SW_SHOWDEFAULT);
#endif
#ifdef PLATFORM_POSIX
	system("htmlview " + url + "&");
#endif
}

String GetDataFile(const char *filename)
{
	String s = Environment().Get("UPP_MAIN__",
#ifdef PLATFORM_WIN32
	                             GetFileFolder(GetExeFilePath())
#endif
#ifdef PLATFORM_POSIX
	                             GetHomeDirectory()
#endif
	);

	return AppendFileName(s, filename);
}
