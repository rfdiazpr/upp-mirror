#include "Debuggers.h"

void DbgDisas::MouseWheel(Point, int zdelta, dword)
{
	sb.Wheel(zdelta);
}

Size DbgDisas::GetBox() const
{
	return ScreenInfo().GetTextSize("12345678", Courier(12));
}

void DbgDisas::Layout()
{
	sb.SetPage(GetSize().cy / GetBox().cy);
}

void DbgDisas::Paint(Draw& w)
{
	Size sz = GetSize();
	Size box = GetBox();
	int nl = sz.cy / box.cy + 1;
	int i = sb;
	int y = 0;
	while(i < inst.GetCount() && y < sz.cy) {
		Inst& n = inst[i];
		Color ink = HasFocus() && i == cursor ? SWhite : SBlack;
		int x = 0;
		w.DrawRect(0, y, sz.cx, box.cy, i == cursor ? HasFocus() ? SBlue : SLtGray : SWhite);
		if(sz.cx > 3 * box.cx) {
			w.DrawText(0, y, Sprintf("%08X", addr[i]), Courier(12),
			           HasFocus() && i == cursor ? SWhite
			                                     : taddr.Find(addr[i]) >= 0 ? SLtRed : SBlack);
			x += box.cx;
		}
		if(i == ip)
			DrawHighlightImage(w, x + 2, y + (box.cy - 12) / 2, ipimg);
		x += 18;
		w.DrawText(x, y, n.code, StdFont(), ink);
		w.DrawText(x + codecx, y, n.args, StdFont(), ink);
		y += box.cy;
		i++;
	}
	w.DrawRect(0, y, sz.cx, sz.cy, SWhite);
}

void DbgDisas::LeftDown(Point p, dword)
{
	SetFocus();
	cursor = minmax(p.y / GetBox().cy + sb, 0, addr.GetCount() - 1);
	WhenCursor();
	Refresh();
}

bool DbgDisas::Key(dword key, int)
{
	int p = GetSize().cy / GetBox().cy;
	switch(key) {
	case K_UP:
		cursor--;
		break;
	case K_DOWN:
		cursor++;
		break;
	case K_HOME:
		cursor = 0;
		break;
	case K_END:
		cursor = INT_MAX;
		break;
	case K_PAGEUP:
		cursor -= p;
		break;
	case K_PAGEDOWN:
		cursor += p;
		break;
	default:
		return false;
	}
	cursor = minmax(cursor, 0, addr.GetCount() - 1);
	sb.ScrollInto(cursor);
	Refresh();
	WhenCursor();
	return true;
}

void DbgDisas::Clear()
{
	low = 0xffffffff;
	high = 0;
	cursor = -1;
	inst.Clear();
	taddr.Clear();
	addr.Clear();
	codecx = ScreenInfo().GetTextSize("movlmo").cx;
}

void  DbgDisas::Add(dword adr, const String& code, const String& args)
{
	if(adr < low)
		low = adr;
	if(adr > high)
		high = adr;
	addr.Add(adr);
	Inst& n = inst.Add();
	n.code = code;
	n.args = args;
	sb.SetTotal(inst.GetCount());
	Refresh();
	int cx = ScreenInfo().GetTextSize(n.code).cx;
	if(cx > codecx)
		codecx = cx;
}

void DbgDisas::Scroll()
{
	Refresh();
}

void DbgDisas::SetCursor(dword adr)
{
	cursor = addr.Find(adr);
	if(cursor >= 0)
		sb.ScrollInto(cursor);
	Refresh();
}

void DbgDisas::SetIp(dword adr, const Image& img)
{
	ip = addr.Find(adr);
	ipimg = img;
	Refresh();
}

void DbgDisas::GotFocus()
{
	WhenFocus();
	Refresh();
}

void DbgDisas::LostFocus()
{
	WhenFocus();
	Refresh();
}

DbgDisas::DbgDisas()
{
	SetFrame(InsetFrame());
	AddFrame(sb);
	sb.WhenScroll = THISBACK(Scroll);
	low = high = 0;
}
