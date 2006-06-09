#include "ide.h"

static void StringIndex(Index<String>& out, WString w)
{
	const wchar *p = w;
	for(;;) {
		while(*p && !IsAlpha(*p))
			p++;
		const wchar *b = p;
		while(IsAlpha(*++p))
			;
		if(p - b >= 2)
			out.FindAdd(ToUtf8(ToLower(WString(b, p))));
	}
}

static void ParaIndex(Index<String>& out, const RichPara& rt)
{
	for(int i = 0; i < rt.GetCount(); i++) {
		const RichPara::Part& rp = rt[i];
		if(rp.IsText())
			StringIndex(out, rp.text);
	}
}

static void TxtIndex(Index<String>& out, const RichTxt& rt, const RichStyles& rs);

static void TableIndex(Index<String>& out, const RichTable& rt, const RichStyles& rs)
{
	for(int y = 0; y < rt.cell.GetCount(); y++)
		for(int x = 0; x < rt.cell[y].GetCount(); x++)
			TxtIndex(out, rt.cell[y][x].text, rs);
}

static void TxtIndex(Index<String>& out, const RichTxt& rt, const RichStyles& rs)
{
	for(int i = 0; i < rt.GetPartCount(); i++)
		if(rt.IsPara(i))
			ParaIndex(out, rt.Get(i, rs));
		else if(rt.IsTable(i))
			TableIndex(out, rt.GetTable(i), rs);
}

Vector<String> MakeIndex(const RichText& rt)
{
	Index<String> out;
	TxtIndex(out, rt, rt.GetStyles());
	return out.PickKeys();
}
