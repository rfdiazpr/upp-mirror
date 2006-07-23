#include "CtrlLib.h"

TreeCtrl::Node::Node()
{
	Init();
}

TreeCtrl::Node::Node(const Image& img, const Value& v)
{
	Init();
	image = img;
	key = value = v;
}

TreeCtrl::Node::Node(const Image& img, const Value& v, const Value& t)
{
	Init();
	image = img;
	key = v;
	value = t;
}

TreeCtrl::Node::Node(const Value& v)
{
	Init();
	key = value = v;
}

TreeCtrl::Node::Node(const Value& v, const Value& t)
{
	Init();
	key = v;
	value = t;
}

TreeCtrl::Node::Node(Ctrl& ctrl)
{
	Init();
	SetCtrl(ctrl);
}

TreeCtrl::Node::Node(const Image& img, Ctrl& ctrl, int cx, int cy)
{
	Init();
	SetCtrl(ctrl);
	image = img;
	size = ctrl.GetMinSize();
	if(cx > 0)
		size.cx = cx;
	if(cy > 0)
		size.cy = cy;
}

TreeCtrl::TreeCtrl()
{
	levelcx = 16;
	nocursor = false;
	noroot = false;
	dirty = true;
	isselection = false;
	multiselect = false;
	Clear();
	SetFrame(ViewFrame());
	AddFrame(sb);
	sb.WhenScroll = THISBACK(Scroll);
}

TreeCtrl::~TreeCtrl() {}

void   TreeCtrl::Layout()
{
	sb.SetPage(sb.GetReducedViewSize());
	sb.SetLine(item[0].GetSize().cy);
}

Size   TreeCtrl::Item::GetValueSize() const
{
	if(IsNull(size))
		if(ctrl)
			return ctrl->GetMinSize();
		else
			return display ? display->GetStdSize(value) : StdDisplay().GetStdSize(value);
	else
		return size;
}

Size   TreeCtrl::Item::GetSize() const
{
	Size sz = GetValueSize();
	sz += Size(2 * margin, 2 * margin);
	Size isz = image.GetSize();
	sz.cx += isz.cx;
	sz.cy = max(sz.cy, isz.cy);
	return sz;
}

void   TreeCtrl::SetRoot(const TreeCtrl::Node& n)
{
	(TreeCtrl::Node &)item[0] = n;
	RefreshItem(0);
}

void   TreeCtrl::SetRoot(const Image& img, Value v)
{
	SetRoot(TreeCtrl::Node(img, v).CanOpen());
}

void   TreeCtrl::SetRoot(const Image& img, Value v, Value t)
{
	SetRoot(TreeCtrl::Node(img, v, t).CanOpen());
}

void   TreeCtrl::SetRoot(const Image& img, Ctrl& ctrl, int cx, int cy)
{
	SetRoot(TreeCtrl::Node(img, ctrl, cx, cy).CanOpen());
}

int    TreeCtrl::Insert(int parentid, int i, const TreeCtrl::Node& n)
{
	int id;
	if(freelist >= 0) {
		id = freelist;
		freelist = item[id].freelink;
	}
	else {
		id = item.GetCount();
		item.Add();
	}
	Item& m = item[id];
	m.parent = parentid;
	(TreeCtrl::Node&)m = n;
	if(parentid >= 0) {
		Item& parent = item[parentid];
		parent.child.Insert(i, id);
	}
	dirty = true;
	Dirty(parentid);
	return id;
}

int    TreeCtrl::Add(int parentid, const TreeCtrl::Node& n)
{
	return Insert(parentid, item[parentid].child.GetCount(), n);
}

int    TreeCtrl::Insert(int parentid, int i)
{
	return Insert(parentid, i, TreeCtrl::Node());
}

int    TreeCtrl::Add(int parentid)
{
	return Add(parentid, TreeCtrl::Node());
}

int    TreeCtrl::Insert(int parentid, int i, const Image& img, Value v, bool withopen)
{
	return Insert(parentid, i, TreeCtrl::Node(img, v).CanOpen(withopen));
}

