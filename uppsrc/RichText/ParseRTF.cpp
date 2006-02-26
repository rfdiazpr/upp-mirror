#include "RichText.h"

#define LLOG(x) // LOG(x)

#include <Draw/Draw.h>

RichObject CreateDrawingObject(const Drawing& dwg, Size dot_size, Size out_size = Null);
RichObject CreateImageObject(const Image& img, Size dot_size, Size out_size = Null);

static String FromCString(const char *p, const char **endptr = NULL)
{
	if(endptr) {
		const char *e = p;
		if(*e == '\"')
			e++;
		while(*e && *e != '\"')
			if(*e++ == '\\' && *e)
				e++;
		if(*e == '\"')
			e++;
		*endptr = e;
	}

	try {
		CParser parser(p);
		return parser.ReadOneString();
	}
	catch(Exc e) {
		return Null;
	}
}

class RTFParser
{
public:
	RTFParser(const char *rtf);

	RichText      Run();

private:
	enum TOKEN { T_EOF, T_TEXT, T_COMMAND, T_GROUP, T_END_GROUP };

	void          Flush(bool force, int itap);
	void          OpenTable(int level);
	void          FlushTable(int level);
	TOKEN         Fetch();
	void          Skip();
	TOKEN         Token()                  { if(!is_full) Fetch(); return token; }
	bool          PassIf(bool c)           { is_full &= !c; return c; }
	bool          PassText()               { return PassIf(Token() == T_TEXT); }
	bool          PassGroup()              { return PassIf(Token() == T_GROUP); }
	bool          PassEndGroup()           { return PassIf(Token() == T_END_GROUP || token == T_EOF); }
	bool          PassEndGroup(int level);
	bool          PassCmd(const char *cmd) { return PassIf(Token() == T_COMMAND && command == cmd); }
	bool          PassQ(const char *cmd)   { return PassIf(command == cmd); }

	void          SkipGroup()              { SkipGroup(stack.GetCount()); }
	void          SkipGroup(int level);
	int           Level() const            { return stack.GetCount(); }

	void          ReadItem();
	void          ReadItemGroup(int level);
	void          ReadItemGroup()          { ReadItemGroup(Level()); }
	void          ReadText();
	void          ReadText(const WString& text);
	void          ReadChar(word ch)        { ReadText(WString(ch, 1)); }
	void          ReadCommand();

	void          ReadHeader();
		void          ReadFaceTable();
		void          ReadColorTable();
		void          ReadCharSet();

	void          ReadMisc();
		void          ReadField();
		bool          ReadField(const char *def);
		void          ReadPict();

	void          ReadParaStyle();
	void          ReadTableStyle();
	void          DefaultParaStyle();

	void          ReadCharStyle();

	String        ReadBinHex(char& odd) const;

private:
	const char   *rtf_begin;
	const char   *rtf;

	TOKEN         token;
	bool          is_full;
	bool          new_dest;
	bool          next_command;
	WString       text;
	String        command;
	int           command_arg;

	struct Cell {
		Cell() : end_dots(0), merge_first(false), merge(false) {}
		RichTxt text;
		RichCell::Format format;
		int end_dots;
		bool merge_first;
		bool merge;
	};

	struct TableState {
		TableState() : textcol(0), stylecol(0) { cells.Add(); }

		RichTable::Format tableformat;
		Vector< Array<Cell> > cells;
		int textcol;
		int stylecol;
	};

	struct State {
		String               dest;
		RichPara::Format     format;
		RichPara::CharFormat charformat;
		RichCell::Format     cellformat;
		Rect                 rowmargin;
		Rect                 rowmarginunits;
		Rect                 cellmarginunits;
		int                  uc_value;
		int                  left_margin;
		int                  right_margin;
		int                  first_indent;
		bool                 in_table;
		int                  itap;
		bool                 nestprop;
	};

	Array<State>  stack;
	Array<TableState> table_stack;
	State         state;
	RichPara::CharFormat plain_format;
	RichPara::Format pard_format;
	RichCell::Format std_cell_format;
	byte          charset;
	int           default_font;
	Alignment     tab_align;
	byte          tab_fill;
	Vector<int>   face_table;
	Vector<Color> color_table;
	int           paper_width;
	int           left_margin;
	int           right_margin;

	RichText      output;
	RichPara      para;
};

