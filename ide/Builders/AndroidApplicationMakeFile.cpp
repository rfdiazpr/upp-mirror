#include "Builders.h"

AndroidApplicationMakeFile::AndroidApplicationMakeFile()
{
	
}

AndroidApplicationMakeFile::~AndroidApplicationMakeFile()
{
	
}

String AndroidApplicationMakeFile::ToString() const
{
	String makeFile;
	
	AppendArchitectures(makeFile);
	
	return makeFile;
}

void AndroidApplicationMakeFile::AddArchitecture(const String& architecture)
{
	architectures.Add(architecture);
}

void AndroidApplicationMakeFile::AppendArchitectures(String& makeFile) const
{
	makeFile << "APP_ABI := ";
	if(architectures.IsEmpty())
		makeFile << "all";
	else {
		for(int i = 0; i < architectures.GetCount(); i++) {
			makeFile << architectures[i];
			if(i + 1 < architectures.GetCount())
				makeFile << " ";
		}
	}
	makeFile << "\n";
}
