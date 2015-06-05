#include "Android.h"

#include <plugin/pcre/Pcre.h>

NAMESPACE_UPP

AndroidSDK::AndroidSDK()
{
	
}

AndroidSDK::AndroidSDK(const String& path, bool deduceValues)
{
	this->path = path;
	if(deduceValues)
		DeducePathReleatedValues();
}

AndroidSDK::~AndroidSDK()
{
	
}

void AndroidSDK::DeducePathReleatedValues()
{
	DeducePlatform();
	DeduceBuildToolsRelease();
}

void AndroidSDK::DeducePlatform()
{
	platform = FindDefaultPlatform();
}

void AndroidSDK::DeduceBuildToolsRelease()
{
	buildToolsRelease = FindDefaultBuildToolsRelease();
}

bool AndroidSDK::Validate() const
{
	if(!DirectoryExists(path) || !FileExists(AndroidPath())) return false;
	
	return true;
}

bool AndroidSDK::ValidatePlatform() const
{
	return DirectoryExists(ConcretePlatformDir());
}

bool AndroidSDK::ValidateBuildTools() const
{
	return DirectoryExists(ConcreteBuildToolsDir());
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

END_UPP_NAMESPACE
