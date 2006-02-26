#include "ide.h"

Console::Console() {
	wrap_text = true;
	outfile = NULL;
	console = false;
	SetReadOnly();
	NoHorzScrollbar();
	SetColor(PAPER_READONLY, SColorPaper);
}

void Console::LeftDouble(Point p, dword) {
	WhenSelect();
}

void Console::RightDown(Point p, dword)
{
	MenuBar::Execute(WhenBar);
}

static int sAppf(int c) {
	return c >= ' ' || c == '\n' ? c : c == '\t' ? ' ' : 0;
}

void Console::Append(const String& s) {
	if(s.IsEmpty()) return;
	if(console) {
		Puts(s);
		return;
	}
	int l, h;
	GetSelection(l, h);
	if(GetCursor() == GetLength()) l = -1;
	EditPos p = GetEditPos();
	SetEditable();
	MoveTextEnd();
	String t = Filter(s, sAppf);
	if(wrap_text) {
		int mg = sb.GetReducedViewSize().cx / ScreenInfo().GetFontInfo(GetFont()).GetAveWidth();
		for(const char *q = t; *q; q++) {
			if(mg > 2 && GetColumnLine(GetCursor()).x >= mg - 1)
				Paste(ToUnicode("\n\t", CHARSET_WIN1252));
			Paste(String(*q, 1).ToWString());
		}
	}
	else
		Paste(t.ToWString());
	SetReadOnly();
	if(l >= 0) {
		SetEditPos(p);
		SetSelection(l, h);
	}
}

bool Console::Key(dword key, int count) {
	if(key == K_ENTER) {
		WhenSelect();
		return true;
	}
	return LineEdit::Key(key, count);
}

int Console::FlushProcess()
{
	String s;
	if(!process || !process->Read(s))
		return -1;
	if(!s.IsEmpty())
	{
		if(outfile)
			outfile->Put(s);
		if(!quiet)
			Append(s);
	}
	return s.GetLength();
}

int Console::Execute(One<SlaveProcess> p, Stream *out, bool q)
{
	process = p;
	outfile = out;
	quiet = q;
	int sleep = 0;
	while(IsOpen() && process) {
		int f = FlushProcess();
		if(f < 0)
			break;
		sleep = (f ? 0 : min(sleep + 5, 20));
		ProcessEvents();
		Ctrl::GuiSleep(sleep);
	}
	int code = -1;
	if(process) {
		while(process->IsRunning()) {
			int f = FlushProcess();
			if(f < 0)
				break;
			sleep = (f ? 0 : min(sleep + 5, 20));
			Sleep(sleep);
		}
		code = process->GetExitCode();
		process->Kill();
		process.Clear();
	}
	ProcessEvents();
	outfile = NULL;
	return code;
}

int Console::Execute(const char *command, Stream *out, const char *envptr, bool q)
{
	outfile = out;
	try {
		process = StartProcess(command, envptr, REMOTE_TIMEOUT);
	}
	catch(Exc e) {
		ProcessEvents();
		outfile = NULL;
		return Null;
	}
	return Execute(process, out, q);
}

void Console::Kill()
{
	if(process)
		process->Kill();
}
