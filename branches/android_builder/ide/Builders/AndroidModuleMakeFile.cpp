#include "Builders.h"

AndroidModuleMakeFile::AndroidModuleMakeFile()
{
	
}

AndroidModuleMakeFile::AndroidModuleMakeFile(const String& name)
{
	this->name = name;
}

AndroidModuleMakeFile::~AndroidModuleMakeFile()
{
	
}

String AndroidModuleMakeFile::ToString() const
{
	String makeFile;
	
	makeFile << "\ninclude $(CLEAR_VARS)\n";
	AppendName(makeFile);
	AppendSourceFiles(makeFile);
	AppendCppFlags(makeFile);
	makeFile << "\ninclude $(BUILD_SHARED_LIBRARY)\n";
	
	return makeFile;
}

void AndroidModuleMakeFile::AddSourceFile(const String& path)
{
	sourceFiles.Add(path);
}

void AndroidModuleMakeFile::AddCppFlag(const String& name, const String& value)
{
	cppFlags.Add(name, value);
}

void AndroidModuleMakeFile::AppendName(String& makeFile) const
{
	makeFile << "LOCAL_MODULE := " << name << "\n";
}

void AndroidModuleMakeFile::AppendSourceFiles(String& makeFile) const
{
	if(!sourceFiles.IsEmpty()) {
		makeFile << "LOCAL_SRC_FILES := ";
		for(int i = 0; i < sourceFiles.GetCount(); i++) {
			makeFile << sourceFiles[i];
			if(i + 1 < sourceFiles.GetCount())
				makeFile << " ";
		}
		makeFile << "\n";
	}
	
}

void AndroidModuleMakeFile::AppendCppFlags(String& makeFile) const
{
	if(!cppFlags.IsEmpty()) {
		makeFile << "LOCAL_CPPFLAGS := ";
		for(int i = 0; i < cppFlags.GetCount(); i++) {
			String value = cppFlags[i];
			makeFile << "-D" << cppFlags.GetKey(i);
			if(!value.IsEmpty())
				makeFile << "=" << value;
			if(i + 1 < sourceFiles.GetCount())
				makeFile << " ";
		}
		makeFile << "\n";
	}
}
