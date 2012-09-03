#include "Server.h"

namespace Upp {
namespace Ini {
	INI_STRING(svn, ".", "URL/path of the svn repository");
	INI_INT(max_build_time, 86400, "Number of seconds before an 'In progress' record is deleted from results");
}
}

struct SvnLog{
	Vector<String> dirs;
	String author;
	Time date;
	String msg;
	int revision;
	void Load(String& log);
	void Store();
	void Clear();
	String AffectedPath();
	String ToString() const;
	SvnLog():msg("None"){};
};

Time XmlToTime(const String& text) {
	Time var;
	if(text.GetCount() > 15) {
		var.year = ScanInt(text.Left(4));
		var.month = ScanInt(text.Mid(5, 2));
		var.day = ScanInt(text.Mid(8, 2));
		var.hour = ScanInt(text.Mid(11, 2));
		var.minute = ScanInt(text.Mid(14, 2));
		var.second = ScanInt(text.Mid(17,2));
		if(var.IsValid())
			return var;
	}
	var = Null;
}

void SvnLog::Load(String& log) {
	Clear();
	XmlParser p(log);
	while(!p.End()){
		if(p.Tag("logentry")) {
			revision=p.Int("revision");
		} else if (p.Tag("author")){
			author = p.ReadText();
			p.SkipEnd();
		} else if (p.Tag("date")){
			date=XmlToTime(p.ReadText());
			p.SkipEnd();
		} else if (p.Tag("paths")){
			while(p.Tag("path")){
				String file = p.ReadText();
				dirs.Add(GetFileDirectory(file));
				p.SkipEnd();
			}
			p.SkipEnd();
		} else if (p.Tag("msg")){
			msg = p.ReadText();
			if(IsNull(msg))
				msg = "None";
			p.SkipEnd();
		} else {
			p.Skip();
		}
	}
}

void SvnLog::Store() {
	Sql sql;
	sql * Insert(WORK)(REVISION,revision)
	                  (DT,date)
	                  (AUTHOR,author)
	                  (MSG, msg)
	                  (PATH, AffectedPath());
}

void SvnLog::Clear() {
	author = msg = "";
	date = Null;
	dirs.Clear();
	revision = -1;
}

String SvnLog::AffectedPath() {
	for(int i = 0; i < dirs.GetCount(); i++){
		for(int j = i+1; j < dirs.GetCount(); j++){
			while(!dirs[j].StartsWith(dirs[i])){
				dirs[i] = GetFileDirectory(dirs[i].Left(dirs[i].GetCount()-1));
				if (dirs.IsEmpty())
					return "/";
			}
			dirs[j] = dirs[i];
		}
	}
	return dirs[0];
}

String SvnLog::ToString() const {
	String s;
	return s.Cat()<<date<<" "<<author<<": "<<msg;
}

void UpdateLogs(){
	String cmd = "svn log --xml --verbose --incremental --revision " + IntStr(lastrev()+1) + " " + Ini::svn;
	String xml;
	SvnLog svnlog;
	while(Sys(cmd, xml) == 0){
		svnlog.Load(xml);
		svnlog.Store();
		lastrev()++;
		cmd = "svn log --xml --verbose --incremental --revision " + IntStr(lastrev()+1) + " " + Ini::svn;
	}
}

void CleanResults(){
	Sql sql;
	sql * Delete(RESULT)
	       .Where(STATUS==WD_INPROGRESS && START < GetSysTime()-int(Ini::max_build_time));
}

VectorMap<String,int> Paging(Http& http){
	int count = 3;
	int pagesize = max(min(Nvl(http.Int("cnt"), Nvl(http.Int(".cnt"), 10)),100),1);
	http.SessionSet("cnt",pagesize);
	int last = lastrev();
	int current = Nvl(http.Int("rev"), last);
	
	ValueArray va;
	ValueMap vm;
	for(int i = min(current+count*pagesize, last); i > current+pagesize-1; i-=pagesize){
		vm.Set("REV", i);
		vm.Set("TEXT", Format("[%i-%i]", min(last, i), i-pagesize+1));
		va.Add(vm);
	}
	if(current < last){
		vm.Set("REV", current + pagesize);
		vm.Set("TEXT", "< Newer");
		va.Add(vm);
	}
	if(current > pagesize){
		vm.Set("REV", current - pagesize);
		vm.Set("TEXT", "Older >");
		va.Add(vm);
	}
	for(int i = current - pagesize; i > max(current-(count+1)*pagesize, 0); i-=pagesize){
		vm.Set("REV", i);
		vm.Set("TEXT", Format("[%i-%i]", i, max(i-pagesize+1,0)));
		va.Add(vm);
	}
	http("PAGING", va);
	VectorMap<String,int> result;
	result.Add("min", current-pagesize+1);
	result.Add("max", current);
	result.Add("offfset", current-pagesize);
	result.Add("pagesize", pagesize);
	result.Add("current", current);
	return result;
}

bool CheckLocal(Http& http){
	if(http.GetHeader("host").StartsWith("localhost")
	 ||http.GetHeader("host").StartsWith("127.0.0.1"))
		return true;
	http.Response(403, "Forbiden");
	return false;
}

int& lastrev(){
	static Time last(Null);
	static int rev(0);
	if(GetSysTime() - last > 60){
		Sql sql;
		sql * Select(REVISION).From(WORK).OrderBy(Descending(REVISION)).Limit(1);
		if(sql.Fetch()){
			rev = sql[0];
			last = GetSysTime();
		} else {
			rev = 0;
			last = Null;
		}
	}
	return rev;
};
