#include <Ole/Ctrl/OleCtrl.h>

#ifdef PLATFORM_WIN32

NAMESPACE_UPP

enum { HIMETRIC_INCH = 2540 }; // HIMETRIC units per inch

Draw& ScreenInfo();

Size ToHiMetric(Size pixel_size)
{
	static Size logdpi = ScreenInfo().GetSizeCaps(LOGPIXELSX, LOGPIXELSY);
	return Size(
		iscaleceil(pixel_size.cx, HIMETRIC_INCH, logdpi.cx),
		iscaleceil(pixel_size.cy, HIMETRIC_INCH, logdpi.cy));
}

Size FromHiMetric(Size himetric_size)
{
	static Size logdpi = ScreenInfo().GetSizeCaps(LOGPIXELSX, LOGPIXELSY);
	return Size(
		iscalefloor(himetric_size.cx, logdpi.cx, HIMETRIC_INCH),
		iscalefloor(himetric_size.cy, logdpi.cy, HIMETRIC_INCH));
}

Rect GetWindow(HDC hdc)
{
	Point pt;
	Size sz;
	GetWindowOrgEx(hdc, pt);
	GetWindowExtEx(hdc, sz);
	return Rect(pt, sz);
}

Rect GetViewport(HDC hdc)
{
	Point pt;
	Size sz;
	GetViewportOrgEx(hdc, pt);
	GetViewportExtEx(hdc, sz);
	return Rect(pt, sz);
}

IUnknown *OcxObject::GetUnknown()
{
	ASSERT(!interface_map.IsEmpty());
	return (IUnknown *)interface_map[0];
}

HRESULT OcxObject::RawQueryInterface(const GUID& iid, void **ppv)
{
	int i = 0;

	if(interface_map.IsEmpty() || iid != IID_IUnknown && (i = interface_map.Find(iid)) < 0) {
		*ppv = 0;
#if LOG_QUERIES >= 1
		String name;
		if(ocx_info)
			name = ocx_info->GetName();
		else
			name = "<unknown object>";
#endif
		LOGQUERY("\t\tcast ERROR: " << name << " -> " << GetInterfaceName(iid));
#if LOG_QUERIES >= 2
		OCXLOG("\tGUID = " << Format(iid));
		for(int i = 0; i < interface_map.GetCount(); i++) {
			const GUID& imid = interface_map.GetKey(i);
			String n = GetInterfaceName(imid);
			if(n.GetLength() < 30)
				n.Cat(' ', 30 - n.GetLength());
			OCXLOG("\t\t\t" << n << " - " << Format(imid));
		}
#endif
		return E_NOINTERFACE;
	}
#if LOG_QUERIES >= 2
	String name;
	if(ocx_info)
		name = ocx_info->GetName();
	else
		name = "<unknown object>";
	OCXLOG("\t\tcast OK:    " << name << " -> " << GetInterfaceName(iid));
#endif
	*ppv = interface_map[i];
	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return S_OK;
}

HRESULT OcxObject::RawGetTypeInfo(unsigned tinfo, LCID lcid, ITypeInfo **ppinfo)
{
	if(tinfo != 0)
		return DISP_E_BADINDEX;
	if(!ocx_info)
		return E_FAIL;
	IRef<ITypeInfo> typeinfo = ocx_info->GetDispatchTypeInfo();
	if(!typeinfo)
		return E_FAIL;
	// currently ignore lcid
	*ppinfo = typeinfo.AddRef();
	return S_OK;
}

HRESULT OcxObject::RawGetIDsOfNames(REFIID riid, OLECHAR **names, unsigned cnames, LCID lcid, DISPID *dispid)
{
	if(!ocx_info)
		return E_FAIL;
	IRef<ITypeInfo> typeinfo = ocx_info->GetDispatchTypeInfo();
	if(!typeinfo)
		return E_FAIL;
	return typeinfo->GetIDsOfNames(names, cnames, dispid);
}

