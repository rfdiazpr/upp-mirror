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

AndroidBuilderSetup::AndroidBuilderSetup()
{
	CtrlLayout(*this);
	
	ndkBrowse.SetImage(CtrlImg::right_arrow());
	ndkBrowse <<= callback1(InsertPath, &ndk_path);
	ndk_path.AddFrame(ndkBrowse);
	
	jdkBrowse.SetImage(CtrlImg::right_arrow());
	jdkBrowse <<= callback1(InsertPath, &jdk_path);
	jdk_path.AddFrame(jdkBrowse);
}

VectorMap<Id, Ctrl*> AndroidBuilderSetup::GetSetupCtrlsMap()
{
	VectorMap<Id, Ctrl*> map;
	
	map.Add("NDK_PATH", &ndk_path);
	map.Add("JDK_PATH", &jdk_path);
	
	return map;
}

AndroidBuilderSetupHelper::AndroidBuilderSetupHelper()
{
	this->setup = NULL;
}

void AndroidBuilderSetupHelper::New(const String& builder)
{
	OnLoad();
}

void AndroidBuilderSetupHelper::OnLoad()
{
	if(!setup)
		return;
	
	setup->sdk_path.SetData(GetAndroidSDKPath());
}

DefaultBuilderSetup::DefaultBuilderSetup()
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

VectorMap<Id, Ctrl*> DefaultBuilderSetup::GetSetupCtrlsMap()
{
	VectorMap<Id, Ctrl*> map;
	
	map.Add("COMPILER",                  &compiler);
	map.Add("COMMON_OPTIONS",            &common_options);
	map.Add("COMMON_CPP_OPTIONS",        &common_cpp_options);
	map.Add("COMMON_C_OPTIONS",          &common_c_options);
	map.Add("COMMON_FLAGS",              &common_flags);
	map.Add("DEBUG_INFO",                &debug_info);
	map.Add("DEBUG_BLITZ",               &debug_blitz);
	map.Add("DEBUG_LINKMODE",            &debug_linkmode);
	map.Add("DEBUG_OPTIONS",             &debug_options);
	map.Add("DEBUG_FLAGS",               &debug_flags);
	map.Add("DEBUG_LINK",                &debug_link);
	map.Add("RELEASE_BLITZ",             &release_blitz);
	map.Add("RELEASE_LINKMODE",          &release_linkmode);
	map.Add("RELEASE_OPTIONS",           &speed_options);
	map.Add("RELEASE_SIZE_OPTIONS",      &size_options);
	map.Add("RELEASE_FLAGS",             &release_flags);
	map.Add("RELEASE_LINK",              &release_link);
	map.Add("DEBUGGER",                  &debugger);
	map.Add("ALLOW_PRECOMPILED_HEADERS", &allow_pch);
	map.Add("PATH",                      &path);
	map.Add("INCLUDE",                   &include);
	map.Add("LIB",                       &lib);
	
	return map;
}

DefaultBuilderSetupHelper::DefaultBuilderSetupHelper()
{
	this->setup = NULL;
}

void DefaultBuilderSetupHelper::New(const String& builder)
{
	if(!setup)
		return;
		
	bool gcc = builder == "GCC" || builder == "GCC32" || builder == "GCC_ARM";
	if(IsNull(setup->speed_options)) {
		if(gcc)
			setup->speed_options <<= "-O3 -ffunction-sections -fdata-sections";
		else
			setup->speed_options <<= "-O2";
	}
	if(IsNull(setup->size_options)) {
		if(gcc)
			setup->size_options <<= "-Os -finline-limit=20 -ffunction-sections -fdata-sections";
		else
			setup->size_options <<= "-O1";
	}
	if(IsNull(setup->debug_options)) {
		if(gcc)
			setup->debug_options <<= "-O0";
		else
			setup->debug_options <<= "-Od";
	}
	if(IsNull(setup->debugger)) {
		if(gcc)
			setup->debugger <<= "gdb";
		else
			setup->debugger <<= "msdev";
	}
	if(IsNull(setup->release_link) && gcc)
		setup->release_link <<= "-Wl,--gc-sections";
}

