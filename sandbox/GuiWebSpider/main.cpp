#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct WebSpider : public TopWindow {
	ArrayCtrl list;
	
	BiVector<String> todo;
	Index<String>    done;
	
	struct Work {
		HttpRequest http;
		int         line;
	};
	Array<Work>      work;
	
	void ExtractUrls(const String& html);

public:
	void Run();

	WebSpider();
};

bool IsUrlChar(int c)
{
	return c == ':' || c == '.' || IsAlNum(c) || c == '_' || c == '%' || c == '/';
}

void WebSpider::ExtractUrls(const String& html)
{
	int q = 0;
	while(q < html.GetCount()) {
		q = html.Find("http://", q);
		if(q < 0)
			return;
		int b = q;
		while(q < html.GetCount() && IsUrlChar(html[q]))
			q++;
		String url = html.Mid(b, q - b);
		if(done.Find(url) < 0) {
			done.Add(url);
			todo.AddTail(url);
		}
	}
}

void WebSpider::Run()
{
	String seed = "www.ultimatepp.org";
	todo.AddTail(seed);
	done.Add(seed);
	Open();
	int n = 0;
	while(IsOpen()) {
		Title(AsString(n++));
		ProcessEvents();
		while(todo.GetCount() && work.GetCount() < 30) {
			String url = todo.Head();
			todo.DropHead();
			Work& w = work.Add();
			w.http.Url(url).MaxRetries(0).RequestTimeout(20000).Timeout(0);
			w.line = list.GetCount();
			list.Add(url);
			list.GoEnd();
		}
		SocketWaitEvent we;
		for(int i = 0; i < work.GetCount(); i++)
			we.Add(work[i].http);
		we.Wait(10);
		int i = 0;
		while(i < work.GetCount()) {
			Work& w = work[i];
			w.http.Do();
			if(w.http.InProgress()) {
				list.Set(w.line, 1, String().Cat() << "Working: " << w.http.GetPhase());
				i++;
			}
			else {
				if(w.http.IsSuccess()) {
					String html = w.http;
					list.Set(w.line, 1, String().Cat() << "SUCCESS: " << w.http.GetStatusCode() << ' ' << w.http.GetReasonPhrase() << " (" << html.GetCount() << " bytes)");
					ExtractUrls(html);
				}
				else
					if(w.http.IsError()) {
						list.Set(w.line, 1, String().Cat() << "FAILED: " << w.http.GetErrorDesc());
						DLOG(list.Get(w.line, 0) << ' ' << w.http.GetErrorDesc());
					}
					else
						list.Set(w.line, 1, String().Cat() << "FAILED: " << w.http.GetStatusCode() << ' ' << w.http.GetReasonPhrase());
				work.Remove(i);
			}
		}
	}
}

WebSpider::WebSpider()
{
	Title("WebSpider");
	list.AddColumn("URL");
	list.AddColumn("Status");
	Add(list.SizePos());
	Zoomable().Sizeable();
}

GUI_APP_MAIN
{
//	HttpRequest::Trace();

	WebSpider().Run();
}