HRESULT OcxObject::RawInvoke(IDispatch *dispatch, DISPID dispid, word flags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *excep, unsigned *arg_err)
{
	LOGINVOKE("IDispatch::Invoke");
	if(!ocx_info)
		return E_FAIL;
	if(dispid == 0) { // convert object to dispatch value
		if(!result) return E_INVALIDARG;
		result->vt = VT_DISPATCH;
		result->pdispVal = dispatch;
		if(dispatch) dispatch->AddRef();
		return S_OK;
	}
	IRef<ITypeInfo> typeinfo = ocx_info->GetDispatchTypeInfo();
#if LOG_INVOKES >= 1
	BSTR names[1] = { NULL };
	unsigned count = 0;
	typeinfo->GetNames(dispid, names, 1, &count);
	String name = "(unknown)";
	if(names[0])
		name = BSTRToString(names[0]);
	switch(flags) {
	case DISPATCH_METHOD:         name << " - method"; break;
	case DISPATCH_PROPERTYGET:    name << " - propget"; break;
	case DISPATCH_PROPERTYPUT:    name << " - propput"; break;
	case DISPATCH_PROPERTYPUTREF: name << " - propputref"; break;
	}
	LOG("Member name: " << name);
#endif
	if(!typeinfo)
		return E_FAIL;
	TYPEATTR *attr;
	typeinfo->GetTypeAttr(&attr);
	LOGSYSOCX("GetTypeInfo: typekind = " << (int)attr->typekind);
	typeinfo->ReleaseTypeAttr(attr);
	HRESULT res = typeinfo->Invoke(dispatch, dispid, flags, params, result, excep, arg_err);
	LOGINVOKE("//IDispatch::Invoke");
	return LOGRESULT(res);
}

HRESULT OcxProvideClassInfo::GetClassInfo(ITypeInfo **info)
{
	if(!info) return E_INVALIDARG;
	if(!ocx_info) {
		*info = NULL;
		return E_POINTER;
	}
	*info = ocx_info->GetCoClassTypeInfo().AddRef();
	return S_OK;
}

HRESULT OcxProvideClassInfo::GetGUID(dword guidkind, GUID *guid)
{
	if(!guid)
		return E_POINTER;
	if(!ocx_info) {
		*guid = __uuidof(0);
		return E_NOTIMPL;
	}
	if(guidkind == GUIDKIND_DEFAULT_SOURCE_DISP_IID) {
		*guid = ~ocx_info->GetEventGUID();
		return ocx_info->GetEventGUID().IsEmpty() ? E_UNEXPECTED : S_OK;
	}
	return E_INVALIDARG;
}

OcxTypeInfo::OcxTypeInfo(const GUID& coclass_guid, const GUID& dispatch_guid, const GUID& event_guid,
	New new_fn, String name, const char* help, int ver, bool is_control)
: coclass_guid(coclass_guid), dispatch_guid(dispatch_guid), event_guid(event_guid)
, new_fn(new_fn), name(name), help(help), ver(ver), is_control(is_control)
, object_count(0)
, refcount(1)
, lock_count(0)
#ifndef _USRDLL
, registration(0)
#endif
{
	OcxTypeLib::Get().Add(*this);
}

int OcxTypeInfo::IncRef()
{
	return InterlockedIncrement(&refcount);
}

int OcxTypeInfo::DecRef()
{
	int result = InterlockedDecrement(&refcount);
	VERIFY(result > 0); // if this throws, the factory has been over-released
	return result;
}

GLOBAL_VAR(InitProc, LateInitProc);

static void DoLateInit()
{
	static bool late_init = false;
	if(!late_init) {
		INTERLOCKED
			if(!late_init) {
				late_init = true;
				if(LateInitProc())
					LateInitProc()();
			}
	}
}

HRESULT OcxTypeInfo::CreateInstance(IUnknown *outer, REFIID iid, void **object)
{
	try {
		DoLateInit();
		IRef<IUnknown> unk = new_fn(*this);
		LOGCREATE("\tnew " << name << ": " << object_count << " instances in system");
		HRESULT hr = unk->QueryInterface(iid, object);
		return hr;
	}
	catch(Exc e) {
		RLOG("OcxTypeInfo::CreateInstance error: " << e);
		return E_OUTOFMEMORY;
	}
	catch(...) {
		return E_OUTOFMEMORY;
	}
}

