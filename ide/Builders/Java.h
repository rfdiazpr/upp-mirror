#ifndef _ide_Builders_Java_h_
#define _ide_Builders_Java_h_

#include <Core/Core.h>

NAMESPACE_UPP

class Java {
public:
	static String GetDelimiter();
	
};

class Jdk {
public:
	Jdk();
	Jdk(const String& path);
	virtual ~Jdk();
	
public:
	String GetBinDir() const        { return path + DIR_SEPS + "bin"; }
	
	String GetJavacPath() const     { return GetBinDir() + DIR_SEPS + "javac"; }
	String GetJavahPath() const     { return GetBinDir() + DIR_SEPS + "javah"; }
	String GetJarsignerPath() const { return GetBinDir() + DIR_SEPS + "jarsigner"; }
	String GetKeytoolPath() const   { return GetBinDir() + DIR_SEPS + "keytool"; }
	
	void SetPath(const String& path) { this->path = path; }
	
private:
	String path;
};

END_UPP_NAMESPACE

#endif
