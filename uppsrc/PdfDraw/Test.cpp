#include "PdfDraw.h"

#ifdef flagMAIN

void Paint(Draw& w)
{
	for(int i = 0; i < 32; i++) {
		WString text;
		for(int q = 0; q < 64; q++)
			text.Cat(64 * i + q);
		w.DrawText(0, 100 * i, 0, text, Roman(80), SBlack);
	}
	String q = "Pepíèek je èùráèek";
	for(int i = 5; i < 10; i++) {
		w.DrawText(0, 300 * i, 0, q, Arial(20 * i).Italic().Underline(), SBlue);
		w.DrawText(2000, 300 * i, 0, q, Arial(20 * i), SRed);
		w.DrawText(0, 2000 + 300 * i, 0, q, Courier(20 * i), SLtRed);
		w.DrawText(2000, 2000 + 300 * i, 0, q, Roman(20 * i).Italic(), SMagenta);

		w.DrawText(0, 300 * i + 100, 0, q, Arial(20 * i).Bold(), SBlue);
		w.DrawText(2000, 300 * i + 100, 0, q, Arial(20 * i), SRed);
		w.DrawText(0, 2000 + 300 * i + 100, 0, q, Courier(20 * i), SLtRed);
		w.DrawText(2000, 2000 + 300 * i + 100, 0, q, Roman(20 * i), SMagenta);
	}

	w.DrawRect(1000, 1000, 500, 500, SLtGray);
	q = "Test of angle";

	int u = 0;
	int batang = Font::FindFaceNameIndex("Batang");
	for(int i = 0; i < 3600; i += 450) {
		w.DrawText(1000, 1000, i, q, Tahoma(150).Underline(), SBlack);
		if(batang >= 0)
			w.DrawText(2500, 1000, i, q, Font(batang, 150).Italic(), SGreen);
		w.DrawText(1000, 2500, i, q, Roman(150).Italic(), SGreen);
		w.DrawText(2500, 2500, i, q, Roman(150).Bold(), SGreen);
	}

	w.DrawLine(2500, 2500, 1000, 700, 50, SBlue);
	w.DrawImage(RectC(1000, 1000, 500, 500), CtrlImg::HelpCursor1());

	w.Clip(1300, 1300, 800, 800);
	w.DrawRect(1000, 1000, 3000, 3000, LtBlue);
	w.DrawImage(RectC(1500, 1500, 700, 700), CtrlImg::HelpCursor1(), Rect(5, 5, 20, 20));
	w.DrawRect(1000, 1000, 50, 50, LtRed);
	w.End();

	w.DrawText(0, 0, "0, 0", Arial(200));
	w.Clipoff(500, 500, 500, 500);
	w.DrawText(0, 0, 0, "Just a test test test test test !", Arial(200));
	w.Clipoff(100, 100, 200, 200);
	w.DrawText(0, 0, "Just a test test test test test !", Roman(200), Red);
	w.End();
	w.End();
}

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_WIN1250);
	PdfDraw pdf;
	pdf.StartPage();
	Paint(pdf);
	pdf.EndPage();
	pdf.StartPage();
	Paint(pdf);
	pdf.EndPage();
	SaveFile("E:\\pdftest.pdf", pdf.Finish());

	Report r;
	Paint(r);
	r.Perform();
	return;
/*
	FileIn in("V:/oo_unicode.pdf");
	int q = 0;
	while(!in.IsEof()) {
		String s = in.GetLine();
		if(s == "stream") {
			s.Clear();
			while(!in.IsEof()) {
				s.Cat(in.Get());
				if(s.GetLength() > 9 && memcmp(s.End() - 9, "endstream", 9) == 0) {
					String data = ZDecompress(s);
					TTFReader ttf;
					LOG("------------------------------------");
					if(ttf.Open(data))
						SaveFile("V:/ttf/" + AsString(q++) + ".ttf", data);
					LOG("....................................");
					break;
				}
			}
		}
	}

	String& out = pdf.page;
	out << "BT 1 0 0.25 1 0 0 Tm "
		<< "/F1 50 Tf <0102030405060708090A> Tj ET ";
	out << "BT 1.3 0 0 1 100 100 Tm "
		<< "/F1 50 Tf <0102030405060708090A> Tj ET ";
	for(int i = 0; i < 10; i++) {
		double sina = sin(i * M_PI * 2 / 10);
		double cosa = cos(i * M_PI * 2 / 10);
		out << "BT "
		    << sina << " " << cosa << " " << sina << " " << -sina << " " << cosa << " 500 100 Tm "
		 // << "1 0 0 1 " << 20 * i << " 0 cm "
			<< "/F1 24 Tf <0102030405060708090A> Tj ET ";
	}

	{
	TTFReader ttf;
	int max = 0;
	if(ttf.Open(LoadFile("E:\\t.ttf")))
		for(int i = 0; i < 65535; i++)
			if(ttf.GetGlyph(i)) {
				if(ttf.GetGlyph(i) > max)
					max = ttf.GetGlyph(i);
				LOG(i << " -> " << ttf.GetGlyph(i));
			}
	LOG("Max glyph: " << max);
	const char *s = "ìšèøžýáíéúù§ÌŠÈØŽÝÁÍÉÙÚ§";// ìšèøìšèøèøžýáøžíéýáíéÌŠÉÁÝŠÈÉÍŽÁÌŠÈÁÍØÝÌŽŠÍÈÝŽ";
	Vector<wchar> cs;
	while(*s)
		cs.Add(ToUnicode(*s++, CHARSET_WIN1250));
	SaveFile("E:\\subsetted.ttf", ttf.Subset(cs, 'A'));
	}

	TTFReader ttf;
	if(ttf.Open(LoadFile("D:/WINNT/Fonts/webdings.ttf"), true)) {
		Vector<wchar> cs;
		for(int q = 40; q < 50; q++)
			cs.Add(q);
		SaveFile("E:\\wss.ttf", ttf.Subset(cs, 'A'));
	}

	LOG("-----------------------------------------------------------------------");
	{
	TTFReader ttf;
	ttf.Open(LoadFile("E:\\subsetted.ttf"));
	}

*/

/*
	FileIn in("V:/oo_unicode.pdf");
	FileOut out("V:/oo_unicode_d.pdf");
	while(!in.IsEof()) {
		String s = in.GetLine();
		out << s << "\n";
		if(s == "stream") {
			s.Clear();
			while(!in.IsEof()) {
				s.Cat(in.Get());
				if(s.GetLength() > 9 && memcmp(s.End() - 9, "endstream", 9) == 0) {
					out << ZDecompress(s) << "\nendstream\n";
					break;
				}
			}
		}
	}
*/

/*
	FileIn in("V:/image.pdf");
	FileOut out("V:/image_d.pdf");
	while(!in.IsEof()) {
		String s = in.GetLine();
		out << s << "\n";
		if(s == "stream") {
			s.Clear();
			while(!in.IsEof()) {
				s.Cat(in.Get());
				if(s.GetLength() > 9 && memcmp(s.End() - 9, "endstream", 9) == 0) {
					out << ZDecompress(s) << "\nendstream\n";
					break;
				}
			}
		}
	}
	return;
//*/
/*
	TTFReader ttf;
	int max = 0;
	if(ttf.Open(LoadFile("V:/ttf/0.ttf")))
		for(int i = 0; i < 65535; i++)
			if(ttf.GetGlyph(i))
				LOG(i << ": " << ttf.GetAdvanceWidth(i));
//*/
}

#endif
