#include "ide.h"

extern FileSel& sSD();

static void sSetFolder(EditField *f)
{
	if(!sSD().ExecuteSelectDir()) return;
	*f <<= ~sSD();
}

void DirSel(EditField& f, FrameRight<Button>& b)
{
	f.AddFrame(b);
	b <<= callback1(&sSetFolder, &f);
	b.SetImage(CtrlImg::smallright()).NoWantFocus();
}

String NormalizePathNN(const String& path)
{
	return IsNull(path) ? path : NormalizePath(path);
}

#ifdef PLATFORM_WIN32
void AutoSetup()
{
	WithAutoSetupLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Ultimate++ methods auto-setup");
	String mw = GetExeDirFile("mingw");
	FindFile ff(mw);
	if(ff && ff.IsDirectory())
		dlg.mingw <<= mw;
	else
		dlg.mingw <<=
			NormalizePathNN(GetWinRegString("Inno Setup: App Path",
		       "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW_is1"));
	dlg.mingwmethod <<= "MINGW";
	dlg.domingw <<= !IsNull(dlg.mingw);


	String vs = GetWinRegString("ProductDir", "SOFTWARE\\Microsoft\\VisualStudio\\7.1\\Setup\\VC");
	dlg.visualcpp <<=
		NormalizePathNN(
			Nvl(
				GetWinRegString("InstallLocation",
	                            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"
	                            "{362882AE-E40A-4435-B214-6420634C401F}"),
	            vs)
	    );
	dlg.visualcppmethod <<= "MSC71";
	dlg.dovisualcpp <<= !IsNull(dlg.visualcpp);

	String sdk = GetWinRegString("InstallationFolder",
	                             "Software\\Microsoft\\Microsoft SDKs\\Windows\\v6.0",
	                             HKEY_CURRENT_USER);
	String bin8;
	if(!IsNull(sdk)) {
		sdk = NormalizePath(sdk);
		dlg.sdk <<= sdk;
		dlg.visualcpp8 <<= bin8 = sdk;
	}
	else {
		dlg.visualcpp8 <<= bin8 = NormalizePathNN(
			GetWinRegString("8.0", "SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7")
		);
		dlg.sdk <<= NormalizePathNN(GetWinRegString("Install Dir", "SOFTWARE\\Microsoft\\MicrosoftSDK\\InstalledSDKs\\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3"));
	}
	dlg.visualcppmethod8 <<= "MSC8";
	dlg.dovisualcpp8 <<= !IsNull(dlg.visualcpp8);

	if(bin8.GetLength() && FileExists(AppendFileName(bin8, "VC\\Bin\\x64\\cl.exe"))) {
		dlg.dovisualcpp8x64 = true;
	}
	dlg.visualcppmethod8x64 <<= "MSC8x64";

	dlg.mysql <<= NormalizePathNN(GetWinRegString("Location", "SOFTWARE\\MySQL AB\\MySQL Server 4.1"));

	String sdl = NormalizePathNN(ConfigFile("SDL-1.2.9"));
	if(DirectoryExists(sdl))
		dlg.sdl <<= sdl;

	Array< FrameRight<Button> > bd;
	DirSel(dlg.mingw, bd.Add());
	DirSel(dlg.visualcpp, bd.Add());
	DirSel(dlg.sdk, bd.Add());
	DirSel(dlg.sdl, bd.Add());
	DirSel(dlg.mysql, bd.Add());
	if(dlg.Run() != IDOK)
		return;

	String dir = GetFileFolder(GetExeFilePath());

	String exe;
	String include;
	String lib;

	sdl = ~dlg.sdl;
	if(!IsNull(sdl)) {
		exe << ";" << AppendFileName(sdl, "lib");
		include << ";" << AppendFileName(sdl, "include");
		lib << ";" << AppendFileName(sdl, "lib");
	}

	String mysql = ~dlg.mysql;
	String mlib;
	if(!IsNull(mysql)) {
		exe << ";" << AppendFileName(mysql, "bin");
		include << ";" << AppendFileName(mysql, "include");
		mlib << ";" << AppendFileName(mysql, "bin");
	}

	String mingw = ~dlg.mingw;
	if(!IsNull(mingw) && dlg.domingw) {
		String m = ~dlg.mingwmethod;
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"GCC\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-O0 -gstabs\";\n"
			"RELEASE_BLITZ = \"1\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O3 -ffunction-sections\";\n" // -funroll-loops ?
			"RELEASE_SIZE_OPTIONS = \"-Os -finline-limit=20 -ffunction-sections\";\n"
			"DEBUGGER = \"gdb\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(AppendFileName(~dlg.mingw, "bin") + exe) + ";\n"
			"INCLUDE = " + AsCString(AppendFileName(~dlg.mingw, "include") + include) + ";\n"
			"LIB = " + AsCString(AppendFileName(~dlg.mingw, "lib") + lib + mlib) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(mysql)) {
		lib << ";" << AppendFileName(mysql, "lib");
	}

	String vs8 = ~dlg.visualcpp8;

	if(!IsNull(vs8) && dlg.dovisualcpp8x64) {
		String vc = AppendFileName(vs8, "Vc");
		String m = ~dlg.visualcppmethod8x64;
		String sdk = ~dlg.sdk;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC8X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs8, "Common7\\Ide") + ';' +
							AppendFileName(vc, "Bin\\x64") + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc, "Lib\\x64") + ';' +
							AppendFileName(sdk, "Lib\\x64") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(vs8) && dlg.dovisualcpp8) {
		String vc = AppendFileName(vs8, "Vc");
		String m = ~dlg.visualcppmethod8;
		String sdk = ~dlg.sdk;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC8\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs8, "Common7\\Ide") + ';' +
							AppendFileName(vc, "Bin") + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc, "Lib") + ';' +
							AppendFileName(sdk, "Lib") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	String vc7 = ~dlg.visualcpp;
	if(PathIsEqual(vc7, vs))
		vs = AppendFileName(GetFileFolder(NormalizePath(vs)), "Common7\\ide") + ";";
	else
		vs.Clear();
	if(!IsNull(vc7) && dlg.dovisualcpp) {
		String m = ~dlg.visualcppmethod;
		String sdk = ~dlg.sdk;
		if(IsNull(sdk))
			sdk = AppendFileName(vc7, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC71\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                vs +
							AppendFileName(vc7, "Bin") + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc7, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc7, "Lib") + ';' +
							AppendFileName(sdk, "Lib") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}
}
#endif

#ifdef PLATFORM_X11

void AutoSetup() {}

#endif

void Ide::AutoSetup()
{
	::AutoSetup();
	SyncBuildMode();
	SetBar();
}