RichText ParseRTF(const char *rtf) { return RTFParser(rtf).Run(); }

RTFParser::RTFParser(const char *rtf)
: rtf_begin(rtf)
, rtf(rtf)
{
	is_full = false;
	next_command = false;
	default_font = 0;
	charset = CHARSET_WIN1250;
	state.uc_value = 1;
	plain_format.Face(Font::ARIAL).Height(100);
	std_cell_format.align = ALIGN_TOP;
	std_cell_format.margin = Rect(25, 25, 25, 25);
	DefaultParaStyle();
	state.charformat = plain_format;
	tab_align = ALIGN_LEFT;
	tab_fill = 0;
	paper_width = 5100;
	left_margin = right_margin = 750;
}

RichText RTFParser::Run()
{
	if(!PassGroup() || !PassCmd("rtf") || command_arg != 1)
		return output;
	while(Token() != T_EOF)
		ReadItem();
	Flush(false, 1);
	FlushTable(0);
	return output;
}

void RTFParser::FlushTable(int level)
{
	while(table_stack.GetCount() > level) {
		TableState& child = table_stack.Top();
		while(!child.cells.IsEmpty() && child.cells.Top().IsEmpty())
			child.cells.Drop();
		if(child.cells.IsEmpty()) {
			table_stack.Drop();
			continue;
		}
		Index<int> dot_index;
		int pos = child.tableformat.lm;
		dot_index.Add(pos);
		for(int r = 0; r < child.cells.GetCount(); r++) {
			Array<Cell>& rw = child.cells[r];
			int pos = 0;
			for(int c = 0; c < rw.GetCount(); c++)
				dot_index.FindAdd(rw[c].end_dots = pos = max(pos + 100, rw[c].end_dots));
		}
		Vector<int> dot_order = dot_index.PickKeys();
		Sort(dot_order);
		RichTable table;
		if(table_stack.GetCount() == 1)
			child.tableformat.rm = max(paper_width - left_margin - right_margin - dot_order.Top(), 0);
//		child.tableformat.before = state.format.before;
//		child.tableformat.after = state.format.after;
		table.SetFormat(child.tableformat);
		for(int c = 1; c < dot_order.GetCount(); c++)
			table.AddColumn(dot_order[c] - dot_order[c - 1]);
		dot_index = dot_order;
		for(int r = 0; r < child.cells.GetCount(); r++) {
			Array<Cell>& rw = child.cells[r];
			int pos = child.tableformat.lm;
			for(int c = 0; c < rw.GetCount(); c++) {
				Cell& cell = rw[c];
				if(cell.merge)
					continue;
				int vspan = 0;
				if(cell.merge_first) {
					for(int m = r + 1; m < child.cells.GetCount(); m++) {
						const Array<Cell>& mrw = child.cells[m];
						int mc = mrw.GetCount();
						while(--mc >= 0 && mrw[mc].end_dots > cell.end_dots)
							;
						if(mc >= 0 && mrw[mc].end_dots == cell.end_dots && mrw[mc].merge)
							vspan++;
						else
							break;
					}
				}
				int nbegin = dot_index.Find(pos);
				int hspan = dot_index.Find(pos = cell.end_dots) - nbegin - 1;
				hspan = max(hspan, 0);
				vspan = max(vspan, 0);
//				ASSERT(hspan >= 0 && vspan >= 0);
				if(hspan || vspan)
					table.SetSpan(r, nbegin, vspan, hspan);
				table.SetFormat(r, nbegin, cell.format);
				cell.text.Normalize();
				table.SetPick(r, nbegin, cell.text);
			}
		}
		table.Normalize();
		table_stack.Drop();
		if(table_stack.IsEmpty())
			output.CatPick(table);
		else {
			TableState& par = table_stack.Top();
			par.cells.Top().At(par.textcol).text.CatPick(table);
		}
	}
}

