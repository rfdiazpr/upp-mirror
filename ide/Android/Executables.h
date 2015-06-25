#ifndef _ide_Android_Executable_h_
#define _ide_Android_Executable_h_

#include <Core/Core.h>

#include <ide/Core/Core.h>

NAMESPACE_UPP

class NDKBuild {
public:
	NDKBuild(const String& path);
	virtual ~NDKBuild();
	
	void SetJobs(int jobs)                       { this->jobs = jobs; }
	void SetWorkingDir(const String& workingDir) { this->workingDir = workingDir; }
	
	String ToString() const;
	
private:
	String path;
	String workingDir;
	int    jobs;
};

END_UPP_NAMESPACE

#endif
