#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <GuiWebSpider2/webspider.lay>
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
	void Url(ArrayCtrl *a);
	
	typedef WebSpider CLASSNAME;

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
	if(!EditText(seed, "GuiWebSpider", "Seed URL"))
		return;
	todo.AddTail(seed);
	done.Add(seed);
	Open();
	int n = 0;
	while(IsOpen()) {
		Title(AsString(n++));
		ProcessEvents();
		while(todo.GetCount() && http.GetCount() < 60) {
			String url = todo.Head();
			todo.DropHead();
			Work& w = http.Add();
			w.url = url;
			w.http.Url(url)
			      .UserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:11.0) Gecko/20100101 Firefox/11.0")
			      .Timeout(0);
		}
		SocketWaitEvent we;
		for(int i = 0; i < http.GetCount(); i++)
			we.Add(http[i].http);
		we.Wait(10);
		int i = 0;
		while(i < http.GetCount()) {
			Work& w = http[i];
			w.http.Do();
			if(w.http.InProgress()) {
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
					ArrayCtrl& f = w.http.GetStatusCode() == 404 ? notfound : failed;
					f.Add(w.url, w.http.IsError() ? String().Cat() << w.http.GetErrorDesc()
					                              : String().Cat() << w.http.GetStatusCode()
					                                               << ' '
					                                               << w.http.GetReasonPhrase());
					f.HeaderTab(0).SetText(Format("failed (%d)", f.GetCount()));
				}
				http.Remove(i);
			}
		}
	}
}

void WebSpider::Url(ArrayCtrl *a)
{
	String url = a->GetKey();
	if(url.GetCount()) {
		WriteClipboardText(url);
		LaunchWebBrowser(url);
	}
}

WebSpider::WebSpider()
{
	CtrlLayout(*this, "WebSpider");
	success.AddColumn("Success");
	success.AddColumn("Response");
	success.WhenLeftDouble = THISBACK1(Url, &success);
	notfound.AddColumn("Not found");
	notfound.AddColumn("Reason");
	notfound.WhenLeftDouble = THISBACK1(Url, &notfound);
	failed.AddColumn("Failed");
	failed.AddColumn("Reason");
	failed.WhenLeftDouble = THISBACK1(Url, &failed);
	total = 0;
	Zoomable().Sizeable();
}

GUI_APP_MAIN
{
	WebSpider().Run();
}
