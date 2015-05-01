#include "Android.h"

NAMESPACE_UPP

AndroidSDK::AndroidSDK()
{
	
}

AndroidSDK::AndroidSDK(const String& path)
{
	this->path = path;
}

AndroidSDK::~AndroidSDK()
{
	
}

bool AndroidSDK::CheckIntegrity() const
{
	if(!DirectoryExists(path) || !FileExists(AndroidPath()))
		return false;
	
	return true;
}

Vector<String> AndroidSDK::GetAVDNames() const
{
	Vector<String> names;
	
	String out;
	if(Sys(EmulatorPath() + " -list-avds", out) == 0)
		names = Split(out, '\n');
	
	return names;
}

String AndroidSDK::BuildToolsDir() const
{
	// TODO: add build tools detection.
	return path + DIR_SEPS + "build-tools" + DIR_SEPS + "19.1.0";
}

String AndroidSDK::PlatformDir() const
{
	// TODO: add platform detection.
	return path + DIR_SEPS + "platforms" + DIR_SEPS + "android-22";
}

String AndroidSDK::PlatformToolsDir() const
{
	return path + DIR_SEPS + "platform-tools";
}

String AndroidSDK::ToolsDir() const
{
	return path + DIR_SEPS + "tools";
}

String AndroidSDK::ExecutableExt() const
{
// TODO: do we have any core function/method to do this???
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ".exe";
#else
	return "";
#endif
}

END_UPP_NAMESPACE
