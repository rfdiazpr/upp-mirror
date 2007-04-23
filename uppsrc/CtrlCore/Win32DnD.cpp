#include "CtrlCore.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#define LLOG(x) LOG(x)

int  GetClipboardFormatCode(const char *format_id);

int ToWin32CF(const char *s)
{
	String fmt = s;
	if(fmt == "text")
		return CF_TEXT;
	if(fmt == "unicodetext")
		return CF_UNICODETEXT;
	if(fmt == "image")
		return CF_DIB;
	return GetClipboardFormatCode(s);
}

String FromWin32CF(int cf)
{
	if(cf == CF_TEXT)
		return "text";
	if(cf == CF_UNICODETEXT)
		return "unicodetext";
	if(cf == CF_DIB)
		return "image";
	char h[256];
	GetClipboardFormatNameA(cf, h, 255);
	return h;
}

FORMATETC ToFORMATETC(const char *s)
{
	FORMATETC fmtetc;
	fmtetc.cfFormat = ToWin32CF(s);
	fmtetc.dwAspect = DVASPECT_CONTENT;
	fmtetc.lindex = -1;
	fmtetc.ptd = NULL;
	fmtetc.tymed = TYMED_HGLOBAL;
	return fmtetc;
}

String AsString(POINTL p)
{
	return String().Cat() << "[" << p.x << ", " << p.y << "]";
}

struct UDropTarget : public IDropTarget {
	ULONG         rc;
	Ptr<Ctrl>     ctrl;
	Ptr<Ctrl>     dndctrl;
	LPDATAOBJECT  data;
	Index<String> fmt;

	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(DragEnter)(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragOver)(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragLeave)();
	STDMETHOD(Drop)(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);

	bool DnD(POINTL p, bool drop, DWORD *effect, DWORD keys);
	void FreeData();
	String Get(const char *fmt) const;

	UDropTarget() { rc = 1; data = NULL; }
	~UDropTarget();
};

STDMETHODIMP UDropTarget::QueryInterface(REFIID iid, void ** ppv)
{
	if(iid == IID_IUnknown || iid == IID_IDropTarget) {
		*ppv = this;
		AddRef();
		return S_OK;
	}
	*ppv = NULL;
	return E_NOINTERFACE;
}

String UDropTarget::Get(const char *fmt) const
{
	FORMATETC fmtetc = ToFORMATETC(fmt);
	STGMEDIUM s;
	if(data->GetData(&fmtetc, &s) == S_OK && s.tymed == TYMED_HGLOBAL) {
		char *val = (char *)GlobalLock(s.hGlobal);
		String data(val, GlobalSize(s.hGlobal));
		GlobalUnlock(s.hGlobal);
		ReleaseStgMedium(&s);
		return data;
    }
	return Null;
}

bool UDropTarget::DnD(POINTL pl, bool drop, DWORD *effect, DWORD keys)
{
	if(!ctrl) return false;
	Point p(pl.x, pl.y);
	DnDEvent d;
	d.dt = this;
	d.action = 0;
	d.drop = drop;
	p -= ctrl->GetScreenRect().TopLeft();
	Ctrl *c = ctrl->ChildFromPoint(p);
	if(c != dndctrl) {
		if(dndctrl) dndctrl->DragLeave();
		dndctrl = c;
		if(dndctrl) dndctrl->DragEnter(p, d);
	}
	dword e = *effect;
	*effect = DROPEFFECT_NONE;
	if(c) {
		c->DragAndDrop(p, d);
		if(d.action) {
			*effect = DROPEFFECT_MOVE;
			if(d.action & DND_COPY)
				*effect = DROPEFFECT_COPY;
			if(d.action & DND_MOVE)
				*effect = DROPEFFECT_MOVE;
			if(keys & MK_CONTROL)
				*effect = DROPEFFECT_COPY;
			if(keys & (MK_ALT|MK_SHIFT))
				*effect = DROPEFFECT_MOVE;
			if((*effect & e) == 0) {
				*effect = DROPEFFECT_NONE;
				if(e & DROPEFFECT_COPY)
					*effect = DROPEFFECT_COPY;
				if(e & DROPEFFECT_MOVE)
					*effect = DROPEFFECT_MOVE;
			}
		}
	}
	return true;
}

