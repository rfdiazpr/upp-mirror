#include "Methods.h"

void  TextOption::SetData(const Value& data)
{
	String s = data;
	Set(!(IsNull(s) || s == "0"));
}

Value TextOption::GetData() const
{
	return Get() ? "1" : "0";
}

void  TextSwitch::SetData(const Value& data)
{
	String s = data;
	Switch::SetData(atoi((String)data));
}

Value TextSwitch::GetData() const
{
	return AsString(Switch::GetData());
}

void DirTable::SetData(const Value& data)
{
	Vector<String> l = Split((String)data, ';');
	Clear();
	for(int i = 0; i < l.GetCount(); i++)
		Add(l[i]);
}

Value DirTable::GetData() const
{
	String s;
	for(int i = 0; i < GetCount(); i++) {
		if(i) s << ';';
		s << (String)Get(i, 0);
	}
	return s;
}

void DirTable::Init(const char *name)
{
	AutoHideSb();
	AddColumn(name).Edit(edit);
	Appending().Removing().Moving();
	edit_dir.Attach(edit);
	edit_dir.AllFilesType();
	WhenArrayAction = edit <<= callback(this, &DirTable::Modify);
}

DirTable::DirTable()
{
	Init();
	NoHeader();
}

DirTable::DirTable(const char *name)
{
	Init(name);
}

void DirMap::SetData(const Value& data)
{
	Vector<String> l = Split((String)data, ';');
	Clear();
	for(int i = 0; i < l.GetCount(); i++) {
		String li = l[i];
		int f = li.Find('>');
		if(f >= 0)
			Add(li.Left(f), li.Mid(f + 1));
	}
}

Value DirMap::GetData() const
{
	String s;
	for(int i = 0; i < GetCount(); i++) {
		if(i) s << ';';
		s << (String)Get(i, 0) << '>' << (String)Get(i, 1);
	}
	return s;
}

DirMap::DirMap()
{
	AutoHideSb();
	AddColumn("Map local path").Edit(localpath);
	AddColumn("To remote path").Edit(remotepath);
	Appending().Removing().Moving();
	edit_dir.Attach(localpath);
	WhenArrayAction = localpath <<= remotepath <<= callback(this, &DirMap::Modify);
}

BuilderSetup::BuilderSetup(const String& prefix_) :
	prefix(prefix_)
{
	
}

Index<String> BuilderSetup::GetCoreIds()
{
	Index<String> ids;
	
	VectorMap<Id, Ctrl*> map = GetSetupCtrlsMap();
	for(int i = 0; i < map.GetCount(); i++) {
		String key = map.GetKey(i);
		if(key.StartsWith(prefix))
			key.Remove(0, prefix.GetCount());
		ids.Add(key);
	}
	
	return ids;
}

AndroidBuilderSetup::AndroidBuilderSetup()
	: BuilderSetup("ANDROID_")
{
	CtrlLayout(*this);
}

void AndroidBuilderSetup::New()
{
	sdkDir.SetData(GetAndroidSDKPath());
}

VectorMap<Id, Ctrl*> AndroidBuilderSetup::GetSetupCtrlsMap()
{
	VectorMap<Id, Ctrl*> map;
	
	map.Add(prefix + "NDK_DIR", &ndkDir);
	map.Add(prefix + "JDK_DIR", &jdkDir);
	
	return map;
}

DefaultBuilderSetup::DefaultBuilderSetup() :
	BuilderSetup("DEFAULT_")
{
	CtrlLayout(*this);
	
	allow_pch.SetLabel("Allow precompiled headers");
	
	paths.Add(path.SizePos(), "PATH - executable directories");
	paths.Add(include.SizePos(), "INCLUDE directories");
	paths.Add(lib.SizePos(), "LIB directories");

	debug_info.Add("0", "None");
	debug_info.Add("1", "Minimal");
	debug_info.Add("2", "Full");
}

