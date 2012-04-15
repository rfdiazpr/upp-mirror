#include "Skylark.h"

#define LLOG(x)    DLOG(x)
#define LTIMING(x) RTIMING(x)

enum { DISPATCH_VARARGS = -1 };

struct DispatchNode : Moveable<DispatchNode> {
	VectorMap<String, int> subnode;
	void   (*view)(Http&);
	int    argpos;
	int    method;
	String id;
	
	enum { GET, POST };

	DispatchNode() { view = NULL; argpos = Null; method = GET; }
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
	if(q < 0)
		throw Exc("Invalid view");
	if(q < 0)
		return String();
	StringBuffer out;
	MakeLink(out, sLinkMap()[q], arg);
	return out;
}

void RegisterView0(void (*view)(Http&), const char *id, String path, bool primary)
{
	LLOG("RegisterView " << path);
	Vector<String>& linkpart = sLinkMap().GetAdd(id);
	sViewIndex().FindAdd((uintptr_t)view);
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	int method = DispatchNode::GET;
	int q = path.Find(':');
	if(q >= 0) {
		if(path.Mid(q + 1) == "POST")
			method = DispatchNode::POST;
		path = path.Mid(0, q);
	}
	Vector<String> h = Split(path, '/');
	if(DispatchMap.GetCount() == 0)
		DispatchMap.Add();
	q = 0;
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
	DispatchMap[q].method = method;
	DispatchMap[q].id = id;
//	DumpDispatchMap();
}

struct ViewData {
	void (*view)(Http&);
	String id;
	String path;
};

static Array<ViewData>& sViewData()
{
	static Array<ViewData> x;
	return x;
}

void RegisterView(void (*view)(Http&), const char *id, const char *path)
{
	Array<ViewData>& v = sViewData();
	ViewData& w = v.Add();
	w.view = view;
	w.id = id;
	w.path = path;
}

static VectorMap<String, String>& sViewVar()
{
	static VectorMap<String, String> x;
	return x;
}

void SetViewVar(const char *id, const char *value)
{
	sViewVar().GetAdd(id) = value;
}

static String& sRoot()
{
	static String x;
	return x;
}

void SetViewRoot(const char *root)
{
	sRoot() = root;
}

void FinalizeViews()
{
	Array<ViewData>& w = sViewData();
	for(int i = 0; i < w.GetCount(); i++) {
		const ViewData& v = w[i];
		ASSERT_(sViewIndex().Find((uintptr_t)v.view) < 0, "duplicate view function registration " + String(v.id));
		Vector<String> h = Split(ReplaceVars(sRoot() + '/' + v.path, sViewVar(), '$'), ';');
		for(int i = 0; i < h.GetCount(); i++)
			RegisterView0(v.view, v.id, h[i], i == 0);
	}
	w.Clear();
}

struct BestDispatch {
	void            (*view)(Http&);
	int             matched_parts;
	int             matched_params;
	Vector<String>& arg;
	String          id;
	
	BestDispatch(Vector<String>& arg) : arg(arg) { matched_parts = -1; matched_params = 0; view = NULL; }
};