void RTFParser::Flush(bool force, int itap)
{
	if(!para.part.IsEmpty() || force) {
		int fi = state.first_indent, li = state.left_margin, ri = state.right_margin;
		if(state.format.bullet != RichPara::BULLET_NONE) {
			li += fi;
			fi = -fi;
		}
		state.format.indent = minmax<int>(fi, 0, MAX_DOTS);
		state.format.lm = minmax<int>(li, 0, MAX_DOTS);
		state.format.rm = minmax<int>(ri, 0, MAX_DOTS);
		para.format = state.format;
		if(state.in_table) {
			FlushTable(itap);
			OpenTable(itap);
			TableState& ts = table_stack[itap - 1];
			ts.cells.Top().At(ts.textcol).text.Cat(para, output.GetStyles());
		}
		else {
			FlushTable(0);
			output.Cat(para);
		}
		para.part.Clear();
	}
	else
		FlushTable(itap);
}

RTFParser::TOKEN RTFParser::Fetch()
{
	is_full = true;
	text = WString();
	if(next_command)
	{
		next_command = false;
		return token = T_COMMAND;
	}

	text = Null;
	command = Null;
	command_arg = Null;

	int skip = 0;
	while(*rtf && *rtf != '{' && *rtf != '}')
	{
		int c = 0, nskip = max(skip - 1, 0);
		if((byte)*rtf < ' ')
			rtf++;
		else if(*rtf != '\\')
			c = ToUnicode(*rtf++, charset);
		else
			switch(rtf++, *rtf++)
			{
				case 0: {
					rtf--;
					break;
				}

				case '{':
				case '}':
				case '\\': {
					c = rtf[-1];
					break;
				}

				case '~': {
					c = 160;
					break;
				}

				case '|':
				case '-':
				case '_':
				case ':': {
					command = String(rtf - 1, 1);
					if(text.IsEmpty())
						return token = T_COMMAND;
					next_command = true;
					return token = T_TEXT;
				}

				case '\'': {
					int c1 = ctoi(*rtf);
					if(c1 < 16) {
						int c2 = ctoi(*++rtf);
						if(c2 < 16) {
							c1 = c1 * 16 + c2;
							rtf++;
						}
						c = ToUnicode(c1, charset);
					}
					break;
				}

				default: {
					if(IsAlpha(*--rtf) || *rtf == '*' && rtf[1] == '\\' && IsAlpha(rtf[2])) {
						new_dest = false;
						if(*rtf == '*') {
							rtf += 2;
							new_dest = true;
						}
						const char *b = rtf;
						while(IsAlpha(*++rtf))
							;
						command = String(b, rtf);
						if(IsDigit(*rtf) || *rtf == '-')
							command_arg = strtol(rtf, (char **)&rtf, 10);
						if(*rtf == ' ')
							rtf++;
						if(command == "uc")
							state.uc_value = command_arg;
						else if(command == "u") {
							c = command_arg;
							nskip = state.uc_value;
						}
						else { // command - quit reading text
							if(text.IsEmpty())
								return token = T_COMMAND;
							next_command = true;
							return token = T_TEXT;
						}
					}
					break;
				}
			}
		if(c && !skip)
			text.Cat(c);
		skip = nskip;
	}

	if(!text.IsEmpty())
		return token = T_TEXT;

	if(*rtf == '{') {
		stack.Add(state);
		rtf++;
		return token = T_GROUP;
	}

	if(*rtf == '}') {
		if(!stack.IsEmpty()) {
			state = stack.Top();
			stack.Drop();
		}
		rtf++;
		return token = T_END_GROUP;
	}

	return token = T_EOF;
}

bool RTFParser::PassEndGroup(int level)
{
	if(Token() == T_EOF)
		return true;
	if(token != T_END_GROUP)
		return false;
	is_full = false;
	return Level() < level;
}

void RTFParser::Skip()
{
	if(token == T_GROUP || token == T_COMMAND && new_dest)
		SkipGroup();
	is_full = false;
}

void RTFParser::SkipGroup(int level)
{
	while(!PassEndGroup(level))
		is_full = false;
}

void RTFParser::ReadItem()
{
	const char *p = rtf;
	if(token == T_COMMAND)
		ReadCommand();
	else if(token == T_TEXT)
		ReadText();
	if(rtf == p && is_full) {
		is_full = false;
		if(token == T_COMMAND && new_dest)
			SkipGroup();
	}
}

void RTFParser::ReadItemGroup(int level)
{
	while(!PassEndGroup(level))
		ReadItem();
}

void RTFParser::ReadText()
{
	if(!IsNull(text))
		ReadText(text);
}

