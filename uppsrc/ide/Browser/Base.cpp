#include "Browser.h"

#include <plugin/lz4/lz4.h>

#define LTIMING(x)    RTIMING(x)
#define LLOG(x)
#define LTIMESTOP(x)  RTIMESTOP(x)

#define LDUMP(x)


VectorMap<String, String>        sSrcFile;
ArrayMap<String, SourceFileInfo> source_file;

void SourceFileInfo::Serialize(Stream& s)
{
	s % time % usedmacro % used_macros % namespace_info % using_info % defined_macros;
}

String CodeBaseCacheFile()
{
	return AppendFileName(ConfigFile("cfg"), GetVarsName() + '.' + GetCurrentBuildMethod() + ".codebase");
}

CppBase& CodeBase()
{
	static CppBase b;
	return b;
}

static bool   s_console;

void BrowserScanError(int line, const String& text, int file)
{
	if(s_console)
		PutConsole(String().Cat() << source_file.GetKey(file) << " (" << line << "): " << text);
}

void SerializeCodeBase(Stream& s)
{
	source_file.Serialize(s);
	// TODO: Serialize PP files?
}

void SaveCodeBase(bool force)
{
	RealizeDirectory(ConfigFile("cfg"));
	StringStream ss;
	Store(callback(SerializeCodeBase), ss, 0x00000);
	String data = ss.GetResult();
	String path = CodeBaseCacheFile();
	if(force || data.GetCount() > GetFileLength(path)) {
		SaveFile(path, data); // TODO: LZ4?
	#if 0
		{ LTIMING("LZ4 compress");
		SaveFile(path + ".lz4", LZ4Compress(data)); }
		{ LTIMING("zlib compress");
		SaveFile(path + ".z", ZCompress(data)); }
	#endif
	}
}

void LoadCodeBase()
{
	LoadFromFile(callback(SerializeCodeBase), CodeBaseCacheFile(), 0x00000);
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

void GatherSources(const String& master_path, const String& path_, const String& include_path)
{
	RHITCOUNT("GatherSources");
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
	RTIMING("Gathering files");
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
	return source_file.Put(path);
}

String GetSourceFilePath(int file)
{
	if(file < 0 || file >= source_file.GetCount())
		return Null;
	return source_file.GetKey(file);
}

bool CheckFile(const SourceFileInfo& f, const String& path)
{
	RTIMING("CheckFile");
	if(f.time != FileGetTime(path))
		return false;
	Cpp pp;
	pp.include_path = GetIncludePath();
	FileIn in(path);
	pp.Preprocess(path, in, GetMasterFile(path), true);
	String used_macros = pp.GetUsedMacroValues(f.usedmacro);
	DDUMP(used_macros);
	DDUMP(f.used_macros);
	return f.used_macros == used_macros;
}

static int parse_file_count;
static int keep_file_count;

void UpdateCodeBase(Progress& pi)
{
	BaseInfoSync(pi);

	const Workspace& wspc = GetIdeWorkspace();

	pi.SetText("Checking source files");
	pi.SetTotal(sSrcFile.GetCount());
	pi.SetPos(0);
	Index<int>  keep_file;
	Index<int>  parse_file;
	for(int i = 0; i < sSrcFile.GetCount(); i++) {
		pi.Step();
		String path = sSrcFile.GetKey(i);
		int q = GetSourceFileIndex(path);
		const SourceFileInfo& f = source_file[q];
		DLOG("=========== " << path);
		if(CheckFile(f, path))
			keep_file.Add(q);
		else {
			DLOG("PARSE!");
			parse_file.Add(q);
		}
	}
	
	CppBase& base = CodeBase();

	base.Sweep(keep_file);

	for(int i = 0; i < source_file.GetCount(); i++)
		if(keep_file.Find(i) < 0 && parse_file.Find(i) < 0) {
			LLOG("Unlink " << i);
			source_file.Unlink(i);
		}

	pi.SetTotal(parse_file.GetCount());
	pi.SetPos(0);
	pi.AlignText(ALIGN_LEFT);
	for(int i = 0; i < parse_file.GetCount(); i++) {
		String path = GetSourceFilePath(parse_file[i]);
		pi.SetText(GetFileName(GetFileFolder(path)) + "/" + GetFileName(path));
		pi.Step();
		FileIn fi(path);
		LDUMP(path);
		LDUMP(parse_file[i]);
		ParseSrc(fi, parse_file[i], callback1(BrowserScanError, i), true);
	}
}

bool ParseSrc(Stream& in, int file, Callback2<int, const String&> error, bool do_macros)
{
	String path = GetSourceFilePath(file);
	LLOG("====== Parse " << path);
	Vector<String> pp;
	String ext = ToLower(GetFileExt(path));
	int filetype = FILE_OTHER;
	SourceFileInfo& sfi = source_file[file];
	String defined_macros;
	bool b = false;
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
		if(do_macros) {
			sfi.usedmacro = cpp.usedmacro.PickKeys();
			LDUMP(sfi.usedmacro);
			sfi.used_macros = cpp.GetUsedMacroValues(sfi.usedmacro);
			LDUMP(sfi.used_macros);
			sfi.time = GetFileTime(path);
			LDUMP(cpp.defined_macros);
			if(sfi.defined_macros != cpp.defined_macros) {
				sfi.defined_macros = cpp.defined_macros;
				b = true;
			}
		}
	}

	for(int i = 0; i < pp.GetCount(); i++) {
		StringStream pin(pp[i]);
		Parse(pin, CodeBase(), file, filetype, error);
	}
	return b;
}

void CodeBaseScanFile(Stream& in, const String& fn, bool check_macros)
{
	LLOG("===== CodeBaseScanFile " << fn);

	PPSync();

	LTIMING("CodeBaseScan");
	
	TimeStop tm;
	int file = GetSourceFileIndex(fn);
	CppBase& base = CodeBase();
	LLOG("Scan2 " << tm);
	base.RemoveFile(file);
	LLOG("Scan3 " << tm);
	SourceFileInfo& f = source_file[file];
	String dm = f.defined_macros;
	bool b = ParseSrc(in, file, CNULL, check_macros);
	LDUMP(dm);
	LDUMP(f.defined_macros);
	LDUMP(check_macros);
	if(f.defined_macros != dm && check_macros)
		SyncCodeBase();
	LLOG("Scan4 " << tm);
	FinishBase();
	LLOG("Scan total " << tm);
	LLOG("---------");
}

void CodeBaseScanFile(const String& fn, bool check_macros)
{
	FileIn in(fn);
	CodeBaseScanFile(in, fn, check_macros);
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
		pi.Title("Parsing source files");
		LoadCodeBase();
		UpdateCodeBase(pi);
		FinishBase();
		SaveCodeBase(parse_file_count > keep_file_count / 4); // Simple heuristic
	}
	start--;
}

void SyncCodeBase()
{
	if(CodeBase().GetCount()) {
		Progress pi;
		pi.Title("Parsing source files");
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
