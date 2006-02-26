#include "ide.h"

class BaseSetupDlg : public WithBaseSetupLayout<TopWindow>
{
public:
	typedef BaseSetupDlg CLASSNAME;
	BaseSetupDlg();

	bool Run(String& vars);

private:
	void OnUpp();
	void OnBrowseUpp();

private:
	FrameRight<Button> browse_upp;
	SelectDirButton    browse_out;
	SelectDirButton    browse_common;
	bool               new_base;
};

bool BaseSetup(String& vars) { return BaseSetupDlg().Run(vars); }

BaseSetupDlg::BaseSetupDlg()
: browse_out("Output & intermediate files")
, browse_common("Common files")
{
	CtrlLayoutOKCancel(*this, "Assembly setup");
	browse_upp.SetImage(CtrlImg::right_arrow());
	browse_upp <<= THISBACK(OnBrowseUpp);
	upp.AddFrame(browse_upp);
	upp <<= THISBACK(OnUpp);
	browse_out.Attach(output);
	browse_common.Attach(common);
}

bool BaseSetupDlg::Run(String& vars)
{
	upp <<= GetVar("UPP");
	output <<= GetVar("OUTPUT");
	common <<= GetVar("COMMON");
	base <<= vars;
	new_base = IsNull(vars);
	while(TopWindow::Run() == IDOK)
	{
		String varname = ~base;
		String varfile = VarFilePath(varname);
		if(varname != vars)
		{
			if(FileExists(varfile) && !PromptOKCancel(NFormat("Overwrite existing base file [* \1%s\1]?", varfile)))
				continue;
			if(!SaveVars(varname))
			{
				Exclamation(NFormat("Error writing base file [* \1%s\1].", VarFilePath(varname)));
				continue;
			}
		}
		SetVar("UPP", ~upp);
		SetVar("OUTPUT", ~output);
		SetVar("COMMON", ~common);
		Vector<String> paths = SplitDirs(upp.GetText().ToString());
		for(int i = 0; i < paths.GetCount(); i++)
			RealizeDirectory(paths[i]);
		RealizeDirectory(output);
		vars = varname;
		return true;
	}
	return false;
}

void BaseSetupDlg::OnBrowseUpp()
{
	String s = ~upp;
	int b, e;
	if(upp.HasFocus())
		upp.GetSelection(b, e);
	else
		e = s.GetLength();
	b = e;
	while(b > 0 && s[b - 1] != ';')
		b--;
	while(e < s.GetLength() && s[e] != ';')
		e++;
	FileSel fsel;
	String pre = s.Left(b), post = s.Mid(e);
	fsel.ActiveDir(s.Mid(b, e - b));
	if(fsel.ExecuteSelectDir()) {
		String newdir = ~fsel;
		upp <<= pre + newdir + Nvl(post, ";");
		upp.SetWantFocus();
		OnUpp();
	}
}

void BaseSetupDlg::OnUpp()
{
	if(new_base) {
		String s = ~upp;
		int f = s.Find(';');
		if(f >= 0) s.Trim(f);
		base <<= GetFileTitle(s);
	}
}

class SelectPackageDlg : public WithListLayout<TopWindow> {
	typedef SelectPackageDlg CLASSNAME;
public:
	SelectPackageDlg(const char *title, bool selectvars, bool main);

	String         Run(String startwith);

private:
	void           ToolBase(Bar& bar);

	void           OnBaseAdd();
	void           OnBaseEdit();
	void           OnBaseRemove();

	void           OnOK();

	void           OnNew();
	void           OnBase();
	void           OnAll();

	void           Load();
	void           Load(String upp, String dir, int progress_pos, int progres_count, String& case_fixed);
	void           SyncBase(String initvars);
	void           SyncList();

private:
	ArrayCtrl      base;
	FileList       list;

	bool           selectvars;
	bool           loading;
	String         selected;
	int            update;
	Vector<String> packages;
};

