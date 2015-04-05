#include "Browser.h"

#define LTIMING(x)    RTIMING(x)
#define LLOG(x)
#define LTIMESTOP(x)  RTIMESTOP(x)

static const char s_dbver[] = "CPP-BASE 3.00";

void GC_Cache()
{
	String cfg = ConfigFile("cfg");
	FindFile ff(AppendFileName(cfg, "*.cache"));
	Time tm0 = ToTime(GetSysDate() - 20);
	while(ff) {
		if(ff.IsFile() && Time(ff.GetLastWriteTime()) > tm0)
			FileDelete(AppendFileName(cfg, ff.GetName()));
		ff.Next();
	}
}

String CacheFile(const String& res)
{
	return AppendFileName(ConfigFile("cfg"), MD5String(res) + ".cache");
}

CppBase& CodeBase()
{
	static CppBase b;
	return b;
}

ArrayMap<String, SourceFileInfo> source_file;

static bool   s_console;

void BrowserScanError(int line, const String& text, int file)
{
	if(s_console)
		PutConsole(String().Cat() << source_file.GetKey(file) << " (" << line << "): " << text);
}

void SaveCodeBase()
{
	return;
#if 0
	LTIMING("SaveCodeBase");
	RealizeDirectory(ConfigFile("cfg"));
	GC_Cache();
	CppBase& base = CodeBase();
	if(base.GetCount() == 0)
		return;
	ArrayMap<String, BrowserFileInfo>& fileset = FileSet();
	ArrayMap<String, StringStream> out;
	String s;
	for(int j = 0; j < base.GetCount(); j++) {
		Array<CppItem>& n = base[j];
		for(int k = 0; k < n.GetCount(); k++) {
			CppItem& m = n[k];
			StringStream& o = out.GetAdd(GetSourceFilePath(m.file));
			s = base.GetKey(j);
			o % s;
			o % m;
		}
	}

	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		String package = wspc[i];
		FileOut fo(CacheFile(package));
		s = s_dbver;
		fo % s;
		for(int i = 0; i < fileset.GetCount(); i++)
			if(fileset[i].package == package) {
				String fn = fileset.GetKey(i);
				String q = out.GetAdd(fn).GetResult();
				BrowserFileInfo& f = fileset[i];
				fo % fn;
				fo % f.file;
				fo % f.time;
				fo % q;
			}
	}
#endif
}

void LoadCodeBase(Progress& pi)
{
	return;
#if 0
	LTIMING("LoadCodeBase");
	CppBase& base = CodeBase();
	ArrayMap<String, BrowserFileInfo>& fileset = FileSet();
	String s;
	const Workspace& wspc = GetIdeWorkspace();
	pi.SetText("Assist++ loading packages");
	pi.SetTotal(wspc.GetCount());
	pi.SetPos(0);
	for(int i = 0; i < wspc.GetCount(); i++) {
		pi.Step();
		String package = wspc[i];
		FileIn in(CacheFile(package));
		in.LoadThrowing();
		if(in) {
			try {
				in % s;
				if(s != s_dbver)
					throw LoadingError();
				while(!in.IsEof()) {
					String fn;
					in % fn;
					BrowserFileInfo& f = fileset.GetAdd(fn);
					in % f.file;
					in % f.time;
					f.package = package;
					String q;
					in % q;
					StringStream ss(q);
					ss.LoadThrowing();
					while(!ss.IsEof()) {
						String s;
						ss % s;
						CppItem& m = base.GetAdd(s).Add();
						ss % m;
						m.file = GetSourceFileIndex(fn);
					}
				}
			}
			catch(LoadingError) {
				CodeBase().Clear();
				fileset.Clear();
				return;
			}
		}
	}
#endif
}

void FinishBase()
{
	Qualify(CodeBase());
}

void ReQualifyCodeBase()
{
	Qualify(CodeBase());
}

Vector<String> SortedNests()
{
	LTIMING("SortedNests()");
	CppBase& base = CodeBase();
	Vector<String> n;
	for(int i = 0; i < base.GetCount(); i++)
		if(!base.IsUnlinked(i))
			n.Add(base.GetKey(i));
	Sort(n);
	return n;
}

String GetIncludePath()
{
	return TheIde() ? TheIde()->IdeGetIncludePath() : Null;
}

bool IsCPPFile(const String& path)
{
	return findarg(ToLower(GetFileExt(path)) , ".c", ".cpp", ".cc" , ".cxx", ".icpp") >= 0;
}

int  GetMatchLen(const String& a, const String& b)
{
	LTIMING("GetMatchLen");
	int l = min(a.GetLength(), b.GetLength());
	for(int i = 0; i < l; i++)
		if(a[i] != b[i])
			return i;
	return l;
}

VectorMap<String, String> sSrcFile;

void GatherSources(const String& master_path, const String& path_, const String& include_path)
{
	String path = NormalizePath(path_);
	if(sSrcFile.Find(path) >= 0)
		return;
	sSrcFile.Add(path, master_path);
	const PPFile& f = GetPPFile(path);
	for(int i = 0; i < f.includes.GetCount(); i++) {
		String p = GetIncludePath(f.includes[i], GetFileFolder(path), include_path);
		if(p.GetCount())
			GatherSources(master_path, p, include_path);
	}
}

