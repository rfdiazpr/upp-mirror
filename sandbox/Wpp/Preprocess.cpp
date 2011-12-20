#include "Skylark.h"

String template_path;

void SetTemplatePath(const char *path)
{
	template_path = path;
}

String LoadTemplate(const char *file, const char *search_path)
{
	String path = GetFileOnPath(file, search_path);
	if(IsNull(path))
		return Null;
	FileIn in(path);
	String r;
	while(in && !in.IsEof()) {
		String line = in.GetLine();
		CParser p(line);
		if(p.Char('#') && p.Id("include")) {
			String file = p.GetPtr();
			int q = file.Find(' ');
			if(q < 0)
				q = file.Find('\t');
			if(q >= 0)
				file = file.Mid(0, q);
			r << LoadTemplate(file, GetFileFolder(path) + ';' + search_path);
		}
		else
			r << line;
		r << "\r\n";
	}
	return r;
}

String LoadTemplate(const char *file)
{
	return LoadTemplate(file, template_path);
}

VectorMap<String, String> GetTemplateDefs(const char *file)
{
	String s = LoadTemplate(file);
	VectorMap<String, String> def;
	int ti = def.FindAdd("MAIN");
	StringStream ss(s);
	while(!ss.IsEof()) {
		String line = ss.GetLine();
		CParser p(line);
		if(p.Char('#') && p.Id("define")) {
			String id = "MAIN";
			if(p.IsId())
				id = p.ReadId();
			ti = def.FindAdd(id);
			def[ti].Clear();
			def[ti] << p.GetPtr();
		}
		else
			def[ti] << line << "\r\n";		
	}
	return def;
}

int CharFilter20toHash(int c)
{
	return c == 20 ? '#' : c;
}

int CharFilterIsCrLf(int c)
{
	return c == '\r' || c == '\n' ? c : 0;
}

String GetPreprocessedTemplate(const String& name)
{
	String id = "MAIN";
	String file = name;
	int q = file.Find(':');
	if(q >= 0) {
		id = file.Mid(q + 1);
		file = file.Mid(0, q);
	}
	Index<String> expanded;
	VectorMap<String, String> def = GetTemplateDefs(file);
	String r = def.Get(id, Null);
	bool again;
	do {
		again = false;
		String rr;
		const char *s = ~r;
		for(;;) {
			const char *q = strchr(s, '#');
			if(q) {
				rr.Cat(s, q);
				CParser p(q + 1);
				if(p.Char('#') || !p.IsId())
					rr << (char)20;
				else {
					String id = p.ReadId();
					if(expanded.Find(id) >= 0)
						rr.Cat(q, p.GetSpacePtr());
					else {
						rr.Cat(def.Get(id, Null));
						expanded.Add(id);
						again = true;
					}
				}
				s = p.GetSpacePtr();
			}
			else {
				rr.Cat(s, r.End());
				break;
			}
		}
		r = rr;
	}
	while(again);
	r = Filter(r, CharFilter20toHash);
	return Join(Split(r, CharFilterIsCrLf), "\r\n");
}
