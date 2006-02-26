#include "LayDes.h"

void LayoutUndo::Push(const String& state)
{
	if(stack.GetCount()) {
		if(state != stack.Top()) {
			stack.Top() = DiffPack(state, stack.Top());
			stack.Add(state);
		}
	}
	else
		stack.Add(state);
}

String LayoutUndo::Pop()
{
	ASSERT(stack.GetCount());
	String state = stack.Pop();
	if(stack.GetCount())
		stack.Top() = DiffUnpack(state, stack.Top());
	return state;
}

String ReadVar(CParser& p) {
	String var;
	for(;;) {
		if(p.IsId())
			var << p.ReadId();
		else
		if(p.IsInt())
			var << p.ReadInt();
		else
		if(p.Char('.'))
			var << '.';
		else
		if(p.Char('['))
			var << '[';
		else
		if(p.Char(']'))
			var << ']';
		else
			return var;
	}
}

Array<LayoutItem> ReadItems(CParser& p, byte charset)
{
	Array<LayoutItem> items;
	for(;;) {
		String type;
		if(p.Id("ITEM")) {
			p.PassChar('(');
			if(p.IsId()) type = p.ReadIdt();
			p.PassChar(',');
		}
		else
		if(p.Id("UNTYPED"))
			p.PassChar('(');
		else
			return items;
		LayoutItem& m = items.Add();
		m.Create(type);
		if(p.IsId()) m.variable = ReadVar(p);
		if(strncmp(m.variable, "dv___", 5) == 0)
			m.variable.Clear();
		p.PassChar(',');
		m.ReadProperties(p, charset);
		p.PassChar(')');
	}
}

void  LayoutData::Read(CParser& p, byte charset)
{
	p.PassId("LAYOUT");
	p.PassChar('(');
	name = p.ReadId();
	p.PassChar(',');
	size.cx = p.ReadInt();
	p.PassChar(',');
	size.cy = p.ReadInt();
	p.PassChar(')');
	item = ReadItems(p, charset);
	p.PassId("END_LAYOUT");
}

String LayoutData::Save(byte charset)
{
	String out;
	out << "LAYOUT(" << name << ", " << size.cx << ", " << size.cy << ")\r\n";
	for(int i = 0; i < item.GetCount(); i++)
		out << item[i].Save(charset, i);
	out << "END_LAYOUT\r\n";
	return out;
}

String LayoutData::Save(byte charset, const Vector<int>& sel)
{
	Vector<int> cs(sel, 1);
	Sort(cs);
	String out;
	out << "LAYOUT(" << name << ", " << size.cx << ", " << size.cy << ")\r\n";
	for(int i = 0; i < cs.GetCount(); i++)
		out << item[cs[i]].Save(charset, cs[i]);
	out << "END_LAYOUT\r\n";
	return out;
}

String LayoutData::MakeState()
{
	return Save(CHARSET_UTF8);
}

void LayoutData::LoadState(const String& s)
{
	CParser p(s);
	Read(p, CHARSET_UTF8);
}

void LayoutData::SaveState()
{
	undo.Push(MakeState());
	redo.Clear();
}

bool LayoutData::IsUndo()
{
	return undo && (undo.GetCount() > 1 || undo.Top() != MakeState());
}

bool LayoutData::IsRedo()
{
	return redo && (redo.GetCount() > 1 || redo.Top() != MakeState());
}

void LayoutData::Do(LayoutUndo& u1, LayoutUndo& u2)
{
	String s = MakeState();
	u2.Push(s);
	String q = u1.Pop();
	if(q == s && u1)
		q = u1.Pop();
	LoadState(q);
}

void LayoutData::Undo()
{
	Do(undo, redo);
}

void LayoutData::Redo()
{
	Do(redo, undo);
}
