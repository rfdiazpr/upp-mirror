#include <Core/Core.h>
#include <CppBase/CppBase.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String path = GetDataFile("test.in");
	PPSync(GetFileFolder(path));
	
	Cpp cpp;
	FileIn in(path);
	cpp.Preprocess(path, in, path);
	
	DDUMP(cpp.output);
}
