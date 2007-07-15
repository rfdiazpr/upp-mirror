#include "NText.h"

bool operator==(const Vector<Paragraph::Tab>& a, const Vector<Paragraph::Tab>& b)
{
	if(a.GetCount() != b.GetCount()) return false;
	for(int i = 0; i < a.GetCount(); i++)
		if(a[i].pos != b[i].pos)
			return false;
	return true;
}

void Paragraph::CharFormat::SetDefault()
{
	(Font &)*this = Arial(120);
	ink = SBlack;
	paper = Null;
	language = 0;
	link = Null;
}

void Paragraph::Format::SetDefault()
{
	CharFormat::SetDefault();
	align = ALIGN_LEFT;
	before = lm = rm = indent = after = 0;
	bullet = 0;
	keep = newpage = false;
	tab.Clear();
}

void Paragraph::Charformat(Stream& out, Paragraph::CharFormat o, Paragraph::CharFormat n)
{
	if(o.IsBold() != n.IsBold()) out.Put(BOLD);
	if(o.IsItalic() != n.IsItalic()) out.Put(ITALIC);
	if(o.IsUnderline() != n.IsUnderline()) out.Put(UNDERLINE);
	if(o.GetFace() != n.GetFace()) {
		out.Put(FACE);
		out.PutW(n.GetFace());
	}
	if(o.GetHeight() != n.GetHeight()) {
		out.Put(HEIGHT);
		out.PutW(n.GetHeight());
	}
}

void Paragraph::Cat(const String& s, Paragraph::CharFormat f)
{
	part.Add();
	part.Top().text = s;
	part.Top().format = f;
	(CharFormat&)format = f;
}

struct TabLess {
	bool operator () (const Paragraph::Tab& a, const Paragraph::Tab& b) const {
		return a.pos == b.pos ? a.align < b.align : a.pos < b.pos;
	}
};

void Paragraph::Format::SortTabs()
{
	Sort(tab, TabLess());
}

String Paragraph::Pack(const Paragraph::Format& style)
{
	StringStream out;

	format.SortTabs();

	word pattr = 0;
	if(format.align != style.align)     pattr |= 1;
	if(format.before != style.before)   pattr |= 2;
	if(format.lm != style.lm)           pattr |= 4;
	if(format.indent != style.indent)   pattr |= 8;
	if(format.rm != style.rm)           pattr |= 0x10;
	if(format.after != style.after)     pattr |= 0x20;
	if(format.bullet != style.bullet)   pattr |= 0x40;
	if(format.keep != style.keep)       pattr |= 0x80;
	if(format.newpage != style.newpage) pattr |= 0x100;
	if(format.tab != style.tab)         pattr |= 0x8000;
	out.PutW(pattr);
	if(pattr & 1)      out.PutW(format.align);
	if(pattr & 2)      out.PutW(format.before);
	if(pattr & 4)      out.PutW(format.lm);
	if(pattr & 8)      out.PutW(format.indent);
	if(pattr & 0x10)   out.PutW(format.rm);
	if(pattr & 0x20)   out.PutW(format.after);
	if(pattr & 0x40)   out.PutW(format.bullet);
	if(pattr & 0x80)   out.Put(format.keep);
	if(pattr & 0x100)  out.Put(format.newpage);
	if(pattr & 0x8000) {
		out.PutW(format.tab.GetCount());
		for(int i = 0; i < format.tab.GetCount(); i++) {
			const Paragraph::Tab& w = format.tab[i];
			out.PutW(w.pos);
			out.PutW(w.align);
		}
	}
	if(part.GetCount() == 0)
		Charformat(out, style, format);
	else {
		CharFormat cf = style;
		for(int i = 0; i < part.GetCount(); i++) {
			Charformat(out, cf, part[i].format);
			cf = part[i].format;
			out.Put(ToCharset(part[i].text, CHARSET_UTF8));
		}
	}
	String r = out;
	r.Shrink();
	return r;
}

void Paragraph::Unpack(const String& data, const Paragraph::Format& style)
{
	part.Clear();
	StringStream in(data);
	format = style;
	word pattr = in.GetW();

	if(pattr & 1)      format.align = in.GetW();
	if(pattr & 2)      format.before = in.GetW();
	if(pattr & 4)      format.lm = in.GetW();
	if(pattr & 8)      format.indent = in.GetW();
	if(pattr & 0x10)   format.rm = in.GetW();
	if(pattr & 0x20)   format.after = in.GetW();
	if(pattr & 0x40)   format.bullet = in.GetW();
	if(pattr & 0x80)   format.keep = in.Get();
	if(pattr & 0x100)  format.newpage = in.Get();
	if(pattr & 0x8000) {
		format.tab.Clear();
		int n = in.GetW();
		format.tab.Reserve(n);
		for(int i = 0; i < n; i++) {
			Paragraph::Tab& w = format.tab.Add();
			w.pos = in.GetW();
			w.align = in.GetW();
		}
	}
	part.Add();
	part.Top().format = format;
	while(!in.IsEof())
		if(in.Term() < 32) {
			do
				switch(in.Get()) {
				case BOLD:
					format.Bold(!format.IsBold());
					break;
				case ITALIC:
					format.Italic(!format.IsItalic());
					break;
				case UNDERLINE:
					format.Underline(!format.IsUnderline());
					break;
				case FACE:
					format.Face(in.GetW());
					break;
				case HEIGHT:
					format.Height(in.GetW());
					break;
				}
			while(in.Term() < 32);
			if(part.Top().text.GetLength())
				part.Add();
			part.Top().format = format;
		}
		else
			part.Top().text.Cat(in.GetUtf8());
	if(part.Top().text.GetLength() == 0)
		part.Drop();
}

void Paragraph::Dump()
{
	LOG("Paragraph dump" << BeginIndent);
	LOG("BEFORE: " << format.before);
	LOG("INDENT: " << format.indent);
	LOG("LM: " << format.lm);
	LOG("RM: " << format.rm);
	LOG("AFTER: " << format.after);
	LOG("KEEP: " << format.keep);
	LOG("NEWPAGE: " << format.newpage);
	LOG("BULLET: " << format.bullet);
	LOG("FONT: " << ::Dump(format));
	int i;
	for(i = 0; i < format.tab.GetCount(); i++)
		LOG("TAB " << format.tab[i].pos << " : " << format.tab[i].align);
	for(i = 0; i < part.GetCount(); i++)
		LOG("Part[" << i << "] = \"" << ToCharset(part[i].text, CHARSET_WIN1250) << "\" "
		    << ::Dump(part[i].format)) << " charset:" << part[i].text.GetCharset();
	LOG("---------" << EndIndent);
}