HRESULT OcxTypeInfo::LockServer(BOOL lock)
{
	if(lock)
		AtomicInc(lock_count);
	else
		VERIFY(AtomicDec(lock_count) >= 0); // over-unlock check
	return S_OK;
}

IRef<ITypeInfo> OcxTypeInfo::GetCoClassTypeInfo()
{
	if(!coclass_info && !coclass_guid.IsEmpty()) { // oportunity lock used to initialize type info
		critical.Enter();
		if(!coclass_info)
			OcxTypeLib::Get()->GetTypeInfoOfGuid(coclass_guid, coclass_info.Set());
		critical.Leave();
	}
	return coclass_info;
}

IRef<ITypeInfo> OcxTypeInfo::GetDispatchTypeInfo()
{
	if(!dispatch_info && !dispatch_guid.IsEmpty()) { // oportunity lock used to initialize type info
		critical.Enter();
		if(!dispatch_info)
			OcxTypeLib::Get()->GetTypeInfoOfGuid(dispatch_guid, dispatch_info.Set());
		critical.Leave();
	}
	return dispatch_info;
}

/*
ITypeInfoPtr& OcxTypeInfo::GetEventTypeInfo()
{
	if(!(bool)event_info && !event_guid.IsEmpty()) { // oportunity lock used to initialize type info
		critical.Enter();
		if(!(bool)event_info)
			OcxTypeLib::Get()->GetTypeInfoOfGuid(event_guid, &event_info);
		critical.Leave();
	}
	return event_info;
}
*/

OcxTypeLib::OcxTypeLib()
: lib_ver(1)
{
	CoInitialize(0);
}

OcxTypeLib& OcxTypeLib::Get()
{
	static OcxTypeLib ocx;
	return ocx;
}

String OcxTypeLib::GetLibName() const
{
	if(!IsNull(lib_name))
		return lib_name;
	String s = ToLower(GetFileTitle(GetModuleFileName()));
	if(!IsNull(s))
		s.Set(0, ToUpper(s[0]));
	return s;
}

bool OcxTypeLib::CanUnload() const
{
	for(int i = 0; i < objects.GetCount(); i++)
		if(!objects[i]->CanUnload()) {
			OCXLOG("OcxTypeLib::CanUnload failed for " << objects[i]->GetName());
			return false;
		}
	OCXLOG("OcxTypeLib::CanUnload succeeded");
	return true;
}