void RTFParser::ReadText(const WString& text)
{
	if(!IsNull(state.dest))
		return;
	LLOG("Output text: <" << FromUnicode(text, charset) << ">, " << state.charformat);
	para.Cat(text, state.charformat);
}

void RTFParser::ReadCommand()
{
	if(Token() == T_COMMAND) ReadHeader();
	if(Token() == T_COMMAND) ReadMisc();
	if(Token() == T_COMMAND) ReadParaStyle();
	if(Token() == T_COMMAND) ReadTableStyle();
	if(Token() == T_COMMAND) ReadCharStyle();
}

void RTFParser::ReadHeader()
{
	if(PassCmd("deff"))
		default_font = command_arg;
	else if(PassQ("fonttbl")) {
		state.dest = command;
		ReadFaceTable();
	}
	else if(PassQ("colortbl")) {
		state.dest = command;
		ReadColorTable();
	}
	else if(PassQ("stylesheet") || PassQ("list") || PassQ("listoverride") || PassQ("info")) {
		state.dest = command;
		SkipGroup();
	}
	else if(Token() == T_COMMAND)
		ReadCharSet();
}

void RTFParser::ReadCharSet()
{
	if(PassQ("ansi")) {}
	else if(PassQ("mac")) {}
	else if(PassQ("pc")) {}
	else if(PassQ("pca")) {}
	else if(PassQ("ansicpg")) {
		static const struct
		{
			int  ansicpg;
			byte charset;
		}
		charsets[] =
		{
			{ 1250, CHARSET_WIN1250 },
			{ 1251, CHARSET_WIN1251 },
			{ 1252, CHARSET_WIN1252 },
			{ 1253, CHARSET_WIN1253 },
			{ 1254, CHARSET_WIN1254 },
			{ 1255, CHARSET_WIN1255 },
			{ 1256, CHARSET_WIN1256 },
			{ 1257, CHARSET_WIN1257 },
		};
		for(int c = 0; c < __countof(charsets); c++)
			if(charsets[c].ansicpg == command_arg) {
				charset = charsets[c].charset;
				break;
			}
	}
}

void RTFParser::ReadFaceTable()
{
	int fx = 0;
	while(!PassEndGroup()) {
		if(!PassGroup()) {
			Skip();
			continue;
		}
		int f = -1, b = Font::ARIAL;
		while(!PassEndGroup()) {
			if(PassCmd("f"))
				fx = command_arg;
			else if(PassCmd("fnil"))
				;
			else if(PassCmd("froman"))
				b = Font::ROMAN;
			else if(PassCmd("fswiss"))
				b = Font::ARIAL;
			else if(PassCmd("fmodern"))
				b = Font::COURIER;
			else if(PassCmd("ftech"))
				b = Font::SYMBOL;
/*			else if(PassText()) {
				String s = FromUnicode(text, charset);
				if(!s.IsEmpty() && *s.Last() == ';')
					s.Trim(s.GetLength() - 1);
				if(!s.IsEmpty())
					f = Font::FindFaceNameIndex(s);
			}
			else if(PassGroup()) {
				int level = Level();
				if(PassCmd("falt") && PassText() && f < 0)
					f = Font::FindFaceNameIndex(FromUnicode(text, charset));
				SkipGroup(level);
			}*/ //Cxl 2005-11-29 - "Arial CE" makes mess here!
			else
				Skip();
		}
		if(fx >= 0 && fx < MAX_FONTS)
		{
//			if(f < 0) // Cxl 2005-11-29
				f = b;
			if(default_font == fx)
				plain_format.Face(f);
			face_table.At(fx++, Font::ARIAL) = f;
		}
	}
}

void RTFParser::ReadColorTable()
{
	int r = Null, g = Null, b = Null;
	for(; !PassEndGroup(); Skip())
		if(PassCmd("red"))
			r = command_arg;
		else if(PassCmd("green"))
			g = command_arg;
		else if(PassCmd("blue"))
			b = command_arg;
		else if(PassText())
		{
			Color c = Null;
			if(!IsNull(r) || !IsNull(g) || !IsNull(b))
				c = Color(Nvl(r, 0), Nvl(g, 0), Nvl(b, 0));
			color_table.Add(c);
		}
}

