#include "Builders.h"

AndroidMakeFile::AndroidMakeFile()
{
	hasHeader = false;
}

AndroidMakeFile::~AndroidMakeFile()
{
	
}

bool AndroidMakeFile::IsEmpty() const
{
	return ToString().IsEmpty();
}

void AndroidMakeFile::Clear()
{
	this->hasHeader = false;
	this->modulesMakeFile.Clear();
}

void AndroidMakeFile::AddHeader()
{
	this->hasHeader = true;
}

void AndroidMakeFile::AddModuleMakeFile(const AndroidModuleMakeFile& moduleMakeFile)
{
	modulesMakeFile.Add(moduleMakeFile);
}

void AndroidMakeFile::AddInclusion(const String& inclusion)
{
	inclusions.Add(inclusion);
}

void AndroidMakeFile::AddModuleImport(const String& moduleName)
{
	importedModules.Add(moduleName);
}

void AndroidMakeFile::UpdateModuleMakeFile(const AndroidModuleMakeFile& moduleMakeFile)
{
	bool isFound = false;
	for(int i = 0; i < modulesMakeFile.GetCount(); i++) {
		if(modulesMakeFile[i].GetName() == moduleMakeFile.GetName()) {
			modulesMakeFile[i] = moduleMakeFile;
			isFound = true;
			break;
		}
	}
	if(!isFound)
		AddModuleMakeFile(moduleMakeFile);
}

void AndroidMakeFile::LoadMakeFile(const String& makeFile)
{
	
}

String AndroidMakeFile::ToString() const
{
	String makeFile;
	
	if(hasHeader)
		AppendHeader(makeFile);
	AppendModulesMakeFiles(makeFile);
	AppendInclusions(makeFile);
	AppendImportedModules(makeFile);
	
	return makeFile;
}

void AndroidMakeFile::AppendHeader(String& makeFile) const
{
	makeFile << "LOCAL_PATH := $(call my-dir)\n";
}

void AndroidMakeFile::AppendModulesMakeFiles(String& makeFile) const
{
	for(int i = 0; i < modulesMakeFile.GetCount(); i++)
		makeFile << modulesMakeFile[i].ToString();
}

void AndroidMakeFile::AppendInclusions(String& makeFile) const
{
	if(!inclusions.IsEmpty())
		makeFile << "\n";
	for(int i = 0; i < inclusions.GetCount(); i++)
		makeFile << "include $(LOCAL_PATH)" << DIR_SEPS << inclusions[i] << "\n";
}

void AndroidMakeFile::AppendImportedModules(String& makeFile) const
{
	if(!importedModules.IsEmpty())
		makeFile << "\n";
	for(int i = 0; i < importedModules.GetCount(); i++)
		makeFile << "$(call import-module, " << importedModules[i] << ")\n";
}