STDMETHODIMP UDropTarget::DragEnter(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	LLOG("DragEnter " << pt);
	data = pDataObj;
	data->AddRef();
	fmt.Clear();
	IEnumFORMATETC *fe;
	if(!ctrl || pDataObj->EnumFormatEtc(DATADIR_GET, &fe) != NOERROR) {
		*pdwEffect = DROPEFFECT_NONE;
		return NOERROR;
	}
	FORMATETC fmtetc;
	while(fe->Next(1, &fmtetc, 0) == S_OK) {
		fmt.FindAdd(FromWin32CF(fmtetc.cfFormat));
		if(fmtetc.ptd)
			CoTaskMemFree(fmtetc.ptd);
	}
	fe->Release();
	DnD(pt, false, pdwEffect, grfKeyState);
	return NOERROR;
}


STDMETHODIMP UDropTarget::DragOver(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	LLOG("DragOver " << pt << " keys: " << grfKeyState);
	DnD(pt, false, pdwEffect, grfKeyState);
	return NOERROR;
}

void UDropTarget::FreeData()
{
	if(data) {
		data->Release();
		data = NULL;
	}
}

STDMETHODIMP UDropTarget::DragLeave()
{
	LLOG("DragLeave");
	if(dndctrl) {
		dndctrl->DragLeave();
		dndctrl = NULL;
	}
	FreeData();
	return NOERROR;
}

STDMETHODIMP UDropTarget::Drop(LPDATAOBJECT, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	LOG("Drop");
	DnD(pt, true, pdwEffect, grfKeyState);
	FreeData();
	return NOERROR;
}

UDropTarget::~UDropTarget()
{
	if(data) data->Release();
}

bool ClipData::Has(const char *type) const
{
	return dt->fmt.Find(type) >= 0;
}

String ClipData::Get(const char *type) const
{
	return dt->Get(type);
}

// --------------------------------------------------------------------------------------------

struct  UDataObject : public IDataObject {
	ULONG              rc;
	DnDAction         *action;
	dword              effect;

	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(GetData)(FORMATETC *fmtetc, STGMEDIUM *medium);
	STDMETHOD(GetDataHere)(FORMATETC *, STGMEDIUM *);
	STDMETHOD(QueryGetData)(FORMATETC *fmtetc);
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *, FORMATETC *pformatetcOut);
	STDMETHOD(SetData)(FORMATETC *fmtetc, STGMEDIUM *medium, BOOL release);
	STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC **ief);
	STDMETHOD(DAdvise)(FORMATETC *, DWORD, IAdviseSink *, DWORD *);
	STDMETHOD(DUnadvise)(DWORD);
	STDMETHOD(EnumDAdvise)(LPENUMSTATDATA *);

	UDataObject() { rc = 1; }
};

struct UEnumFORMATETC : public IEnumFORMATETC {
	ULONG        rc;
	int          ii;
	UDataObject *data;

	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(Next)(ULONG n, FORMATETC *fmtetc, ULONG *fetched);
	STDMETHOD(Skip)(ULONG n);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumFORMATETC **newEnum);

	UEnumFORMATETC()  { ii = 0; rc = 1; }
	~UEnumFORMATETC() { data->Release(); }
};

struct UDropSource : public IDropSource {
	ULONG rc;
	Image move, copy;

	STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObj);
	STDMETHOD_(ULONG, AddRef)(void)  { return ++rc; }
	STDMETHOD_(ULONG, Release)(void) { if(--rc == 0) { delete this; return 0; } return rc; }

	STDMETHOD(QueryContinueDrag)(BOOL fEscapePressed, DWORD grfKeyState);
	STDMETHOD(GiveFeedback)(DWORD dwEffect);

	UDropSource() { rc = 1; }
};

STDMETHODIMP UDataObject::QueryInterface(REFIID iid, void ** ppv)
{
	if(iid == IID_IUnknown || iid == IID_IDataObject) {
		*ppv = this;
		AddRef();
		return S_OK;
	}
	*ppv = NULL;
	return E_NOINTERFACE;
}

void SetMedium(STGMEDIUM *medium, const String& data)
{
	int sz = data.GetCount();
	HGLOBAL hData = GlobalAlloc(0, sz + 4);
	if (hData) {
		char *ptr = (char *) GlobalLock(hData);
		memcpy(ptr, ~data, sz);
		memset(ptr + sz, 0, 4);
		GlobalUnlock(hData);
		medium->tymed = TYMED_HGLOBAL;
		medium->hGlobal = hData;
		medium->pUnkForRelease = 0;
	}
}

