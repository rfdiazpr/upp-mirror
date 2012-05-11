#ifndef _FSMon_FSMon_h
#define _FSMon_FSMon_h

#include <Core/Core.h>

#ifdef flagGUI
#include <CtrlLib/CtrlLib.h>
#endif

NAMESPACE_UPP

class FSMon
{
	public:
		// flags stating type of changes
		typedef enum
		{
			FSM_NOP				= 0x00,
			FSM_Created			= 0x01,
			FSM_Deleted			= 0x02,
			FSM_FolderCreated	= 0x04,
			FSM_FolderDeleted	= 0x08,
			FSM_Modified		= 0x10,
			FSM_Moved			= 0x20,
			FSM_AttribChange	= 0x40
		} Flags;

		struct Info :  public Moveable<Info>
		{
			// path of changed file
			String path;
			
			// new file path for renamed/moved files
			String newPath;
			
			// type of change flags
			dword flags;
		};
	
	private:

#ifdef PLATFORM_WIN32

		//	Enable/Disable process privileges.
		BOOL EnablePrivilege(LPCTSTR pszPrivName, BOOL fEnable);

		// struct containing info needed for ReadDirectoryChangesW
		#define READ_DIR_CHANGE_BUFFER_SIZE	8192
		struct CHANGESINFO
		{
			HANDLE hDir;
			OVERLAPPED overlapped;
			byte buffer[READ_DIR_CHANGE_BUFFER_SIZE];
		};
		Array<CHANGESINFO>monitoredInfo;
		
		// error code
		DWORD errCode;

		// get error string from code
		String GetErrorStr(HRESULT err);

		// completion port for async I/O
		HANDLE completionPort;
		
		// keys for watched folders -- need to be passed to completion port
		ULONG lastDescriptor;
		
		// scans result buffer for FILE_NOTIFY_INFORMATION records
		// and process them
		void ProcessNotify(FILE_NOTIFY_INFORMATION *buf, String const &path);

#else	
		// error code
		int errCode;
		
		// get error string from code
		String GetErrorStr(int err);
		
		int iNotifyHandle;

		// pending-move descriptor
		// a IN_MOVE_FROM should be followed by IN_MOVE_TO if renaming
		// or moving into some other watched folder
		// otherwise we consider it as a DELETE operation
		// still not sure if some other event can happen between both
		// if so, it would be quite difficult to differentiate between moves and deletes
		uint32_t pendingMoveCookie;
		String pendingMovePath;
		
		// scans a newly created folder to look for files
		// being created BEFORE notify handler was in place
		void ScanCreatedFolder(String path);
		
		// recursively add or remove monitors for paths
		bool AddWatch(String const &path);
		bool RemoveWatch(String const &path);

		// event handling selector
		void EventsSelector(uint32 mask, uint32 cookie, String const &name);

#endif

		// flag stating that we want to be notified only on file close
		// this spares many notifications and the caveats of working on
		// opened file, but don't work for files always opened
		// (example : big database files)
		bool notifyOnClose;

		// error string
		String errMsg;
		
		// array of errors paths and codes filled by
		// recursive watcher -- if can be useful to
		// allow some errors when watching
		VectorMap<String, int> errMap;
	
		// mutex for thread locking
		Mutex fsmMutex;
		
		// a secondary mutex, used for locking on folder creation
		// this one avoids waiting when not needed
		Mutex fsmMutex2;
		
		// the checking thread
		Thread fsmThread;
	
		// changed files/folders list
		Vector<Info> changed;
		
		// monitored paths and descriptors
		// separated for quick access of both (instead of a single map..)
		Index<String> monitoredPaths;
		Index<int> monitoredDescriptors;
		
		// actually opened files -- may be handy
		// for a sync application and for locking purposes
		Index<String>openedFiles;
		
		// sets error code message from errno
		void SetError(int err) { errCode = err; errMsg = GetErrorStr(err); }
		
		// monitoring callback (runs in a separate thread)
		volatile bool shutDown;
		void monitorCb(void);
		
		// callback to call event handler in maint thread
		// (via PostCallback) when using GUI
		void callHandlerCb(void);
		
	protected:
	
	public:
	
		typedef FSMon CLASSNAME;
	
		// constructor
		FSMon(bool notifyOnClose = false);
		
		// destructor
		~FSMon();
	
		// add a monitored path
		bool Add(String const &path);
		
		// remove a monitored path
		bool Remove(String const &path);
		
		// query for changed files/folders
		Vector<Info> GetChanged(void);
		
		// gets actually opened files
		Index<String>GetOpenedFiles(void);
		
		// check error contidion and get error message
		bool IsError(void) { return errCode != 0; }
		bool GetErrorCode(void) { return errCode; }
		String GetErrorMsg(void) { return errMsg; }
		VectorMap<String, int> GetErrorMap(void);
		
		// callback to signal happened event
		// avoids polling, if needed
		Callback EventHandler;
};

END_UPP_NAMESPACE

#endif
