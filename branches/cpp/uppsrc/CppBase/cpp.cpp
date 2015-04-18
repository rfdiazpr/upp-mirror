#include "CppBase.h"

NAMESPACE_UPP

#define LTIMING(x)  RTIMING(x)
#define LLOG(x)     // DLOG(x)

const char *Cpp::SkipString(const char *s)
{
	CParser p(s);
	try {
		p.ReadOneString(*s);
	}
	catch(CParser::Error) {}
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
			ids.FindAdd(id);
			if(notmacro.Find(id) < 0) {
				const PPMacro *pp = macro.FindPtr(id);
				int segmenti = pp ? segment_id.Find(pp->segment_id) : -1;
				const CppMacro *m = FindMacro(id, segment_id, segmenti);
				if(!m && pp)
					m = &pp->macro;
				if(m && m->IsUndef())
					m = NULL;
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
		if(*s == '\"') {
			const char *e = SkipString(s);
			r.Cat(s, e);
			s = e;
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

Index<String> Cpp::kw;

bool Cpp::Preprocess(const String& sourcefile, Stream& in, const String& currentfile,
                     bool get_macros)
{
	LLOG("===== Preprocess " << sourcefile << " <- " << currentfile);
	LTIMING("Cpp::Preprocess");
	macro.Clear();
	macro.Reserve(1000);
	segment_id.Clear();
	segment_id.Reserve(100);
	Vector<String> ignorelist = Split("__declspec;__cdecl;__attribute__;__stdcall;"
                                      "__out;__in;__inout;__deref_in;__deref_inout;__deref_out;"
                                      "__AuToQuOtE;__xin;__xout;"
                                      "$drv_group;$allowed_on_parameter;__clrcall",
                                      ';');

	for(int i = 0; i < ignorelist.GetCount(); i++) {
		PPMacro& pp = macro.GetAdd(ignorelist[i]);
		pp.macro.param = ".";
		pp.segment_id = -999999999;
	}
	segment_id.Add(-999999999);

	std_macros = macro.GetCount();

	ONCELOCK {
		const char **h = CppKeyword();
		while(*h) {
			kw.Add(*h);
			h++;
		}
	}
	notmacro = clone(kw);

	done = false;
	incomment = false;
	Index<String> visited;
	Do(NormalizePath(sourcefile), in, NormalizePath(currentfile), visited, get_macros);
	return done;
}

void Cpp::DoFlatInclude(const String& header_path)
{
	if(header_path.GetCount()) {
		const PPFile& pp = GetFlatPPFile(header_path);
		LLOG("DoFlatInclude " << header_path << ", " << pp.item.GetCount() << " items");
		for(int i = 0; i < pp.item.GetCount() && !done; i++) {
			const PPItem& m = pp.item[i];
			if(m.type == PP_DEFINES)
				segment_id.FindAdd(m.segment_id);
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
	}
}

void Cpp::Do(const String& sourcefile, Stream& in, const String& currentfile,
             Index<String>& visited, bool get_macros)
{
	if(visited.Find(currentfile) >= 0 || visited.GetCount() > 20000)
		return;
	visited.Add(currentfile);
	String current_folder = GetFileFolder(currentfile);
	bool notthefile = sourcefile != currentfile;
	if(notthefile || get_macros) {
		const PPFile& pp = GetPPFile(currentfile);
		for(int i = 0; i < pp.item.GetCount() && !done; i++) {
			const PPItem& m = pp.item[i];
			if(m.type == PP_DEFINES) {
				LTIMING("PP_DEFINES");
				if(notthefile) // if getting macros, we are interested in included macros only
					segment_id.FindAdd(m.segment_id);
			}
			else
			if(m.type == PP_INCLUDE) {
				String s = GetIncludePath(m.text, current_folder);
				if(s.GetCount()) {
					if(notthefile && IncludesFile(s, sourcefile)) {
						LLOG("Include IN " << s);
						Do(sourcefile, in, s, visited, get_macros);
						RHITCOUNT("Include IN");
					}
					else {
						LLOG("Include FLAT " << s);
						RHITCOUNT("Include FLAT");
						DoFlatInclude(s);
					}
				}
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
		if(sourcefile != currentfile)
			return;
	}
	
	if(!get_macros) {
		LTIMING("Expand");
		incomment = false;
		StringBuffer result;
		result.Clear();
		result.Reserve(16384);
		int lineno = 0;
		bool incomment = false;
		int segment_serial = 0;
		segment_id.Add(--segment_serial);
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
					CppMacro m;
					String id = m.Define(p.GetPtr());
					if(id.GetCount()) {
						PPMacro& pp = macro.Add(id);
						pp.macro = m;
						pp.segment_id = segment_serial;
						notmacro.Trim(kw.GetCount());
					}
				}
				else
				if(p.Id("undef")) {
					result.Cat(l + "\n");
					if(p.IsId()) {
						segment_id.Add(--segment_serial);
						PPMacro& m = macro.Add(p.ReadId());
						m.segment_id = segment_serial;
						m.macro.SetUndef();
						notmacro.Trim(kw.GetCount());
						segment_id.Add(--segment_serial);
					}
				}
				else {
					result.Cat('\n');
					if(p.Id("include")) {
						LTIMING("Expand include");
						DoFlatInclude(GetIncludePath(p.GetPtr(), current_folder));
						segment_id.Add(--segment_serial);
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
	done = true;
}

VectorMap<String, String> Cpp::GetDefinedMacros()
{
	LTIMING("GetDefinedMacros");
	VectorMap<String, String> r;
	for(int i = std_macros; i < macro.GetCount(); i++)
		r.GetAdd(macro.GetKey(i)) << macro[i].macro << "\n";
	for(int i = 0; i < r.GetCount(); i++)
		r[i] = MD5String(r[i]);
	return r;
}

String Cpp::GetIncludedMacroValues(const Vector<String>& m)
{
	String r;
	LTIMING("GetUsedMacroValues");
//	DDUMP(m);
//	DDUMP(segment_id);
	for(int i = 0; i < m.GetCount(); i++) {
		String mm = GetAllMacros(m[i], segment_id);
		if(mm.GetCount())
			r << '#' << m[i] << '\n' << mm << '\n';
	}
//	DLOG("GetUsedMacroValues: " << r);
//	return r;
	return MD5String(r);
}

END_UPP_NAMESPACE
