#include "ide.h"

#ifdef PLATFORM_WIN32
#include "shellapi.h"
#endif

#define TOPICFILE <ide/app.tpp/all.i>
#include <Core/topic_group.h>
#define TOPICFILE <Esc/srcdoc.tpp/all.i>
#include <Core/topic_group.h>

Size MakeLogo(Ctrl& parent, Array<Ctrl>& ctrl)
{
	Image logo = IdeImg::logo();
	Size  isz = logo.GetSize();
	ImageCtrl& l = ctrl.Create<ImageCtrl>();
	Label& v = ctrl.Create<Label>();
	l.SetImage(logo);
	Size sz = Size(isz.cx, isz.cy/* + 100*/);
	v = IDE_VERSION;
	v.LeftPos(300, 100).TopPos(90, 40);
	if(sizeof(void *) == 8) {
		v = IDE_VERSION " (64 bit)";
		v.RightPos(3, Ctrl::MINSIZE).TopPos(90, 40);
	}
	l.Add(v);
	v.SetFont(Arial(20));
	v.SetInk(Blend(Gray, Blue));
	parent.Add(ctrl.Create<StaticRect>().Color(White).SizePos());
	parent.Add(l.TopPos(0, isz.cy).LeftPos(0, isz.cx));
/*	parent.Add(ctrl.Create<StaticRect>().Color(Blue).LeftPos(2, isz.cx - 4).TopPos(isz.cy, 1));
	RichTextView& w = ctrl.Create<RichTextView>();
	w.SetQTF(GetTopic("ide/app/Sponsor$en-us"));
	w.PageWidth(4000);
	w.NoSb();
	w.SetFrame(NullFrame());
	parent.Add(w.TopPos(isz.cy + 3, 99).LeftPos(0, isz.cx));*/
	return sz;
}

struct Splash : Ctrl {
	Array<Ctrl> ctrl;

	Splash() {
		SetRect(GetWorkArea().CenterRect(MakeLogo(*this, ctrl) + 2));
		SetFrame(BlackFrame());
	}
};

void HideSplash()
{
	if(Single<Splash>().IsOpen())
		Single<Splash>().Close();
}

void ShowSplash()
{
	Single<Splash>().PopUp(NULL, false, false);
	SetTimeCallback(750, callback(HideSplash));
}

bool IsSplashOpen()
{
	return Single<Splash>().IsOpen();
}

struct AboutDlg : TopWindow {
	Array<Ctrl>  ctrl;
	RichTextView about;

	typedef AboutDlg CLASSNAME;

	AboutDlg() {
		Size isz = MakeLogo(*this, ctrl);
		SetRect(0, 0, 1000, isz.cy);
		about.SetQTF(GetTopic("ide/app/About$en-us"), Zoom(130, 1024));
		about.SetZoom(Zoom(1, 1));
		about.RightPos(0, 1000 - isz.cx).VSizePos();
		about.HMargins(4);
		about.SetFrame(NullFrame());
		Background(PaintRect(ColorDisplay(), SWhite()));
		Add(about);
		Title("About TheIDE");
	}
};

void Ide::About()
{
	AboutDlg().Execute();
}

void Ide::SyncDocTree()
{
	SyncRefs();
	doc.ClearTree();
	VectorMap<String, VectorMap<String, Vector<String> > > map;
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		String package = wspc[i];
		String pdir = PackageDirectory(package);
		for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff; ff.Next())
			if(ff.IsFolder()) {
				String group = GetFileTitle(ff.GetName());
				String dir = AppendFileName(pdir, ff.GetName());
				for(FindFile ft(AppendFileName(dir, "*.tpp")); ft; ft.Next())
					if(ft.IsFile())
						map.GetAdd(group).GetAdd(package).Add(GetFileTitle(ft.GetName()));
			}
	}
	for(int i = 0; i < map.GetCount(); i++) {
		TopicLink tl;
		tl.group = map.GetKey(i);
		VectorMap<String, Vector<String> >& group = map[i];
		int gid = doc.AddTree(0, Null, "\1" + tl.group, tl.group);
		for(int i = 0; i < group.GetCount(); i++) {
			Vector<String>& package = group[i];
			int q = FindIndex(package, "index$en-us");
			tl.package = group.GetKey(i);
			int pid;
			if(q >= 0) {
				tl.topic = "index$en-us";
				pid = doc.AddTree(gid, TopicImg::Package(), '\2' + TopicLinkString(tl),
				                  group.GetKey(i));
			}
			else
				pid = doc.AddTree(gid, TopicImg::Package(), Null, group.GetKey(i));
			for(int i = 0; i < package.GetCount(); i++) {
				tl.topic = package[i];
				String p = TopicLinkString(tl);
				String t = GetTopicTitle(p);
				doc.AddTree(pid, TopicImg::Topic(), p, t);
			}
		}
	}
	doc.FinishTree();
//	SetBar();
}

void Ide::ShowTopics()
{
	if(!doc.IsOpen()) {
		doc.OpenMain();
		SyncDocTree();
	}
	else
		doc.SetForeground();
}

void Ide::RefreshBrowser()
{
	browser.Refresh();
	SyncDocTree();
}

void Ide::ShowTopic(String link)
{
	ShowTopics();
	doc.GoTo(link + '#' + browser.GetItem());
}

Topic TopicWindow::AcquireTopic(const String& t)
{
	String topic = t;
	if(*topic == '\1') {
		Topic t;
		t.title = "Summary";
		String group = topic.Mid(1);
		const Workspace& wspc = GetIdeWorkspace();
		for(int i = 0; i < wspc.GetCount(); i++) {
			String package = wspc[i];
			String path = AppendFileName(AppendFileName(PackageDirectory(wspc[i]), group + ".tpp"), "index$en-us.tpp");
			if(FileExists(path)) {
				t.text << "[R5* " << wspc[i] << "]&" << ReadTopic(LoadFile(path)).text << "&&&";
			}
		}
		return t;
	}
	if(*topic == '\2')
		topic = topic.Mid(1);
	TopicLink tl = ParseTopicLink(topic);
	if(!IsNull(tl.package)) {
		String path = AppendFileName(
						AppendFileName(PackageDirectory(tl.package), tl.group + ".tpp"),
						tl.topic + ".tpp");
		return ReadTopic(LoadFile(path));
	}
	return Topic();
}

void TopicWindow::OpenTopic()
{
	WhenTopic();
}

void  TopicWindow::BarEx(Bar& bar)
{
	bar.GapRight();
	bar.Add(IdeKeys::AK_TOPICEDITOR, TopicImg::Topic(), THISBACK(OpenTopic));
}

TopicWindow::TopicWindow()
{
	Icon(IdeImg::doc());
}

void Ide::OpenHelp()
{
	if(!help.IsOpen())
		help.OpenMain();
	else
		help.SetForeground();
	help.GoTo("topic://ide/app/HelpPage$en-us");
}
