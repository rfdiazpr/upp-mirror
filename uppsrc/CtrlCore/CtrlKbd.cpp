#include "CtrlCore.h"

#define LLOG(x)    LOG(x)

Ptr<Ctrl> Ctrl::focusCtrl;
Ptr<Ctrl> Ctrl::focusCtrlWnd;
Ptr<Ctrl> Ctrl::lastActiveWnd;
Ptr<Ctrl> Ctrl::caretCtrl;
Rect      Ctrl::caretRect;
bool      Ctrl::ignorekeyup;

Ptr<Ctrl>           Ctrl::defferedSetFocus;
Vector< Ptr<Ctrl> > Ctrl::defferedChildLostFocus;


#ifdef flagSO
Ptr<Ctrl> Ctrl::FocusCtrl() { return focusCtrl; }
void Ctrl::FocusCtrl(Ptr<Ctrl> fc) { focusCtrl = fc; }
#endif

static bool s_hotkey;

void Ctrl::RefreshAccessKeys()
{
	if(GetAccessKeys())
		Refresh();
	for(Ctrl *ctrl = GetFirstChild(); ctrl; ctrl = ctrl->GetNext())
		ctrl->RefreshAccessKeys();
}

void Ctrl::RefreshAccessKeysDo(bool vis)
{
	if(IsFlag(ALTACCESSKEYS) && vis != akv) {
		akv = vis;
		RefreshAccessKeys();
	}
}

bool Ctrl::DispatchKey(dword keycode, int count)
{
	if(IsFlag(ALTACCESSKEYS)) {
		bool alt = GetAlt();
		Ctrl *c = GetActiveCtrl();
		if(c)
			c->RefreshAccessKeysDo(alt);
	}
//	RLOGBLOCK("Ctrl::DispatchKey");
//	RLOG("DispatchKey: focusCtrl = " << FormatIntHex((int)~focusCtrl) << ", wnd = " << FormatIntHex((int)~focusCtrlWnd) << ")");
	LLOG("DispatchKey " << keycode << " (0x" << Sprintf("%08x", keycode)
		<< ", " << GetKeyDesc(keycode) << "), count:" << count
		<< " focusCtrl:" << ::Name(focusCtrl) << " focusCtrlWnd:" << ::Name(focusCtrlWnd));
	if((keycode & K_KEYUP) && ignorekeyup)
	{
		ignorekeyup = false;
		return true;
	}
	for(int i = 0; i < keyhook().GetCount(); i++)
		if((*keyhook()[i])(focusCtrl, keycode, count))
			return true;
	dword k = keycode;
	word l = LOWORD(keycode);
	if(!(k & K_DELTA) && l >= 32 && l != 127 && GetDefaultCharset() != CHARSET_UNICODE)
		k = MAKELONG((word)FromUnicode(l, CHARSET_DEFAULT), HIWORD(keycode));
	if(!focusCtrl)
		return false;
	Ptr<Ctrl> p = focusCtrl;
	if(IsUsrLog()) {
		String kl;
		dword k = keycode;
		int l = 0;
		if(k < 65536) {
			kl << "CHAR \'" << ToUtf8((wchar)keycode) << "\' (" << keycode << ')';
			l = 2;
		}
		else {
			kl << "KEY";
			if(k & K_KEYUP) {
				kl << "UP";
				k &= ~K_KEYUP;
				l = 2;
			}
			kl << " " << GetKeyDesc(k);
		}
		UsrLogT(l, kl);
	}
	for(;;) {
		LLOG("Trying to DispatchKey: p = " << Desc(p));
		if(p->IsEnabled() && p->Key(p->unicode ? keycode : k, count))
		{
			LLOG("Ctrl::DispatchKey(" << FormatIntHex(keycode) << ", " << GetKeyDesc(keycode)
				<< "): eaten in " << Desc(p));
			if(IsUsrLog())
				UsrLogT(2, String().Cat() << "-> " << Desc(p));
			return true;
		}
		s_hotkey = true;
		if(!p->GetParent())
			return p->HotKey(keycode);
		p = p->GetParent();
	}

	UsrLogT(2, "key was ignored");

	return false;
}

bool Ctrl::HotKey(dword key)
{
	if(!IsEnabled() || !IsVisible()) return false;
	for(Ptr<Ctrl> ctrl = firstchild; ctrl; ctrl = ctrl->next)
	{
		if(ctrl->IsOpen() && ctrl->IsVisible() && ctrl->IsEnabled() && ctrl->HotKey(key))
		{
			if(IsUsrLog() && s_hotkey) {
				UsrLogT(2, String().Cat() << "HOT-> " << ::Name(ctrl));
				s_hotkey = false;
			}
			return true;
		}
	}
	return false;
}

