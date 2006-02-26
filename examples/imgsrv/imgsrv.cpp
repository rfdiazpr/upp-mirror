#include <Web/Web.h>
#include <Draw/Draw.h>
#include <plugin/png/png.h>

#define APP_VERSION "1.0.r3"
#define APP_DATE    Date(2004, 4, 22)

CONSOLE_APP_MAIN
{
	HttpServer server;
	int next_id = 0;
	server.Logging();

	int color = 0xFFFF00;
	Vector<String> cmd;
	cmd <<= CommandLine();
	int port = 1536;
	if(cmd.GetCount() >= 1)
		port = Nvl(ScanInt(cmd[0]), port);

	if(cmd.GetCount() >= 2)
		color = Nvl(ScanInt(cmd[1], NULL, 16), color);

	Color bg((color >> 16) & 255, (color >> 8) & 255, color & 255);

	if(!server.Open(port))
	{
		puts(Socket::GetErrorText());
		return;
	}

	for(;;)
	{
		server.Wait(1000);
		if(server.IsError())
		{
			puts("Server error: " + Socket::GetErrorText());
			puts("Reopening socket...");
			fflush(stdout);
			if(!server.Reopen())
			{
				puts("Reopen on server socket failed: " + Socket::GetErrorText());
				return;
			}
		}
		fputs(NFormat("\rserver(%d) running at %`", port, GetSysTime()), stdout);
		fflush(stdout);
		One<HttpRequest> request = server.GetRequest();
		if(!request)
			continue;
		HttpQuery query = request->GetQuery();
		String svr = query.GetString("SVR", "test");
		String img = query.GetString("IMG");
		fputs(NFormat("img = %s, svr = %s\n", img, svr), stdout);
		String header, body;
		Size image_size(50, 20);
		if(!IsNull(img))
		{
			Image image(image_size);
			{
				ImageDraw draw(image);
				draw.DrawRect(image_size, bg);
				DrawFatFrame(draw, image_size, Black, 1);
				Font font = Arial(10).Bold();
				Size tsz = draw.GetTextSize(img, font);
				draw.DrawText((image_size.cx - tsz.cx) >> 1,
					(image_size.cy - tsz.cy) >> 1, img, font, Black);
			}
			header = HttpContentType(HttpImagePng());
			body = PngEncoder().SaveImage(image);
		}
		else
		{
			int items = query.GetInt("ITEMS", 0, 10000, 10);
			Htmls image_page;
			int i;
			image_page <<
			"IMGSRV, version " << APP_VERSION << ", release date: " << Format(APP_DATE) << "<br>\n"
			"Copyright &copy; 2003, 2004 Tomas Rylek<p>\n"
			"Program uses the following third-party components:\n"
			"<ul>\n"
			"<li>libpng version 1.0.9, Copyright (c) 1998-2001 Glenn Randers-Pehrson "
			"(Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger); "
			"(Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.).<p>\n"
			"<li>zlib version, version 1.1.4, Copyright (C) 1995-2002 Jean-loup Gailly and Mark Adler.<p>\n"
			"</ul>\n"
			"IMGSRV parameters: &lt;port number, default = 1536&gt; &lt;"
			"rrggbb background color, default = ffff00&gt<br>\n"
			"Current parameters: <b>imgsrv " << port << " " << FormatIntHex(color, 6) << "</b><p>\n"
			"<b>items</b> (<b>" << FormatInt(items) << "</b>) = number of images to generate (default = 10)<br>\n"
			"<b>svr</b> (" << (IsNull(svr) ? "<i>empty</i>" : "<b>" + svr + "</b>")
			<< ") = identifier for generated image references in this page (default = test)<br>\n"
			"<b>img</b> = image identifier (empty = generate this page)<br>\n"
			"Example: <b>http://localhost";
			String wisapi = GetFileName(query.GetString("$$WISAPI"));
			String sample_query;
			sample_query << "items=" << minmax(2 * items, 10, 10000) << "&amp;svr=A";
			if(!IsNull(wisapi))
				image_page << "/dir/<a href=\"" << wisapi << "?" << sample_query << "\">" << wisapi << "?";
			else
				image_page << ':' << port << "/<a href=\"any?" << sample_query << "\">" << "any?";
			image_page << sample_query << "</a><p>\n"
			"Current request:<br>\n";
			for(i = 0; i < query.GetCount(); i++)
				image_page << ToHtml(query.GetKey(i)) << "=<b>" << ToHtml(query.GetValue(i)) << "</b><br>\n";
			for(i = 0; i < items; i++)
				image_page << HtmlTag("img").Src(
					String().Cat() << "?img=" << UrlEncode(svr) << "-" << i << "&svr="
						<< UrlEncode(svr) << "&id=" << ++next_id)
						.Alt(ToHtml(String().Cat() << svr << "-" << i))
						.Width(image_size.cx).Height(image_size.cy)
				<< "\n" << (i % 15 == 14 ? "<br>\n" : "");

			header = HttpContentType(HttpTextHtml(CHARSET_WIN1252));
			body = HtmlTitlePage("Simple image server", image_page,
				Color(128 + (bg.GetR() >> 1), 128 + (bg.GetG() >> 1), 128 + (bg.GetB() >> 1)));
		}
		request->Write(header, body);
		request.Clear();
	}
}
