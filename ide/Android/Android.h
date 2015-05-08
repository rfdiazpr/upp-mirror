#ifndef _Android_Android_h_
#define _Android_Android_h_

#include <Core/Core.h>

NAMESPACE_UPP

class AndroidSDK {
public:
	AndroidSDK();
	AndroidSDK(const String& path);
	virtual ~AndroidSDK();
	
	// TODO: Method may be too high level, but for testing purpose it should be OK.
	void DeducePathReleatedValues();
	
	bool CheckIntegrity() const;
	
	Vector<String> FindPlatforms() const;
	Vector<String> FindBuildToolsReleases() const;
	Vector<String> FindAVDNames() const;
	String         FindDefaultPlatform() const;
	String         FindDefaultBuildToolsRelease() const;
	
public:
	String BuildToolsDir() const;
	String PlatformsDir() const;
	String ConcreteBuildToolsDir() const;
	String ConcretePlatformDir() const;
	String PlatformToolsDir() const;
	String ToolsDir() const;
	
	String AaptPath() const       { return ConcreteBuildToolsDir() + DIR_SEPS + "aapt" + ExecutableExt(); }
	String DxPath() const         { return ConcreteBuildToolsDir() + DIR_SEPS + "dx" + ExecutableExt(); }
	String ZipalignPath() const   { return ConcreteBuildToolsDir() + DIR_SEPS + "zipalign" + ExecutableExt(); }
	String AndroidJarPath() const { return ConcretePlatformDir() + DIR_SEPS + "android.jar"; }
	String AdbPath() const        { return PlatformToolsDir() + DIR_SEPS + "adb" + ExecutableExt(); }
	String AndroidPath() const    { return ToolsDir() + DIR_SEPS + "android" + ExecutableExt(); }
	String EmulatorPath() const   { return ToolsDir() + DIR_SEPS + "emulator" + ExecutableExt(); }
	
public:
	String GetPath() const              { return this->path; }
	String GetPlatform() const          { return this->platform; }
	String GetBuildToolsRelease() const { return this->buildToolsRelease; }
	
	void SetPath(const String& path)                           { this->path = path; }
	void SetPlatform(const String& platform)                   { this->platform = platform; }
	void SetBuildToolsRelease(const String& buildToolsRelease) { this->buildToolsRelease = buildToolsRelease; }
	
protected:
	String ExecutableExt() const;
	
private:
	String path;
	String platform;
	String buildToolsRelease;
};

class AndroidNDK {
public:
	AndroidNDK();
	AndroidNDK(const String& path);
	virtual ~AndroidNDK();
	
	bool CheckIntegrity() const;
	
public:
	// TODO: MS Windows exe.
	String NdkBuildPath() const { return path + DIR_SEPS + "ndk-build"; }
	
public:
	String GetPath() const { return this->path; }
	
	void SetPath(const String& path) { this->path = path; }
	
private:
	String path;
	
};

class Apk {
public:
	Apk(const String& path, const AndroidSDK& androidSDK);
	virtual ~Apk();
	
	String FindPackageName() const;
	String FindLauchableActivity() const;
	
private:
	String FindValueInAndroidManifest(const String& badge, const String& tag) const;
	String FindBadgeTagValue(const String& badge, const String& tag) const;
	
private:
	String path;
	AndroidSDK sdk;
};

class AndroidManifest {
public:
	AndroidManifest();
	virtual ~AndroidManifest();
	
private:
	
};

END_UPP_NAMESPACE

#endif