void Ctrl::DoDeactivate(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl)
{
	if(pfocusCtrl) {
		Ctrl *ptop = pfocusCtrl->GetTopCtrl();
		Ctrl *ntop = nfocusCtrl ? nfocusCtrl->GetTopCtrl() : NULL;
		LLOG("DoDeactivate " << ::Name(ptop) << " in favor of " << ::Name(ntop));
		if(ntop != ptop && !ptop->destroying) {
			ptop->Deactivate();
			ptop->StateH(DEACTIVATE);
			ptop->RefreshAccessKeysDo(false);
		}
	}
}

void Ctrl::DoKillFocus(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl)
{
	if(pfocusCtrl && !pfocusCtrl->destroying) {
		pfocusCtrl->StateH(FOCUS);
		LLOG("LostFocus: " << Desc(pfocusCtrl));
		pfocusCtrl->LostFocus();
	}
	if(pfocusCtrl && pfocusCtrl->parent && !pfocusCtrl->parent->destroying)
		pfocusCtrl->parent->ChildLostFocus();
	SyncCaret();
}

void Ctrl::DoSetFocus(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl, bool activate)
{
	if(activate && focusCtrl == nfocusCtrl && nfocusCtrl) {
		Ctrl *top = nfocusCtrl->GetTopCtrl();
		if((!pfocusCtrl || pfocusCtrl->GetTopCtrl() != top) && !top->destroying) {
			top->StateH(ACTIVATE);
			top->Activate();
			top->RefreshAccessKeysDo(top->VisibleAccessKeys());
		}
	}

	if(focusCtrl == nfocusCtrl && nfocusCtrl && !nfocusCtrl->destroying) {
		nfocusCtrl->GotFocus();
		nfocusCtrl->StateH(FOCUS);
	}
	if(focusCtrl == nfocusCtrl && nfocusCtrl && nfocusCtrl->parent &&
	   !nfocusCtrl->parent->destroying)
		nfocusCtrl->parent->ChildGotFocus();
	SyncCaret();
}

bool Ctrl::SetFocus0(bool activate)
{
	if(IsUsrLog())
		UsrLogT(6, String().Cat() << "SETFOCUS " << Desc(this));
	LLOG("Ctrl::SetFocus " << Desc(this));
	LLOG("focusCtrlWnd " << ::Name(focusCtrlWnd));
	defferedSetFocus = NULL;
	if(focusCtrl == this) return true;
	if(!IsOpen() || !IsEnabled() || !IsVisible()) return false;
	Ptr<Ctrl> pfocusCtrl = focusCtrl;
	Ptr<Ctrl> topwindow = GetTopWindow();
	Ptr<Ctrl> topctrl = GetTopCtrl();
	Ptr<Ctrl> _this = this;
	if(!topwindow) topwindow = topctrl;
	LLOG("SetFocus -> SetWndFocus: topwindow = " << ::Name(topwindow) << ", focusCtrlWnd = " << ::Name(focusCtrlWnd));
	if(!topwindow->HasWndFocus() && !topwindow->SetWndFocus()) return false;// cxl 31.1.2004
	LLOG("SetFocus -> focusCtrl = this: " << FormatIntHex(this) << ", _this = " << FormatIntHex(~_this) << ", " << ::Name(_this));
	focusCtrl = _this;
	focusCtrlWnd = topwindow;
	DoKillFocus(pfocusCtrl, _this);
	LLOG("SetFocus 2");
	DoDeactivate(pfocusCtrl, _this);
	DoSetFocus(pfocusCtrl, _this, activate);
	if(topwindow)
		lastActiveWnd = topwindow;
	return true;
}

bool Ctrl::SetFocus()
{
	return SetFocus0(true);
}

void Ctrl::ActivateWnd()
{
	// notification, don't set physical focus here
	LLOG("ActivateWnd " << Name());
	Ptr<Ctrl> nfocusCtrl = this;
	Ptr<Ctrl> pfocusCtrl = focusCtrl;
	LLOG("About to set focus: " << ::Name(nfocusCtrl));
	DoDeactivate(pfocusCtrl, nfocusCtrl);
	focusCtrl = nfocusCtrl;
	focusCtrlWnd = this;
	DoKillFocus(pfocusCtrl, nfocusCtrl);
	DoSetFocus(pfocusCtrl, nfocusCtrl, true);
	LLOG("Focus: " << ::Name(focusCtrl) << " FocusWnd:" << ::Name(focusCtrlWnd));
}

void Ctrl::SetFocusWnd()
{
	// notification, don't set physical focus here
	LLOG("Ctrl::SetFocusWnd");
	if(focusCtrlWnd != this)
		ActivateWnd();
}

