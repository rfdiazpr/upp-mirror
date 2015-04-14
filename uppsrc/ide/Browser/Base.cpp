#include "Browser.h"

#include <plugin/lz4/lz4.h>

#define LTIMING(x)    RTIMING(x)
#define LLOG(x)       // DLOG(x)
#define LTIMESTOP(x)  RTIMESTOP(x)

#define LDUMP(x)      // DDUMP(x)


VectorMap<String, String>        sSrcFile;
ArrayMap<String, SourceFileInfo> source_file;

void SourceFileInfo::Serialize(Stream& s)
{
	s % time % ids % included_id_macros % namespace_info % using_info % defined_macros;
}

String CodeBaseCacheDir()
{
	return ConfigFile("cfg/codebase");
}

void ReduceCodeBaseCache()
{
	struct FileInfo {
		String path;
		Time   time;
		int64  length;
		
		bool operator<(const FileInfo& a) const { return time > a.time; }
	};
	Array<FileInfo> file;
	FindFile ff(AppendFileName(CodeBaseCacheDir(), "*.*"));
	int64 total = 0;
	while(ff) {
		if(ff.IsFile()) {
			FileInfo& m = file.Add();
			m.path = ff.GetPath();
			m.time = ff.GetLastAccessTime();
			m.length = ff.GetLength();
			total += m.length;
		}
		ff.Next();
	}
	Sort(file);
	while(total > 120000000 && file.GetCount()) {
		DeleteFile(file.Top().path);
		total -= file.Top().length;
		file.Drop();
	}
}

String CodeBaseCacheFile()
{
	return AppendFileName(CodeBaseCacheDir(), GetVarsName() + '.' + GetCurrentMainPackage() + '.' + GetCurrentBuildMethod() + ".codebase");
}

CppBase& CodeBase()
{
	static CppBase b;
	return b;
}

static bool   s_console;

void IdePutErrorLine(const String& line);

void BrowserScanError(int line, const String& text, int file)
{
	if(s_console)
		IdePutErrorLine(String().Cat() << source_file.GetKey(file) << " (" << line << "): " << text);
}

void SerializeCodeBase(Stream& s)
{
	source_file.Serialize(s);
	SerializePPFiles(s);
}

void SaveCodeBase()
{
	LTIMING("SaveCodeBase");
	LLOG("Save code base");
	RealizeDirectory(ConfigFile("cfg/codebase"));
	StringStream ss;
	Store(callback(SerializeCodeBase), ss, 1);
	String data = ss.GetResult();
	String path = CodeBaseCacheFile();
	SaveFile(path, ZCompress(data)); // TODO: LZ4?
}

bool TryLoadCodeBase(const char *pattern)
{
	FindFile ff(pattern);
	String path;
	int64  len = -1;
	while(ff) { // Load biggest file, as it has the most chances to have the data we need
		if(ff.IsFile() && ff.GetLength() > len) {
			path = ff.GetPath();
			len = ff.GetLength();
		}
		ff.Next();
	}
	if(path.GetCount()) {
		StringStream ss(ZDecompress(LoadFile(path)));
		if(Load(callback(SerializeCodeBase), ss, 1)) {
			LLOG("Loaded " << ff.GetPath());
			return true;
		}
	}
	return false;
}

void LoadCodeBase()
{
	TryLoadCodeBase(CodeBaseCacheFile()) ||
	TryLoadCodeBase(AppendFileName(CodeBaseCacheDir(), GetVarsName() + ".*." + GetCurrentBuildMethod() + ".codebase")) ||
	TryLoadCodeBase(AppendFileName(CodeBaseCacheDir(), GetVarsName() + ".*.codebase")) ||
	TryLoadCodeBase(AppendFileName(CodeBaseCacheDir(), "*.codebase"));
}

void FinishCodeBase()
{
	RTIMING("FinishBase");

	Qualify(CodeBase());
}

/*
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
*/

