#include "Server.h"

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
	SQL * Delete(RESULT)
	       .Where(STATUS==WD_INPROGRESS && START < GetSysTime()-int(Ini::max_build_time));
}

void CleanAuth(){
	SQL * Delete(AUTH).Where(VALID < GetSysTime()-600);
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
//	http("cnt", pagesize);
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

bool CheckAuth(Http& http, Sql& sql){
	String nonce = http.GetHeader("wd-nonce");
	sql * Delete(AUTH).Where(NONCE==nonce);
	if(sql.GetRowsProcessed() == 0){
		http << "Auth FAIL";
		http.Response(403,"Auth FAIL (nonce)");
		return false;
	}
	sql * Select(PASSWORD)
	      .From(CLIENT).Where(ID == http.Int("client_id"));
	String pwd;
	sql.Fetch(pwd);
	if (http.GetHeader("wd-auth")!=MD5String(nonce+pwd)) {
		http << "Auth FAIL";
		http.Response(403,"Auth FAIL (auth)");
		return false;
	}
	return true;
}

bool CheckAuth2(Http& http, Sql& sql, int client, const String& action){
	if(http.Int("client_id")!=client && http.Int("client_id")!=0){
		http << "Permission denied";
		http.Response(403,"Permission denied");
		return false;
	}
	if(http["wd_action"] != action) {
		http << "Auth FAIL (action)";
		http.Response(403,"Permission denied");
		return false;
	}
	String nonce = http["wd_nonce"];
	String auth = http["wd_auth"];
	sql * Delete(AUTH).Where(NONCE==nonce);
	if(sql.GetRowsProcessed() == 0){
		http << "Auth FAIL (nonce)";
		http.Response(403,"Permission denied");
		return false;
	}
	sql * Select(PASSWORD)
	      .From(CLIENT).Where(ID == http.Int("client_id"));
	String pwd;
	sql.Fetch(pwd);
	if (auth!=MD5String(nonce+String(http["wd_action"])+pwd)) {
		http << "Auth FAIL (auth)";
		http.Response(403,"Permission denied");
		return false;
	}
	return true;
}

void SendEmail(const String& to, const String& subject, const String& text){
	//TODO
	RLOG("Sending email");
	SaveFile("/tmp/mail","TO:"+to+"\nSUBJECT:"+subject+"\n"+text);
	RLOG("Email sent");
}

ValueArray ParseFilter(const String& filter){
	Vector<String> v = Split(filter,"&");
	ValueArray res;
	if(v.IsEmpty()) {
		res.Add("All the results");
		return res;
	}
	for(int i = 0; i < v.GetCount(); i++){
		if(v[i].StartsWith("author=")){
			v[i].Replace("author=","Author is ");
			res.Add(v[i]);
		} else if(v[i].StartsWith("path=")){
			v[i].Replace("path=","Commit affects path ");
			res.Add(v[i]);
		} else if(v[i].StartsWith("client=")){
			v[i].Replace("client=","");
			Sql sql;
			sql * Select(NAME).From(CLIENT).Where(ID==StrInt(v[i]));
			String name;
			sql.Fetch(name);
			res.Add("Client is " + name);
		} else if(v[i].StartsWith("status=")){
			res.Add(v[i].EndsWith("=ok")?"Only successfull":"Only failed");
		}
	}
	return res;
}

SqlVal SqlEmptyString(){
	static SqlVal s("''",SqlS::HIGH);
	return s;
}