SelectPackageDlg::SelectPackageDlg(const char *title, bool selectvars_, bool main)
: selectvars(selectvars_)
{
	CtrlLayoutOKCancel(*this, title);
	Sizeable().Zoomable();
	Icon(IdeImg::Package(), IdeImg::Package());
	base.AutoHideSb();
	base.NoGrid();
	base.AddColumn("Assembly");
	base.WhenCursor = THISBACK(OnBase);
	base.WhenBar = THISBACK(ToolBase);
	base.WhenLeftDouble = THISBACK(OnBaseEdit);
	ok <<= list.WhenLeftDouble = THISBACK(OnOK);
	list.Columns(3);
	splitter.Horz(base, list);
	splitter.SetPos(2000);
	splitter.Zoom(selectvars ? -1 : 1);
	newu <<= THISBACK(OnNew);
	all <<= THISBACK(OnAll);
	all.Show(main);
	all = !main;
	progress.Hide();
}

inline bool PackageLess(String a, String b)
{
	int nc = CompareNoCase(a, b);
	if(nc) return nc < 0;
	return a < b;
};

String SelectPackageDlg::Run(String startwith)
{
	if(!IsSplashOpen())
		Open();
	if(selectvars)
		SyncBase(GetVarsName());
	else
		OnBase();
	ActiveFocus(list);
	list.FindSetCursor(startwith);
	switch(TopWindow::Run())
	{
	case IDOK:  return list.GetCurrentName();
	case IDYES: return selected;
	default:    return Null;
	}
}

void SelectPackageDlg::OnOK()
{
	loading = false;
	AcceptBreak(IDOK);
}

void SelectPackageDlg::OnAll()
{
	OnBase();
}

void SelectPackageDlg::OnBase()
{
	Load();
}

void SelectPackageDlg::OnNew() {
	TemplateDlg dlg;
	LoadFromGlobal(dlg, "NewPackage");
	dlg.Load(GetUppDirs(), !all);
	while(dlg.Run() == IDOK) {
		String nest = ~dlg.nest;
		String name = NativePath(String(~dlg.package));
		String path = AppendFileName(nest, AppendFileName(name, GetFileName(name) + ".upp"));
		if(FileExists(path) && !PromptYesNo("Package [* \1" + path + "\1] already exists.&"
		                                    "Do you wish to recreate the files?"))
			continue;
		RealizePath(path);
		if(!SaveFile(path, Null)) {
			Exclamation("Error writing the file [* \1" + path + "\1].");
			continue;
		}
		dlg.Create();
		selected = name;
		Break(IDYES);
		break;
	}
	StoreToGlobal(dlg, "NewPackage");
}

void SelectPackageDlg::ToolBase(Bar& bar)
{
	bar.Add("New assembly", THISBACK(OnBaseAdd))
		.Key(K_INSERT);
	bar.Add(base.IsCursor(), "Edit assembly", THISBACK(OnBaseEdit))
		.Key(K_CTRL_ENTER);
	bar.Add(base.IsCursor(), "Remove assembly", THISBACK(OnBaseRemove))
		.Key(K_CTRL_DELETE);
}

void SelectPackageDlg::OnBaseAdd()
{
	String vars;
	if(BaseSetup(vars))
		SyncBase(vars);
}

void SelectPackageDlg::OnBaseEdit()
{
	if(!base.IsCursor())
		return;
	String vars = base.Get(0), oldvars = vars;
	if(BaseSetup(vars))
	{
		if(vars != oldvars)
			DeleteFile(VarFilePath(oldvars));
		SyncBase(vars);
	}
}

void SelectPackageDlg::OnBaseRemove()
{
	int c = base.GetCursor();
	if(c < 0)
		return;
	String next;
	if(c + 1 < base.GetCount())
		next = base.Get(c + 1);
	else if(c > 0)
		next = base.Get(c - 1);
	String vars = base.Get(0);
	String varpath = VarFilePath(vars);
	if(PromptOKCancel(NFormat("Remove base file [* \1%s\1]?", varpath)) && !FileDelete(varpath))
		Exclamation(NFormat("Error deleting file [* \1%s\1].", varpath));
	SyncBase(next);
}

