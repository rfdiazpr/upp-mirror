#include "Browser.h"
#include <Topic/Topic.h>

VectorMap< String, Vector<RefInfo> >& ref_base()
{
	static VectorMap< String, Vector<RefInfo> > ref;
	return ref;
}

VectorMap<String, String>& topic_title()
{
	static VectorMap<String, String> title;
	return title;
}

VectorMap<String, Time>& ref_fileset()
{
	static VectorMap<String, Time> set;
	return set;
}

void LoadRefs(VectorMap< String, Vector<RefInfo> >& base, VectorMap<String, String>& title_map,
              const char *path, const String& link)
{
	FileIn in(path);
	if(!in)
		return;
	try {
		String ln = in.GetLine();
		CParser p(ln);
		p.PassId("TITLE");
		p.PassChar('(');
		String title = p.ReadString();
		title_map.GetAdd(link) = title;
		p.PassChar(')');
		for(;;) {
			ln = in.GetLine();
			CParser p(ln);
			if(!p.Id("REF"))
				break;
			p.PassChar('(');
			String ref = p.ReadString();
			RefInfo& r = base.GetAdd(ref).Add();
			r.link = link;
			r.title = title;
			p.PassChar(')');
		}
	}
	catch(CParser::Error&) {}
}

void SyncRefs()
{
	VectorMap<String, Time>& set = ref_fileset();
	VectorMap<String, Time> nset;
	Index<String> unchanged;
	const Workspace& wspc = GetIdeWorkspace();
	VectorMap< String, Vector<RefInfo> > base;
	VectorMap<String, String> title;
	Progress pi;
	pi.SetTotal(wspc.GetCount());
	for(int i = 0; i < wspc.GetCount(); i++) {
		pi.Step();
		const Package& pk = wspc.GetPackage(i);
		String package = wspc[i];
		String pdir = PackageDirectory(package);
		TopicLink tl;
		tl.package = package;
		for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff; ff.Next()) {
			if(ff.IsFolder()) {
				String group = GetFileTitle(ff.GetName());
				tl.group = group;
				String dir = AppendFileName(pdir, ff.GetName());
				for(FindFile ft(AppendFileName(dir, "*.tpp")); ft; ft.Next()) {
					if(ft.IsFile()) {
						String path = AppendFileName(dir, ft.GetName());
						tl.topic = GetFileTitle(ft.GetName());
						String link = TopicLinkString(tl);
						int q = set.Find(path);
						if(q < 0 || ft.GetLastWriteTime() != set[q])
							LoadRefs(base, title, path, link);
						else
							unchanged.Add(link);
						nset.Add(path, ft.GetLastWriteTime());
					}
				}
			}
		}
	}
	set = nset;
	VectorMap< String, Vector<RefInfo> >& obase = ref_base();
	VectorMap<String, String>& otitle = topic_title();
	for(int i = 0; i < otitle.GetCount(); i++)
		if(unchanged.Find(otitle.GetKey(i)))
			title.GetAdd(otitle.GetKey(i)) = otitle[i];
	for(int i = 0; i < obase.GetCount(); i++) {
		String ref = obase.GetKey(i);
		const Vector<RefInfo>& r = obase[i];
		for(int j = 0; j < r.GetCount(); j++)
			if(unchanged.Find(r[j].link) >= 0)
				base.GetAdd(ref).Add() = r[j];
	}
	obase = base;
	otitle = title;
}

void SyncRefsFile(const String& path, const String& link)
{
	VectorMap< String, Vector<RefInfo> >& base = ref_base();
	if(base.GetCount() == 0)
		return;
	for(int i = 0; i < base.GetCount(); i++) {
		String ref = base.GetKey(i);
		Vector<RefInfo>& r = base[i];
		int j = 0; 
		while(j < r.GetCount()) {
			if(r[j].link == link)
				r.Remove(j);
			else
				j++;
		}
	}
	topic_title().RemoveKey(link);
	LoadRefs(base, topic_title(), path, link);
}

const Vector<RefInfo>& GetRefInfo(const String& ref)
{
	if(ref_base().GetCount() == 0)
		SyncRefs();
	static Vector<RefInfo> empty;
	return ref_base().Get(ref, empty);
}

String GetTopicTitle(const String& link)
{
	return topic_title().Get(link, Null);
}
