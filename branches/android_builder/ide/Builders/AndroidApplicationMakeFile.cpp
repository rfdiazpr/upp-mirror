#include "Android.h"

NAMESPACE_UPP

AndroidApplicationMakeFile::AndroidApplicationMakeFile()
{
	
}

AndroidApplicationMakeFile::~AndroidApplicationMakeFile()
{
	
}

String AndroidApplicationMakeFile::ToString() const
{
	String makeFile;
	
	AppendPlatform(makeFile);
	AppendArchitectures(makeFile);
	
	return makeFile;
}

void AndroidApplicationMakeFile::SetPlatform(const String& platform)
{
	this->platform = platform;
}

void AndroidApplicationMakeFile::SetArchitectures(const Index<String>& architectures)
{
	this->architectures = clone(architectures);
}

void AndroidApplicationMakeFile::AddArchitecture(const String& architecture)
{
	architectures.Add(architecture);
}

void AndroidApplicationMakeFile::AppendPlatform(String& makeFile) const
{
	if(!platform.IsEmpty())
		makeFile << "APP_PLATFORM := " << platform << "\n";
}

void AndroidApplicationMakeFile::AppendArchitectures(String& makeFile) const
{
	if(architectures.IsEmpty())
		return;
	
	makeFile << "APP_ABI := ";
	for(int i = 0; i < architectures.GetCount(); i++) {
		makeFile << architectures[i];
		if(i + 1 < architectures.GetCount())
			makeFile << " ";
	}
	makeFile << "\n";
}

END_UPP_NAMESPACE