void RTFParser::ReadMisc()
{
	if(PassQ("field"))
		ReadField();
	else if(PassQ("pict"))
		ReadPict();
	else if(PassQ("endash"))
		ReadChar(0x2013);
	else if(PassQ("emdash"))
		ReadChar(0x2014);
	else if(PassQ("tab"))
		ReadText(WString(9, 1));
	else if(PassQ("enspace"))
		ReadText(WString(" ")); // todo
	else if(PassQ("emspace"))
		ReadText(WString(" ")); // todo
	else if(PassQ("bullet"))
		ReadChar(0x2022);
	else if(PassQ("lquote"))
		ReadChar(0x2018);
	else if(PassQ("rquote"))
		ReadChar(0x2019);
	else if(PassQ("ldblquote"))
		ReadChar(0x201C);
	else if(PassQ("rdblquote"))
		ReadChar(0x201D);
}

void RTFParser::ReadField()
{
	bool ign_rslt = false;
	while(!PassEndGroup())
		if(PassGroup()) {
			int level = Level();
			if(PassCmd("fldinst")) {
				WString source;
				for(; !PassEndGroup(); Skip())
					if(PassText())
						source.Cat(text);
				if(ReadField(FromUnicode(source, charset)))
					ign_rslt = true;
			}
			else if(PassCmd("fldrslt")) {
				if(!ign_rslt)
					ReadItemGroup();
			}
			SkipGroup(level);
		}
		else
			Skip();
}

bool RTFParser::ReadField(const char *p)
{
	Index<String> symdef;
	while(*p)
		if((byte)*p <= ' ')
			p++;
		else if(*p == '\"')
			symdef.Add(FromCString(p, &p));
		else {
			const char *b = p;
			while(*++p && *p != ' ')
				;
			symdef.Add(String(b, p));
		}
	if(symdef.IsEmpty())
		return false;
	if(symdef[0] == "SYMBOL" && symdef.GetCount() >= 2 && IsDigit(*symdef[1])) {
		int code = atoi(symdef[1]);
		int face = -1;
		int height = 0;
		int f = symdef.Find("\\f");
		if(f >= 0 && f + 1 < symdef.GetCount())
			face = Font::FindFaceNameIndex(symdef[f + 1]);
		f = symdef.Find("\\s");
		if(f >= 0 && f + 1 < symdef.GetCount())
			height = PointDots(fround(2 * atof(symdef[f + 1]))) >> 1;
		if(face < 0)
			face = Font::SYMBOL;
		if(height <= 0 || height >= MAX_DOT_HEIGHT)
			height = state.charformat.GetHeight();
		if(code >= 0 && code < 255) {
			state.charformat.Face(face).Height(height);
			ReadText(WString(ToUnicode(code, charset), 1));
			return true;
		}
	}
	return false;
}

void RTFParser::DefaultParaStyle()
{
	state.format = pard_format;
	state.first_indent = state.left_margin = state.right_margin = 0;
	state.cellformat = std_cell_format;
	state.in_table = false;
	state.itap = 1;
	state.nestprop = false;
	state.rowmargin = Rect(25, 25, 25, 25);
	state.cellmarginunits = state.rowmarginunits = Rect(0, 0, 0, 0);
}