HRESULT OcxTypeLib::Register()
{
	String modname = GetModuleFileName();
	String lib_name = GetLibName();
	LOGSYSOCX("Registering library \"" << modname << '\"');

	HRESULT hr;
	IRef<ITypeLib>& typelib = GetTypeLib();
	if(!typelib)
		return SELFREG_E_TYPELIB;
	TLIBATTR *attr;
	if(FAILED(hr = typelib->GetLibAttr(&attr)))
		return hr;
	String lib_guid = Guid(attr->guid);
	typelib->ReleaseTLibAttr(attr);

	for(int i = 0; i < objects.GetCount(); i++) {
		const OcxTypeInfo& entry = *objects[i];
		String progID = lib_name;
		String verID = progID << '.' << entry.name;
		verID << '.' << (entry.ver ? entry.ver : lib_ver);
		String guid = entry.coclass_guid;
		LOGSYSOCX("Registering object " << verID << ", CLSID = " << guid);
		String clss_key = "CLSID\\" + guid;
#ifdef _USRDLL
		String server = clss_key + "\\InprocServer32";
#endif
		String version;
		version << lib_ver << '.' << entry.ver;
		String help = entry.help;
		if(IsNull(help)) {
			if(!IsNull(display_name))
				help << display_name << ' ';
			help << entry.name << (entry.IsControl() ? " Control " : " Object ") << version;
		}
		DeleteWinReg(verID, HKEY_CLASSES_ROOT);
		DeleteWinReg(progID, HKEY_CLASSES_ROOT);
		DeleteWinReg(clss_key, HKEY_CLASSES_ROOT);
		if(!SetWinRegString(help,        NULL,              verID, HKEY_CLASSES_ROOT)
		|| !SetWinRegString(guid,        NULL,              verID + "\\CLSID", HKEY_CLASSES_ROOT)
		|| !SetWinRegString(help,        NULL,              progID, HKEY_CLASSES_ROOT)
		|| !SetWinRegString(guid,        NULL,              progID + "\\CLSID", HKEY_CLASSES_ROOT)
		|| !SetWinRegString(help,        NULL,              clss_key, HKEY_CLASSES_ROOT)
		|| !SetWinRegString(verID,       NULL,              clss_key + "\\ProgID", HKEY_CLASSES_ROOT)
		|| !SetWinRegString(progID,      NULL,              clss_key + "\\VersionIndependentProgID", HKEY_CLASSES_ROOT)
		|| !SetWinRegString(lib_guid,    NULL,              clss_key + "\\TypeLib", HKEY_CLASSES_ROOT)
#ifdef _USRDLL
		|| !SetWinRegString(modname,     NULL,              server, HKEY_CLASSES_ROOT)
		|| !SetWinRegString("Apartment", "ThreadingModel",  server, HKEY_CLASSES_ROOT)
#else
		|| !SetWinRegString(modname,     NULL,              clss_key + "\\LocalServer32", HKEY_CLASSES_ROOT)
		|| !SetWinRegString(modname,     NULL,              clss_key + "\\LocalServer", HKEY_CLASSES_ROOT)
		|| !SetWinRegString("ole32.dll", NULL,              clss_key + "\\InprocHandler32", HKEY_CLASSES_ROOT)
		|| !SetWinRegString("ole2.dll",  NULL,              clss_key + "\\InprocHandler", HKEY_CLASSES_ROOT)
#endif
		|| !SetWinRegString(version,     NULL,              clss_key + "\\Version", HKEY_CLASSES_ROOT))
			return REGDB_E_WRITEREGDB;
		if(entry.IsControl()) {
			if(!SetWinRegString("",              NULL, clss_key + "\\Control", HKEY_CLASSES_ROOT)
			|| !SetWinRegString(modname + ", 1", NULL, clss_key + "\\ToolboxBitmap32", HKEY_CLASSES_ROOT)
			|| !SetWinRegString("",              NULL, verID + "\\Insertable", HKEY_CLASSES_ROOT)
			|| !SetWinRegString("",              NULL, progID + "\\Insertable", HKEY_CLASSES_ROOT)
			|| !SetWinRegString("",              NULL, clss_key + "\\Insertable", HKEY_CLASSES_ROOT))
				return REGDB_E_WRITEREGDB;
		}
	}

//	UniStr ulib = modname;
	OleBstr ulib(modname);
	if(FAILED(hr = RegisterTypeLib(~typelib, (wchar_t *)~ulib, ~ulib)))
		return hr;
	return S_OK;
}

HRESULT OcxTypeLib::Unregister()
{
	String lib = GetModuleFileName();
	String lib_name = GetLibName();
	LOGSYSOCX("Unregistering library \"" << lib << '\"');
	for(int i = 0; i < objects.GetCount(); i++) {
		const OcxTypeInfo& entry = *objects[i];
		String progID = lib_name;
		String verID = progID << '.' << entry.name;
		verID << '.' << (entry.ver ? entry.ver : lib_ver);
		DeleteWinReg(verID, HKEY_CLASSES_ROOT);
		DeleteWinReg(progID, HKEY_CLASSES_ROOT);
		DeleteWinReg("CLSID\\" + String(entry.coclass_guid), HKEY_CLASSES_ROOT);
		DeleteWinReg("SOFTWARE\\Classes\\" + verID, HKEY_LOCAL_MACHINE);
	}

	IRef<ITypeLib>& typelib = GetTypeLib();
	if(!typelib)
		return SELFREG_E_TYPELIB;
	TLIBATTR *attr;
	HRESULT hr;
	if(FAILED(hr = typelib->GetLibAttr(&attr)))
		return hr;
	hr = UnRegisterTypeLib(attr->guid,
		attr->wMajorVerNum, attr->wMinorVerNum,
		attr->lcid, attr->syskind);
	typelib->ReleaseTLibAttr(attr);
	return hr;
}