STDMETHODIMP UDataObject::GetData(FORMATETC *fmtetc, STGMEDIUM *medium)
{
	String fmt = FromWin32CF(fmtetc->cfFormat);
	String *s = action->datasrc.FindPtr(fmt);
	if(s) {
		SetMedium(medium, s->GetCount() ? *s : action->ctrl->GetClip(fmt, CLIP_DROP));
		return S_OK;
	}
	return DV_E_FORMATETC;
}

STDMETHODIMP UDataObject::GetDataHere(FORMATETC *, STGMEDIUM *)
{
    return DV_E_FORMATETC;
}

STDMETHODIMP UDataObject::QueryGetData(FORMATETC *fmtetc)
{
	return action->datasrc.Find(FromWin32CF(fmtetc->cfFormat)) >= 0 ? S_OK : DV_E_FORMATETC;
}

STDMETHODIMP UDataObject::GetCanonicalFormatEtc(FORMATETC *, FORMATETC *pformatetcOut)
{
    pformatetcOut->ptd = NULL;
    return E_NOTIMPL;
}

static int CF_PERFORMEDDROPEFFECT = RegisterClipboardFormat("Performed DropEffect");

STDMETHODIMP UDataObject::SetData(FORMATETC *fmtetc, STGMEDIUM *medium, BOOL release)
{
	if(fmtetc->cfFormat == CF_PERFORMEDDROPEFFECT && medium->tymed == TYMED_HGLOBAL) {
        DWORD *val = (DWORD*)GlobalLock(medium->hGlobal);
        effect = *val;
        GlobalUnlock(medium->hGlobal);
        if(release)
            ReleaseStgMedium(medium);
        return S_OK;
    }
	return E_NOTIMPL;
}

STDMETHODIMP UDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ief)
{
	UEnumFORMATETC *ef = new UEnumFORMATETC;
	ef->data = this;
	AddRef();
	*ief = ef;
	return S_OK;
}

STDMETHODIMP UDataObject::DAdvise(FORMATETC *, DWORD, IAdviseSink *, DWORD *)
{
	return OLE_E_ADVISENOTSUPPORTED;
}