void RTFParser::ReadParaStyle()
{
	if(PassQ("par"))
		Flush(true, state.itap);
	else if(PassQ("cell")) {
		Flush(false, 1);
		if(!table_stack.IsEmpty())
			table_stack[0].textcol++;
 	}
 	else if(PassQ("nestcell")) {
 		Flush(false, state.itap);
 		if(state.itap <= table_stack.GetCount())
 			table_stack[state.itap - 1].textcol++;
 	}
	else if(PassQ("pard"))
		DefaultParaStyle();
	else if(PassQ("pntext"))
		SkipGroup();
	else if(PassQ("pn")) {
		SkipGroup();
		state.format.bullet = RichPara::BULLET_ROUND;
	}
	else if(PassQ("ql"))
		state.format.align = ALIGN_LEFT;
	else if(PassQ("qc"))
		state.format.align = ALIGN_CENTER;
	else if(PassQ("qr"))
		state.format.align = ALIGN_RIGHT;
	else if(PassQ("qj"))
		state.format.align = ALIGN_JUSTIFY;
	else if(PassQ("fi"))
		state.first_indent = TwipDots(command_arg);
	else if(PassQ("li"))
		state.left_margin = TwipDots(command_arg);
	else if(PassQ("ri"))
		state.right_margin = TwipDots(command_arg);
	else if(PassQ("sb"))
		state.format.before = TwipDots(command_arg);
	else if(PassQ("sa"))
		state.format.after = TwipDots(command_arg);
	else if(PassQ("widctlpar"))
		state.format.orphan = true;
	else if(PassQ("nowidctlpar"))
		state.format.orphan = false;
	else if(PassQ("tql"))
		tab_align = ALIGN_LEFT;
	else if(PassQ("tqc"))
		tab_align = ALIGN_CENTER;
	else if(PassQ("tqr"))
		tab_align = ALIGN_RIGHT;
	else if(PassQ("tqdec"))
		tab_align = ALIGN_RIGHT; // todo
	else if(PassQ("tldot"))
		tab_fill = 0;
	else if(PassQ("tlhyph"))
		tab_fill = 0;
	else if(PassQ("tlul"))
		tab_fill = 0;
	else if(PassQ("tlth"))
		tab_fill = 0;
	else if(PassQ("tleq"))
		tab_fill = 0;
	else if(PassQ("tx") || PassQ("tb")) { // todo: bar tab ?
		int pos = TwipDotSize(command_arg);
		RichPara::Tab& tab = state.format.tab.Add();
		tab.align = tab_align;
		tab.fillchar = tab_fill;
		tab.pos = pos;
		state.format.SortTabs();
	}
	else if(PassQ("intbl"))
		state.in_table = true;
	else if(PassQ("itap")) {
		state.itap = minmax(command_arg, 1, 10);
		if(table_stack.GetCount() < state.itap)
			OpenTable(state.itap);
	}
}

void RTFParser::ReadCharStyle()
{
	if(PassQ("plain"))
		state.charformat = plain_format;
	else if(PassQ("b"))
		state.charformat.Bold(command_arg != 0);
	else if(PassQ("i"))
		state.charformat.Italic(command_arg != 0);
	else if(PassQ("ul") || PassQ("uld") || PassQ("uldb")
	|| PassQ("uldash") || PassQ("uldashd") || PassQ("uldashdd")
	|| PassQ("ulth") || PassQ("ulw") || PassQ("ulwave"))
		state.charformat.Underline();
	else if(PassQ("ulnone"))
		state.charformat.Underline(false);
	else if(PassQ("strike") || PassQ("strikedl"))
		state.charformat.Strikeout(command_arg != 0);
	else if(PassQ("caps") || PassQ("scaps"))
		state.charformat.capitals = (command_arg != 0);
	else if(PassQ("super") || PassQ("up"))
		state.charformat.sscript = 1;
	else if(PassQ("sub") || PassQ("dn"))
		state.charformat.sscript = 2;
	else if(PassQ("nosupersub"))
		state.charformat.sscript = 0;
	else if(PassQ("f") && command_arg >= 0 && command_arg < face_table.GetCount())
	{
		LLOG("font = " << command_arg << ", face = " << face_table[command_arg]);
		state.charformat.Face(face_table[command_arg]);
	}
	else if(PassQ("fs"))
		state.charformat.Height(PointDotHeight(command_arg));
	else if(PassQ("cf") && command_arg >= 0 && command_arg < color_table.GetCount())
		state.charformat.ink = Nvl(color_table[command_arg], Black);
	else if(PassQ("cb") && command_arg >= 0 && command_arg < color_table.GetCount())
		state.charformat.paper = color_table[command_arg];
	else if(PassQ("lang"))
	{} // state.language = ...
}