void DefaultBuilderSetup::New(const String& builder)
{
	bool gcc = builder == "GCC" || builder == "GCC32" || builder == "GCC_ARM";
	if(IsNull(speed_options)) {
		if(gcc)
			speed_options <<= "-O3 -ffunction-sections -fdata-sections";
		else
			speed_options <<= "-O2";
	}
	if(IsNull(size_options)) {
		if(gcc)
			size_options <<= "-Os -finline-limit=20 -ffunction-sections -fdata-sections";
		else
			size_options <<= "-O1";
	}
	if(IsNull(debug_options)) {
		if(gcc)
			debug_options <<= "-O0";
		else
			debug_options <<= "-Od";
	}
	if(IsNull(debugger)) {
		if(gcc)
			debugger <<= "gdb";
		else
			debugger <<= "msdev";
	}
	if(IsNull(release_link) && gcc)
		release_link <<= "-Wl,--gc-sections";
}

VectorMap<Id, Ctrl*> DefaultBuilderSetup::GetSetupCtrlsMap()
{
	VectorMap<Id, Ctrl*> map;
	
	map.Add(prefix + "COMPILER",                  &compiler);
	map.Add(prefix + "COMMON_OPTIONS",            &common_options);
	map.Add(prefix + "COMMON_CPP_OPTIONS",        &common_cpp_options);
	map.Add(prefix + "COMMON_C_OPTIONS",          &common_c_options);
	map.Add(prefix + "COMMON_FLAGS",              &common_flags);
	map.Add(prefix + "DEBUG_INFO",                &debug_info);
	map.Add(prefix + "DEBUG_BLITZ",               &debug_blitz);
	map.Add(prefix + "DEBUG_LINKMODE",            &debug_linkmode);
	map.Add(prefix + "DEBUG_OPTIONS",             &debug_options);
	map.Add(prefix + "DEBUG_FLAGS",               &debug_flags);
	map.Add(prefix + "DEBUG_LINK",                &debug_link);
	map.Add(prefix + "RELEASE_BLITZ",             &release_blitz);
	map.Add(prefix + "RELEASE_LINKMODE",          &release_linkmode);
	map.Add(prefix + "RELEASE_OPTIONS",           &speed_options);
	map.Add(prefix + "RELEASE_SIZE_OPTIONS",      &size_options);
	map.Add(prefix + "RELEASE_FLAGS",             &release_flags);
	map.Add(prefix + "RELEASE_LINK",              &release_link);
	map.Add(prefix + "DEBUGGER",                  &debugger);
	map.Add(prefix + "ALLOW_PRECOMPILED_HEADERS", &allow_pch);
	map.Add(prefix + "PATH",                      &path);
	map.Add(prefix + "INCLUDE",                   &include);
	map.Add(prefix + "LIB",                       &lib);
	
	return map;
}

