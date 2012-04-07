#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <GuiWebSpider/webspider.lay>
#include <CtrlCore/lay.h>

struct WebSpider : public WithSpiderLayout<TopWindow> {
	BiVector<String> todo;
	Index<String>    done;
	
	struct Work {
		HttpRequest http;
		String      url;
	};
	Array<Work>      http;
	int64            total;
	
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
		while(todo.GetCount() && http.GetCount() < 30) {
			String url = todo.Head();
			todo.DropHead();
			Work& w = http.Add();
			w.url = url;
			w.http.Url(url).MaxRetries(0).RequestTimeout(20000).Timeout(0);
			work.Add(url);
			work.HeaderTab(0).SetText(Format("URL (%d)", work.GetCount()));
		}
		SocketWaitEvent we;
		for(int i = 0; i < http.GetCount(); i++)
			we.Add(http[i].http);
		we.Wait(10);
		int i = 0;
		while(i < http.GetCount()) {
			Work& w = http[i];
			w.http.Do();
			int q = work.Find(w.url);
			if(w.http.InProgress()) {
				if(q >= 0)
					work.Set(q, 1, w.http.GetPhaseName());
				i++;
			}
			else {
				if(w.http.IsSuccess()) {
					String html = w.http;
					total += html.GetCount();
					success.Add(w.url, String().Cat() << w.http.GetStatusCode() << ' '
					                   << w.http.GetReasonPhrase() << " (" << html.GetCount() << " bytes)");
					success.HeaderTab(0).SetText(Format("Success (%d)", success.GetCount()));
					success.HeaderTab(1).SetText(Format("Response (%` KB)", total >> 10));
					ExtractUrls(html);
				}
				else {
					failed.Add(w.url, w.http.IsError() ? String().Cat() << w.http.GetErrorDesc()
					                                   : String().Cat() << w.http.GetStatusCode()
					                                     << ' ' << w.http.GetReasonPhrase());
					failed.HeaderTab(0).SetText(Format("failed (%d)", failed.GetCount()));
				}
				http.Remove(i);
				work.Remove(q);
			}
		}
	}
}

WebSpider::WebSpider()
{
	CtrlLayout(*this, "WebSpider");
	work.AddColumn("URL");
	work.AddColumn("Status");
	success.AddColumn("Success");
	success.AddColumn("Response");
	failed.AddColumn("Failed");
	failed.AddColumn("Reason");
	total = 0;
	Zoomable().Sizeable();
}

GUI_APP_MAIN
{
	HttpRequest::Trace();

	WebSpider().Run();
}
