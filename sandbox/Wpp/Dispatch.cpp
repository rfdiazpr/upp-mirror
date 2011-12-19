#include "Wpp.h"

#define LLOG(x)   DLOG(x)
#define LTIMING(x)

enum { DISPATCH_VARARGS = -1 };

struct DispatchNode : Moveable<DispatchNode> {
	VectorMap<String, int> subnode;
	void   (*view)(Http&);
	String method;
	int    argpos;

	DispatchNode() { view = NULL; argpos = Null; }
};

static Vector<DispatchNode>& sDispatchMap()
{
	static Vector<DispatchNode> x;
	return x;
}

static VectorMap<String, Vector<String> >& sLinkMap()
{
	static VectorMap<String, Vector<String> > x;
	return x;
}

static Index<uintptr_t>& sViewIndex()
{
	static Index<uintptr_t> x;
	return x;
}

void DumpDispatchMap()
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	for(int i = 0; i < DispatchMap.GetCount(); i++) {
		LLOG("-------------");
		String sub;
		for(int j = 0; j < DispatchMap[i].subnode.GetCount(); j++)
			sub << DispatchMap[i].subnode.GetKey(j) << "->" << DispatchMap[i].subnode[j] << ", ";
		LLOG(i << " " << (bool)DispatchMap[i].view << ": " << sub);
	}			
}

Vector<String> *GetUrlViewLinkParts(const String& id)
{
	int q = sLinkMap().Find(id);
	if(q < 0)
		return NULL;
	return &sLinkMap()[q];
}

String MakeLink(void (*view)(Http&), const Vector<Value>& arg)
{
	int q = sViewIndex().Find((uintptr_t)view);
	ASSERT(q >= 0); // -> Internal server error
	if(q < 0)
		return String();
	StringBuffer out;
	MakeLink(out, sLinkMap()[q], arg);
	return out;
}

void RegisterView0(void (*view)(Http&), const char *id, const char *path, bool primary)
{
	LLOG("RegisterView " << path);
	Vector<String>& linkpart = sLinkMap().GetAdd(id);
	sViewIndex().FindAdd((uintptr_t)view);
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	Vector<String> h = Split(path, '/');
	if(DispatchMap.GetCount() == 0)
		DispatchMap.Add();
	int q = 0;
	int linkargpos = 0;
	for(int i = 0; i < h.GetCount(); i++) {
		String s = h[i];
		LLOG(" Node " << h[i]);
		DispatchNode& n = DispatchMap[q];
		if(*s == '*') {
			int argpos = Null;
			if(IsDigit(s[1]))
				linkargpos = argpos = minmax(atoi(~s + 1), 0, 30);
			else
			if(s[1] == '*')
				argpos = DISPATCH_VARARGS;
			q = DispatchMap.GetCount();
			LLOG(" Adding arg " << argpos << ": " << q);
			n.subnode.Add(Null, q);
			DispatchMap.Add();
			DispatchMap[q].argpos = argpos;
			if(primary)
				linkpart.Add(String(linkargpos++, 1));
		}
		else {
			if(primary)
				if(linkpart.GetCount() && (byte)*linkpart.Top() >= 32)
					linkpart.Top() << '/' << s;
				else
					linkpart.Add(s);
			q = n.subnode.Get(s, -1);
			if(q < 0) {
				q = DispatchMap.GetCount();
				LLOG(" Adding " << s << ": " << q);
				n.subnode.Add(s, q);
				DispatchMap.Add();
			}
		}
	}
	ASSERT_(!DispatchMap[q].view, "duplicate view " + String(path));
	DispatchMap[q].view = view;
	DumpDispatchMap();
}

void RegisterView(void (*view)(Http&), const char *id, const char *path)
{
	ASSERT_(sViewIndex().Find((uintptr_t)view) < 0, "duplicate view function registration " + String(id));
	Vector<String> h = Split(path, ';');
	for(int i = 0; i < h.GetCount(); i++)
		RegisterView0(view, id, h[i], i == 0);
}

struct BestDispatch {
	void            (*view)(Http&);
	int             matched_parts;
	int             matched_params;
	Vector<String>& arg;
	
	BestDispatch(Vector<String>& arg) : arg(arg) { matched_parts = -1; matched_params = 0; view = NULL; }
};

