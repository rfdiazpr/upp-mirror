#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	const Vector<String>& cmdline = CommandLine();
	int i;
	for(i = 0; i < cmdline.GetCount(); i++)
		RLOG("Updater/WinMain, cmdline[" << i << "] = " << cmdline[i]);
	SetDefaultCharset(CHARSET_WIN1250);
	if(cmdline.IsEmpty())
	{
		Exclamation("[* UPDATER] should be run from another applications");
		return;
	}
	String name = cmdline[0];
	RLOG("Updater: UpdateFile = " << name);
	UpdateFile(name);
	String exec = GetExeDirFile(name);
	for(i = 1; i < cmdline.GetCount(); i++)
		if(cmdline[i].Find(' ') >= 0)
			exec << " \"" << cmdline[i] << "\"";
		else
			exec << " " << cmdline[i];
	RLOG("Updater: WinExec = " << exec);
	WinExec(exec, SW_SHOWNORMAL);
}
