#include "Android.h"

NAMESPACE_UPP

AndroidNDK::AndroidNDK()
{
	
}

AndroidNDK::AndroidNDK(const String& path)
{
	this->path = path;
}

AndroidNDK::~AndroidNDK()
{
	
}

bool AndroidNDK::CheckIntegrity() const
{
	if(!DirectoryExists(path) || !FileExists(NdkBuildPath()))
		return false;
	
	return true;
}

END_UPP_NAMESPACE