void GetBestDispatch(const Vector<String>& h, int ii, const DispatchNode& n, Vector<String>& arg,                     
                     BestDispatch& bd, int matched_parts, int matched_params)
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	if(ii >= h.GetCount()) {
		if(n.view && (matched_parts > bd.matched_parts ||
		              matched_parts == bd.matched_parts && matched_params > bd.matched_params)) {
			bd.arg <<= arg;
			bd.view = n.view;
			bd.matched_parts = matched_parts;
		}
		if(h.GetCount() == 0) {
			int q = n.subnode.Find(String());
			while(q >= 0) {
				const DispatchNode& an = DispatchMap[n.subnode[q]];
				if(an.argpos == DISPATCH_VARARGS && an.view) {
					bd.view = an.view;
					bd.arg.Clear();
					break;
				}
				q = n.subnode.FindNext(q);
			}
		}
		return;
	}
	int qq = n.subnode.Get(h[ii], -1);
	if(qq >= 0)
		GetBestDispatch(h, ii + 1, DispatchMap[qq], arg, bd, matched_parts + 1, matched_params);
	int q = n.subnode.Find(String());
	while(q >= 0) {
		int qq = n.subnode[q];
		int ac = arg.GetCount();
		const DispatchNode& an = DispatchMap[qq];
		int apos = an.argpos;
		LLOG(" *" << qq << " apos: " << apos);
		if(apos == DISPATCH_VARARGS) {
			if(an.view && (matched_parts > bd.matched_parts ||
			               matched_parts == bd.matched_parts && matched_params > bd.matched_params)) {
				bd.arg <<= arg;
				bd.arg.Append(h, ii, h.GetCount() - ii);
				bd.view = an.view;
				bd.matched_parts = matched_parts;
			}
		}
		else {
			String pv;
			if(IsNull(apos))
				arg.Add(h[ii]);
			else {
				String& at = arg.At(apos);
				pv = at;
				at = h[ii];
			}
			GetBestDispatch(h, ii + 1, an, arg, bd, matched_parts, matched_params + 1);
			if(!IsNull(apos))
				arg[apos] = pv;
		}
		arg.SetCount(ac);
		q = n.subnode.FindNext(q);
	}
}

void Http::Dispatch(Socket& socket)
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	if(Read(socket)) {
		int len = GetLength();
		content = socket.ReadCount(len);
		LLOG(content);
		Cout() << method << " " << uri << "\n";
		int q = uri.Find('?');
		if(q >= 0) {
			ParseRequest(~uri + q + 1);
			uri.Trim(q);
		}
		for(int i = hdrfield.Find("cookie"); i >= 0; i = hdrfield.FindNext(i)) {
			const String& h = hdrfield[i];
			int q = 0;
			for(;;) {
				int qq = h.Find('=', q);
				if(qq < 0)
					break;
				String id = ToLower(TrimBoth(h.Mid(q, qq - q)));
				qq++;
				DUMP(id);
				q = h.Find(';', qq);
				if(q < 0) {
					request.Add(id, UrlDecode(h.Mid(qq)));
					break;
				}
				request.Add(id, UrlDecode(h.Mid(qq, q - qq)));
				q++;
			}
		}
		request_content_type = GetHeader("content-type");
		String rc = ToLower(request_content_type);
		if(method == "POST")
			if(rc.StartsWith("application/x-www-form-urlencoded"))
				ParseRequest(content);
			else
			if(rc.StartsWith("multipart/"))
				ReadMultiPart(content);
		DUMPM(request);
		Vector<String> h = Split(uri, '/');
		DUMPC(h);
		Vector<String> a;
		BestDispatch bd(arg);
		if(DispatchMap.GetCount())
			GetBestDispatch(h, 0, DispatchMap[0], a, bd, 0, 0);
		DUMPC(arg);
		if(bd.view) {
			try {
				(*bd.view)(*this);
			}
			catch(SqlExc e) {
				response << "Internal server error<br>"
				         << "SQL ERROR: " << e;
				code = 500;
				code_text = "Internal server error";
			}
		}
		String r;
		if(redirect.GetCount()) {
			r << "HTTP/1.1 " << code << " Found\r\n";
			r << "Location: " << redirect << "\r\n";
		}
		else
			r <<
				"HTTP/1.0 " << code << ' ' << code_text << "\r\n"
				"Date: " <<  WwwFormat(GetUtcTime()) << "\r\n"
				"Server: U++\r\n"
				"Content-Length: " << response.GetCount() << "\r\n"
				"Connection: close\r\n"
				"Content-Type: " << content_type << "\r\n"
				<< cookies <<
				"\r\n";
		LLOG(r);
		socket.Write(r);
		LLOG(response);
		socket.Write(response);
	}
}
