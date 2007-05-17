#include "CtrlCore.h"

#ifdef PLATFORM_X11

NAMESPACE_UPP

int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions, const VectorMap<String, String>& data)
{
	// Implement! :)
}

bool AcceptFiles(PasteClip& clip) {}

Vector<String> GetFiles(PasteClip& clip) {}

Ptr<Ctrl> sDnDSource;

Ctrl * Ctrl::GetDragAndDropSource()
{
	return sDnDSource;
}

Point            dndpos;
static Ptr<Ctrl> dndctrl;
Index<String>    drop_formats;

Ctrl *Ctrl::GetDragAndDropTarget()
{
	return dndctrl;
}

void Ctrl::DropEvent(XWindow& w, XEvent *event)
{
	if(event->type != ClientMessage)
		return;
	LOG("DropEvent Client Message");
	DUMP(XAtomName(event->xclient.message_type));
	static Atom XdndEnter = XAtom("XdndEnter");
	if(event->xclient.message_type == XdndEnter) {
		LOG("DnDEnter");
		drop_formats.Clear();
		if(event->xclient.data.l[0] & 1) {
			Vector<int> v = GetPropertyInts(event->xclient.data.l[0],
			                                XAtom("XdndTypeList"), XA_ATOM);
			for(int i = 0; i < v.GetCount(); i++)
				drop_formats.Add(XAtomName(v[i]));
		}
		else
			for(int i = 2; i <= 4; i++)
				drop_formats.Add(XAtomName(event->xclient.data.l[0]));
		DUMPC(drop_formats);
	}
	static Atom XdndPosition = XAtom("XdndPosition");
	if(event->xclient.message_type == XdndPosition) {
		dword x = event->xclient.data.l[2];
		Point p(HIWORD(x), LOWORD(x));
		PasteClip d;
		d.dnd = true;
		d.paste = false;
		d.accepted = false;
		Point hp = p - GetScreenRect().TopLeft();
		Ctrl *c = FindCtrl(this, hp);
		Rect sw = c->GetScreenView();
		if(sw.Contains(p))
			p -= sw.TopLeft();
		else
			c = NULL;
		d.allowed = DND_MOVE|DND_COPY;
		d.action = 0;
		if(c != dndctrl) {
			if(dndctrl) dndctrl->DragLeave();
			dndctrl = c;
			if(dndctrl) dndctrl->DragEnter(p, d);
		}
		if(c) {
			dndpos = p;
			c->DragAndDrop(p, d);
			while(c && !d.IsAccepted()) {
				c->ChildDragAndDrop(p, d);
				c = c->GetParent();
			}
			XEvent e;
			Zero(e);
			event->xclient.data.l[0] = GetWindow();
			event->xclient.type = ClientMessage;
			event->xclient.display = Xdisplay;
			event->xclient.window = event->xclient.data.l[0];
			event->xclient.message_type = XAtom("XdndStatus");
			event->xclient.format = 32;
			if(d.IsAccepted())
				event->xclient.data.l[1] = 1;
			XSendEvent(Xdisplay, event->xclient.data.l[0], XFalse, 0, &e);
		}
	}
	if(event->xclient.message_type == XAtom("XdndLeave")) {
		dndctrl->DragLeave();
		dndctrl = NULL;
	}
//	if(event->xclient.message_type == XAtom("XdndDrop")) {
//	}
}

bool   XDnDHas(const char *fmt)
{
	return false;
}

String XDnDGet(const char *fmt)
{
	return Null;
}

END_UPP_NAMESPACE

#endif