void Ctrl::KillFocusWnd()
{
	// notification, don't set physical focus here
	LLOG("KillFocusWnd " << Name());
	if(this == ~focusCtrlWnd) {
		Ptr<Ctrl> pfocusCtrl = focusCtrl;
		DoDeactivate(pfocusCtrl, NULL);
		focusCtrl = focusCtrlWnd = NULL;
		DoKillFocus(pfocusCtrl, NULL);
	}
}

void Ctrl::ClickActivateWnd()
{
	LLOG("Ctrl::ClickActivateWnd");
	if(this == ~focusCtrlWnd && focusCtrl && focusCtrl->GetTopCtrl() != this)
		ActivateWnd();
}

void Ctrl::DefferedFocusSync()
{
	while(defferedChildLostFocus.GetCount() || defferedSetFocus) {
		Vector< Ptr<Ctrl> > b = defferedChildLostFocus;
		defferedChildLostFocus.Clear();
		for(int i = 0; i < b.GetCount(); i++)
			if(b[i])
				b[i]->ChildLostFocus();
		if(defferedSetFocus)
			defferedSetFocus->SetFocus();
		defferedSetFocus = NULL;
		SyncCaret();
	}
}

void Ctrl::SyncCaret() {
	Rect cr;
	cr.Clear();
	if(focusCtrl && focusCtrl->IsVisible()) {
		bool inframe = focusCtrl->InFrame();
		cr = focusCtrl->GetScreenView();
		cr = RectC(focusCtrl->caretx + cr.left, focusCtrl->carety + cr.top,
				   focusCtrl->caretcx, focusCtrl->caretcy) & cr;
		for(Ctrl *q = focusCtrl->GetParent(); q; q = q->GetParent()) {
			cr &= inframe ? q->GetScreenRect() : q->GetScreenView();
			inframe = q->InFrame();
		}
	}
	if(focusCtrl != caretCtrl || cr != caretRect) {
		LLOG("Do SyncCaret focusCtrl: " << ::Name(focusCtrl) << ", caretCtrl: " << ::Name(caretCtrl));
		WndDestroyCaret();
		if(focusCtrl && !cr.IsEmpty())
			focusCtrl->GetTopCtrl()->WndCreateCaret(cr);
		caretCtrl = focusCtrl;
		caretRect = cr;
	}
}

Ctrl *Ctrl::GetActiveWindow()
{
	Ctrl *q = GetActiveCtrl();
	return q ? q->GetTopWindow() : NULL;
}

String GetKeyDesc(int key)
{
	String desc;
//	key &= 0xFFFF;

	if(key == 0)
		return desc;

	if(key & K_CTRL)  desc << "Ctrl+";
	if(key & K_ALT)   desc << "Alt+";
	if(key & K_SHIFT) desc << "Shift+";


	key &= ~(K_CTRL | K_ALT | K_SHIFT);
	if(key < K_DELTA && key > 32 && key != K_DELETE)
		return desc + String(key, 1);
	if(key >= K_NUMPAD0 && key <= K_NUMPAD9)
		desc << "Num " << (char)(key - K_NUMPAD0 + '0');
	else if(key >= K_0 && key <= K_9)
		desc << (char)('0' + key - K_0);
	else if(key >= K_A && key <= K_Z)
		desc << (char)('A' + key - K_A);
	else if(key >= K_F1 && key <= K_F12)
		desc << Format("F%d", key - K_F1 + 1);
	else {
		static struct {
			int key;
			const char *name;
		} nkey[] = {
			K_TAB, "Tab", K_SPACE, "Space", K_RETURN, "Enter", K_BACKSPACE, "Backspace",
			K_CAPSLOCK, "Caps Lock", K_ESCAPE, "Esc",
			K_PAGEUP, "Page Up", K_PAGEDOWN, "Page Down",
			K_END, "End", K_HOME, "Home",
			K_LEFT, "Left", K_UP, "Up", K_RIGHT, "Right", K_DOWN, "Down",
			K_INSERT, "Insert", K_DELETE, "Delete", K_BREAK, "Break",
			K_MULTIPLY, "Num[*]", K_ADD, "Num[+]", K_SUBTRACT, "Num[-]", K_DIVIDE, "Num[/]",
			K_ALT_KEY, "Alt", K_SHIFT_KEY, "Shift", K_CTRL_KEY, "Ctrl",
			0x100c0, "[`]", 0x100bd, "[-]", 0x100bb, "[=]", 0x100dc, "[\\]",
			0x100db, "[[]", 0x100dd, "[]]",
			0x100ba, "[;]", 0x100de, "[']",
			0x100bc, "[,]", 0x100be, "[.]", 0x100bf, "[/]",
			0, NULL
		};
		for(int i = 0; nkey[i].key; i++)
			if(nkey[i].key == key) {
				desc << nkey[i].name;
				return desc;
			}
		desc << Format("%04x", key);
	}
	return desc;
}
