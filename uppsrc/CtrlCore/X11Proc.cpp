#include "CtrlCore.h"

#ifdef PLATFORM_X11

#define LLOG(x)        //LOG(x)

#define XTIMING(x)     //TIMING(x)

#define LOGEVENTPROC

static dword sKbdState;
static dword sModState;

void ClearKbdState_()
{
	sKbdState = 0;
}

bool GetShift() { return sKbdState & ShiftMask; }
bool GetCtrl() { return sKbdState & ControlMask; }
bool GetAlt() { return sKbdState & Mod1Mask; }
bool GetCapsLock() { return sKbdState & LockMask; }
bool GetMouseLeft() { return sModState & Button1Mask; }
bool GetMouseRight() { return sModState & (Ctrl::Xbuttons >= 3 ? Button3Mask : Button2Mask); }
bool GetMouseMiddle() { return sModState & (Ctrl::Xbuttons >= 3 ? Button2Mask : 0); }

int  Ctrl::Xbuttontime;

dword Ctrl::KEYtoK(dword key)
{
	if(key != K_CTRL_KEY && key != K_SHIFT_KEY) {
		if(GetCtrl()) key |= K_CTRL;
		if(GetAlt()) key |= K_ALT;
		if(GetShift()) key |= K_SHIFT;
	}
	return key;
}

void Ctrl::SetLastActive(XWindow *w, Ctrl *la)
{
	while(w) {
		LLOG("  to " << ::Name(w->ctrl));
		w->last_active = la;
		w = w->owner ? w->owner->GetXWindow() : NULL;
	}
}

