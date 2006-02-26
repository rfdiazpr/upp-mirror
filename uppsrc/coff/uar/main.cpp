#include "uar.h"
#pragma hdrstop

#include "lib.h"

static void TryMain()
{
#ifndef flagTEST
	if(CommandLine().IsEmpty())
	{
		PutStdOut(UARArchiveJob::Usage());
		return;
	}
#endif

	UARArchiveJob archjob;
	archjob.ReadCommand(
#ifdef flagTEST
/*
"-sr \"j:/temp/coff.a\" "
"\"j:/temp/uppnew/coff/MINGW-CONSOLE-DEBUG-DEBUG_FULL-GCC-ST-WIN32/defs.o\" "
"\"j:/temp/uppnew/coff/MINGW-CONSOLE-DEBUG-DEBUG_FULL-GCC-ST-WIN32/util.o\" "
//*/
"-nologo "
"-out:\"j:/temp/coff.lib\" "
"\"j:/temp/uppnew/coff/VC71-CONSOLE-DEBUG-DEBUG_FULL-MSC71-ST-WIN32/defs.obj\" "
"\"j:/temp/uppnew/coff/VC71-CONSOLE-DEBUG-DEBUG_FULL-MSC71-ST-WIN32/util.obj\" "
#else
	CommandLine()
#endif
	);
	archjob.Build();
}

CONSOLE_APP_MAIN
{
	try
	{
		TryMain();
	}
	catch(Exc e)
	{
		puts(e);
		SetExitCode(1);
	}
}
