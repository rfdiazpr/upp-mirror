#include "uld.h"
#pragma hdrstop
#include "obj.h"

void TryMain()
{
//	PutConsole("ULD::TryMain");
	int start = msecs();
	LinkJob linkjob;
	String cfgfile = LoadFile(ForceExt(GetExeFilePath(), ".cfg"));

#ifndef flagTEST
	if(CommandLine().IsEmpty())
	{
		PutStdOut(LinkJob::Usage());
		return;
	}

	linkjob.ReadCommand(cfgfile, false);
	linkjob.ReadCommand(CommandLine(), true);
	linkjob.Link();
#else
//*
	linkjob.ReadCommand(

"--subsystem "
"windows "
//"-dumpsec:all "
"-M "
"-Bstatic "
"-o "
"e:\\temp\\AnimatedHello.exe "
"e:/upp.342/bin/../lib/gcc/mingw32/3.4.2/../../../crt2.o "
"e:/upp.342/bin/../lib/gcc/mingw32/3.4.2/crtbegin.o "
"-Le:\\upp.342\\lib "
"-Le:/upp.342/bin/../lib/gcc/mingw32/3.4.2 "
"-Le:/upp.342/bin/../lib/gcc "
"-Le:/upp.342/bin/../lib/gcc/mingw32/3.4.2/../../../../mingw32/lib "
"-Le:/upp.342/bin/../lib/gcc/mingw32/3.4.2/../../.. "
"--gc-sections "
"-O "
"2 "
"e:/temp/uppnew/AnimatedHello/MINGW342.Debug.Debug_full.Gcc.Gui.Main.St.Win32/Hello.o "
"e:/temp/uppnew/CtrlLib/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/CtrlLib.o "
"--start-group "
"e:/temp/uppnew/CtrlLib/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/CtrlLib.a "
"-ladvapi32 "
"-lcomdlg32 "
"-lcomctl32 "
"e:/temp/uppnew/CtrlCore/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/CtrlCore.a "
"e:/temp/uppnew/DocTypes/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/DocTypes.a "
"e:/temp/uppnew/RichText/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/RichText.a "
"-luser32 "
"-lgdi32 "
"e:/temp/uppnew/Draw/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/Draw.a "
"-ladvapi32 "
"-lshell32 "
"-lwinmm "
"-lole32 "
"-loleaut32 "
"-luuid "
"e:/temp/uppnew/Core/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/Core.a "
"e:/temp/uppnew/plugin/png/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/png.a "
"e:/temp/uppnew/Image/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/Image.a "
"e:/temp/uppnew/plugin/z/MINGW342.Debug.Debug_full.Gcc.Gui.St.Win32/z.a "
"--end-group "
"-lstdc++ "
"-lmingw32 "
"-lgcc "
"-lmoldname "
"-lmingwex "
"-lmsvcrt "
"-lgdi32 "
"-lcomdlg32 "
"-luser32 "
"-lkernel32 "
"-ladvapi32 "
"-lshell32 "
"-lmingw32 "
"-lgcc "
"-lmoldname "
"-lmingwex "
"-lmsvcrt "
"e:/upp.342/bin/../lib/gcc/mingw32/3.4.2/crtend.o "

	, true);
	linkjob.Link();
#endif
}

CONSOLE_APP_MAIN
{
/*
	{
		RTIMING("VectorMap");
		VectorMap<int, int> temp;
		for(int i = 0; i < 1000000; i++)
			temp.Add(i, -1);
	}
*/

//	static void *hovno;
//	__asm int 3
//	__asm jmp [hovno]

//	PutConsole("ULD::Main");

	try
	{
		TryMain();
	}
	catch(Exc e)
	{
		PutStdOut(e);
		SetExitCode(1);
	}
}
