#include "AdrBook.h"

URL_VIEW(Params, "*/*/normal;*1/*0/revert;all/**")
{
	ValueArray va;
	for(int i = 0; i < http.GetParamCount(); i++)
		va.Add(http[i]);
	http
		("NAME", "normal")
		("PARAM", va)
	.Render("AdrBook/Index.witz");
}

URL_VIEW(VarArgs, "**")
{
	ValueArray va;
	for(int i = 0; i < http.GetParamCount(); i++)
		va.Add(http[i]);
	http
		("NAME", "varargs")
		("PARAM", va)
	.Render("AdrBook/Index.witz");
}

URL_VIEW(New, "/new")
{
	http.Render("AdrBook/Dialog.witz");
}

/*
INITBLOCK {
	RegisterView("*", Testy);
}

String firstname, lastname;

Vector<String> picture;

void Picture(Http& http)
{
	int ii = atoi(http[0]);
	DDUMP(ii);
	DDUMP(picture[ii].GetCount());
	http.Content("image/jpeg", ii >= 0 && ii < picture.GetCount() ? picture[ii] : Null);
}

void Info(Http& h)
{
	h("FIRSTNAME", firstname)
	 ("LASTNAME", lastname)
	 ("COOKIE", h["mycookie"])
	 ("PICTURE", Format("/picture/%d", atoi(h[0])))
	.Render("uwf/submit.whtml");
}

INITBLOCK {
	RegisterView(Info, "info/*", Info);
	RegisterView("picture/*", Picture);
}

void Submit(Http& h)
{
	firstname = h["firstname"];
	lastname = h["lastname"];
	int n = picture.GetCount();
	picture.Add(h["file"]);
	h.Redirect(Format("/info/%d", n));
}

INITBLOCK {
	RegisterView("submit", Submit);
}
*/