int    TreeCtrl::Insert(int parentid, int i, const Image& img, Value v, Value t, bool withopen)
{
	return Insert(parentid, i, TreeCtrl::Node(img, v, t).CanOpen(withopen));
}

int    TreeCtrl::Add(int parentid, const Image& img, Value v, bool withopen)
{
	return Add(parentid, TreeCtrl::Node(img, v).CanOpen(withopen));
}

int    TreeCtrl::Add(int parentid, const Image& img, Value v, Value t, bool withopen)
{
	return Add(parentid, TreeCtrl::Node(img, v, t).CanOpen(withopen));
}

int    TreeCtrl::Insert(int parentid, int i, const Image& img, Ctrl& ctrl, int cx, int cy, bool withopen)
{
	return Insert(parentid, i, TreeCtrl::Node(img, ctrl, cx, cy).CanOpen(withopen));
}

int    TreeCtrl::Add(int parentid, const Image& img, Ctrl& ctrl, int cx, int cy, bool withopen)
{
	return Add(parentid, TreeCtrl::Node(img, ctrl, cx, cy).CanOpen(withopen));
}

Value  TreeCtrl::Get(int id) const
{
	const Item& m = item[id];
	return m.ctrl ? m.ctrl->GetData() : m.key;
}

Value  TreeCtrl::GetValue(int id) const
{
	const Item& m = item[id];
	return m.ctrl ? m.ctrl->GetData() : m.value;
}

void  TreeCtrl::Set(int id, Value v)
{
	Item& m = item[id];
	if(m.ctrl)
		m.ctrl->SetData(v);
	else {
		m.value = m.key = v;
		RefreshItem(id);
	}
}

void  TreeCtrl::Set(int id, Value k, Value v)
{
	Item& m = item[id];
	if(m.ctrl)
		m.ctrl->SetData(v);
	else {
		m.key = k;
		m.value = v;
		RefreshItem(id);
	}
}

void   TreeCtrl::SetNode(int id, const TreeCtrl::Node& n)
{
	(TreeCtrl::Node&)item[id] = n;
	Dirty(id);
}

void   TreeCtrl::RemoveChildren(int id)
{
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		RemoveSubtree(m.child[i]);
	m.child.Clear();
	Dirty(id);
}

void   TreeCtrl::RemoveSubtree(int id)
{
	Item& m = item[id];
	if(m.linei == cursor)
		cursor = item[m.parent].linei;
	m.value = Null;
	m.image = Null;
	RemoveChildren(id);
	m.freelink = freelist;
	freelist = id;
}

void   TreeCtrl::Remove(int id)
{
	ASSERT(id > 0);
	int pi = item[id].parent;
	Item& parent = item[item[id].parent];
	parent.child.Remove(FindIndex(parent.child, id));
	RemoveSubtree(id);
	Dirty(pi);
}

void   TreeCtrl::Clear()
{
	item.Clear();
	item.Add();
	item[0].linei = -1;
	item[0].parent = -1;
	freelist = -1;
	Dirty();
	cursor = -1;
}

void TreeCtrl::RemoveCtrls(int itemi)
{
	Item& m = item[itemi];
	if(m.ctrl)
		m.ctrl->Remove();
	for(int i = 0; i < m.child.GetCount(); i++)
		RemoveCtrls(m.child[i]);
}

void TreeCtrl::ReLine(int itemi, int level, Size& sz)
{
	int ii = line.GetCount();
	Line& l = line.Add();
	l.level = level;
	l.itemi = itemi;
	l.y = sz.cy;
	l.ll = -1;
	Item& m = item[itemi];
	if(m.ctrl) {
		hasctrls = true;
		m.ctrl->Remove();
	}
	m.linei = ii;
	Size msz = m.GetSize();
	sz.cy += msz.cy;
	sz.cx = max(sz.cx, level * levelcx + msz.cx);
	level++;
	for(int i = 0; i < m.child.GetCount(); i++)
		if(m.isopen) {
			line[ii].ll = line.GetCount();
			ReLine(m.child[i], level, sz);
		}
		else
			RemoveCtrls(m.child[i]);
}

