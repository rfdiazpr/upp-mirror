#include "Builders.h"

AndroidMakeFile::AndroidMakeFile()
{
	
}

AndroidMakeFile::~AndroidMakeFile()
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

void AndroidMakeFile::AddHeader()
{
	makeFile << GenerateHeader();
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