int CharFilterFileName(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

BuildMethods::BuildMethods()
{
	CtrlLayoutOKCancel(*this, "Build methods");
	Sizeable().Zoomable();
	method.AddColumn("Method").Edit(name);
	name.SetFilter(CharFilterFileName);
	
	method.AddCtrl("BUILDER", builder);
	AddBuilderSetupCtrls(androidSetup);
	AddBuilderSetupCtrls(defaultSetup);
	
#if 0 // REMOTE REMOVED
	method.AddCtrl("REMOTE_OS", remote_os);
	remote_os.Add("WIN32");
	remote_os.Add("LINUX");
	remote_os.Add("WINCE");
	remote_os.Add("UNIX");
	remote_os.Add("SOLARIS");
	remote_os.Add("BSD");
	method.AddCtrl("REMOTE_TRANSFER", remote_file_access);
	remote_file_access.Add("0", "direct (SAMBA)");
	remote_file_access.Add("1", "indirect (transfer)");
	method.AddCtrl("REMOTE_MAP", remote_path_map);
#endif
	method.AddCtrl("SCRIPT", scriptfile);
	method.AddCtrl("LINKMODE_LOCK", linkmode_lock);
	
	open_script.Attach(scriptfile);
	open_script.Type("Build scripts (*.bsc)", "*.bsc")
		.AllFilesType();
	open_script.DefaultExt("bsc");
	method.Appending().Removing().Duplicating();
	method.WhenCursor = THISBACK(ChangeMethod);
	method.WhenBar = THISBACK(MethodMenu);

	for(int i = 0; i < BuilderMap().GetCount(); i++)
		builder.Add(BuilderMap().GetKey(i));

	builder <<= THISBACK(NewBuilder);
	setdefault <<= THISBACK(SetDefault);

	linkmode_lock.SetLabel("Lock link mode");
	
	androidSetup.SizePos();
	defaultSetup.SizePos();
}

void BuildMethods::MethodMenu(Bar& bar)
{
	method.StdBar(bar);
	bar.Separator();
	bar.Add("Import", THISBACK(Import));
}

void BuildMethods::Import()
{
	if(!Save())
		return;
	FileSel fsel;
	fsel.Type("Build methods (*.bm)", "*.bm")
		.AllFilesType()
		.Multi()
		.DefaultExt("bm");
	if(!fsel.ExecuteOpen())
		return;
	for(int i = 0; i < fsel.GetCount(); i++) {
		String f = LoadFile(fsel[i]);
		if(f.IsVoid()) {
			if(!PromptOKCancel(NFormat("Failed to load [* \1%s\1]. Continue?", fsel[i])))
				break;
			continue;
		}
		String nf = ConfigFile(GetFileNamePos(fsel[i]));
		if(FileExists(nf) && !PromptOKCancel(NFormat("File already exists: [* \1%s\1]. Overwrite?", nf)))
			continue;
		if(!SaveFile(nf, f))
			if(!PromptOKCancel(NFormat("Failed to save [* \1%s\1]. Continue?", nf)))
				break;
	}
	Load();
}

static int sCompare(const Value& v1, const Value& v2)
{
	return (String)v1 < (String)v2;
}

void BuildMethods::NewBuilder()
{
	String builderName = ~builder;
	if(builderName == "ANDROID")
		androidSetup.New();
	else
		defaultSetup.New(builderName);
	SwitchSetupView();
}

void BuildMethods::ChangeMethod()
{
	String b;
	if(method.IsCursor())
		b = method.Get("BUILDER");
	scriptfile.Enable(b == "SCRIPT");
	SwitchSetupView();
}

void BuildMethods::Load()
{
	FindFile ff(ConfigFile("*.bm"));
	while(ff) {
		VectorMap<String, String> map;
		String fn = ConfigFile(ff.GetName());
		if(LoadVarFile(fn, map)) {
			map = MapBuilderVars(map);
			origfile.Add(fn);
			method.Add(GetFileTitle(fn));
			for(int j = 1; j < method.GetIndexCount(); j++)
				method.Set(method.GetCount() - 1, j, map.Get(method.GetId(j).ToString(), Null));
		}
		ff.Next();
	}
	method.Sort(0, sCompare);
	method.GoBegin();
}

bool BuildMethods::Save()
{
	int i;
	Index<String> name;
	for(i = 0; i < method.GetCount(); i++) {
		String n = method.Get(i, 0);
		if(name.Find(n) >= 0) {
			Exclamation("Duplicate method [* " + DeQtf(n) + "] !");
			return false;
		}
		name.Add(n);
	}
	Index<String> saved;
	for(i = 0; i < method.GetCount(); i++) {
		VectorMap<String, String> map;
		for(int j = 1; j < method.GetIndexCount(); j++)
			map.Add(method.GetId(j).ToString(), method.Get(i, j));
		if(map.Get("BUILDER", "") != "SCRIPT")
			map.RemoveKey("SCRIPT");
		
		map = SieveBuilderVars(map);
		
		String fn = ConfigFile(String(method.Get(i, 0)) + ".bm");
		if(!SaveVarFile(fn, map)) {
			Exclamation("Error saving [* " + fn + "] !");
			return false;
		}
		saved.Add(fn);
	}
	for(i = 0; i < origfile.GetCount(); i++)
		if(saved.Find(origfile[i]) < 0)
			DeleteFile(origfile[i]);
	return true;
}

struct BoldDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
					   Color ink, Color paper, dword style) const {
		w.DrawRect(r, paper);
		DrawSmartText(w, r.left, r.top, r.Width(), (String)q, StdFont().Bold(), ink);
	}
};