void TreeCtrl::SyncTree()
{
	if(!dirty)
		return;
	if(noroot)
		Open(0);
	Ptr<Ctrl> restorefocus = GetFocusChildDeep();
	hasctrls = false;
	int cursorid = GetCursor();
	for(int i = 0; i < item.GetCount(); i++)
		item[i].linei = -1;
	line.Clear();
	Size treesize = Size(0, 0);
	if(noroot) {
		if(GetChildCount(0))
			treesize.cy = -item[0].GetSize().cy;
		ReLine(0, -1, treesize);
	}
	else
		ReLine(0, 0, treesize);
	treesize.cy = max(0, treesize.cy);
	treesize.cx += levelcx;
	sb.SetTotal(treesize);
	cursor = -1;
	dirty = false;
	if(cursorid >= 0)
		SetCursor(cursorid, false);
	SyncCtrls(true, restorefocus);
}

void TreeCtrl::SyncCtrls(bool add, Ctrl *restorefocus)
{
	if(!hasctrls)
		return;
	Point org = sb;
	for(int i = noroot; i < line.GetCount(); i++) {
		const Line& l = line[i];
		Item& m = item[l.itemi];
		if(m.ctrl) {
			if(add)
				AddChildBefore(m.ctrl, GetLastChild());
			if(m.ctrl == restorefocus || m.ctrl->HasChildDeep(restorefocus))
				restorefocus->SetFocus();
			Size msz = m.GetSize();
			Size isz = m.image.GetSize();
			Size vsz = m.GetValueSize();
			m.ctrl->SetRect(levelcx + l.level * levelcx + isz.cx + m.margin - org.x,
			                l.y + (msz.cy - vsz.cy) / 2 - org.y, vsz.cx, vsz.cy);
		}
	}
}

bool TreeCtrl::IsOpen(int id) const
{
	return item[id].isopen;
}

void TreeCtrl::Dirty(int id)
{
	ClearSelection();
	Size sz = GetSize();
	dirty = true;
	while(id >= 0) {
		int q = item[id].linei;
		if(q >= 0) {
			int y = line[q].y - sb.GetY();
			Refresh(0, y, sz.cx, sz.cy - y);
			return;
		}
		id = item[id].parent;
	}
	Refresh();
}

void TreeCtrl::Open(int id, bool open)
{
	Item& m = item[id];
	if(m.isopen != open) {
		m.isopen = open;
		int q = GetCursor();
		while(q >= 0) {
			if(q == id) {
				SetCursor(id);
				break;
			}
			q = GetParent(q);
		}
		Dirty(id);
		if(open)
			WhenOpen(id);
		else
			WhenClose(id);
	}
}

void TreeCtrl::MakeVisible(int id)
{
	id = GetParent(id);
	while(id >= 0) {
		Open(id);
		id = GetParent(id);
	}
}

int TreeCtrl::FindLine(int y) const
{
	int l = FindUpperBound(line, y, LineLess());
	return l > 0 ? l - 1 : 0;
}

void TreeCtrl::RefreshLine(int i)
{
	SyncTree();
	if(i >= 0) {
		Size sz = GetSize();
		int y = line[i].y - sb.GetY();
		Refresh(0, y, sz.cx, item[line[i].itemi].GetSize().cy);
	}
}

void TreeCtrl::RefreshItem(int id)
{
	SyncTree();
	RefreshLine(item[id].linei);
}

int  TreeCtrl::GetItemAtLine(int i)
{
	SyncTree();
	return line[i].itemi;
}

int  TreeCtrl::GetLineAtItem(int id)
{
	SyncTree();
	return item[id].linei;
}

int  TreeCtrl::GetLineCount()
{
	SyncTree();
	return line.GetCount();
}

