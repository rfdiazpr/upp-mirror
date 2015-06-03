#include "Core.h"

void Builder::ChDir(const String& path)
{
	host->ChDir(path);
}

int Builder::Execute(const char *cmdline)
{
	return host->Execute(cmdline);
}

int Builder::Execute(const char *cl, Stream& out)
{
	return host->Execute(cl, out);
}

