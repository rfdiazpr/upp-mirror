#include "Android.h"

#include <plugin/pcre/Pcre.h>

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

void AndroidSDK::DeducePathReleatedValues()
{
	platform = FindDefaultPlatform();
	buildToolsRelease = FindDefaultBuildToolsRelease();
}

bool AndroidSDK::CheckIntegrity() const
{
	if(!DirectoryExists(path) || !FileExists(AndroidPath()))
		return false;
	
	return true;
}

Vector<String> AndroidSDK::FindPlatforms() const
{
	Vector<String> platforms;
	
	for(FindFile ff(AppendFileName(PlatformsDir(), "*")); ff; ff.Next()) {
		if(!ff.IsHidden() && ff.IsDirectory())
			platforms.Add(ff.GetName());
	}
	
	return platforms;
}

Vector<String> AndroidSDK::FindBuildToolsReleases() const
{
	Vector<String> buildTools;
	
	for(FindFile ff(AppendFileName(BuildToolsDir(), "*")); ff; ff.Next()) {
		if(!ff.IsHidden() && ff.IsDirectory()) {
			String name = ff.GetName();
			// We definitly want to list only main releases.
			if(RegExp("^[1-9][0-9.]*$").Match(name))
				buildTools.Add(ff.GetName());
		}
	}
	
	return buildTools;
}

Vector<String> AndroidSDK::FindAVDNames() const
{
	Vector<String> names;
	
	String out;
	if(Sys(EmulatorPath() + " -list-avds", out) == 0)
		names = Split(out, '\n');
	
	return names;
}

String AndroidSDK::FindDefaultPlatform() const
{
	Vector<String> platforms = FindPlatforms();
	if(platforms.GetCount()) {
		Sort(platforms);
		return platforms[platforms.GetCount() - 1];
	}
	return "";
}

String AndroidSDK::FindDefaultBuildToolsRelease() const
{
	Vector<String> releases = FindBuildToolsReleases();
	if(releases.GetCount()) {
		Sort(releases);
		return releases[releases.GetCount() - 1];
	}
	return "";
}

String AndroidSDK::BuildToolsDir() const
{
	return path + DIR_SEPS + "build-tools";
}

String AndroidSDK::PlatformsDir() const
{
	return path + DIR_SEPS + "platforms";
}

String AndroidSDK::ConcreteBuildToolsDir() const
{
	return BuildToolsDir() + DIR_SEPS + buildToolsRelease;
}

String AndroidSDK::ConcretePlatformDir() const
{
	return PlatformsDir() + DIR_SEPS + platform;
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