void TreeCtrl::SetCursorLine(int i, bool sc)
{
	if(nocursor)
		return;
	if(i != cursor) {
		i = minmax(i, 0, line.GetCount() - 1);
		if(i < 0) return;
		Item& m = item[line[i].itemi];
		if(sc)
			sb.ScrollIntoY(line[i].y, m.GetSize().cy);
		RefreshLine(cursor);
		cursor = i;
		RefreshLine(cursor);
		if(m.ctrl && m.ctrl->SetWantFocus())
			return;
		WhenCursor();
	}
}

void TreeCtrl::SetCursorLine(int i)
{
	SetCursorLine(i, true);
}

void TreeCtrl::SetCursorLineSync(int i)
{
	if(nocursor)
		return;
	if(i != cursor) {
		if(cursor < 0)
			Refresh();
		RefreshLine(cursor);
		if(i < 0)
			cursor = -1;
		else
			cursor = minmax(i, (int)noroot, line.GetCount() - 1);
		RefreshLine(cursor);
		Item& m = item[line[cursor].itemi];
		if(cursor < 0)
			Refresh();
		else {
			Sync();
			sb.ScrollIntoY(line[cursor].y, m.GetSize().cy);
		}
		if(!(m.ctrl && m.ctrl->SetWantFocus()))
			SetFocus();
		WhenCursor();
	}
}

void TreeCtrl::KillCursor()
{
	RefreshLine(cursor);
	cursor = -1;
	Refresh();
	WhenCursor();
}

void TreeCtrl::SetCursor(int id, bool sc)
{
	while(id > 0) {
		ASSERT(id >= 0 && id < item.GetCount());
		MakeVisible(id);
		SyncTree();
		const Item& m = item[id];
		if(m.linei >= 0) {
			SetCursorLine(m.linei, sc);
			return;
		}
		id = m.parent;
	}
	SetCursorLine(0, sc);
}

void TreeCtrl::SetCursor(int id)
{
	SetCursor(id, true);
}

int  TreeCtrl::GetCursor() const
{
	return cursor >= 0 ? line[cursor].itemi : -1;
}

Value  TreeCtrl::Get() const
{
	return IsCursor() ? Get(GetCursor()) : Value();
}

Value  TreeCtrl::GetValue() const
{
	return IsCursor() ? GetValue(GetCursor()) : Value();
}

int    TreeCtrl::Find(Value key)
{
	for(int i = 0; i < item.GetCount(); i++)
		if(Get(i) == key)
			return i;
	return -1;
}

bool   TreeCtrl::FindSetCursor(Value key)
{
	int q = Find(key);
	if(q < 0)
		return false;
	SetCursor(q);
	return true;
}

void TreeCtrl::ShiftSelect(int l1, int l2)
{
	if(!multiselect)
		return;
	bool b = false;
	if(l1 > l2)
		Swap(l1, l2);
	for(int i = l1; i <= l2; i++)
		SelectOne(line[i].itemi, true);
	UpdateSelect();
}

void TreeCtrl::LeftDown(Point p, dword flags)
{
	Point org = sb;
	if(p.y + org.y > sb.GetTotal().cy)
		return;
	int i = FindLine(p.y + org.y);
	const Line& l = line[i];
	int x = levelcx + l.level * levelcx - org.x - (levelcx >> 1) - org.x;
	if(p.x > x - 6 && p.x < x + 6)
		Open(l.itemi, !IsOpen(l.itemi));
	else {
		SetFocus();
		int q = cursor;
		SetCursorLine(i);
		if(cursor != q)
			WhenAction();
		if(multiselect) {
			if(flags & K_CTRL) {
				int id = GetCursor();
				SelectOne(id, !IsSelected(id));
			}
			else {
				ClearSelection();
				if(flags & K_SHIFT) {
					ShiftSelect(q, cursor);
					cursor = q;
				}
			}
		}
	}
}

