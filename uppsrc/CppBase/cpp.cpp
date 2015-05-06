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
	const char *l0 = s;
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
			const char *bid = s;
			s++;
			while(iscid(*s))
				s++;
			String id(bid, s);
			DLOG("Expand " << id);
			LTIMING("Expand ID2");
			ids.FindAdd(id);
			if(notmacro.Find(id) < 0) {
				DLOG("Expand2 " << id);
				const PPMacro *pp = macro.FindLastPtr(id);
				int segmenti = pp ? segment_id.Find(pp->segment_id) : -1;
				const CppMacro *m = FindMacro(id, segment_id, segmenti);
				if(!m && pp)
					m = &pp->macro;
				if(m && m->IsUndef()) DLOG("UNDEF"),
					m = NULL;
				if(m) {
					DLOG("Expand3 " << id);
					LTIMING("Expand macro");
					Vector<String> param;
					bool function_like = false;
					if(m->param.GetCount()) {
						const char *s0 = s;
						while(*s && (byte)*s <= ' ')
							s++;
						if(*s == '(') {
							function_like = true;
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
								if(*s == '\0') { // macro use spread into more lines
									if(bid == l0) // begin of line
										prefix_macro = bid;
									else
										prefix_macro = String(' ', 1) + bid; // do not want to emit grounding in body
									return r;
								}
								else
								if(*s == '\"' || *s == '\'')
									s = SkipString(s);
								else
									s++;
						}
					}
					if(!!m->param.GetCount() == function_like) {
						int ti = notmacro.GetCount();
						Vector<String> eparam;
						eparam.SetCount(param.GetCount());
						for(int i = 0; i < param.GetCount(); i++)
							eparam[i] = Expand(param[i]);
						notmacro.Add(id);
						id = '\x1f' + Expand(m->Expand(param, eparam)); // \x1f is info for Pre that there was a macro expansion
						notmacro.Trim(ti);
					}
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

	const Vector<String>& ignorelist = GetIgnoreList();

	for(int i = 0; i < ignorelist.GetCount(); i++) {
		PPMacro h;
		PPMacro& pp = macro.GetAdd(h.macro.Define(ignorelist[i]));
		pp = h;
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
	DLOG("Flat include " << header_path);
	if(header_path.GetCount()) {
		const PPFile& pp = GetFlatPPFile(header_path);
		LLOG("DoFlatInclude " << header_path << ", " << pp.item.GetCount() << " items");
		for(int i = 0; i < pp.item.GetCount() && !done; i++) {
			const PPItem& m = pp.item[i];
			if(m.type == PP_DEFINES)
				segment_id.FindAdd(m.segment_id);
			else
			if(m.type == PP_NAMESPACE) {
			//	namespace_stack.Add(m.text); we are ignoring namespace in included files
			//	DLOG("flat pp namespace " << m.text << " " << namespace_stack);
			}
			else
			if(m.type == PP_NAMESPACE_END && namespace_stack.GetCount()) {
			//	namespace_stack.Drop();
			//	DLOG("flat pp end namespace " << namespace_stack);
			}
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
	DDUMP(currentfile);
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
						DLOG("Include IN " << s);
						Do(sourcefile, in, s, visited, get_macros);
						RHITCOUNT("Include IN");
					}
					else {
						DLOG("Include FLAT " << s);
						RHITCOUNT("Include FLAT");
						DoFlatInclude(s);
					}
				}
			}
			else
			if(m.type == PP_NAMESPACE) {
				namespace_stack.Add(m.text);
				DLOG("pp namespace " << m.text << " " << namespace_stack);
			}
			else
			if(m.type == PP_NAMESPACE_END && namespace_stack.GetCount()) {
				namespace_stack.Drop();
				DLOG("pp end namespace " << namespace_stack);
			}
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
		prefix_macro.Clear();
		StringBuffer result;
		result.Clear();
		result.Reserve(16384);
		int lineno = 0;
		bool incomment = false;
		int segment_serial = 0;
		segment_id.Add(--segment_serial);
		while(!in.IsEof()) {
			String l = prefix_macro + in.GetLine();
			prefix_macro.Clear();
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