void BaseInfoSync(Progress& pi)
{ // clears temporary caches (file times etc..)
	PPSync();

	LTIMING("sSrcFile make");
	
	sSrcFile.Clear();
	const Workspace& wspc = GetIdeWorkspace();
	pi.SetText("Gathering files");
	pi.SetTotal(wspc.GetCount());
	for(int i = 0; i < wspc.GetCount(); i++) {
		pi.Step();
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(int i = 0; i < pk.file.GetCount(); i++) {
			String path = SourcePath(n, pk.file[i]);
			if(IsCPPFile(path))
				GatherSources(path, path, GetIncludePath());
		}
	}
}

String GetMasterFile(const String& file)
{
	return sSrcFile.Get(file, Null);
}

int GetSourceFileIndex(const String& path)
{
	int q = source_file.Find(path);
	if(q < 0) {
		q = source_file.GetCount();
		source_file.Add(path);
	}
	return q;
}

String GetSourceFilePath(int file)
{
	if(file < 0 || file >= source_file.GetCount())
		return Null;
	return source_file.GetKey(file);
}

void UpdateCodeBase(Progress& pi)
{
	BaseInfoSync(pi);

	const Workspace& wspc = GetIdeWorkspace();

	pi.SetText("Assist++ checking files");
	pi.SetTotal(sSrcFile.GetCount());
	pi.SetPos(0);
	Index<int>  keep_file;
	Index<int>  scan_file;
	for(int i = 0; i < sSrcFile.GetCount(); i++) {
		pi.Step();
		String path = sSrcFile.GetKey(i);
		int q = GetSourceFileIndex(path);
		SourceFileInfo& f = source_file[q];
		if(f.time == FileGetTime(path))
			keep_file.Add(q);
		else
			scan_file.Add(q);
	}
	
	CppBase& base = CodeBase();

	base.Sweep(keep_file);

	for(int i = 0; i < source_file.GetCount(); i++)
		if(keep_file.Find(i) < 0 && scan_file.Find(i) < 0)
			source_file.Unlink(i);
	source_file.Sweep();

	pi.SetTotal(scan_file.GetCount());
	pi.SetPos(0);
	pi.AlignText(ALIGN_LEFT);
	for(int i = 0; i < scan_file.GetCount(); i++) {
		String file = GetSourceFilePath(scan_file[i]);
		pi.SetText(GetFileName(GetFileFolder(file)) + "/" + GetFileName(file));
		pi.Step();
		FileIn fi(file);
		ParseSrc(fi, scan_file[i], callback1(BrowserScanError, i));
	}
}

void ParseSrc(Stream& in, int file, Callback2<int, const String&> error)
{
	String path = GetSourceFilePath(file);
	Vector<String> pp;
	String ext = ToLower(GetFileExt(path));
	int filetype = FILE_OTHER;
	if(ext == ".lay")
		pp.Add(PreprocessLayFile(path));
	else
	if(ext == ".iml")
		pp.Add(PreprocessImlFile(path));
	else
	if(ext == ".sch")
		pp.Append(PreprocessSchFile(path));
	else {
		Cpp cpp;
		cpp.include_path = GetIncludePath();
		cpp.Preprocess(path, in, GetMasterFile(GetSourceFilePath(file)));
		pp.Add(cpp.output);
		filetype = decode(ext, ".h", FILE_H, ".hpp", FILE_HPP,
		                       ".cpp",FILE_CPP, ".c", FILE_C, FILE_OTHER);
	}

	for(int i = 0; i < pp.GetCount(); i++) {
		StringStream pin(pp[i]);
		Parse(pin, CodeBase(), file, filetype, error);
	}
}


void CodeBaseScan(Stream& in, const String& fn)
{
	PPSync();

	LTIMING("CodeBaseScan");
	
	TimeStop tm;
	int file = GetSourceFileIndex(fn);
	CppBase& base = CodeBase();
	LLOG("Scan2 " << tm);
	base.Remove(file);
	LLOG("Scan3 " << tm);
	ParseSrc(in, file, CNULL);
	LLOG("Scan4 " << tm);
	FinishBase();
	LLOG("Scan total " << tm);
	LLOG("---------");
}

void ClearCodeBase()
{
	CodeBase().Clear();
	source_file.Clear();
}

void StartCodeBase()
{
	static int start;
	if(start) return;
	start++;
	if(CodeBase().GetCount() == 0) {
		Progress pi;
		pi.Title("Assist++");
		LoadCodeBase(pi);
		UpdateCodeBase(pi);
		FinishBase();
	}
	start--;
}

void SyncCodeBase()
{
	if(CodeBase().GetCount()) {
		Progress pi;
		pi.Title("Assist++");
		UpdateCodeBase(pi);
		FinishBase();
	}
}

void RescanCodeBase()
{
	ClearCodeBase();
	s_console = true;
	Progress pi;
	pi.Title("Assist++");
	UpdateCodeBase(pi);
	FinishBase();
	s_console = false;
}

bool ExistsBrowserItem(const String& item)
{
	return GetCodeRefItem(item);
}