void TreeCtrl::LeftDouble(Point p, dword flags)
{
	Point org = sb;
	if(p.y + org.y > sb.GetTotal().cy)
		return;
	if(FindLine(p.y + org.y) == GetCursorLine())
		WhenLeftDouble();
}

void TreeCtrl::RightDown(Point p, dword flags)
{
	Point org = sb;
	if(p.y + org.y < sb.GetTotal().cy) {
		int i = FindLine(p.y + org.y);
		if(i >= 0) {
			SetFocus();
			SetCursorLine(i);
		}
	}
	if(WhenBar)
		MenuBar::Execute(WhenBar);
}

void TreeCtrl::Paint(Draw& w)
{
	SyncTree();
	Size sz = GetSize();
	Point org = sb;
	scroller.Set(org);
	w.DrawRect(sz, SColorPaper);
	int levelcx2 = levelcx >> 1;
	for(int i = 0; i < line.GetCount(); i++) {
		Line& l = line[i];
		if(l.ll >= 0) {
			int yl = line[i].y + item[l.itemi].GetSize().cy - org.y;
			int yh = line[l.ll].y + item[line[l.ll].itemi].GetSize().cy / 2 - org.y;
			if(yh >= 0 && yl < sz.cy) {
				int x = levelcx + levelcx * l.level + levelcx2 - org.x;
				w.DrawRect(x, yl, 1, yh - yl, SColorShadow);
			}
		}
	}
	for(int i = FindLine(org.y); i < line.GetCount(); i++) {
		Line& l = line[i];
		const Item& m = item[l.itemi];
		const Display *d = m.display;
		if(!d)
			d = &StdDisplay();
		Size msz = m.GetSize();
		Size isz = m.image.GetSize();
		Size vsz = m.GetValueSize();
		int y = l.y - org.y;
		if(y > sz.cy)
			break;
		if(w.IsPainting(0, y, sz.cx, msz.cy)) {
			int x = 0;
			x = levelcx + l.level * levelcx - org.x;
			Point op = Point(x - levelcx2, y + msz.cy / 2);
			w.DrawRect(op.x, op.y, levelcx2, 1, SColorShadow);
			if(m.canopen || m.child.GetCount()) {
				Image im = m.isopen ? CtrlImg::treeminus() : CtrlImg::treeplus();
				op -= im.GetSize() / 2;
				w.DrawImage(op.x, op.y, im);
			}
			w.DrawImage(x, y + (msz.cy - isz.cy) / 2, m.image);
			x += isz.cx;
			dword st = 0;
			Color fg = SColorText;
			Color bg = SColorPaper;
			bool hasfocus = HasFocus();
			if(IsReadOnly())
				st |= Display::READONLY;
			if(m.sel) {
				st |= Display::SELECT;
				fg = hasfocus ? SColorHighlightText : SColorText;
				bg = hasfocus ? SColorHighlight : SColorFace;
			}
			if(i == cursor && !nocursor) {
				st |= Display::CURSOR;
				fg = hasfocus ? SColorHighlightText : SColorText;
				bg = hasfocus ? (isselection ? Blend(SColorLight, SColorHighlight) : SColorHighlight)
				              : SColorFace;
			}
			if(hasfocus) st |= Display::FOCUS;
			if(!(m.ctrl && m.ctrl->IsWantFocus())) {
				w.DrawRect(x, y, vsz.cx + 2 * m.margin, msz.cy, bg);
				d->Paint(w, RectC(x + m.margin, y + (msz.cy - vsz.cy) / 2, vsz.cx, vsz.cy), m.value,
				         fg, bg, st);
			}
		}
	}
}

void TreeCtrl::Scroll()
{
	SyncTree();
	if(hasctrls) {
		Refresh();
		SyncCtrls(false, NULL);
	}
	else
		scroller.Scroll(*this, sb);
}

void TreeCtrl::MouseWheel(Point, int zdelta, dword)
{
	sb.WheelY(zdelta);
}