void GetBestDispatch(int method,
                     const Vector<String>& part, int ii, const DispatchNode& n, Vector<String>& arg,                     
                     BestDispatch& bd, int matched_parts, int matched_params)
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	if(ii >= part.GetCount()) {
		if(n.view && n.method == method &&
		   (matched_parts > bd.matched_parts ||
		    matched_parts == bd.matched_parts && matched_params > bd.matched_params)) {
			bd.arg <<= arg;
			bd.view = n.view;
			bd.matched_parts = matched_parts;
			bd.id = n.id;
		}
		int q = n.subnode.Find(String());
		while(q >= 0) {
			const DispatchNode& an = DispatchMap[n.subnode[q]];
			if(an.argpos == DISPATCH_VARARGS && an.view && an.method == method) {
				bd.view = an.view;
				bd.arg.Clear();
				break;
			}
			q = n.subnode.FindNext(q);
		}
		return;
	}
	int qq = n.subnode.Get(part[ii], -1);
	if(qq >= 0)
		GetBestDispatch(method, part, ii + 1, DispatchMap[qq], arg, bd, matched_parts + 1, matched_params);
	int q = n.subnode.Find(String());
	while(q >= 0) {
		int qq = n.subnode[q];
		int ac = arg.GetCount();
		const DispatchNode& an = DispatchMap[qq];
		int apos = an.argpos;
		LLOG(" *" << qq << " apos: " << apos);
		if(apos == DISPATCH_VARARGS) {
			if(an.view && an.method == method &&
			   (matched_parts > bd.matched_parts || matched_parts == bd.matched_parts && matched_params > bd.matched_params)) {
				bd.arg <<= arg;
				bd.arg.Append(part, ii, part.GetCount() - ii);
				bd.view = an.view;
				bd.matched_parts = matched_parts;
				bd.id = an.id;
			}
		}
		else {
			String pv;
			if(IsNull(apos))
				arg.Add(part[ii]);
			else {
				String& at = arg.At(apos);
				pv = at;
				at = part[ii];
			}
			GetBestDispatch(method, part, ii + 1, an, arg, bd, matched_parts, matched_params + 1);
			if(!IsNull(apos))
				arg[apos] = pv;
		}
		arg.SetCount(ac);
		q = n.subnode.FindNext(q);
	}
}

void Http::Dispatch(TcpSocket& socket)
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	if(Read(socket)) {
		int len = GetLength();
		content = socket.GetAll(len);
		LLOG(content);
		Cout() << method << " " << uri << "\n";
		String r;
		for(int i = 0; i < benchmark; i++) {
			var.Clear();
			arg.Clear();
			LTIMING("Request processing");
			request_content_type = GetHeader("content-type");
			String rc = ToLower(request_content_type);
			bool post = method == "POST";
			if(post)
				if(rc.StartsWith("application/x-www-form-urlencoded"))
					ParseRequest(content);
				else
				if(rc.StartsWith("multipart/"))
					ReadMultiPart(content);
			int q = uri.Find('?');
			if(q >= 0) {
				if(!post)
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
						var.Add(id, UrlDecode(h.Mid(qq)));
						break;
					}
					var.Add(id, UrlDecode(h.Mid(qq, q - qq)));
					q++;
				}
			}
			DUMPM(var);
			Vector<String> part = Split(uri, '/');
			for(int i = 0; i < part.GetCount(); i++)
				part[i] = UrlDecode(part[i]);
			DUMPC(part);
			Vector<String> a;
			BestDispatch bd(arg);
			if(DispatchMap.GetCount())
				GetBestDispatch(post ? DispatchNode::POST : DispatchNode::GET, part, 0, DispatchMap[0], a, bd, 0, 0);
			DUMPC(arg);
			response.Clear();
			if(bd.view) {
				try {
					SQL.Begin();
					LoadSession();
					viewid = bd.id;
					(*bd.view)(*this);
					SaveSession();
					SQL.Commit();
				}
				catch(SqlExc e) {
					response << "Internal server error<br>"
					         << "SQL ERROR: " << e;
					code = 500;
					code_text = "Internal server error";
					SQL.Rollback();
				}
				catch(Exc e) {
					response << "Internal server error<br>"
					         << e;
					code = 500;
					code_text = "Internal server error";
					SQL.Rollback();
				}
			}
			else {
				response << "Page not found";
				code = 404;
				code_text = "Not found";
			}
			r.Clear();
			if(redirect.GetCount()) {
				r << "HTTP/1.1 " << code << " Found\r\n";
				r << "Location: " << redirect << "\r\n";
			}
			else {
				r <<
					"HTTP/1.0 " << code << ' ' << code_text << "\r\n"
					"Date: " <<  WwwFormat(GetUtcTime()) << "\r\n"
					"Server: U++\r\n"
					"Content-Length: " << response.GetCount() << "\r\n"
					"Connection: close\r\n"
					"Content-Type: " << content_type << "\r\n";
				for(int i = 0; i < cookies.GetCount(); i++)
					r << cookies[i];
				r << "\r\n";
			}
		}
		socket.PutAll(r);
		socket.PutAll(response);
	}
}