void BuildMethods::ShowDefault()
{
	String m = GetDefaultMethod();
	for(int i = 0; i < method.GetCount(); i++)
		if((String)method.Get(i, 0) == m)
			method.SetDisplay(i, 0, Single<BoldDisplay>());
		else
			method.SetDisplay(i, 0, StdDisplay());
}

void BuildMethods::SetDefault()
{
	if(method.IsCursor()) {
		SaveFile(ConfigFile("default_method"), method.GetKey());
		ShowDefault();
	}
}

void BuildMethods::SwitchSetupView()
{
	if(!method.IsCursor()) {
		builder.Hide();
		builderLabel.Hide();
		setupParent.Hide();
		return;
	}
	else {
		builder.Show();
		builderLabel.Show();
	}
	String builderName = ~builder;
	builderName.IsEmpty() ? setupParent.Hide() : setupParent.Show();
	
	setupParent.ClearFrames(); // FIXME: this works well with TopWindow parametrization
	if(builderName == "ANDROID") {
		setupParent.Add(androidSetup);
	}
	else {
		setupParent.Add(defaultSetup);
	}
}

void BuildMethods::AddBuilderSetupCtrls(BuilderSetup& builderSetup)
{
	VectorMap<Id, Ctrl*> map = builderSetup.GetSetupCtrlsMap();
	for(int i = 0; i < map.GetCount(); i++)
		method.AddCtrl(map.GetKey(i), *map[i]);
}

VectorMap<String, String> BuildMethods::SieveBuilderVars(const VectorMap<String, String>& map)
{
	VectorMap<String, String> sievedMap;
	
	String builder = map.Get("BUILDER");
	if(builder.IsEmpty())
		return VectorMap<String, String>();
	
	String androidPrefix = androidSetup.GetPrefix();
	String defaultPrefix = defaultSetup.GetPrefix();
	for(int i = 0; i < map.GetCount(); i++) {
		String key = map.GetKey(i);
		String value = map[i];
		
		if(key.StartsWith(androidPrefix)) {
			// TODO: remove hardcoded android builder name
			if(builder == "ANDROID") {
				key.Remove(0, androidPrefix.GetCount());
				sievedMap.Add(key, value);
			}
		}
		else
		if(key.StartsWith(defaultPrefix)) {
			// Different than all special builders -> can be vectorized in the future.
			if(builder != "ANDROID") {
				key.Remove(0, defaultPrefix.GetCount());
				sievedMap.Add(key, value);
			}
		}
		else
			sievedMap.Add(key, value);
	}
	
	return sievedMap;
}

VectorMap<String, String> BuildMethods::MapBuilderVars(const VectorMap<String, String>& map)
{
	VectorMap<String, String> mapedMap;
	Index<String> varsToMaped;
	
	String builder = map.Get("BUILDER");
	if(builder.IsEmpty())
		return VectorMap<String, String>();
	
	if(builder == "ANDROID")
		varsToMaped = androidSetup.GetCoreIds();
	else
	if(builder != "ANDROID") {
		// Different than all special builders -> can be vectorized in the future.
		varsToMaped = defaultSetup.GetCoreIds();
	}
	
	for(int i = 0; i < map.GetCount(); i++) {
		String key = map.GetKey(i);
		String value = map[i];
		
		if(varsToMaped.Find(key) > -1) {
			if(builder == "ANDROID")
				key = androidSetup.GetPrefix() + key;
			else
			if(builder != "ANDROID")
				key = defaultSetup.GetPrefix() + key;
		}
		
		mapedMap.Add(key, value);
	}
	
	return mapedMap;
}

void Ide::SetupBuildMethods()
{
	BuildMethods m;
	m.Load();
	m.ShowDefault();
	m.use_target = use_target;
	m.method.FindSetCursor(method);
	for(;;) {
		int c = m.Run();
		if(c == IDCANCEL)
			break;
		if(c == IDOK && m.Save()) {
			use_target = m.use_target;
			break;
		}
	}
	SyncBuildMode();
	SetBar();
}
