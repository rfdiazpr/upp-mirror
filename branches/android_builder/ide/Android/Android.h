#ifndef _Android_Android_h_
#define _Android_Android_h_

#include <Core/Core.h>

NAMESPACE_UPP

class AndroidSDK {
public:
	AndroidSDK();
	AndroidSDK(const String& path);
	virtual ~AndroidSDK();
	
	bool CheckIntegrity() const;
	
	Vector<String> GetAVDNames() const;
	
	String BuildToolsDir() const;
	String PlatformDir() const;
	String PlatformToolsDir() const;
	String ToolsDir() const;
	
	// TODO: check if on windows below files contains .exe in their names :(
	String AaptPath() const       { return BuildToolsDir() + DIR_SEPS + "aapt" + ExecutableExt(); }
	String DxPath() const         { return BuildToolsDir() + DIR_SEPS + "dx" + ExecutableExt(); }
	String ZipalignPath() const   { return BuildToolsDir() + DIR_SEPS + "zipalign" + ExecutableExt(); }
	String AndroidJarPath() const { return PlatformDir() + DIR_SEPS + "android.jar"; }
	String AdbPath() const        { return PlatformToolsDir() + DIR_SEPS + "" + ExecutableExt(); }
	String AndroidPath() const    { return ToolsDir() + DIR_SEPS + "android" + ExecutableExt(); }
	String EmulatorPath() const   { return ToolsDir() + DIR_SEPS + "emulator" + ExecutableExt(); }
	
public:
	String GetPath() const { return this->path; }
	
	void SetPath(const String& path) { this->path = path; }
	
protected:
	String ExecutableExt() const;
	
protected:
	String path;
};

END_UPP_NAMESPACE

#endif
