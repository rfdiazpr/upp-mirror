#include "Builders.h"

AndroidMakeFile::AndroidMakeFile()
{
	
}

bool AndroidMakeFile::IsEmpty() const
{
	return makeFile.IsEmpty();
}

void AndroidMakeFile::Clear()
{
	makeFile.Clear();
}

bool AndroidMakeFile::HasFooter()
{
	String footer = GenerateFooter();
	int makeFileCount = makeFile.GetCount();
	int footerCount = footer.GetCount();
	
	if(makeFileCount >= footerCount)
		return makeFile.Mid(makeFileCount - footerCount).Compare(footer) == 0;
	
	return false;
}

void AndroidMakeFile::AddHeader()
{
	makeFile << GenerateHeader();
}

void AndroidMakeFile::AddFooter()
{
	makeFile << GenerateFooter();
}

void AndroidMakeFile::AddPackageMakeFile(String packageMakeFile)
{
	makeFile << packageMakeFile;
}

String AndroidMakeFile::ToString() const
{
	return makeFile;
}

String AndroidMakeFile::GenerateHeader()
{
	String header;
	
	header << "LOCAL_PATH := $(call my-dir)\n\n";
	
	return header;
}

String AndroidMakeFile::GenerateFooter()
{
	String footer;
	
	footer << "\n\ninclude $(BUILD_SHARED_LIBRARY)\n";
	
	return footer;
}