void GatherSources(const String& master_path, const String& path_)
{
	RHITCOUNT("GatherSources");
	String path = NormalizePath(path_);
	if(sSrcFile.Find(path) >= 0)
		return;
	sSrcFile.Add(path, master_path);
	const PPFile& f = GetPPFile(path);
	for(int i = 0; i < f.includes.GetCount(); i++) {
		String p = GetIncludePath(f.includes[i], GetFileFolder(path));
		if(p.GetCount())
			GatherSources(master_path, p);
	}
}

void BaseInfoSync(Progress& pi)
{ // clears temporary caches (file times etc..)
	PPSync(TheIde()->IdeGetIncludePath());

	RTIMESTOP("Gathering files");
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
				GatherSources(path, path);
		}
	}

	SweepPPFiles(sSrcFile.GetIndex());
}

String GetMasterFile(const String& file)
{
	return sSrcFile.Get(file, Null);
}

int GetSourceFileIndex(const String& path)
{
	return source_file.FindPut(path);
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
	LDUMP(f.time);
	LDUMP(FileGetTime(path));
	if(f.time != FileGetTime(path))
		return false;
	RTIMING("CheckFile 2");
	Cpp pp;
	FileIn in(path);
	pp.Preprocess(path, in, GetMasterFile(path), true);
	String included_id_macros = pp.GetIncludedMacroValues(f.ids.GetKeys());
	LDUMP(included_id_macros);
	LDUMP(f.included_id_macros);
	return f.included_id_macros == included_id_macros;
}

void ParseFiles(Progress& pi, const Index<int> parse_file)
{
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
		ParseSrc(fi, parse_file[i], callback1(BrowserScanError, i), true, false);
	}
}

void UpdateCodeBase2(Progress& pi)
{
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
		LLOG("== CHECK == " << q << ": " << path);
		if(CheckFile(f, path))
			keep_file.Add(q);
		else {
			LLOG("PARSE!");
			parse_file.Add(q);
		}
	}
	
	CppBase& base = CodeBase();

	base.Sweep(keep_file);

	for(int i = 0; i < source_file.GetCount(); i++)
		if(keep_file.Find(i) < 0 && parse_file.Find(i) < 0 && !source_file.IsUnlinked(i))
			source_file.Unlink(i);
	
	ParseFiles(pi, parse_file);
}

void UpdateCodeBase(Progress& pi)
{
	BaseInfoSync(pi);

	UpdateCodeBase2(pi);
}

Vector<String> ParseSrc(Stream& in, int file, Callback2<int, const String&> error, bool do_macros, bool get_changes)
{
	Vector<String> cm;
	String path = GetSourceFilePath(file);
	LLOG("====== Parse " << file << ": " << path);
	Vector<String> pp;
	String ext = ToLower(GetFileExt(path));
	int filetype = FILE_OTHER;
	SourceFileInfo& sfi = source_file[file];
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
		cpp.Preprocess(path, in, GetMasterFile(GetSourceFilePath(file)));
		LLOG(path << ": " << cpp.ids.GetCount());
		LDUMP(cpp.ids);
		pp.Add(cpp.output);
		filetype = decode(ext, ".h", FILE_H, ".hpp", FILE_HPP,
		                       ".cpp",FILE_CPP, ".c", FILE_C, FILE_OTHER);
		if(do_macros) {
			sfi.ids = cpp.ids.PickKeys();
			sfi.included_id_macros = cpp.GetIncludedMacroValues(sfi.ids.GetKeys());
			LDUMP(sfi.ids);
			sfi.time = FileGetTime(path);
			LDUMP(sfi.time);
			VectorMap<String, String> dm = cpp.GetDefinedMacros();
			LDUMP(dm);
			if(sfi.defined_macros != dm) {
				LTIMING("Find changed macros");
				if(get_changes) {
					Buffer<bool> found(sfi.defined_macros.GetCount(), false);
					for(int i = 0; i < dm.GetCount() && cm.GetCount() < 10; i++) {
						String id = dm.GetKey(i);
						int q = sfi.defined_macros.Find(id);
						if(q < 0 || sfi.defined_macros[q] != dm[i])
							cm.Add(id);
						if(q >= 0)
							found[q] = true;
					}
					for(int i = 0; i < sfi.defined_macros.GetCount() && cm.GetCount() < 10; i++)
						if(!found[i])
							cm.Add(sfi.defined_macros.GetKey(i));
				}
				sfi.defined_macros = pick(dm);
			}
		}
	}

	for(int i = 0; i < pp.GetCount(); i++) {
		StringStream pin(pp[i]);
		Parse(pin, CodeBase(), file, filetype, error);
	}
	return cm;
}

