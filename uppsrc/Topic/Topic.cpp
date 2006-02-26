#include "Topic.h"

TopicLink TopicEditor::ParseTopicFilePath(const String& path)
{
	TopicLink tl;
	tl.topic = GetFileTitle(path);
	String q = GetFileFolder(path);
	tl.group = GetFileTitle(q);
	q = GetFileFolder(q);
	for(int i = 0; i < packagedir.GetCount(); i++)
		if(PathIsEqual(packagedir[i], q)) {
			tl.package = package[i].name;
			return tl;
		}
	return TopicLink();
}


String TopicEditor::ActualPackageDir()
{
	return package.IsCursor() ? packagedir[package.GetCursor()] : Null;
}

struct ListOrder : FileList::Order {
public:
	virtual bool operator()(const FileList::File& a, const FileList::File& b) const {
		return ToUpper(a.name) < ToUpper(b.name);
	}
};

void LoadGroups(FileList& group, const String& dir)
{
	FindFile ff(AppendFileName(dir, "*.tpp"));
	while(ff) {
		if(ff.IsFolder()) {
			group.Add(GetFileTitle(ff.GetName()), TopicImg::Group());
			group.Enable();
		}
		ff.Next();
	}
	group.Sort(ListOrder());
}

void TopicEditor::EnterPackage()
{
	String pg = group.GetCurrentName();
	group.Clear();
	group.Disable();
	topic.Clear();
	topic.Disable();
	if(!package.IsCursor())
		return;
	group.Enable();
	topic.Enable();
	LoadGroups(group, ActualPackageDir());
	if(!IsNull(pg))
		group.FindSetCursor(pg);
	EnterGroup();
}

void LoadTopics(FileList& topic, const String& dir, const String& filepath)
{
	FindFile ff(AppendFileName(dir, "*.tpp"));
	while(ff) {
		if(ff.IsFile()) {
			topic.Add(GetFileTitle(ff.GetName()), TopicImg::Topic());
			if(PathIsEqual(AppendFileName(dir, ff.GetName()), filepath))
				topic.SetCursor(topic.GetCount() - 1);
		}
		ff.Next();
	}
	topic.Sort(ListOrder());
}

void TopicEditor::EnterGroup()
{
	topic.Clear();
	if(!package.IsCursor() || IsNull(group.GetCurrentName()))
		return;
	LoadTopics(topic, AppendFileName(ActualPackageDir(), group.GetCurrentName() + ".tpp"), filepath);
	topic.Enable();
}

String TopicEditor::GetCurrentTopicPath()
{
	return NormalizePath(
			AppendFileName(
				AppendFileName(ActualPackageDir(), group.GetCurrentName() + ".tpp"),
	            topic.GetCurrentName() + ".tpp"));
}

void TopicEditor::ShowTopic(bool b)
{
	title.Enable(b);
	title.Show(b);
	editor.Enable(b);
	editor.Show(b);
}

void TopicEditor::EnterTopic()
{
	HideTopic();
	if(!topic.IsCursor())
		return;

	String h = GetCurrentTopicPath();
	if(h != filepath)
		Load(h);
	else
		ShowTopic();
}

void TopicEditor::Load(const String& fn)
{
	Flush();

	Topic t = ReadTopic(LoadFile(fn));
	if(t.text.IsVoid()) {
		Exclamation("Error loading the topic file:&[* " + DeQtf(fn));
		topic.KillCursor();
		return;
	}

	title <<= t.title;
	editor <<= t.text;
	filepath = fn;
	AddLru();

	int q = editstate.Find(fn);
	if(q >= 0) {
		FileInfo& fi = editstate[q];
		if(fi.time == FileGetTime(fn)) {
			editor.SetPickUndoInfo(fi.undo);
			fi.time = Time(1, 1, 1);
		}
		editor.SetPosInfo(fi.pos);
	}

	ShowTopic();

	ActiveFocus(editor);
	editor.ClearModify();
	title.ClearModify();

	Title(fn);
}

bool TopicEditor::Open(const String& fn)
{
	TopicLink tl = ParseTopicFilePath(fn);
	if(!tl)
		return false;
	package.FindSetCursor(tl.package);
	group.FindSetCursor(tl.group);
	topic.FindSetCursor(tl.topic);
	return true;
}

bool TopicEditor::OpenLink(const String& lnk)
{
	TopicLink tl = ParseTopicLink(lnk);
	if(!tl)
		return false;
	package.FindSetCursor(tl.package);
	group.FindSetCursor(tl.group);
	topic.FindSetCursor(tl.topic);
	return true;
}


void TopicEditor::Save()
{
	if(IsNull(filepath))
		return;
	if(IsNull(~title)) {
		const RichText& txt = editor.Get();
		if(txt.IsPara(0)) {
			RichPara para = txt.Get(0);
			WString t;
			for(int i = 0; i < para.GetCount(); i++)
				if(para[i].IsText())
					for(const wchar *s = para[i].text; *s; s++)
						if(*s == '\t' || *s == 160)
							t.Cat(' ');
						else
						if(*s >= ' ')
							t.Cat(*s);
			if(!IsNull(t))
				title <<= t;
		}
	}
	if(!editor.IsModified() && !title.IsModified())
		return;
	String r = WriteTopic((String)~title, editor.Get());
	if(LoadFile(filepath) != r) {
		SaveFile(filepath, r);
		TopicLink tl = ParseTopicFilePath(filepath);
		if(tl)
			SyncFile(filepath, TopicLinkString(tl));
	}
}

void TopicEditor::Flush()
{
	Save();
	FileInfo& fi = editstate.GetAdd(filepath);
	fi.time = FileGetTime(filepath);
	fi.pos = editor.GetPosInfo();
	fi.undo = editor.PickUndoInfo();
	filepath.Clear();
	editor.Clear();
	HideTopic();
}

void TopicEditor::SaveInc(const String& packagedir, const String& group)
{
	String gh;
	FindFile ff(AppendFileName(AppendFileName(packagedir, group + ".tpp"), "*.tpp"));

	while(ff) {
		if(ff.IsFile()) {
			gh << "TOPIC(" << AsCString(GetFileTitle(ff.GetName())) << ")\r\n";
			gh << "#include \"" << ff.GetName() << "\"\r\n";
			gh << "END_TOPIC\r\n\r\n";
		}
		ff.Next();
	}

	String fn = AppendFileName(AppendFileName(packagedir, group + ".tpp"), "all.i");
	if(LoadFile(fn) != gh)
		if(IsNull(gh))
			DeleteFile(fn);
		else
			SaveFile(fn, gh);
}
