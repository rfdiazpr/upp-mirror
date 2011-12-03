#include "Wpp.h"

#define LLOG(x)  // DLOG(x)
#define LTIMING(x)

struct DispatchNode : Moveable<DispatchNode> {
	VectorMap<String, int> subnode;
	Callback1<Http&>       view;
};

static Vector<DispatchNode>& sDispatchMap()
{
	static Vector<DispatchNode> x;
	return x;
}

void DumpDispatchMap()
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	for(int i = 0; i < DispatchMap.GetCount(); i++) {
		LLOG("-------------");
		String sub;
		for(int j = 0; j < DispatchMap[i].subnode.GetCount(); j++)
			sub << DispatchMap[i].subnode.GetKey(j) << "->" << Nvl(DispatchMap[i].subnode[j], "*") << ", ";
		LLOG(i << " " << (bool)DispatchMap[i].view << ": " << sub);
	}			
}

void RegisterView(const char *path, Callback1<Http&> view)
{
	LLOG("RegisterView " << path);
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	Vector<String> h = Split(path, '/');
	if(DispatchMap.GetCount() == 0)
		DispatchMap.Add();
	int q = 0;
	for(int i = 0; i < h.GetCount(); i++) {
		String s = h[i];
		LLOG(" Node " << h[i]);
		if(s == "*")
			s.Clear();
		DispatchNode& n = DispatchMap[q];
		q = n.subnode.Get(s, -1);
		if(q < 0) {
			q = DispatchMap.GetCount();
			LLOG(" Adding " << s << ": " << q);
			n.subnode.Add(s, q);
			DispatchMap.Add();
		}
	}
	ASSERT_(!DispatchMap[q].view, "duplicate view!");
	DispatchMap[q].view = view;
	DumpDispatchMap();
}

void RegisterView(const char *path, void (*view)(Http&))
{
	RegisterView(path, callback(view));
}

void GetBestDispatch(const Vector<String>& h, int ii, const DispatchNode& n, Vector<String>& arg,
                     Callback1<Http&>& view, Vector<String>& final_arg)
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	LOGBEGIN();
	if(ii >= h.GetCount()) {
		if(n.view && (!view || arg.GetCount() < final_arg.GetCount())) {
			view = n.view;
			final_arg <<= arg;
		}
		return;
	}
	int qq = n.subnode.Get(h[ii], -1);
	if(qq >= 0)
		GetBestDispatch(h, ii + 1, DispatchMap[qq], arg, view, final_arg);
	qq = n.subnode.Get(String(), -1);
	if(qq >= 0) {
		LLOG(" *" << qq);
		arg.Add(h[ii]);
		GetBestDispatch(h, ii + 1, DispatchMap[qq], arg, view, final_arg);
	}
	LOGEND();
}

void Dispatch(Socket& http)
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	Http hdr;
	if(hdr.Read(http)) {
		int len = hdr.GetLength();
		String data = http.ReadCount(len);
		Cout() << hdr.method << " " << hdr.uri << "\n";
		if(hdr.method == "GET") {
			Vector<String> h = Split(hdr.uri, '/');
			for(int i = 0; i < 1000; i++) {
				LTIMING("Benchmark");
				hdr.response.Clear();
				if(h.GetCount()) {
					Vector<String> arg;
					Callback1<Http&> view;
					if(DispatchMap.GetCount())
						GetBestDispatch(h, 0, DispatchMap[0], arg, view, hdr.arg);
					view(hdr);
				}
			}
			LLOG("Reponse: ");
			LLOG(hdr.response);
			http.Write(HttpResponse(hdr.code, hdr.code_text, hdr.response, hdr.content_type));
		}
	}
}
