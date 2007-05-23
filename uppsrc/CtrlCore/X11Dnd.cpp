#include "CtrlCore.h"

#define LLOG(x) LOG(x)

#ifdef PLATFORM_X11

NAMESPACE_UPP

int Xdnd_waiting_status;
int Xdnd_status_timeout;
int Xdnd_status;

XEvent ClientMsg(Window src, const char *type, int format = 32)
{
	XEvent e;
	Zero(e);
	e.xclient.type = ClientMessage;
	e.xclient.display = Xdisplay;
	e.xclient.window = src;
	e.xclient.message_type = XAtom(type);
	e.xclient.format = format;
	return e;
}

struct DnDLoop : LocalLoop {
	Image move, copy, reject;
	Vector<Atom> fmt;
	const VectorMap<String, String> *data;
	Ptr<Ctrl> source;
	void SetFmts(Window w, Atom property);
	Window src, target;

	void Request(XSelectionRequestEvent *se);
	void Sync();
	virtual void  LeftUp(Point, dword);
	virtual void  MouseMove(Point p, dword);
	virtual Image CursorImage(Point, dword);
};


void DnDLoop::Sync()
{
	if(Xdnd_waiting_status)
		return;
	Window root, child;
	unsigned int d1;
	int x, y, d2;
	Window tgt = Xroot;
	int version;
	for(;;) {
		if(XQueryPointer(Xdisplay, tgt, &root, &tgt, &x, &y, &d2, &d2, &d1)) {
			if(!tgt)
				break;
			Vector<int> x = GetPropertyInts(tgt, XAtom("XdndAware"));
			LLOG("XdndAware " << tgt << ": " << x.GetCount());
			if(x.GetCount()) {
				version = min(3, x[0]);
				break;
			}
		}
		else {
			tgt = None;
			break;
		}
	}
	if(tgt != target) {
		if(target) {
			LLOG("Sending XdndLeave to " << target);
			XEvent e = ClientMsg(target, "XdndLeave");
			e.xclient.data.l[0] = src;
			XSendEvent(Xdisplay, target, XFalse, 0, &e);
		}
		target = tgt;
		if(target) {
			LLOG("Sending XdndEnter to " << target);
			XEvent e = ClientMsg(target, "XdndEnter");
			e.xclient.data.l[0] = src;
			e.xclient.data.l[1] = MAKEWORD(fmt.GetCount() > 3, version);
			for(int i = 0; i < min(3, fmt.GetCount()); i++)
				e.xclient.data.l[i] = fmt[i];
			XSendEvent(Xdisplay, target, XFalse, 0, &e);
		}
	}

	if(target) {
		LLOG("Sending XdndPosition to " << target << " " << x << ", " << y);
		XEvent e = ClientMsg(target, "XdndPosition");
		e.xclient.data.l[0] = src;
		e.xclient.data.l[1] = 0;
		e.xclient.data.l[2] = MAKELONG(y, x);
		e.xclient.data.l[3] = Xeventtime;
		e.xclient.data.l[4] = XAtom("XdndActionCopy");
		XSendEvent(Xdisplay, target, XFalse, 0, &e);
		XFlush(Xdisplay);
		Xdnd_waiting_status = true;
		Xdnd_status_timeout = GetTickCount() + 200;
	}
}

void DnDLoop::LeftUp(Point, dword)
{
	EndLoop();
}

void DnDLoop::MouseMove(Point p, dword)
{
	Sync();
}

Image DnDLoop::CursorImage(Point, dword)
{
	return reject;
	if(Xdnd_status < 0)
		return reject;
	return copy;
}

void DnDLoop::SetFmts(Window w, Atom property)
{
	Buffer<Atom> x(fmt.GetCount());
	for(int i = 0; i < fmt.GetCount(); i++) {
		x[i] = fmt[i];
		LLOG('\t' << XAtomName(x[i]));
	}
	XChangeProperty(Xdisplay, w, property, XAtom("ATOM"),
	                8 * sizeof(Atom), 0, (unsigned char*)~x,
	                fmt.GetCount());
}

void DnDLoop::Request(XSelectionRequestEvent *se)
{
	LLOG("DnDRequest " << XAtomName(se->target));
	XEvent e;
	e.xselection.type      = SelectionNotify;
	e.xselection.display   = Xdisplay;
	e.xselection.requestor = se->requestor;
	e.xselection.selection = XAtom("XdndSelection");
	e.xselection.target    = se->target;
	e.xselection.time      = se->time;
	e.xselection.property  = se->property;
	if(se->target == XAtom("TARGETS")) {
		LLOG("DnDRequest targets:");
		SetFmts(se->requestor, se->property);
	}
	else {
		String f = XAtomName(se->target);
		int i = data->Find(f);
		String d;
		if(i >= 0)
			d = (*data)[i];
		else
			if(source)
				d = source->GetDropData(f);
		if(d.GetCount())
			XChangeProperty(Xdisplay, se->requestor, se->property, se->target, 8, PropModeReplace,
			                d, d.GetCount());
		else
		    e.xselection.property = None;
	}
	XSendEvent(Xdisplay, se->requestor, XFalse, 0, &e);
}

Ptr<DnDLoop> dndloop;

void DnDRequest(XSelectionRequestEvent *se)
{
	if(dndloop) dndloop->Request(se);
}

void DnDClear() {}

Image MakeDragImage(const Image& arrow, Image sample);

