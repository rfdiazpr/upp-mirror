#include "Browser.h"

struct TopicLinkDlg : WithTopicLinkLayout<TopWindow> {
	void   Package();
	void   Group();
	void   Topic();
	void   Label();
	String LinkString();
	String ActualDir();

	Vector<String>& packagedir;

	typedef TopicLinkDlg CLASSNAME;
	TopicLinkDlg(Vector<String>& packagedir, FileList& _package);
};

TopicLinkDlg::TopicLinkDlg(Vector<String>& packagedir, FileList& _package)
: packagedir(packagedir)
{
	CtrlLayoutOKCancel(*this, "Hyperlink");
	package.WhenKillCursor = package.WhenEnterItem = THISBACK(Package);
	group.WhenKillCursor = group.WhenEnterItem = THISBACK(Group);
	topic.WhenKillCursor = topic.WhenEnterItem = THISBACK(Topic);
	label.WhenKillCursor = label.WhenEnterItem = THISBACK(Label);
	for(int i = 0; i < _package.GetCount(); i++)
		package.Add(_package[i].name);
	package.SetCursor(0);
	package.FindSetCursor(_package.GetCurrentName());
	insert <<= Breaker(IDYES);
}

void TopicLinkDlg::Package()
{
	String pg = group.GetCurrentName();
	group.Clear();
	if(!package.IsCursor())
		return;
	LoadGroups(group, packagedir[package.GetCursor()]);
	topic.Clear();
	group.FindSetCursor(pg);
}

void TopicLinkDlg::Group()
{
	topic.Clear();
	if(package.IsCursor() && group.IsCursor())
		LoadTopics(topic, AppendFileName(packagedir[package.GetCursor()], group.GetCurrentName() + ".tpp"));
}

String TopicLinkDlg::LinkString()
{
	return "topic://" + (String)package.GetCurrentName() + '/' + (String)group.GetCurrentName()
	       + '/' + (String)topic.GetCurrentName();
}

void TopicLinkDlg::Topic()
{
	if(package.IsCursor() && group.IsCursor() && topic.IsCursor()) {
		link <<= LinkString();
		RichText txt = ParseQTF(ReadTopic(LoadFile(
						NormalizePath(
							AppendFileName(AppendFileName(packagedir[package.GetCursor()],
								                          group.GetCurrentName() + ".tpp"),
		        			               topic.GetCurrentName() + ".tpp")
		               ))).text);
		Vector<String> ref = GatherLabels(txt);
		label.Clear();
		for(int i = 0; i < ref.GetCount(); i++)
			label.Add(ref[i]);
	}
}

void TopicLinkDlg::Label()
{
	if(package.IsCursor() && group.IsCursor() && topic.IsCursor() && label.IsCursor())
		link <<= LinkString() + '#' + label.GetCurrentName();
}

void TopicEditor::Hyperlink(String& link, WString& text)
{
	TopicLinkDlg d(packagedir, package);
	d.link <<= link;
	String label;
	TopicLink tl;
	int q = link.ReverseFind('#');
	if(q >= 0) {
		label = link.Mid(q + 1);
		tl = ParseTopicLink(link.Mid(0, q));
	}
	else
		tl = ParseTopicLink(link);
	if(IsNull(tl.topic)) {
		d.package.FindSetCursor(package.GetCurrentName()) &&
		d.group.FindSetCursor(group.GetCurrentName()) &&
		d.topic.FindSetCursor(topic.GetCurrentName());
	}
	else {
		d.package.FindSetCursor(tl.package) &&
		d.group.FindSetCursor(tl.group) &&
		d.topic.FindSetCursor(tl.topic) &&
		d.label.FindSetCursor(label);
	}
	int c = d.Run();
	if(c == IDCANCEL)
		return;
	link = ~d.link;
	if(c == IDOK)
		return;
	if(d.topic.IsCursor()) {
		String fn = AppendFileName(
				                   AppendFileName(d.packagedir[d.package.GetCursor()],
				                                  d.group.GetCurrentName() + ".tpp"),
		                           d.topic.GetCurrentName() + ".tpp");
		text = ReadTopic(LoadFile(fn)).title.ToWString();
	}
}