void RTFParser::ReadPict()
{
	Size log_size(1, 1), out_size(1, 1);
	Rect crop(0, 0, 0, 0);
	enum BLIPTYPE { UNK_BLIP, EMF_BLIP, PNG_BLIP, JPEG_BLIP, WMF_BLIP, DIB_BLIP };
	BLIPTYPE blip_type = UNK_BLIP;
#ifdef PLATFORM_WIN32
	int wmf_mode = MM_ANISOTROPIC;
#endif
	String blip_data;
	char odd = 0;
	while(!PassEndGroup())
		if(PassText())
			blip_data.Cat(ReadBinHex(odd));
		else if(Token() == T_COMMAND) {
			if(PassQ("picw"))            log_size.cx = minmax<int>(command_arg, 0, 30000);
			else if(PassQ("pich"))       log_size.cy = minmax<int>(command_arg, 0, 30000);
			else if(PassQ("picwgoal"))   out_size.cx = TwipDotSize(command_arg);
			else if(PassQ("pichgoal"))   out_size.cy = TwipDotSize(command_arg);
			else if(PassQ("picscalex"))  {}
			else if(PassQ("picscaley"))  {}
			else if(PassQ("piccropl"))   crop.left   = TwipDotSize(command_arg);
			else if(PassQ("piccropt"))   crop.top    = TwipDotSize(command_arg);
			else if(PassQ("piccropr"))   crop.right  = TwipDotSize(command_arg);
			else if(PassQ("piccropb"))   crop.bottom = TwipDotSize(command_arg);
			else if(PassQ("emfblip"))    blip_type    = EMF_BLIP;
			else if(PassQ("pngblip"))    blip_type    = PNG_BLIP;
			else if(PassQ("jpegblip"))   blip_type    = JPEG_BLIP;
#ifdef PLATFORM_WIN32
			else if(PassQ("wmetafile"))  { blip_type = WMF_BLIP; wmf_mode = command_arg; }
#endif
			else if(PassQ("dibitmap"))   blip_type = DIB_BLIP;
			else Skip();
		}
		else
			Skip();
	DrawingDraw dd(log_size);
#ifdef PLATFORM_WIN32
	if(blip_type == EMF_BLIP || blip_type == WMF_BLIP)
	{
		WinMetaFile wmf;
		if(blip_type == EMF_BLIP)
			wmf = WinMetaFile(SetEnhMetaFileBits(blip_data.GetLength(), blip_data));
		else {
			METAFILEPICT mfp;
			Zero(mfp);
			mfp.mm = wmf_mode;
			mfp.xExt = log_size.cx;
			mfp.yExt = log_size.cy;
			wmf = WinMetaFile(SetWinMetaFileBits(blip_data.GetLength(), blip_data, ScreenInfo(), &mfp));
		}
		wmf.Paint(dd, log_size);
	}
	else
#endif
	if(blip_type == DIB_BLIP || blip_type == PNG_BLIP || blip_type == JPEG_BLIP)
	{
		One<ImageEncoder> encoder;
		switch(blip_type)
		{
		case DIB_BLIP:  encoder = new BmpEncoder; break;
//		case PNG_BLIP:  encoder = new PngEncoder; break;
//		case JPEG_BLIP: encoder = new JpgEncoder; break;
		}
		if(!encoder)
			return;
		Image image = encoder -> LoadImage(blip_data);
		if(!image.IsEmpty())
			dd.DrawImage(log_size, image);
	}
	else
		return;

	para.Cat(CreateDrawingObject(dd, log_size, out_size), state.charformat);
}

String RTFParser::ReadBinHex(char& odd) const
{
	int t = odd;
	byte v = ctoi(odd);
	String out;
	for(const wchar *s = text.Begin(); *s; s++) {
		byte w = (*s >= '0' && *s <= '9' ? *s - '0'
			: *s >= 'A' && *s <= 'F' ? *s - 'A' + 10
			: *s >= 'a' && *s <= 'f' ? *s - 'a' + 10
			: -1);
		if(w < 16)
			if(v >= 16)
			{
				t = *s;
				v = w;
			}
			else
			{
				out.Cat(16 * v + w);
				v = 255;
			}
	}
	odd = (v < 16 ? t : 0);
	return out;
}

void RTFParser::OpenTable(int level)
{
	if(table_stack.GetCount() < level) {
		TableState& ts = table_stack.At(level - 1);
		ts.stylecol = 0;
		state.cellformat = std_cell_format;
	}
}