bool HasIntersection(const Index<String>& ids, const Vector<String>& cm)
{
	if(cm.GetCount() >= 10)
		return true;
	for(int i = 0; i < cm.GetCount(); i++)
		if(ids.Find(cm[i]) >= 0)
			return true;
	return false;
}

void CodeBaseScanFile(Stream& in, const String& fn, bool check_macros)
{
	LLOG("===== CodeBaseScanFile " << fn);

	PPSync(TheIde()->IdeGetIncludePath());

	LTIMING("CodeBaseScan");
	
	int file = GetSourceFileIndex(fn);
	CppBase& base = CodeBase();
	base.RemoveFile(file);
	SourceFileInfo& f = source_file[file];
	Vector<String> cm = ParseSrc(in, file, CNULL, check_macros, true);
	LDUMP(cm);
	LDUMP(f.defined_macros);
	LDUMP(check_macros);
	if(cm.GetCount() && check_macros) {
		Progress pi;
		BaseInfoSync(pi);
		pi.SetText("Checking source files");
		pi.SetTotal(sSrcFile.GetCount());
		pi.SetPos(0);
		Index<int>  parse_file;
		for(int i = 0; i < sSrcFile.GetCount(); i++) {
			pi.Step();
			String path = sSrcFile.GetKey(i);
			int q = GetSourceFileIndex(path);			
			const SourceFileInfo& f = source_file[q];
			if(HasIntersection(f.ids, cm) && IncludesFile(path, fn) && !CheckFile(f, path))
				parse_file.Add(q);
		}
		
		CodeBase().RemoveFiles(parse_file);
		
		ParseFiles(pi, parse_file);
	}

	FinishCodeBase();
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

void SyncCodeBase()
{
	RTIMING("SyncCodeBase");
	RTIMESTOP("SyncCodeBase");
	Progress pi;
	pi.Title("Parsing source files");
	UpdateCodeBase(pi);
	FinishCodeBase();
}

void NewCodeBase()
{
	ReduceCodeBaseCache();
	static int start;
	if(start) return;
	start++;
	LoadCodeBase();
	SyncCodeBase();
	SaveCodeBase();
	start--;
}

void CheckCodeBase()
{
	RTIMESTOP("CheckCodeBase");
	Progress pi;
	pi.Title("Parsing source files");
	BaseInfoSync(pi);
	for(int i = 0; i < sSrcFile.GetCount(); i++)
		if(source_file.Find(sSrcFile.GetKey(i)) < 0) {
			UpdateCodeBase2(pi);
			FinishCodeBase();
			return;
		}
	for(int i = 0; i < source_file.GetCount(); i++)
		if(!source_file.IsUnlinked(i)) {
			String path = source_file.GetKey(i);
			if(sSrcFile.Find(source_file.GetKey(i)) < 0 || source_file[i].time != FileGetTime(path)) {
				UpdateCodeBase2(pi);
				FinishCodeBase();
				return;
			}
		}
}

void RescanCodeBase()
{
	ClearCodeBase();
	s_console = true;
	Progress pi;
	pi.Title("Parsing source files");
	UpdateCodeBase(pi);
	FinishCodeBase();
	s_console = false;
}

bool ExistsBrowserItem(const String& item)
{
	return GetCodeRefItem(item);
}
