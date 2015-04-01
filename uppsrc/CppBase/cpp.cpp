#include "CppBase.h"

NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)

void Cpp::Define(const char *s)
{
	CppMacro m;
	String id = m.Define(s);
	if(id.GetCount())
		macro.GetAdd(id) = pick(m);
}

const char *Cpp::SkipString(const char *s)
{
	CParser p(s);
	p.ReadOneString(*s);
	s = p.GetPtr();
	while((byte)*(s - 1) <= ' ')
		s--;
	return s;
}

void Cpp::ParamAdd(Vector<String>& param, const char *s, const char *e)
{
	while(s < e && (byte)*s <= ' ') s++;
	while(e > s && (byte)*(e - 1) <= ' ') e--;
	String h;
	while(s < e) {
		if((byte)*s <= ' ') {
			h.Cat(' ');
			s++;
			while(s < e && (byte)*s <= ' ')
				s++;
		}
		else
		if(*s == '\"' || *s == '\'') {
			const char *q = SkipString(s);
			h.Cat(String(s, q));
			s = q;
		}
		else
			h.Cat(*s++);
	}
	param.Add(h);
}

String Cpp::Expand(const char *s)
{
	StringBuffer r;
	while(*s) {
		if(incomment) {
			if(s[0] == '*' && s[1] == '/') {
				incomment = false;
				s += 2;
				r.Cat("*/");
			}
			else
				r.Cat(*s++);
		}
		else
		if(iscib(*s)) {
			LTIMING("Expand ID");
			const char *b = s;
			s++;
			while(iscid(*s))
				s++;
			String id(b, s);
			LTIMING("Expand ID2");
			if(notmacro.Find(id) < 0) {
				const CppMacro *m = macro.FindPtr(id);
				if(!m) {
					m = FindMacro(id, segment_id);
					//TODO benchmark this
				//	if(m)
				//		macro.Add(id, *m);
				}
				if(m && !id.StartsWith("__$allowed_on_")) {
					LTIMING("Expand macro");
					Vector<String> param;
					const char *s0 = s;
					while(*s && (byte)*s <= ' ')
						s++;
					if(*s == '(') {
						s++;
						const char *b = s;
						int level = 0;
						for(;;)
							if(*s == ',' && level == 0) {
								ParamAdd(param, b, s);
								s++;
								b = s;
							}
							else
							if(*s == ')') {
								s++;
								if(level == 0) {
									ParamAdd(param, b, s - 1);
									break;
								}
								level--;
							}
							else
							if(*s == '(') {
								s++;
								level++;
							}
							else
							if(*s == '\0')
								break;
							else
							if(*s == '\"' || *s == '\'')
								s = SkipString(s);
							else
								s++;
					}
					else
						s = s0; // otherwise we eat spaces after parameterless macro
					usedmacro.FindAdd(id);
					int ti = notmacro.GetCount();
					notmacro.Add(id);
					id = '\x1a' + Expand(m->Expand(param));
					notmacro.Trim(ti);
				}
				else
					notmacro.Add(id);
			}
			r.Cat(id);
		}
		else
		if(s[0] == '/' && s[1] == '*') {
			incomment = true;
			s += 2;
			r.Cat("/*");
		}
		else
		if(s[0] == '/' && s[1] == '/') {
			r.Cat(s);
			break;
		}
		else
			r.Cat(*s++);
	}
	return r;
}

bool Cpp::Preprocess(const String& sourcefile, Stream& in, const String& currentfile,
                     const Index<String> *get_macros)
{
	macro.Clear();
	macro.Reserve(1000);
	segment_id.Clear();
	segment_id.Reserve(100);
	Vector<String> ignorelist = Split("__declspec;__cdecl;"
                                      "__out;__in;__inout;__deref_in;__deref_inout;__deref_out;"
                                      "__AuToQuOtE;__xin;__xout;"
                                      "$drv_group;$allowed_on_parameter",
                                      ';');

	static Index<String> kw;
	ONCELOCK {
		const char **h = CppKeyword();
		while(*h) {
			kw.Add(*h);
			h++;
		}
	}
	notmacro = clone(kw);

	for(int i = 0; i < ignorelist.GetCount(); i++)
		macro.GetAdd(ignorelist[i]).param = ".";
	done = false;
	incomment = false;
	Index<String> visited;
	Do(NormalizePath(sourcefile), in, NormalizePath(currentfile), visited, get_macros);
	return done;
}

void Cpp::Do(const String& sourcefile, Stream& in, const String& currentfile,
             Index<String>& visited, const Index<String> *get_macros)
{
	if(visited.Find(currentfile) >= 0 || visited.GetCount() > 20000)
		return;
	visited.Add(currentfile);
	String current_folder = GetFileFolder(currentfile);
	if(sourcefile != currentfile) {
		const PPFile& pp = GetPPFile(currentfile);
	#ifdef _DEBUG
	//	pp.Dump();
	#endif
		for(int i = 0; i < pp.item.GetCount() && !done; i++) {
			const PPItem& m = pp.item[i];
			if(m.type == PP_DEFINES) {
				LTIMING("PP_DEFINES");
				segment_id.Add(m.segment_id);
			}
			else
			if(m.type == PP_INCLUDE) {
				String s = GetIncludePath(m.text, current_folder, include_path);
				if(s.GetCount())
					Do(sourcefile, in, s, visited, get_macros);
			}
			else
			if(m.type == PP_NAMESPACE)
				namespace_stack.Add(m.text);
			else
			if(m.type == PP_NAMESPACE_END && namespace_stack.GetCount())
				namespace_stack.Drop();
			else
			if(m.type == PP_USING)
				namespace_using.FindAdd(m.text);
		}
		return;
	}
	
	done = true;
	if(get_macros)
		return;
	
	LTIMING("Expand");
	incomment = false;
	StringBuffer result;
	result.Clear();
	result.Reserve(16384);
	int lineno = 0;
	bool incomment = false;
	while(!in.IsEof()) {
		String l = in.GetLine();
		lineno++;
		int el = 0;
		while(*l.Last() == '\\' && !in.IsEof()) {
			el++;
			l.Trim(l.GetLength() - 1);
			l.Cat(in.GetLine());
		}
		RemoveComments(l, incomment);
		CParser p(l);
		if(p.Char('#')) {
			if(p.Id("define")) {
				result.Cat(l + "\n");
				LTIMING("Expand define");
				Define(p.GetPtr());
			}
			else {
				result.Cat('\n');
				if(p.Id("include")) {
					LTIMING("Expand include");
					String hdr = Expand(p.GetPtr());
					String header_path = GetIncludePath(hdr, current_folder, include_path);
					if(header_path.GetCount())
						Do(Null, NilStream(), header_path, visited, NULL);
				}
			}
		}
		else {
			LTIMING("Expand expand");
			result.Cat(Expand(l) + "\n");
		}
		while(el--)
			result.Cat("\n");
	}
	output = result;
}

END_UPP_NAMESPACE
