#include "SystemLog.h"

NAMESPACE_UPP

#ifdef ERROR
#undef ERROR
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// Global log object
SystemLog SysLog;

// gets platfor-dependent log level
#ifdef PLATFORM_POSIX
int SystemLog::platform(Levels level)
#elif defined(PLATFORM_WIN32)
dword SystemLog::platform(Levels level)
#endif
{
#ifdef PLATFORM_POSIX
	if(level & EMERGENCY)	return LOG_EMERG;
	if(level & ALERT)		return LOG_ALERT;
	if(level & CRITICAL)	return LOG_CRIT;
	if(level & ERROR)		return LOG_ERR;
	if(level & WARNING)		return LOG_WARNING;
	if(level & NOTICE)		return LOG_NOTICE;
	if(level & INFO)		return LOG_INFO;
	if(level & DEBUG)		return LOG_DEBUG;
#elif defined(PLATFORM_WIN32)
	if(level & EMERGENCY)	return EVENTLOG_ERROR_TYPE;
	if(level & ALERT)		return EVENTLOG_ERROR_TYPE;
	if(level & CRITICAL)	return EVENTLOG_ERROR_TYPE;
	if(level & ERROR)		return EVENTLOG_ERROR_TYPE;
	if(level & WARNING)		return EVENTLOG_WARNING_TYPE;
	if(level & NOTICE)		return EVENTLOG_INFORMATION_TYPE;
	if(level & INFO)		return EVENTLOG_INFORMATION_TYPE;
	if(level & DEBUG)		return EVENTLOG_INFORMATION_TYPE;
#endif

	// should not happen....
	return 0;
	
} // END SystemLog::platform()

////////////////////////////////////////////////////////////////////////////////////////////////
// constructor
SystemLog::SystemLog()
{
	// log not opened upon creation
	FOpened = false;
	
	// disables syslog, disables console, enables CErr() and enables Upp log output
	FCoutEnabled = false;
	FCerrEnabled = true;
	FSysLogEnabled = false;
	FUppLogEnabled = true;
	
} // END Constructor class SystemLog
		
////////////////////////////////////////////////////////////////////////////////////////////////
// destructor
SystemLog::~SystemLog()
{
	// closes the active log
	Close();
	
} // END Destructor class SystemLog

////////////////////////////////////////////////////////////////////////////////////////////////
// closes the active log
void SystemLog::Close()
{
	if (FOpened)
	{
#ifdef PLATFORM_POSIX
		closelog();
#elif defined(PLATFORM_WIN32)
		DeregisterEventSource(log);
#endif
		FOpened = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// opens and initializes log stream
SystemLog &SystemLog::Open(String const &name, String const &dllPath)
{
	// closes, if already opened
	Close();
	
	// stores the user log name
	FLogName = name;
	
#ifdef PLATFORM_POSIX
	openlog(FLogName, LOG_PID, LOG_USER);
#elif defined(PLATFORM_WIN32)
	String path;
	dword eventTypes = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
	dword categoryCount = 0;
	log = NULL;

	path << "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\" << FLogName;
	if (GetWinRegString("EventMessageFile", path, HKEY_LOCAL_MACHINE) != dllPath)
	{
		if (!SetWinRegExpandString(dllPath, "EventMessageFile", path, HKEY_LOCAL_MACHINE))
			return *this;
		if (!SetWinRegInt(eventTypes, "TypesSupported", path, HKEY_LOCAL_MACHINE))
			return *this;
	}
	log = RegisterEventSource(NULL, FLogName);
	if (log == NULL)
		return *this;
#endif

	// marks log stream as opened
	FOpened = true;

	return *this;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// enable specified level(s)
SystemLog &SystemLog::EnableLevels(byte Levels)
{
	FEnabledLevels |= Levels;
	
	return *this;

} // END SystemLog::EnableLevels()

////////////////////////////////////////////////////////////////////////////////////////////////
// disable specified level(s)
SystemLog &SystemLog::DisableLevels(byte Levels)
{
	FEnabledLevels &= ~Levels;
	
	return *this;
	
} // END SystemLog::DisableLevels()

////////////////////////////////////////////////////////////////////////////////////////////////
// sets enabled lebels to 'Levels'
SystemLog &SystemLog::SetLevels(byte Levels)
{
	FEnabledLevels = Levels;
	
	return *this;
	
} // END SystemLog::SetLevels()

////////////////////////////////////////////////////////////////////////////////////////////////
// gets level name from level
String SystemLog::LevelName(Levels level)
{
	switch(level)
	{
		case EMERGENCY :
			return "EMERGENCY:: ";
		case ALERT :
			return "ALERT    :: ";
		case CRITICAL :
			return "CRITICAL :: ";
		case ERROR :
			return "ERROR   :: ";
		case WARNING :
			return "WARNING  :: ";
		case NOTICE :
			return "NOTICE   :: ";
		case INFO :
			return "INFO     :: ";
		case DEBUG :
			return "DEBUG    :: ";
		default:
			return "UNKNOWN  :: ";
	}
	
} // END SystemLog::LevelName()

////////////////////////////////////////////////////////////////////////////////////////////////
// writes log message at required level
SystemLog &SystemLog::WriteLog(Levels level, String const &message)
{
	// nothing to do if log is not opened
	if(!FOpened)
		return *this;

	// checks if level is enabled
	if(!(FEnabledLevels & level))
		return *this;
	
	String msg = LevelName(level) + message;
	
	// if syslog enabled, logs on syslog
	if(FSysLogEnabled)
	{

#ifdef PLATFORM_POSIX
		syslog(platform(level), msg);
#elif defined(PLATFORM_WIN32)
		const char *array[2];
		array[0] = (const char *)msg;
		if (log == NULL)
			return *this;
		if (!ReportEvent(log, (WORD)platform(level), 0, 0x00000001L, NULL, 1, 0, &array[0], NULL))
			return *this;
#endif
	}

	// if upp log enabled, log on upp app log file
	if(FUppLogEnabled)
		LOG(msg);
	
	// if CErr is enabled and message is an error one, logs to CErr()
	if(FCerrEnabled && (level & ALLERR))
		Cerr() << FLogName << " : " << msg << "\n";
	else if(FCoutEnabled)
		Cout() << FLogName << " : " << msg << "\n";

	return *this;
}

END_UPP_NAMESPACE