STDMETHODIMP UDataObject::DUnadvise(DWORD)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP UDataObject::EnumDAdvise(LPENUMSTATDATA FAR*)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP UEnumFORMATETC::QueryInterface(REFIID riid, void FAR* FAR* ppvObj)
{
	if (riid == IID_IUnknown || riid == IID_IEnumFORMATETC) {
		*ppvObj = this;
		AddRef();
		return NOERROR;
    }
	*ppvObj = NULL;
	return ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP UEnumFORMATETC::Next(ULONG n, FORMATETC *t, ULONG *fetched) {
	if(t == NULL)
		return E_INVALIDARG;
	if(fetched) *fetched = 0;
	while(ii < data->action->datasrc.GetCount() && n > 0) {
		if(fetched) (*fetched)++;
		n--;
		*t++ = ToFORMATETC(data->action->datasrc.GetKey(ii++));
	}
	return n ? S_FALSE : NOERROR;
}

STDMETHODIMP UEnumFORMATETC::Skip(ULONG n) {
	ii += n;
	if(ii >= data->action->datasrc.GetCount())
		return S_FALSE;
	return NOERROR;
}

STDMETHODIMP UEnumFORMATETC::Reset()
{
    ii = 0;
    return NOERROR;
}

STDMETHODIMP UEnumFORMATETC::Clone(IEnumFORMATETC **newEnum)
{
	if(newEnum == NULL)
		return E_INVALIDARG;
	UEnumFORMATETC *ef = new UEnumFORMATETC;
	ef->data = data;
	data->AddRef();
	ef->ii = ii;
	*newEnum = ef;
	return NOERROR;
}

STDMETHODIMP UDropSource::QueryInterface(REFIID riid, void **ppvObj)
{
	if (riid == IID_IUnknown || riid == IID_IDropSource) {
		*ppvObj = this;
		AddRef();
		return NOERROR;
	}
	*ppvObj = NULL;
	return ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP UDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
	if(fEscapePressed)
		return DRAGDROP_S_CANCEL;
	else
	if(!(grfKeyState & (MK_LBUTTON|MK_MBUTTON|MK_RBUTTON)))
		return DRAGDROP_S_DROP;
	Ctrl::ProcessEvents();
	return NOERROR;
}

STDMETHODIMP UDropSource::GiveFeedback(DWORD dwEffect)
{
	DUMP(dwEffect);
	Image m = IsNull(move) ? copy : move;
	if((dwEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY) {
		if(!IsNull(copy)) m = copy;
	}
	else
	if((dwEffect & DROPEFFECT_MOVE) == DROPEFFECT_MOVE) {
		if(!IsNull(move)) m = move;
	}
	else
		m = Image::No();
	Ctrl::OverrideCursor(m);
	Ctrl::SetMouseCursor(m);
	return S_OK;
}


Image MakeDragImage(const Image& arrow, const Image& arrow98, Image sample)
{
	if(IsWin2K()) {
		ImageBuffer b;
		if(IsNull(sample)) {
			sample = CtrlCoreImg::DndData();
			b = sample;
			Over(b, Point(0, 0), arrow, arrow.GetSize());
		}
		else {
			b.Create(128, 128);
			memset(~b, 0, sizeof(RGBA) * b.GetLength());
			Size ssz = sample.GetSize();
			Over(b, Point(2, 22), sample, sample.GetSize());
			for(int y = 20; y < 96; y++) {
				RGBA *s = b[y];
				RGBA *e = s + 96;
				while(s < e)
					(s++)->a >>= 1;
				e += 32;
				int q = 128;
				while(s < e) {
					s->a = (s->a * q) >> 8;
					q -= 4;
					s++;
				}
			}
			int qq = 128;
			for(int y = 96; y < 128; y++) {
				RGBA *s = b[y];
				RGBA *e = s + 96;
				while(s < e) {
					s->a = (s->a * qq) >> 8;
					s++;
				}
				e += 32;
				int q = 255;
				while(s < e) {
					s->a = (s->a * q * qq) >> 16;
					q -= 8;
					s++;
				}
				qq -= 4;
			}
		}
		Over(b, Point(0, 0), arrow, arrow.GetSize());
		return b;
	}
	else
		return arrow98;
}

int DnDAction::Do()
{
	UDataObject *obj = new UDataObject;
	obj->action = this;
	UDropSource *dsrc = new UDropSource;
	DWORD result = 0;
	Image m = Ctrl::OverrideCursor(CtrlCoreImg::DndMove());
	if(copy) dsrc->copy = MakeDragImage(CtrlCoreImg::DndCopy(), CtrlCoreImg::DndCopy98(), sample);
	if(move) dsrc->move = MakeDragImage(CtrlCoreImg::DndMove(), CtrlCoreImg::DndMove98(), sample);
	HRESULT r = DoDragDrop(obj, dsrc,
	                       (copy ? DROPEFFECT_COPY : 0) | (move ? DROPEFFECT_MOVE : 0), &result);
	DWORD re = obj->effect;
	obj->Release();
	dsrc->Release();
//	LOG("End of DnD");
	Ctrl::OverrideCursor(m);
	Ctrl::SyncCaret();
	Ctrl::CheckMouseCtrl();
	if(r == DRAGDROP_S_DROP) {
		if(((result | re) & DROPEFFECT_COPY) == DROPEFFECT_COPY && copy)
			return DND_COPY;
		if(((result | re) & DROPEFFECT_MOVE) == DROPEFFECT_MOVE && move)
			return DND_MOVE;
    }
	return DND_NONE;
}

DnDAction::DnDAction(Ctrl *src, const char *fmts)
{
	ctrl = src;
	copy = move = true;
	if(fmts) {
		Vector<String> f = Split(fmts, ';');
		for(int i = 0; i < f.GetCount(); i++)
			datasrc.GetAdd(f[i]);
	}
}

void ReleaseUDropTarget(UDropTarget *dt)
{
	dt->Release();
}

UDropTarget *NewUDropTarget(Ctrl *ctrl)
{
	UDropTarget *dt = new UDropTarget;
	dt->ctrl = ctrl;
	return dt;
}

#endif

END_UPP_NAMESPACE