void TreeCtrl::ChildGotFocus()
{
	for(int i = 0; i < line.GetCount(); i++) {
		Item& m = item[line[i].itemi];
		if(m.ctrl && m.ctrl->HasFocusDeep()) {
			SetCursorLine(i);
			return;
		}
	}
}

bool TreeCtrl::Tab(int d)
{
	if(cursor < 0)
		return false;
	Item& m = item[line[cursor].itemi];
	if(m.ctrl && m.ctrl->HasFocusDeep())
		return false;
	int i = cursor;
	for(int i = cursor + d; i >= 0 && i < line.GetCount(); i += d) {
		Item& m = item[line[i].itemi];
		if(m.ctrl && m.ctrl->SetWantFocus())
			return true;
	}
	return false;
}

bool TreeCtrl::Key(dword key, int)
{
	SyncTree();
	Size sz = GetSize();
	int cid = GetCursor();
	bool shift = key & K_SHIFT;
	key &= ~K_SHIFT;
	switch(key) {
	case K_TAB:
		return Tab(1);
	case K_SHIFT_TAB:
		return Tab(-1);
	case K_UP:
		SetCursorLineSync(cursor > 0 ? cursor - 1 : 0);
		break;
	case K_DOWN:
		SetCursorLineSync(cursor >= 0 ? cursor + 1 : line.GetCount() - 1);
		break;
	case K_PAGEDOWN:
		SetCursorLineSync(cursor >= 0 ? FindLine(line[cursor].y + sz.cy) : line.GetCount() - 1);
		break;
	case K_PAGEUP:
		SetCursorLineSync(cursor >= 0 ? FindLine(line[cursor].y - sz.cy) : 0);
		break;
	case K_LEFT:
		if(cid >= 0)
			Close(cid);
		break;
	case K_RIGHT:
		if(cid >= 0)
			Open(cid);
		break;
	default:
		return false;
	}
	if(cid != cursor)
		if(shift && multiselect)
			ShiftSelect(cid, cursor);
		else
			ClearSelection();
	if(cursor != cid)
		WhenAction();
	return true;
}

void TreeCtrl::GotFocus()
{
	if(dirty)
		return;
	if(hasctrls && cursor >= 0 && item[line[cursor].itemi].ctrl)
		for(int i = 0; i < line.GetCount(); i++) {
			Item& m = item[line[i].itemi];
			if(m.ctrl && m.ctrl->SetWantFocus())
				break;
		}
	else
		RefreshLine(cursor);
}

void TreeCtrl::LostFocus()
{
	if(dirty)
		return;
	RefreshLine(cursor);
}

void TreeCtrl::ChildRemoved(Ctrl *)
{
	Dirty();
}

struct TreeCtrl::SortOrder {
	TreeCtrl         *tree;
	const ValueOrder *order;

	bool operator()(int a, int b) const {
		return (*order)(tree->Get(a), tree->Get(b));
	}

};

void TreeCtrl::Sort0(int id, const ValueOrder& order)
{
	SortOrder so;
	so.tree = this;
	so.order = &order;
	::Sort(item[id].child, so);
}

void TreeCtrl::Sort(int id, const ValueOrder& order)
{
	SyncTree();
	Sort0(id, order);
	Dirty(id);
}

void TreeCtrl::SortDeep0(int id, const ValueOrder& order)
{
	Sort0(id, order);
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		SortDeep0(m.child[i], order);
}

void TreeCtrl::SortDeep(int id, const ValueOrder& order)
{
	SyncTree();
	SortDeep0(id, order);
	Dirty(id);
}

void TreeCtrl::Sort(int id, int (*compare)(const Value& v1, const Value& v2))
{
	Sort(id, FnValueOrder(compare));
}

void TreeCtrl::SortDeep(int id, int (*compare)(const Value& v1, const Value& v2))
{
	SortDeep(id, FnValueOrder(compare));
}

void  TreeCtrl::SetData(const Value& data)
{
	FindSetCursor(data);
}

Value TreeCtrl::GetData() const
{
	return Get();
}