void Ctrl::EventProc(XWindow& w, XEvent *event)
{
	Ptr<Ctrl> _this = this;
	bool pressed = false;
	int  count = 1;
	switch(event->type) {
	case NoExpose:
		LLOG("NoExpose serial " << event->xnoexpose.serial);
		break;
	case GraphicsExpose:
		LLOG("GraphicsExpose serial " << event->xgraphicsexpose.serial);
	case Expose: {
			XExposeEvent& e = event->xexpose;
			w.exposed = true;
			LLOG("Expose " << RectC(e.x, e.y, e.width, e.height));
			Invalidate(w, RectC(e.x, e.y, e.width, e.height));
		}
		return;
	case ConfigureNotify: {
			XConfigureEvent& e = event->xconfigure;
			int x, y;
			Window dummy;
			if(top)
				XTranslateCoordinates(Xdisplay, top->window, Xroot, 0, 0, &x, &y, &dummy);
			Rect rect = RectC(x, y, e.width, e.height);
			LLOG("CongigureNotify " << rect);
			if(GetRect() != rect)
				SetWndRect(rect);
		}
		return;
	default:
		if(!IsEnabled()) return;
	}
	XTIMING("XUserInput");
	switch(event->type) {
	case FocusIn:
		if(w.xic)
			XSetICFocus(w.xic);
		break;
	case FocusOut:
		if(w.xic)
			XUnsetICFocus(w.xic);
		break;
	case KeyPress:
		pressed = true;
		LLOG("event type:" << event->type << " state:" << event->xkey.state <<
		     "keycode:" << event->xkey.keycode);
		for(;;) {
			XEvent ev1[1], ev2[1];
			if(!IsWaitingEvent()) break;
			do
				XNextEvent(Xdisplay, ev1);
			while(ev1->type == NoExpose && IsWaitingEvent());
			LLOG("ev1 type:" << ev1->type << " state:" << ev1->xkey.state <<
			     "keycode:" << ev1->xkey.keycode);
			if(ev1->type != KeyRelease ||
			   ev1->xkey.state != event->xkey.state ||
			   ev1->xkey.keycode != event->xkey.keycode ||
			   !IsWaitingEvent()) {
			   	XPutBackEvent(Xdisplay, ev1);
			   	break;
			}
			do
				XNextEvent(Xdisplay, ev2);
			while(ev2->type == NoExpose && IsWaitingEvent());
			LLOG("ev2 type:" << ev2->type << " state:" << ev2->xkey.state <<
			     "keycode:" << ev2->xkey.keycode);
			if(ev2->type != KeyPress ||
			   ev2->xkey.state != event->xkey.state ||
			   ev2->xkey.keycode != event->xkey.keycode) {
			   	XPutBackEvent(Xdisplay, ev2);
			   	XPutBackEvent(Xdisplay, ev1);
			   	break;
			}
			else {
				XFilterEvent(ev1, None);
				XFilterEvent(ev2, None);
			}
			count++;
		}
	case KeyRelease: {
			mousePos = Point(event->xkey.x_root, event->xkey.y_root);
			char buff[128];
			Xeventtime = event->xkey.time;
			LLOG("Key Xeventtime: " << Xeventtime << " count:" << count);
			KeySym keysym;
			int    chr = 0;
			if(pressed && w.xic) {
				Status status;
				int len = Xutf8LookupString(w.xic, &event->xkey, buff, sizeof(buff), &keysym, &status);
				buff[len] = 0;
				if(status == XLookupChars || status == XLookupBoth)
				    chr = FromUtf8(buff, len)[0];
				else
				if(status != XLookupKeySym && status != XLookupBoth)
				    keysym = 0;
			}
			else {
				XLookupString(&event->xkey, buff, 1, &keysym, NULL);
				chr = *buff;
			}
			if(keysym == XK_Control_L || keysym == XK_Control_R) {
				keysym = XK_Control_L;
				if(pressed)
					sKbdState |= ControlMask;
				else
					sKbdState &= ~ControlMask;
			}
			if(keysym == XK_Shift_L || keysym == XK_Shift_R) {
				keysym = XK_Shift_L;
				if(pressed)
					sKbdState |= ShiftMask;
				else
					sKbdState &= ~ShiftMask;
			}
			if(keysym == XK_Meta_L || keysym == XK_Meta_R || keysym == XK_Alt_L ||
			   keysym == XK_Alt_R || keysym == XK_Super_L || keysym == XK_Super_R ||
			   keysym == XK_Hyper_L || keysym == XK_Hyper_R || keysym == XK_ISO_Prev_Group) {
				keysym == XK_Meta_L;
				if(pressed)
					sKbdState |= Mod1Mask;
				else
					sKbdState &= ~Mod1Mask;
			}
			LLOG("KeySym:" << FormatIntHex(keysym) << " " << (char)keysym << " " << count);
			dword up = pressed ? 0 : K_KEYUP;
			static struct { KeySym keysym; dword key; } tab[] = {
				{ XK_ISO_Left_Tab, K_TAB|K_SHIFT },
				{ XK_BackSpace, K_BACKSPACE },
				{ XK_Tab, K_TAB },
				{ XK_Return, K_ENTER },
				{ XK_KP_Enter, K_ENTER },
				{ XK_Escape, K_ESCAPE },
				{ 0, 0 }
			};
			for(int i = 0; tab[i].keysym; i++)
				if(tab[i].keysym == keysym) {
					DispatchKey(KEYtoK(tab[i].key)|up, count);
					return;
				}
			if(chr >= 1 && chr < 32) {
				DispatchKey(KEYtoK(chr - 1 + K_CTRL_A)|up, count);
				return;
			}
			if(keysym >= 0xff80 && keysym <= 0xffb9 && chr) {
				DispatchKey(KEYtoK(chr)|up, count);
				return;
			}
			if(keysym >= 0xff00 && chr < 128 ||
			   (GetCtrl() || GetAlt()) && keysym >= 0x20 && keysym < 0x7f) {
				if(keysym >= 'a' && keysym <= 'z')
					keysym = keysym - 'a' + 'A';
				DispatchKey(KEYtoK(keysym|K_DELTA)|up, count);
				return;
			}

			if((chr == 32 || chr == 9 || chr == 13) && !pressed)
				DispatchKey(chr|K_KEYUP, count);
			if(chr && pressed)
				DispatchKey(chr, count);
		}
		break;
	case ButtonPress: {
			if(!HasWndFocus() && !popup)
				SetWndFocus();
			ClickActivateWnd();
			mousePos = Point(event->xbutton.x_root, event->xbutton.y_root);
			XUngrabPointer(Xdisplay, CurrentTime);
			XButtonEvent& e = event->xbutton;
			sModState = e.state;
			Xeventtime = e.time;
			if(ignoreclick) break;
			Point p = Point(e.x, e.y);
			dword action = DOWN;
			if((dword)e.time - (dword)Xbuttontime < 800)
				action = DOUBLE;
			Xbuttontime = e.time;
			switch(e.button) {
			case Button1:
				sModState |= Button1Mask;
				DispatchMouse(LEFT|action, p, 0);
				break;
			case Button2:
				sModState |= Button2Mask;
				if(Xbuttons < 3)
					DispatchMouse(RIGHT|action, p, 0);
				else
					DispatchMouse(LEFT|action, p, MIDDLEBUTTON);
				break;
			case Button3:
				sModState |= Button3Mask;
				DispatchMouse(RIGHT|action, p, 0);
				break;
			}
			if(_this) PostInput();
		}
		break;
	case ButtonRelease: {
			mousePos = Point(event->xbutton.x_root, event->xbutton.y_root);
			XButtonEvent& e = event->xbutton;
			sModState = e.state;
			Xeventtime = e.time;
			Point p = Point(e.x, e.y);
			switch(e.button) {
			case Button1:
				sModState &= ~Button1Mask;
				break;
			case Button2:
				sModState &= ~Button2Mask;
				break;
			case Button3:
				sModState &= ~Button3Mask;
				break;
			}
			if(ignoreclick)
				EndIgnore();
			else
				switch(e.button) {
				case Button1:
					DispatchMouse(LEFTUP, p, 0);
					break;
				case Button2:
					if(Xbuttons < 3)
						DispatchMouse(RIGHTUP, p, 0);
					else
						DispatchMouse(LEFTUP, p, MIDDLEBUTTON);
					break;
				case Button3:
					DispatchMouse(RIGHTUP, p, 0);
					break;
				case Button4:
					DispatchMouse(MOUSEWHEEL, p, 120);
					break;
				case Button5:
					DispatchMouse(MOUSEWHEEL, p, -120);
					break;
				}
			if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
				ReleaseCtrlCapture();
			if(_this) PostInput();
		}
		break;
	case MotionNotify:
		while(XCheckWindowEvent(Xdisplay, top->window, PointerMotionMask, event));
		EndIgnore();
		mousePos = Point(event->xmotion.x_root, event->xmotion.y_root);
		Xeventtime = event->xmotion.time;
		Xbuttontime = Xeventtime - 0x80000000;
		sModState = event->xmotion.state;
		DispatchMouse(MOUSEMOVE, Point(event->xmotion.x, event->xmotion.y));
		DoCursorShape();
		break;
	}
}

#endif