BuilderSetup::BuilderSetup(ParentCtrl* setupCtrl,
                           BuilderSetupHelperInterface* setupCtrlHelper,
                           VectorMap<Id, Ctrl*> setupCtrlsMap)
{
	this->setupCtrl       = setupCtrl;
	this->setupCtrlHelper = setupCtrlHelper;
	this->setupCtrlsMap   = setupCtrlsMap;
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
	InitSetups();
	
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
	for(int i = 0; i < setups.GetCount(); i++) {
		Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
		if(currentBuilders.Find(builderName) > -1)
			setups[i].setupCtrlHelper->New(builderName);
	}
	
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
			String builderName = map.Get("BUILDER");
			
			for(int i = 0; i < setups.GetCount(); i++) {
				Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
				if(currentBuilders.Find(builderName) > -1)
					setups[i].setupCtrlHelper->OnLoad();
			}
			
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

String BuildMethods::GetSetupPrefix(const String& setupKey) const
{
	return setupKey + "_";
}

void BuildMethods::InitSetups()
{
	Index<String> builders = GetBuilders();
	
	androidSetupHelper.SetSetup(&androidSetup);
	setups.Add(BuildersToString(AndroidBuilder::GetBuildersNames()),
	           BuilderSetup(&androidSetup, &androidSetupHelper, androidSetup.GetSetupCtrlsMap()));
	SieveBuilders(builders, AndroidBuilder::GetBuildersNames());
	
	defaultSetupHelper.SetSetup(&defaultSetup);
	setups.Add(BuildersToString(builders),
	           BuilderSetup(&defaultSetup, &defaultSetupHelper, defaultSetup.GetSetupCtrlsMap()));
	
	for(int i = 0; i < setups.GetCount(); i++) {
		Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
		if(currentBuilders.IsEmpty())
			continue;
			
		String setupKey = currentBuilders[0];
		
		ParentCtrl *currentSetup = setups[i].setupCtrl;
		setup.Add(currentSetup->SizePos());
		currentSetup->Hide();
		
		for(int j = 0; j < setups[i].setupCtrlsMap.GetCount(); j++) {
			String ctrlKey = setups[i].setupCtrlsMap.GetKey(j);
			Ctrl*  ctrl    = setups[i].setupCtrlsMap[j];
			
			method.AddCtrl(GetSetupPrefix(setupKey) + ctrlKey, *ctrl);
		}
	}
}

void BuildMethods::SwitchSetupView()
{
	if(!method.IsCursor()) {
		builder.Hide();
		builderLabel.Hide();
		setup.Hide();
		return;
	}
	else {
		builder.Show();
		builderLabel.Show();
	}
	String builderName = ~builder;
	builderName.IsEmpty() ? setup.Hide() : setup.Show();
	
	if(!builderName.IsEmpty()) {
		bool showed = false;
		for(int i = 0; i < setups.GetCount(); i++) {
			Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
			
			if(currentBuilders.Find(builderName) > -1)
				setups[i].setupCtrl->Show();
			else
				setups[i].setupCtrl->Hide();
		}
	}
}

VectorMap<String, String> BuildMethods::SieveBuilderVars(const VectorMap<String, String>& map)
{
	VectorMap<String, String> sievedMap;
	
	String builder = map.Get("BUILDER");
	if(builder.IsEmpty())
		return VectorMap<String, String>();
	
	for(int i = 0; i < map.GetCount(); i++) {
		String key = map.GetKey(i);
		String value = map[i];
		
		bool toInsert = true;
		for(int j = 0; j < setups.GetCount(); j++) {
			Index<String> currentBuilders = StringToBuilders(setups.GetKey(j));
			if(currentBuilders.IsEmpty())
				continue;
			String prefix = GetSetupPrefix(currentBuilders[0]);
			if(key.StartsWith(prefix)) {
				if(currentBuilders.Find(builder) > -1)
					key.Remove(0, prefix.GetCount());
				else
					toInsert = false;
			}
		}
		if(toInsert)
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
	
	for(int i = 0; i < setups.GetCount(); i++) {
		Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
		if(currentBuilders.IsEmpty())
			continue;
		
		String currentBuilder = currentBuilders[0];
		
		if(currentBuilder.Find(builder) > -1) {
			String setupPrefix = GetSetupPrefix(currentBuilder);
			
			for(int j = 0; j < map.GetCount(); j++) {
				String ctrlName = map.GetKey(j);
				
				if(setups[i].setupCtrlsMap.Find(ctrlName) > -1)
					mapedMap.Add(setupPrefix + ctrlName, map[j]);
				else
					mapedMap.Add(ctrlName, map[j]);
			}
		}
	}

	return mapedMap;
}

void BuildMethods::SieveBuilders(Index<String>& sievedBuilders,
                                 const Index<String>& builders) const
{
	for(int i = 0; i < builders.GetCount(); i++)
		sievedBuilders.RemoveKey(builders[i]);
}

String BuildMethods::BuildersToString(const Index<String>& builders) const
{
	String str;
	for(int i = 0; i < builders.GetCount(); i++) {
		str << builders[i];
		if(i + 1 < builders.GetCount())
			str << " ";
	}
	return str;
}

Index<String> BuildMethods::StringToBuilders(const String& str) const
{
	Vector<String> vec = Split(str, ' ');
	Index<String> builders;
	for(int i = 0; i < vec.GetCount(); i++)
		builders.Add(vec[i]);
	return builders;
}

Index<String> BuildMethods::GetBuilders() const
{
	Index<String> builders;
	for(int i = 0; i < BuilderMap().GetCount(); i++)
		builders.Add(BuilderMap().GetKey(i));
	return builders;
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