Point  TreeCtrl::GetScroll() const
{
	return sb;
}

void TreeCtrl::ScrollTo(Point sc)
{
	sb = sc;
}

void TreeCtrl::ClearSelTree(int id)
{
	Item& m = item[id];
	m.sel = false;
	for(int i = 0; i < m.child.GetCount(); i++)
		ClearSelTree(m.child[i]);
}

void TreeCtrl::ClearSelection()
{
	if(isselection) {
		ClearSelTree(0);
		isselection = false;
		WhenSelection();
		WhenAction();
		Refresh();
	}
}

bool TreeCtrl::UpdateSelTree(int id)
{
	Item& m = item[id];
	if(m.sel)
		return true;
	for(int i = 0; i < m.child.GetCount(); i++)
		if(UpdateSelTree(m.child[i]))
			return true;
	return false;
}

void TreeCtrl::UpdateSelect()
{
	isselection = UpdateSelTree(0);
	WhenSelection();
	WhenAction();
}

void TreeCtrl::SelectOne(int id, bool sel)
{
	if(!multiselect) {
		ClearSelection();
		return;
	}
	if(item[id].canselect)
		item[id].sel = sel;
	UpdateSelect();
	RefreshItem(id);
}

void OptionTree::SetRoot(const Image& img, Option& opt, const char *text)
{
	if(text)
		opt.SetLabel(text);
	TreeCtrl::SetRoot(img, opt);
	opt.NoNotNull().BlackEdge();
	option.At(0) = &opt;
	opt <<= THISBACK1(SetOption, 0);
}

void OptionTree::Clear()
{
	TreeCtrl::Clear();
	aux.Clear();
	option.Clear();
	aux.Add();
}

void OptionTree::SetRoot(Option& option, const char *text)
{
	SetRoot(Null, option, text);
}

void OptionTree::SetRoot(const Image& img, const char *text)
{
	SetRoot(img, aux[0], text);
}

void OptionTree::SetRoot(const char *text)
{
	SetRoot(Null, aux[0], text);
}

int OptionTree::Add(int parentid, const Image& img, Option& opt, const char *text)
{
	if(text)
		opt.SetLabel(text);
	int id = TreeCtrl::Add(parentid, img, opt);
	option.At(id, NULL) = &opt;
	opt.NoNotNull().BlackEdge();
	opt <<= THISBACK1(SetOption, id);
	return id;
}

int OptionTree::Add(int parentid, Option& opt, const char *text)
{
	return Add(parentid, Null, opt.NoNotNull(), text);
}

int OptionTree::Add(int parentid, const Image& img, const char *text)
{
	return Add(parentid, img, aux.Add().NoNotNull(), text);
}

int OptionTree::Add(int parentid, const char *text)
{
	return Add(parentid, aux.Add().NoNotNull(), text);
}

void OptionTree::SetChildren(int id, bool b)
{
	for(int i = 0; i < GetChildCount(id); i++) {
		int chid = GetChild(id, i);
		Option *opt = option[chid];
		if(opt)
			opt->Set(b);
		SetChildren(chid, b);
	}
}

void OptionTree::SetOption(int id)
{
	Option *opt = option[id];
	ASSERT(opt);
	SetChildren(id, opt->Get());
	for(;;) {
		id = GetParent(id);
		if(id < 0)
			break;
		bool t = false;
		bool f = false;
		bool n = false;
		for(int i = 0; i < GetChildCount(id); i++) {
			int chid = GetChild(id, i);
			Option *opt = option[chid];
			if(opt) {
				int x = opt->Get();
				if(x == 1)
					t = true;
				else
				if(x == 0)
					f = true;
				else
					n = true;
			}
		}
		opt = option[id];
		if(opt) {
			if(t && f || n) {
				opt->Set(Null);
			}
			else
				opt->Set(t);
		}
	}
	WhenOption();
}

OptionTree::OptionTree() { aux.Add(); }
OptionTree::~OptionTree() {}
