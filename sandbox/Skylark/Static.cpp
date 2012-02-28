#include "Skylark.h"

static Vector<String> sPath;

int CharFilterSplitPath(int c)
{
	return c == ';' || c == ':';
}

void SetStaticPath(const String& path)
{
	sPath = Split(path, CharFilterSplitPath);
	sPath.Add(GetFileFolder(GetExeFilePath()));
}

INITBLOCK {
	SetStaticPath(Null);
}

URL_VIEW(ServeStaticPage, "static/**")
{
	String file = "static/" + Join(http.arg, "/");
	for(int i = 0; i < sPath.GetCount(); i++) {
		String path = AppendFileName(sPath[i], file);
		if(FileExists(path))
			http << LoadFile(path);
	}
}

URL_VIEW(ServeIml, "iml/*")
{
	String name = http.arg[0];
	int q = name.Find('.');
	String ext;
	if(q >= 0) {
		ext = name.Mid(q + 1);
		name = name.Mid(0, q);
	}
	Image m = GetImlImage(name);
	if(ext == "jpg" || ext == "JPG" || ext == "jpeg" || ext == "JPEG")
		http.Content("image/jpeg", JPGEncoder().SaveString(m));
	else
		http.Content("image/png", PNGEncoder().SaveString(m));
}
