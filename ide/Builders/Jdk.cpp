#include "Builders.h"

NAMESPACE_UPP

Jdk::Jdk()
{
	
}

Jdk::Jdk(const String& path)
{
	this->path = path;
}

Jdk::~Jdk()
{
	
}

bool Jdk::Validate() const
{
	if(!FileExists(GetJavacPath())) return false;
	
	return true;
}

END_UPP_NAMESPACE
