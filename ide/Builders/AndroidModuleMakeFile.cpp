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
	
	makeFile << "include $(CLEAR_VARS)\n";
	AppendName(makeFile);
	AppendSourceFiles(makeFile);
	AppendCppFlags(makeFile);
	AppendStaticLibraries(makeFile);
	AppendSharedLibraries(makeFile);
	makeFile << "include $(BUILD_SHARED_LIBRARY)\n";
	
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

void AndroidModuleMakeFile::AddStaticLibrary(const String& staticLibrary)
{
	staticLibraries.Add(staticLibrary);
}

void AndroidModuleMakeFile::AddSharedLibrary(const String& sharedLibrary)
{
	sharedLibraries.Add(sharedLibrary);
}

void AndroidModuleMakeFile::AppendName(String& makeFile) const
{
	makeFile << "LOCAL_MODULE := " << name << "\n";
}

void AndroidModuleMakeFile::AppendSourceFiles(String& makeFile) const
{
	AppendStringVector(sourceFiles , "LOCAL_SRC_FILES", makeFile);
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

void AndroidModuleMakeFile::AppendStaticLibraries(String& makeFile) const
{
	AppendStringVector(staticLibraries , "LOCAL_STATIC_LIBRARIES", makeFile);
}

void AndroidModuleMakeFile::AppendSharedLibraries(String& makeFile) const
{
	AppendStringVector(sharedLibraries , "LOCAL_SHARED_LIBRARIES", makeFile);
}

void AndroidModuleMakeFile::AppendStringVector(const Vector<String>& vec, 
                                               const String& variableName,
                                               String& makeFile) const
{
	if(!vec.IsEmpty()) {
		makeFile << variableName << " := ";
		for(int i = 0; i < vec.GetCount(); i++) {
			makeFile << vec[i];
			if(i + 1 < vec.GetCount())
				makeFile << " ";
		}
		makeFile << "\n";
	}
}
