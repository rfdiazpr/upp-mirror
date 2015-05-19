#ifndef _ide_Builders_Android_h_
#define _ide_Builders_Android_h_

#include <Core/Core.h>

NAMESPACE_UPP

class AndroidApplicationMakeFile {
public:
	AndroidApplicationMakeFile();
	virtual ~AndroidApplicationMakeFile();
	
	String ToString() const;
public:
	void SetPlatform(const String& platform);
	void AddArchitecture(const String& architecture);
	
protected:
	void AppendPlatform(String& makeFile) const;
	void AppendArchitectures(String& makeFile) const;
	
private:
	Vector<String> architectures;
	String platform;
};

class AndroidModuleMakeFile : public Moveable<AndroidModuleMakeFile> {
public:
	AndroidModuleMakeFile();
	AndroidModuleMakeFile(const String& name);
	virtual ~AndroidModuleMakeFile();

	void Clear();
	String ToString() const;

public:
	void AddSourceFile(const String& path);
	void AddCppFlag(const String& name, const String& value = "");
	void AddLdLibrary(const String& ldLibrary);
	void AddStaticLibrary(const String& staticLibrary);
	void AddSharedLibrary(const String& sharedLibrary);
	
	String GetName() const { return this->name; }	
	void   SetName(const String& name) { this->name = name; }
	
protected:
	void AppendName(String& makeFile) const;
	void AppendSourceFiles(String& makeFile) const;
	void AppendCppFlags(String& makeFile) const;
	void AppendLdLibraries(String& makeFile) const;
	void AppendStaticLibraries(String& makeFile) const;
	void AppendSharedLibraries(String& makeFile) const;
	
	void AppendStringVector(String& makeFile,
	                        const Vector<String>& vec,
	                        const String& variableName,
	                        const String& variablePrefix = "") const;
	
private:
	String name;
	Vector<String> sourceFiles;
	VectorMap<String, String> cppFlags;
	Vector<String> ldLibraries;
	Vector<String> staticLibraries;
	Vector<String> sharedLibraries;
};

class AndroidMakeFile {
public:
	AndroidMakeFile();
	virtual ~AndroidMakeFile();
	
public:
	bool IsEmpty() const;
	void Clear();
	
	bool HasFooter();
	
	void AddHeader();
	void AddModuleMakeFile(const AndroidModuleMakeFile& moduleMakeFile);
	void AddInclusion(const String& inclusion);
	void AddModuleImport(const String& moduleName);
	
	void UpdateModuleMakeFile(const AndroidModuleMakeFile& moduleMakeFile);
	
	void LoadMakeFile(const String& makeFile);
	
	String ToString() const;
	
protected:
	void AppendHeader(String& makeFile) const;
	void AppendModulesMakeFiles(String& makeFile) const;
	void AppendInclusions(String& makeFile) const;
	void AppendImportedModules(String& makeFile) const;
	
private:
	bool hasHeader;
	Vector<AndroidModuleMakeFile> modulesMakeFile;
	Vector<String> inclusions;
	Vector<String> importedModules;
};

END_UPP_NAMESPACE

#endif
