#include <TCtrlLib/TCtrlLib.h>
#pragma hdrstop

#ifndef flagIDERW
#define EXETITLE "iw"

//////////////////////////////////////////////////////////////////////

void AppMain()
{
	String cfg = ConfigFile();
	if(!LoadFromFile(Configuration::Main(), cfg))
		Exclamation("Konfiguraèní soubor [* \1" + cfg + "\1] je neplatný.");

	void RunDlgImage();
	RunDlgImage();

	if(!StoreToFile(Configuration::Main(), cfg))
		Exclamation("Nelze aktualizovat konfiguraèní soubor [* \1" + cfg + "\1].");
}
#endif//flagIDERW

//////////////////////////////////////////////////////////////////////