/*
#ifndef _USRDLL
HRESULT OcxTypeLib::RegisterObjects()
{
	for(int i = 0; i < objects.GetCount(); i++) {
		OcxTypeInfo& entry = *objects[i];
		HRESULT hr;
		if(FAILED(hr = CoRegisterClassObject(entry.GetCoClassGUID(), &entry,
			CLSCTX_SERVER, REGCLS_MULTIPLEUSE, &entry.registration))) {
			RevokeObjects();
			return hr;
		}
	}
	return S_OK;
}
#endif

#ifndef _USRDLL
void OcxTypeLib::RevokeObjects()
{
	for(int i = 0; i < objects.GetCount(); i++) {
		OcxTypeInfo& entry = *objects[i];
		if(entry.registration) {
			CoRevokeClassObject(entry.registration);
			entry.registration = 0;
		}
	}
}
#endif
*/

HRESULT OcxTypeLib::GetFactory(REFCLSID clsid, REFIID iid, void **ppv)
{
	int i;
	for(i = 0; i < ocx_init.GetCount(); i++)
		ocx_init[i]();
	ocx_init.Clear();

	*ppv = 0;
	i = objects.Find(clsid);
	if(i < 0)
		return CLASS_E_CLASSNOTAVAILABLE;
	OcxTypeInfo *entry = objects[i];
	return entry->QueryInterface(iid, ppv);
}

IRef<ITypeLib>& OcxTypeLib::GetTypeLib()
{
	if(!typelib) { // opportunity lock used to initialize type library info
		critical.Enter();
		if(!typelib)
			LoadTypeLib(GetModuleFileName().ToWString(), typelib.Set());
		critical.Leave();
	}
	return typelib;
}

#ifdef _USRDLL
STDAPI DllCanUnloadNow()
{
	LOGSYSOCX("DllCanUnloadNow");
	DoLateInit();
	return (OcxTypeLib::Get().CanUnload() ? S_OK : S_FALSE);
}
#endif

#ifdef _USRDLL
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	LOGSYSOCX("DllGetClassObject");
	return OcxTypeLib::Get().GetFactory(rclsid, riid, ppv);
}
#endif

#ifdef _USRDLL
STDAPI DllRegisterServer()
{
	LOGSYSOCX("DllRegisterServer");
	try {
		DoLateInit();
		HRESULT hr = OcxTypeLib::Get().Register();
		if(SUCCEEDED(hr))
			LOGSYSOCX("-> succeeded");
		else
			LOGSYSOCX("-> failed: " << GetErrorMessage(hr));
		return hr;
	}
	catch(_com_error e) {
		LOGSYSOCX(e.Error());
		return e.Error();
	}
}
#endif

#ifdef _USRDLL
STDAPI DllUnregisterServer()
{
	LOGSYSOCX("DllUnregisterServer");
	try {
		DoLateInit();
		HRESULT hr = OcxTypeLib::Get().Unregister();
		LOGSYSOCX((SUCCEEDED(hr) ? "-> succeeded" : "-> failed"));
		return hr;
	}
	catch(_com_error e) {
		LOGSYSOCX(e.Error());
		return e.Error();
	}
}
#endif

/*
#ifndef _USRDLL
bool ExeRegisterServer()
{
	LOGSYSOCX("ExeRegisterServer");
	HRESULT hr = OcxTypeLib::Get().Register();
	if(FAILED(hr))
		LOGSYSOCX("-> error: " << GetErrorMessage(hr));
	return SUCCEEDED(hr);
}
#endif
*/

/*
#ifndef _USRDLL
bool ExeUnregisterServer()
{
	LOGSYSOCX("ExeRegisterServer");
	HRESULT hr = OcxTypeLib::Get().Unregister();
	if(FAILED(hr))
		LOGSYSOCX("-> error: " << GetErrorMessage(hr));
	return SUCCEEDED(hr);
}
#endif
*/

/*
#ifndef _USRDLL
void ExeRunServer()
{
	LOGSYSOCX("ExeRunServer");
//	PromptOK("Running server!");
	HRESULT hr;
	if(FAILED(hr = OcxTypeLib::Get().RegisterObjects())) {
		LOGSYSOCX("-> registration error: " << GetErrorMessage(hr));
		return;
	}
	LOGSYSOCX("-> registration OK");

	MSG msg;
	while(GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	LOGSYSOCX("end of message loop");
}
#endif
*/

END_UPP_NAMESPACE

#endif