int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions,
                        const VectorMap<String, String>& data)
{
	DnDLoop d;
	d.reject = MakeDragImage(CtrlCoreImg::DndNone(), sample);
	if(actions & DND_COPY) d.copy = MakeDragImage(CtrlCoreImg::DndCopy(), sample);
	if(actions & DND_MOVE) d.move = MakeDragImage(CtrlCoreImg::DndMove(), sample);
	d.SetMaster(*this);
	d.data = &data;
	d.source = this;
	dndloop = &d;
	Vector<String> f = Split(fmts, ';');
	for(int i = 0; i < f.GetCount(); i++)
		d.fmt.Add(XAtom(f[i]));
	for(int i = 0; i < data.GetCount(); i++)
		d.fmt.Add(XAtom(data.GetKey(i)));
	d.SetFmts(xclipboard().win, XAtom("XdndTypeList"));
	XSetSelectionOwner(Xdisplay, XAtom("XdndSelection"), xclipboard().win, CurrentTime);
	d.src = xclipboard().win;
	d.target = None;
	d.Run();
	SyncCaret();
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
Index<String>    Ctrl::drop_formats;

Ctrl *Ctrl::GetDragAndDropTarget()
{
	return dndctrl;
}

PasteClip sMakeDropClip(bool paste)
{
	PasteClip d;
	d.type = 1;
	d.paste = paste;
	d.accepted = false;
	d.allowed = DND_MOVE|DND_COPY;
	d.action = 0;
	return d;
}

void Ctrl::DnD(Window src, bool paste)
{
	Ctrl *c = dndctrl;
	if(!c) return;
	PasteClip d = sMakeDropClip(paste);
	c->DragAndDrop(dndpos, d);
	while(c && !d.IsAccepted()) {
		c->ChildDragAndDrop(dndpos, d);
		c = c->GetParent();
	}
	XEvent e = ClientMsg(src, "XdndStatus");
	e.xclient.data.l[0] = GetWindow();
	e.xclient.data.l[4] = XAtom("XdndActionCopy");
	if(d.IsAccepted())
		e.xclient.data.l[1] = 1;
	XSendEvent(Xdisplay, src, XFalse, 0, &e);
}

void Ctrl::DropEvent(XWindow& w, XEvent *event)
{
	if(Xdnd_waiting_status && GetTickCount() > Xdnd_status_timeout && dndloop) {
		Xdnd_status = 0;
		Xdnd_waiting_status = false;
		dndloop->Sync();
		LLOG("XdndStatus timeout");
	}
	if(event->type != ClientMessage)
		return;
	LLOG("Client Message " << GetWindow());
	Window src = event->xclient.data.l[0];
	static Atom XdndEnter, XdndPosition, XdndLeave, XdndDrop, XdndStatus;
	if(!XdndEnter) {
		XdndEnter = XAtom("XdndEnter");
		XdndPosition = XAtom("XdndPosition");
		XdndLeave = XAtom("XdndLeave");
		XdndDrop = XAtom("XdndDrop");
		XdndStatus = XAtom("XdndStatus");
	}
	if(event->xclient.message_type == XdndStatus && dndloop &&
	   (event->xclient.data.l[1] & 1) && event->xclient.data.l[0] == dndloop->target) {
		LLOG("XdndStatus");
		Xdnd_status = DND_COPY;
		Xdnd_waiting_status = false;
		dndloop->Sync();
	}
	if(event->xclient.message_type == XdndEnter) {
		LLOG("DnDEnter");
		drop_formats.Clear();
		if(event->xclient.data.l[1] & 1) {
			Vector<int> v = GetPropertyInts(src, XAtom("XdndTypeList"), XA_ATOM);
			for(int i = 0; i < v.GetCount(); i++)
				drop_formats.Add(XAtomName(v[i]));
		}
		else
			for(int i = 2; i <= 4; i++)
				drop_formats.Add(XAtomName(event->xclient.data.l[0]));
	}
	if(event->xclient.message_type == XdndPosition) {
		dword x = event->xclient.data.l[2];
		Point p(HIWORD(x), LOWORD(x));
		LLOG("XdndPosition " << p);
		Point hp = p - GetScreenRect().TopLeft();
		Ctrl *c = FindCtrl(this, hp);
		Rect sw = c->GetScreenView();
		if(sw.Contains(p))
			p -= sw.TopLeft();
		else
			c = NULL;
		LLOG("Target widget " << UPP::Name(c) << ", " << p);
		if(c != dndctrl) {
			if(dndctrl) dndctrl->DragLeave();
			dndctrl = c;
			PasteClip d = sMakeDropClip(false);
			if(dndctrl) dndctrl->DragEnter(p, d);
		}
		if(c) {
			dndpos = p;
			DnD(src, false);
		}
	}
	if(event->xclient.message_type == XdndLeave && dndctrl) {
		dndctrl->DragLeave();
		dndctrl = NULL;
	}
	if(event->xclient.message_type == XdndDrop && dndctrl) {
		LLOG("DROP!");
		DnD(src, true);
		dndctrl->DragLeave();
		dndctrl = NULL;
		XEvent e = ClientMsg(src, "XdndFinished");
		e.xclient.data.l[0] = GetWindow();
		XSendEvent(Xdisplay, src, XFalse, 0, &e);
	}
}

END_UPP_NAMESPACE

#endif