void SelectPackageDlg::Load()
{
	update = msecs();
	list.Clear();
	if(selectvars)
	{
		list.Enable(base.IsCursor());
		if(!base.IsCursor())
			return;
		LoadVars((String)base.Get(0));
	}
	Vector<String> upp = GetUppDirs();
	packages.Clear();
	all.Hide();
	progress.Show();
	loading = true;
	String case_fixed;
	for(int i = 0; i < upp.GetCount() && (all || i < 1) && loading; i++) {
		if(!IsSplashOpen() && !IsOpen())
			Open();
		Load(upp[i], Null, 1000 * i, 1000, case_fixed);
	}
	if(!IsNull(case_fixed))
		PromptOK("Case was fixed in some of the files:[* " + case_fixed);
	progress.Hide();
	while(IsSplashOpen())
		ProcessEvents();
	if(!IsOpen())
		Open();
	all.Show();
	if(loading)
	{
		loading = false;
		SyncList();
	}
}

void FixName(const String& dir, const String& name, String& case_fixed)
{
	if(IsFullPath(name))
		return;
	String rp = AppendFileName(dir, name);
	FindFile ff(rp);
	if(!ff || ff.GetName() == GetFileName(name))
		return;
	MoveFile(rp, rp);
	case_fixed << '&' << DeQtf(rp);
}

bool IsMainPackage(const String& dd, const String& nm, String& case_fixed)
{
	Package p;
	p.Load(AppendFileName(dd, nm));
	FixName(dd, nm, case_fixed);
	for(int i = 0; i < p.GetCount(); i++)
		if(!p[i].separator)
			FixName(dd, p[i], case_fixed);
	return p.config.GetCount();
}

void SelectPackageDlg::Load(String upp, String dir, int progress_pos, int progress_count, String& case_fixed)
{
	if(msecs(update) >= 200)
	{
		progress.Set(progress_pos);
		SyncList();
		ProcessEvents();
		if(!loading)
			return;
		update = msecs();
	}

	String d = AppendFileName(upp, dir);
	Vector<String> folders;
	for(FindFile ff(AppendFileName(d, "*.*")); ff; ff.Next())
		if(ff.IsFolder())
			folders.Add(ff.GetName());
	Sort(folders, &PackageLess);
	for(int i = 0; i < folders.GetCount() && loading; i++)
	{
		String fo = folders[i];
		String dd = AppendFileName(d, fo);
		String nm = fo + ".upp";
		String pkg = AppendFileName(dir, fo);
		if(FileExists(AppendFileName(dd, nm)) && (all || IsMainPackage(dd, nm, case_fixed)))
			packages.Add(pkg);
		int ppos = progress_pos + i * progress_count / folders.GetCount();
		int npos = progress_pos + (i + 1) * progress_count / folders.GetCount();
		Load(upp, pkg, ppos, npos - ppos, case_fixed);
	}
}

void SelectPackageDlg::SyncBase(String initvars)
{
	Vector<String> varlist;
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile())
			varlist.Add(GetFileTitle(ff.GetName()));
	Sort(varlist, &PackageLess);
	base.Clear();
	Append(base, varlist);
	if(!base.FindSetCursor(initvars))
		if(base.GetCount() > 0)
			base.SetCursor(0);
		else
			OnBase();
}

void SelectPackageDlg::SyncList()
{
	Sort(packages, &PackageLess);
	String n = list.GetCurrentName();
	int sc = list.GetSbPos();
	list.Clear();
	for(int i = 0; i < packages.GetCount(); i++)
		list.Add(packages[i], IdeImg::Package());
	list.SetSbPos(sc);
	list.FindSetCursor(n);
}

String SelectPackage(const char *title, const char *startwith, bool selectvars, bool main) {
	return SelectPackageDlg(title, selectvars, main).Run(startwith);
}
