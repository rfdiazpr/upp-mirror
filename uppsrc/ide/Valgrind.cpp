#include "ide.h"

bool Ide::IsValgrind()
{
	static bool is;
	ONCELOCK {
		is = system("which valgrind") == 0;
	}
	return is;
}

void Ide::Valgrind()
{
	static String ValgrindLogFile = "";
	static bool hasValgrind = true;
	
	// runs valgrind --help to find if the correct option is
	// --log-file-exactly (ubuntu 7.04) or --log-file (ubuntu 8.04)
	if(hasValgrind && ValgrindLogFile == "")
	{
		One<Host> h = CreateHostRunDir();
		String fn = GetTempFileName();
		FileOut f(fn);
		String cmdline;
		if(!IsNull(h->Execute("valgrind --help", f)))
		{
			f.Close();
			String txt = LoadFile(fn);
			RLOG(txt << "MAX");
			DeleteFile(fn);
			if(txt.Find("--log-file-exactly") > 0)
			   ValgrindLogFile = "--log-file-exactly";
			else if (txt.Find("--log-file") > 0)
			   ValgrindLogFile = "--log-file";
			else
				hasValgrind = false;
		}
		else
			hasValgrind = false;
	}
	if(!hasValgrind)
		return;
	
	if(!Build())
		return;
	One<Host> h = CreateHostRunDir();
	h->ChDir(Nvl(rundir, GetFileFolder(target)));
	String cmdline;
	String fn = GetTempFileName();
//	cmdline << "valgrind --xml=yes --num-callers=40 --log-file-exactly=" << fn << ' ';
	cmdline << "valgrind --xml=yes --num-callers=40 " << ValgrindLogFile << "=" << fn << ' ';
	cmdline << '\"' << h->GetHostPath(target) << "\" ";
	cmdline << runarg;
	ConsoleClear();
	PutConsole("Valgrind..");
	if(IsNull(h->Execute(cmdline))) {
		PutConsole("Error executing valgrind");
		return;
	}
	PutConsole("Parsing valgrind output..");
	Sync();
	String txt = LoadFile(fn);
	DeleteFile(fn);
	try {
		XmlParser p(txt);
		while(!p.IsTag())
			p.Skip();
		p.PassTag("valgrindoutput");
		while(!p.End()) {
			if(p.Tag("error")) {
				String hdr = "Error (missing description)";
				String pos;
				Vector<String> ln;
				bool src = false;
				while(!p.End()) {
					if(p.Tag("what")) {
						hdr = p.ReadText();
						p.SkipEnd();
					}
					else
					if(p.Tag("stack")) {
						while(!p.End()) {
							String ip = "?";
							String obj;
							String fn;
							String dir;
							String file;
							String line;
							if(p.Tag("frame")) {
								bool hasdir = false;
								bool hasfile = false;
								bool hasline = false;
								bool haspos = false;
								while(!p.End()) {
									if(p.Tag("ip")) {
										ip = p.ReadText();
										p.SkipEnd();
									}
									else
									if(p.Tag("obj")) {
										obj = p.ReadText();
										p.SkipEnd();
										haspos = true;
									}
									else
									if(p.Tag("fn")) {
										fn = p.ReadText();
										p.SkipEnd();
									}
									else
									if(p.Tag("dir")) {
										dir = p.ReadText();
										p.SkipEnd();
										hasdir = true;
									}
									else
									if(p.Tag("file")) {
										file = p.ReadText();
										p.SkipEnd();
										hasfile = true;
									}
									else
									if(p.Tag("line")) {
										line = p.ReadText();
										p.SkipEnd();
										hasline = true;
									}
									else
										p.Skip();
								}
								src = src || hasline && hasdir && hasfile;
								if(pos.IsEmpty() && haspos)
									pos << fn << ' ' << ip << ' '<< obj;
								if(hasline && hasdir && hasfile)
									ln.Add(AppendFileName(dir, file) + ':' + line);
								else {
									String h;
									h << fn << ' ' << ip << ' ' << obj;
									if(hasdir && hasfile)
										h << AppendFileName(dir, file);
									else
										h << file << ' ';
									h << line;
									ln.Add(h);
								}
							}
							else
								p.Skip();
						}
					}
					else
						p.Skip();
				}
				PutConsole(hdr);
				PutConsole("  " + pos);
				if(src)
					for(int i = 0; i < ln.GetCount(); i++)
						PutConsole("     " + ln[i]);
			}
			else
				p.Skip();
		}
	}
	catch(XmlError) {
		PutConsole("Error parsing valgrind output");
	}
}