void RTFParser::ReadTableStyle()
{
	if(PassQ("nesttableprops")) {
		state.nestprop = true;
		return;
	}
	if(PassQ("nonesttables")) {
		SkipGroup();
		return;
	}
	int itap = (state.nestprop ? state.itap : 1);
	if(PassQ("trowd")) {
		OpenTable(itap);
		table_stack[itap - 1].stylecol = 0;
		return;
	}
	if(PassQ("row") && table_stack.GetCount() >= 1) {
		TableState& ts0 = table_stack[0];
		ts0.textcol = ts0.stylecol = 0;
		ts0.cells.Add();
		return;
	}
	if(PassQ("nestrow") && table_stack.GetCount() >= state.itap) {
		TableState& ts = table_stack[state.itap - 1];
		ts.textcol = ts.stylecol = 0;
		ts.cells.Add();
		return;
	}
	if(itap > table_stack.GetCount())
		return;
	TableState& ts = table_stack[itap - 1];
	if(PassQ("trgaph")) {}
	else if(PassQ("trql")) {}
	else if(PassQ("trqr")) {}
	else if(PassQ("trqc")) {}
	else if(PassQ("trleft")) {
		ts.tableformat.lm = TwipDots(command_arg);
	}
	else if(PassQ("trbrdrl")) {}
	else if(PassQ("trbrdrt")) {}
	else if(PassQ("trbrdrr")) {}
	else if(PassQ("trbrdrb")) {}
	else if(PassQ("trbrdrv")) {}
	else if(PassQ("trftsWidth")) {}
	else if(PassQ("trautofit")) {}
	else if(PassQ("trpaddl"))
		state.rowmargin.left = TwipDots(command_arg);
	else if(PassQ("trpaddt"))
		state.rowmargin.top = TwipDots(command_arg);
	else if(PassQ("trpaddr"))
		state.rowmargin.right = TwipDots(command_arg);
	else if(PassQ("trpaddb"))
		state.rowmargin.bottom = TwipDots(command_arg);
	else if(PassQ("trpaddfl"))
		state.rowmarginunits.left = command_arg;
	else if(PassQ("trpaddft"))
		state.rowmarginunits.top = command_arg;
	else if(PassQ("trpaddfr"))
		state.rowmarginunits.right = command_arg;
	else if(PassQ("trpaddfb"))
		state.rowmarginunits.bottom = command_arg;
	else if(PassQ("clpadl"))
		state.cellformat.margin.left = TwipDots(command_arg);
	else if(PassQ("clpadt"))
		state.cellformat.margin.top = TwipDots(command_arg);
	else if(PassQ("clpadr"))
		state.cellformat.margin.right = TwipDots(command_arg);
	else if(PassQ("clpadb"))
		state.cellformat.margin.bottom = TwipDots(command_arg);
	else if(PassQ("clpadfl"))
		state.cellmarginunits.left = command_arg;
	else if(PassQ("clpadft"))
		state.cellmarginunits.top = command_arg;
	else if(PassQ("clpadfr"))
		state.cellmarginunits.right = command_arg;
	else if(PassQ("clpadfb"))
		state.cellmarginunits.bottom = command_arg;
	else if(PassQ("clvertalt")) {}
	else if(PassQ("clbrdrl")) {}
	else if(PassQ("clbrdrt")) {}
	else if(PassQ("clbrdrr")) {}
	else if(PassQ("clbrdrb")) {}
	else if(PassQ("cltxlrtb")) {}
	else if(PassQ("clcbpat")) {
		if(command_arg >= 0 && command_arg < color_table.GetCount())
			state.cellformat.color = color_table[command_arg];
	}
	else if(PassQ("clvmrg"))
		ts.cells.Top().At(ts.stylecol).merge = true;
	else if(PassQ("clvmgf"))
		ts.cells.Top().At(ts.stylecol).merge_first = true;
	else if(PassQ("clftsWidth")) {}
	else if(PassQ("clwWidth")) {}
	else if(PassQ("cellx")) {
		Cell& newcell = ts.cells.Top().At(ts.stylecol++);
		newcell.end_dots = TwipDots(command_arg);
		newcell.format = state.cellformat;
		if(state.cellmarginunits.left == 0) newcell.format.margin.left = state.rowmargin.left;
		if(state.cellmarginunits.top == 0) newcell.format.margin.top = state.rowmargin.top;
		if(state.cellmarginunits.right == 0) newcell.format.margin.right = state.rowmargin.right;
		if(state.cellmarginunits.bottom == 0) newcell.format.margin.bottom = state.rowmargin.bottom;
		state.cellformat = std_cell_format;
	}
	else if(PassQ("clvertalt"))
		state.cellformat.align = ALIGN_TOP;
	else if(PassQ("clvertalc"))
		state.cellformat.align = ALIGN_CENTER;
	else if(PassQ("clvertalb"))
		state.cellformat.align = ALIGN_BOTTOM;
}
