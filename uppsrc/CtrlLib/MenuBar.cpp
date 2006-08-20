#include "CtrlLib.h"

#define LLOG(x)    // LOG(x)
#define LTIMING(x) // RTIMING(x)

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#endif

class SubMenuBase {
protected:
	MenuBar  menu;
	Callback1<Bar&> proc;
	MenuBar *parentmenu;

	void     Pull(Ctrl *item, Point p, Size sz);

public:
	virtual  void Pull() = 0;

	void SetParent(MenuBar *menu)                        { parentmenu = menu; }
	void Set(Callback1<Bar&> _submenu)                   { proc = _submenu; }
	Callback1<Bar&> Get()                                { return proc; }

	SubMenuBase()                                        { parentmenu = NULL; }
};

class SubMenuItem : public MenuItem, public SubMenuBase {
public:
	virtual void MouseEnter(Point, dword);
	virtual void MouseLeave();
	virtual void GotFocus();
	virtual bool HotKey(dword key);
	virtual bool Key(dword key, int count);
	virtual int  GetVisualState();
	virtual void Pull();

protected:
	enum {
		TIMEID_PULL = BarCtrl::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	typedef SubMenuItem CLASSNAME;

	SubMenuItem();
};

class TopSubMenuItem : public MenuItemBase, public SubMenuBase {
public:
	virtual void Paint(Draw& w);
	virtual void MouseEnter(Point, dword);
	virtual void MouseLeave();
	virtual void LeftDown(Point, dword);
	virtual void SyncState();
	virtual Size GetMinSize() const;
	virtual bool Key(dword key, int);
	virtual bool HotKey(dword key);
	virtual void Pull();

private:
	int  GetState();

public:
	TopSubMenuItem() { NoInitFocus(); }
};

class TopMenuItem : public MenuItemBase {
public:
	virtual void  Paint(Draw& w);
	virtual void  MouseEnter(Point, dword);
	virtual void  MouseLeave();
	virtual void  LeftUp(Point, dword);
	virtual void  LeftDown(Point, dword);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual bool  Key(dword key, int count);
	virtual Size  GetMinSize() const;
	virtual void  SyncState();

	static int GetStdHeight(Font font = StdFont());

private:
	int  GetState();

public:
	TopMenuItem() { NoInitFocus(); }
};

// -------------------------------------

MenuItemBase::MenuItemBase()
{
	accel = 0;
	state = 0;
	isenabled = true;
	type = 0;
	font = StdFont();
	leftgap = 16;
	accesskey = 0;
}

Bar::Item& MenuItemBase::Text(const char *s)
{
	accesskey = ExtractAccessKey(s, text);
	Refresh();
	return *this;
}

Bar::Item& MenuItemBase::Key(dword key)
{
	if(key) {
		accel = key;
		Refresh();
	}
	return *this;
}

Bar::Item& MenuItemBase::Image(const ::Image& img)
{
	return *this;
}

Bar::Item& MenuItemBase::Check(bool check)
{
	type = CHECK0 + check;
	return *this;
}

Bar::Item& MenuItemBase::Radio(bool check)
{
	type = RADIO0 + check;
	return *this;
}

Bar::Item& MenuItemBase::Tip(const char *s)
{
	return *this;
}

Bar::Item& MenuItemBase::Help(const char *s)
{
	HelpLine(s);
	return *this;
}

Bar::Item& MenuItemBase::Description(const char *s)
{
	Ctrl::Description(s);
	return *this;
}

Bar::Item& MenuItemBase::Topic(const char *s)
{
	HelpTopic(s);
	return *this;
}

Bar::Item& MenuItemBase::Enable(bool en)
{
	isenabled = en;
	Refresh();
	return *this;
}

String MenuItemBase::GetDesc() const
{
	return text;
}

dword  MenuItemBase::GetAccessKeys() const
{
	return AccessKeyBit(accesskey);
}

void   MenuItemBase::AssignAccessKeys(dword used)
{
	if(!accesskey) {
		accesskey = ChooseAccessKey(text, used);
		used |= AccessKeyBit(accesskey);
	}
	Ctrl::AssignAccessKeys(used);
}

void DrawMnemonicText(Draw& w, int x, int y, const String& s, Font font, Color color,
                      int mnemonic)
{
	int q = s.Find(ToUpper(mnemonic));
	int e = s.Find(ToLower(mnemonic));
	if(q < 0 || e >= 0 && e < q) q = e;
	w.DrawText(x, y, s, font, color);
	if(q < 0) return;
	FontInfo f = font.Info();
	w.DrawRect(x + GetTextSize(~s, font, q).cx, y + f.GetAscent() + 1, f[s[q]], 1,
	           SColorMark());
}

void DrawMenuText(Draw& w, int x, int y, const String& s, Font f, bool enabled,
                  bool hl, int mnemonic, Color color)
{
	if(enabled)
		DrawMnemonicText(w, x, y, s, f, hl ? SColorHighlightText : color, mnemonic);
	else {
		if(Ctrl::IsXPStyle())
			DrawMnemonicText(w, x, y, s, f, SColorDisabled, 0);
		else {
			DrawMnemonicText(w, x + 1, y + 1, s, f, SColorPaper, 0);
			DrawMnemonicText(w, x, y, s, f, SColorDisabled, 0);
		}
	}
}

void MenuItemBase::DrawMenuText(Draw& w, int x, int y, const String& s, Font f, bool enabled, bool hl, Color color)
{
	::DrawMenuText(w, x, y, s, f, enabled, hl, VisibleAccessKeys() ? accesskey : 0, color);
}

void MenuItemBase::PaintTopItem(Draw& w, int state) {
	Size sz = GetSize();
	if(Ctrl::IsXPStyle()) {
		w.DrawRect(0, 0, sz.cx, sz.cy, state ? SColorHighlight : SColorFace);
		String text = GetText();
		Size isz = GetTextSize(text, StdFont());
		DrawMenuText(w, 6, (sz.cy - isz.cy) / 2, text, GetFont(), IsItemEnabled(), state);
	}
	else {
		w.DrawRect(sz, SColorFace);
		static const ColorF b0[] = { (ColorF)1, SColorLight, SColorLight, SColorShadow, SColorShadow, };
		static const ColorF b1[] = { (ColorF)1, SColorShadow, SColorShadow, SColorLight, SColorLight, };
		String text = GetText();
		Size isz = GetTextSize(text, StdFont());
		DrawMenuText(w, 6, (sz.cy - isz.cy) / 2, text, GetFont(), IsItemEnabled(), false);
		if(state)
			DrawBorder(w, 0, 0, sz.cx, sz.cy, state == 2 ? b1 : b0);
	}
}

// -------------------------------------

Bar::Item& MenuItem::Image(const ::Image& img)
{
	licon = img;
	Refresh();
	return *this;
}

MenuItem& MenuItem::RightImage(const ::Image& img)
{
	ricon = img;
	Refresh();
	return *this;
}

void MenuItem::SendHelpLine()
{
	BarCtrl::SendHelpLine(this);
}

void MenuItem::ClearHelpLine()
{
	BarCtrl::ClearHelpLine(this);
}

void MenuItem::MouseEnter(Point, dword)
{
	SetFocus();
	Refresh();
	SendHelpLine();
}

void MenuItem::MouseLeave()
{
	ClearHelpLine();
}

void MenuItem::GotFocus()
{
	Refresh();
	SendHelpLine();
}

void MenuItem::LostFocus()
{
	Refresh();
	ClearHelpLine();
}

int MenuItem::GetVisualState()
{
	return HasFocus() ?
	       (HasMouse() && GetMouseLeft() || GetMouseRight()) ? PUSH : HIGHLIGHT : NORMAL;
}

void MenuItem::SyncState()
{
	int s = GetVisualState();
	if(s != state) {
		state = s;
		Refresh();
	}
}

void MenuItem::Paint(Draw& w)
{
	int q = text.Find('\t');
	String txt, keydesc;
	if(accel)
		keydesc = GetKeyDesc(accel);
	if(q >= 0) {
		keydesc = text.Mid(q + 1);
		txt = text.Mid(0, q);
	}
	else
		txt = text;
	state = GetVisualState();
	bool hl = state != NORMAL;
	Size sz = GetSize();
	w.DrawRect(sz, hl ? SColorHighlight : IsXPStyle() ? SColorMenu : SColorFace);
	::Image li = licon;
	if(li.IsEmpty()) {
		switch(type) {
		case CHECK0: li = CtrlImg::MenuCheck0(); break;
		case CHECK1: li = CtrlImg::MenuCheck1(); break;
		case RADIO0: li = CtrlImg::MenuRadio0(); break;
		case RADIO1: li = CtrlImg::MenuRadio1(); break;
		}
	}
	Size isz = li.GetSize();
	int iy = (sz.cy - isz.cy) / 2;
	bool chk = false;
	if(!licon.IsEmpty() && type) {
		chk = type == CHECK1 || type == RADIO1;
		if(IsXPStyle()) {
			if(chk && !hl) {
				DrawXPButton(w, RectC(0, iy - 2, isz.cx + 4, isz.cy + 4), BUTTON_EDGE|BUTTON_CHECKED);
			}
		}
		else {
			w.DrawRect(1, iy - 1, isz.cx + 2, isz.cy + 2, chk ? Blend(SColorFace, SColorLight)
			                                                  : SColorFace);
			DrawBorder(w, 0, iy - 2, isz.cx + 4, isz.cy + 4, chk ? ThinInsetBorder : ThinOutsetBorder);
		}
	}
	DrawHighlightImage(w, 2, iy, li, hl || chk, isenabled);
	int x = max(isz.cx, leftgap) + 6;
	isz = GetTextSize(text, StdFont());
	DrawMenuText(w, x, (sz.cy - isz.cy) / 2, txt, font, isenabled, hl, SColorMenuText);
	isz = ricon.GetSize();
	DrawHighlightImage(w, sz.cx - isz.cx, (sz.cy - isz.cy) / 2, ricon, hl, isenabled);
	x = sz.cx - max(isz.cx, 16) - 1;
	if(!IsEmpty(keydesc)) {
		isz = GetTextSize(keydesc, StdFont());
		::DrawMenuText(w, x - isz.cx - 2, (sz.cy - isz.cy) / 2, keydesc, font, isenabled, hl,
		               0, SColorMark());
	}
}

Size MenuItem::GetMinSize() const
{
	Size sz1 = GetTextSize(text, font);
	Size sz2(0, 0);
	if(accel) {
		sz2 = GetTextSize(GetKeyDesc(accel), font);
		sz2.cx += 12;
	}
	Size lsz = licon.GetSize();
	Size rsz = ricon.GetSize();
	return AddFrameSize(Size(max(lsz.cx, 16) + sz1.cx + max(sz2.cx, (rsz.cx ? 16 : 0))
	                         + max(rsz.cx, 16) + 16,
	                         max(max(lsz.cy, rsz.cy) + 4, sz1.cy + 6)));
}

void MenuItem::LeftUp(Point, dword)
{
	if(!isenabled) return;
#ifdef PLATFORM_WIN32
	PlaySound("MenuCommand", NULL, SND_ASYNC|SND_NODEFAULT);
#endif
	LLOG("Menu Item pre Action");
	WhenAction();
	LLOG("Menu Item post Action");
}

void MenuItem::RightUp(Point p, dword w)
{
	LeftUp(p, w);
}

bool MenuItem::HotKey(dword key)
{
	if(isenabled && (key == accel || CompareAccessKey(accesskey, key)
	|| key < 256 && IsAlNum((char)key) && CompareAccessKey(accesskey, ToUpper((char)key) + K_DELTA + K_ALT))) {
		LLOG("MenuItem::HotKey(" << key << ") -> SetFocus");
		SetFocus();
		Sync();
		Sleep(50);
		WhenAction();
		return true;
	}
	return false;
}

bool MenuItem::Key(dword key, int count)
{
	if(key == K_ENTER && isenabled) {
		WhenAction();
		return true;
	}
	return false;
}

// ----------------------------------------------------

void SubMenuBase::Pull(Ctrl *item, Point p, Size sz)
{
	if(!item->IsOpen() || menu.IsOpen()) return;
	menu.Clear();
	proc(menu);
	if(parentmenu) {
		parentmenu->SetActiveSubmenu(&menu, item);
		menu.SetParentMenu(parentmenu);
	}
	menu.PopUp(parentmenu, p, sz);
	if(parentmenu)
		parentmenu->doeffect = false;
}

// ----------------------------------------------------

SubMenuItem::SubMenuItem()
{
	RightImage(CtrlImg::right_arrow());
}

void SubMenuItem::GotFocus()
{
	Refresh();
}

void SubMenuItem::Pull()
{
	Rect r = GetScreenRect();
	Point p = r.TopRight();
	p.x -= 3;
	p.y -= 2;
	SubMenuBase::Pull(this, p, Size(-r.Width(), 0));
	if(parentmenu)
		parentmenu->SyncState();
}

void SubMenuItem::MouseEnter(Point, dword)
{
	SetFocus();
	Refresh();
	if(!menu.IsOpen() && isenabled)
		SetTimeCallback(400, THISBACK(Pull), TIMEID_PULL);
}

void SubMenuItem::MouseLeave()
{
	if(HasFocus() && GetParent())
		GetParent()->SetFocus();
	KillTimeCallback(TIMEID_PULL);
}

int SubMenuItem::GetVisualState()
{
	if(menu.IsOpen() && !GetParent()->HasFocusDeep())
		return PUSH;
	return MenuItem::GetVisualState();
}

bool SubMenuItem::Key(dword key, int count)
{
	if(key == K_RIGHT || key == K_ENTER) {
		Pull();
		return true;
	}
	return MenuItem::Key(key, count);
}

bool SubMenuItem::HotKey(dword key)
{
	if(isenabled && (key == accel || CompareAccessKey(accesskey, key))) {
		Pull();
		return true;
	}
	return false;
}


// ----------------------------------------------------

Size TopSubMenuItem::GetMinSize() const
{
	return AddFrameSize(GetTextSize(text, font) + Size(12, 7));
}

int  TopSubMenuItem::GetState()
{
	if(parentmenu && parentmenu->GetActiveSubmenu() == &menu) return PUSH;
	if(HasMouse() && GetParent() && !GetParent()->HasFocusDeep() &&
	                 (!parentmenu || !parentmenu->GetActiveSubmenu() || parentmenu->GetActiveSubmenu() == &menu)
	   || HasFocus())
		return HIGHLIGHT;
	return NORMAL;
}

void TopSubMenuItem::Paint(Draw& w)
{
	PaintTopItem(w, state = GetState());
}

void TopSubMenuItem::Pull()
{
	Rect r = GetScreenRect();
	if(parentmenu && parentmenu->IsChild() && !parentmenu->submenu)
		parentmenu->SetupRestoreFocus();
	Point p = r.BottomLeft();
	if(IsXPStyle())
		p.y -= 1;
	SubMenuBase::Pull(this, p, Size(r.Width(), -r.Height()));
	if(parentmenu)
		parentmenu->SyncState();
}

void TopSubMenuItem::MouseEnter(Point, dword)
{
	Refresh();
	if(isenabled && parentmenu->GetActiveSubmenu())
		Pull();
}

bool TopSubMenuItem::Key(dword key, int) {
	if(isenabled && (key == K_ENTER || key == K_DOWN)) {
		Pull();
		return true;
	}
	return false;
}

bool TopSubMenuItem::HotKey(dword key)
{
	if(BarCtrl::Scan(proc, key))
		return true;
	if(isenabled && (key == accel || CompareAccessKey(accesskey, key))) {
		Pull();
		return true;
	}
	return false;
}

void TopSubMenuItem::MouseLeave()
{
	Refresh();
}

void TopSubMenuItem::LeftDown(Point, dword)
{
	if(isenabled && !menu.IsOpen()) {
		Pull();
		Refresh();
	}
}

void TopSubMenuItem::SyncState()
{
	int q = GetState();
	if(q != state) {
		state = q;
		Refresh();
	}
}

// ----------------------------------------------------

int  TopMenuItem::GetState()
{
	if(!IsEnabled()) return NORMAL;
	if(HasMouse() && GetMouseLeft() || GetMouseRight()) return PUSH;
	if(HasFocus() || HasMouse()) return HIGHLIGHT;
	return NORMAL;
}

void TopMenuItem::Paint(Draw& w)
{
	PaintTopItem(w, state = GetState());
}

void TopMenuItem::MouseEnter(Point, dword)
{
	Refresh();
}

void TopMenuItem::MouseLeave()
{
	Refresh();
}

void TopMenuItem::LeftUp(Point, dword)
{
	if(!isenabled) return;
	WhenAction();
	Refresh();
}

void TopMenuItem::LeftDown(Point, dword)
{
	Refresh();
}

void TopMenuItem::GotFocus()
{
	Refresh();
}

void TopMenuItem::LostFocus()
{
	Refresh();
}

bool TopMenuItem::Key(dword key, int count)
{
	if(isenabled && key == K_ENTER) {
		WhenAction();
		return true;
	}
	return false;
}

Size TopMenuItem::GetMinSize() const
{
	return AddFrameSize(GetTextSize(text, StdFont()) + Size(12, 7));
}

int TopMenuItem::GetStdHeight(Font font)
{
	return font.Info().GetHeight() + 7;
}

void TopMenuItem::SyncState()
{
	if(state != GetState()) {
		state = GetState();
		Refresh();
	}
}

// -------------------------------------------

void MenuBar::Clear()
{
	if(lock) return;
	lock++;
	SetActiveSubmenu(NULL, NULL);
	item.Clear();
	BarCtrl::Clear();
	lock--;
}

Bar::Item& MenuBar::AddItem(Callback cb)
{
	LLOG("MenuBar::AddItem " << Name());
	MenuItemBase *q;
	if(IsChild())
		q = new TopMenuItem;
	else
		q = new MenuItem;
	item.Add(q);
	pane.Add(q, Null);
	q->SetFont(font);
	q->LeftGap(leftgap);
	*q <<= THISBACK(CloseMenu);
	*q << cb;
	return *q;
}

Bar::Item& MenuBar::AddSubMenu(Callback1<Bar&> proc)
{
	LLOG("MenuBar::AddSubMenu " << Name());
	SubMenuBase *w;
	MenuItemBase *q;
	if(IsChild()) {
		TopSubMenuItem *a = new TopSubMenuItem;
		q = a;
		w = a;
	}
	else {
		SubMenuItem *a = new SubMenuItem;
		q = a;
		w = a;
	}
	item.Add(q);
	pane.Add(q, Null);
	q->SetFont(font);
	q->LeftGap(leftgap);
	w->SetParent(this);
	w->Set(proc);
	return *q;
}

void MenuBar::CloseMenu()
{
	LLOG("CloseMenu");
	MenuBar *q = GetLastSubmenu();
	while(q && q->IsPopUp()) {
		if(q->IsOpen()) {
			q->doeffect = true;
			q->Close();
		}
		MenuBar *w = q;
		q = q->parentmenu;
		w->submenu = w->parentmenu = NULL;
	}
	if(q)
		q->submenu = NULL;
	while(q) {
		q->SyncState();
		q->doeffect = true;
		q = q->parentmenu;
	}
}

MenuBar *MenuBar::GetMasterMenu()
{
	MenuBar *q = this;
	while(q->parentmenu)
		q = q->parentmenu;
	return q;
}

MenuBar *MenuBar::GetLastSubmenu()
{
	MenuBar *q = this;
	while(q->submenu)
		q = q->submenu;
	return q;
}

void MenuBar::SetupRestoreFocus()
{
	MenuBar *q = GetMasterMenu();
	while(q) {
		if(q->HasFocusDeep()) return;
		q = q->submenu;
	}
	restorefocus = GetFocusCtrl();
	LLOG("SetupRestoreFocus -> " << ::Name(restorefocus));
}

void MenuBar::PostDeactivate()
{
	MenuBar *q = GetLastSubmenu();
	MenuBar *u = q;
	while(q && q->IsPopUp()) {
		if(q->IsOpen() && q->HasFocusDeep())
			return;
		q = q->parentmenu;
	}
	if(u->IsPopUp()) {
		while(u->parentmenu && u->parentmenu->IsPopUp())
			u = u->parentmenu;
		Ctrl *w = GetActiveCtrl();
		while(w) {
			if(w == u) return;
			w = w->GetOwner();
		}
	}
	CloseMenu();
}

void MenuBar::Deactivate()
{
	if(IsOpen())
		SetTimeCallback(0, THISBACK(PostDeactivate), TIMEID_STOP);
}

void MenuBar::SyncState()
{
	if(lock) return;
	for(int i = 0; i < item.GetCount(); i++)
		item[i].SyncState();
}

void MenuBar::ChildGotFocus()
{
	if(submenu && !submenuitem->HasFocusDeep() && !ExistsTimeCallback()) {
	   	if(submenu->IsOpen())
			submenu->DelayedClose();
	}
	KillDelayedClose();
	SyncState();
}

void MenuBar::ChildLostFocus()
{
	SyncState();
}

bool PullMenu(Ctrl *q)
{
	if(q->IsEnabled()) {
		TopSubMenuItem *w = dynamic_cast<TopSubMenuItem *>(q);
		if(w) {
			w->Pull();
			return true;
		}
	}
	return false;
}

static bool s_doaltkey;

bool MenuKeyHook(Ctrl *ctrl, dword key, int count)
{
	if(key != K_ALT_KEY && key != (K_ALT_KEY|K_KEYUP))
		s_doaltkey = false;
	return false;
}

bool MenuStateHook(Ctrl *ctrl, int reason)
{
	if(reason == Ctrl::ACTIVATE)
		s_doaltkey = false;
	return false;
}

INITBLOCK
{
	Ctrl::InstallKeyHook(MenuKeyHook);
	Ctrl::InstallStateHook(MenuStateHook);
}

bool MenuBar::Key(dword key, int count)
{
	bool horz = IsChild();
	if((horz ? key == K_RIGHT : key == K_DOWN)) {
		Ctrl *ctrl = GetFocusChildDeep();
		LLOG("MenuBar::Key(" << key << ") -> IterateFocusForward for " << ::Name(ctrl) << ", pane " << ::Name(&pane));
		if(HasMouseDeep())
			GetMouseCtrl()->Refresh();
		if(ctrl && IterateFocusForward(ctrl, &pane))
			return true;
		if(!pane.GetFirstChild()) return true;
		if(pane.GetFirstChild()->SetWantFocus()) return true;
		if(IterateFocusForward(pane.GetFirstChild(), &pane)) return true;
	}
	else
	if((horz ? key == K_LEFT : key == K_UP)) {
		Ctrl *ctrl = GetFocusChildDeep();
		LLOG("MenuBar::Key(" << key << ") -> IterateFocusBackward for " << ::Name(ctrl) << ", pane " << ::Name(&pane));
		if(HasMouseDeep())
			GetMouseCtrl()->Refresh();
		if(ctrl && IterateFocusBackward(ctrl, &pane))
			return true;
		if(!pane.GetFirstChild()) return true;
		if(pane.GetLastChild()->SetWantFocus()) return true;
		if(IterateFocusBackward(pane.GetLastChild(), &pane)) return true;
	}
	else
	if(parentmenu && !parentmenu->IsChild() && key == K_LEFT || key == K_ESCAPE) {
		if(HasMouseDeep())
			GetMouseCtrl()->Refresh();
		if(parentmenu && parentmenu->submenu)
			parentmenu->submenuitem->SetFocus();
		else
		if(IsChild() && HasFocusDeep()) {
			if(restorefocus)
				restorefocus->SetFocus();
			doeffect = true;
			return true;
		}
		if(IsPopUp()) {
			SubmenuClose();
			return true;
		}
		doeffect = true;
	}
	if(parentmenu && parentmenu->IsChild() && parentmenu->GetActiveSubmenu() &&
	   parentmenu->pane.GetFirstChild() && parentmenu->submenuitem) {
		Ctrl *smi = parentmenu->submenuitem;
		Ctrl *q = smi;
		if(key == K_RIGHT)
			for(;;) {
				q = q->GetNext();
				if(!q)
					q = parentmenu->pane.GetFirstChild();
				if(q == smi)
					break;
				if(PullMenu(q)) {
					SyncState();
					return true;
				}
			}
		if(key == K_LEFT)
			for(;;) {
				q = q->GetPrev();
				if(!q)
					q = parentmenu->pane.GetLastChild();
				if(q == smi)
					break;
				if(PullMenu(q)) {
					SyncState();
					return true;
				}
			}
	}
	return HotKey(key);
}

void MenuBar::LeftDown(Point, dword)
{
	if(IsChild() && HasFocusDeep()) {
		if(restorefocus)
			restorefocus->SetFocus();
	}
	WhenLeftClick();
}

bool MenuBar::HotKey(dword key)
{
	if(Ctrl::HotKey(key))
		return true;
	if(IsChild()) {
		if((key == (K_ALT_KEY|K_KEYUP) || key == K_F10) && (submenu || HasFocusDeep())) {
			LLOG("CloseMenu()");
			CloseMenu();
			if(restorefocus)
				restorefocus->SetFocus();
			s_doaltkey = false;
			return true;
		}
		if(key == K_ALT_KEY) {
			s_doaltkey = true;
			return true;
		}
		if((key == K_F10 || key == (K_ALT_KEY|K_KEYUP) && s_doaltkey)
		   && !submenu && !HasFocusDeep() && GetTopWindow() && GetTopWindow()->IsForeground()) {
			SetupRestoreFocus();
			for(Ctrl *q = pane.GetFirstChild(); q; q = q->GetNext())
				if(q->SetFocus()) return true;
		}
	}
	return (key == K_LEFT || key == K_RIGHT) && parentmenu ? parentmenu->Key(key, 1) : false;
}

void MenuBar::SetActiveSubmenu(MenuBar *sm, Ctrl *item)
{
	if(submenu && submenu != sm) {
		submenu->Close();
		submenu->parentmenu = NULL;
	}
	if(submenuitem)
		submenuitem->Refresh();
	submenu = sm;
	submenuitem = item;
}

void MenuBar::SubmenuClose()
{
	if(parentmenu && parentmenu->GetActiveSubmenu() == this)
		parentmenu->SetActiveSubmenu(NULL, NULL);
	else
		Close();
	parentmenu = NULL;
	Clear();
}

void MenuBar::DelayedClose()
{
	SetTimeCallback(400, THISBACK(SubmenuClose), TIMEID_SUBMENUCLOSE);
}

void MenuBar::KillDelayedClose()
{
	KillTimeCallback(TIMEID_SUBMENUCLOSE);
	KillTimeCallback(TIMEID_STOP);
}

void MenuBar::Set(Callback1<Bar&> menu)
{
	if(lock) return;
	Clear();
	lock++;
	menu(*this);
	SyncBar();
	DistributeAccessKeys();
	lock--;
}

void MenuBar::Post(Callback1<Bar&> bar)
{
	KillTimeCallback(TIMEID_POST);
	SetTimeCallback(0, THISBACK1(Set, bar), TIMEID_POST);
}

#ifdef PLATFORM_WIN32

DWORD WINAPI PlaySoundThread(LPVOID)
{
	PlaySound("MenuPopup", NULL, SND_ASYNC|SND_NODEFAULT);
	return 0;
}

#endif

static ColorF xpmenuborder[] = {
	(ColorF)3,
	&SColorShadow, &SColorShadow, &SColorShadow, &SColorShadow,
	&SColorMenu, &SColorMenu, &SColorMenu, &SColorMenu,
	&SColorMenu, &SColorMenu, &SColorMenu, &SColorMenu,
};

GLOBAL_VARP(BorderFrame, XPMenuFrame, (xpmenuborder));

CtrlFrame& MenuFrame()
{
	return Ctrl::IsXPStyle() ? (CtrlFrame&)XPMenuFrame() : (CtrlFrame&)OutsetFrame();
}

void MenuBar::PopUp(Ctrl *owner, Point p, Size rsz)
{
	bool szcx = true;
	bool szcy = true;
	bool szx = false;
	bool szy = false;
	if(parentmenu) {
		if(parentmenu->IsChild())
			szcx = false;
		else
			szcy = false;
		WhenHelp = parentmenu->WhenHelp;
	}
	Rect r = GetWorkArea();
	restorefocus = GetFocusCtrl();
	LLOG("PopUp " << ::Name(this) << " set restorefocus:" << ::Name(restorefocus));
	DistributeAccessKeys();
	SetFrame(MenuFrame());
	pane.SubMenu();
	Size sz = pane.Repos(false, r.Height());
	pane.RightPos(0, sz.cx).BottomPos(0, sz.cy);
	sz = AddFrameSize(sz);
	if(p.x + sz.cx > r.right) {
		p.x = max(p.x + rsz.cx - sz.cx, 0);
		szx = szcx;
		pane.LeftPos(0, sz.cx);
	}
	int pyy = p.y;
	if(p.y + sz.cy > r.bottom) {
		p.y = max(p.y + rsz.cy - sz.cy, 0);
		szy = szcy;
		pane.TopPos(0, sz.cy);
	}
	bool eff = parentmenu == NULL || parentmenu->doeffect;
	if(eff && IsFlag(EFFECT_SLIDE))
		SetRect(szx ? p.x + sz.cx : p.x, szy ? p.y + sz.cy : p.y, parentmenu ? sz.cx : 1, 1);
	else
		SetRect(p.x, p.y, sz.cx, sz.cy);
#ifdef PLATFORM_WIN32
	DWORD dummy;
	CreateThread(NULL, 0, PlaySoundThread, NULL, 0, &dummy);
#endif
	doeffect = true;
	Ctrl::PopUp(owner, true, true, IsFlag(DROPSHADOWS), !owner);
#ifdef PLATFORM_X11
	XSync(Xdisplay, false);
	ProcessEvents();
#endif
	if(eff)
		Animate(*this, p.x, p.y, sz.cx, sz.cy);
}

void MenuBar::Execute(Ctrl *owner, Point p)
{
	if(IsEmpty()) return;
	PopUp(owner, p);
	EventLoop(this);
}

void MenuBar::Execute(Ctrl *owner, Callback1<Bar&> proc, Point p)
{
	MenuBar bar;
	proc(bar);
	bar.Execute(owner, p);
}

int MenuBar::GetStdHeight(Font font)
{
	return TopMenuItem::GetStdHeight(font);
}

void MenuBar::CancelMode()
{
}

MenuBar::MenuBar()
{
	LLOG("MenuBar " << Name());
	submenu = parentmenu = NULL;
	submenuitem = NULL;
	doeffect = true;
	font = StdFont();
	leftgap = 16;
	lock = 0;
}

MenuBar::~MenuBar()
{
	lock++;
	LLOG("~MenuBar " << Name());
	LLOG("Parent menu " << ::Name(parentmenu));
	if(parentmenu)
		parentmenu->SetActiveSubmenu(NULL, NULL);
	LLOG("~MenuBar 1");
}
