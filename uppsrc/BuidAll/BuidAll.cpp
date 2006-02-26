#include <Core/Core.h>

String input = "d:\\";
String output = "e:\\all";

void Build(const char *nest, const char *bm, const char *flags)
{
	String n = String().Cat() << nest << '-' << bm << '-' << flags;
	Cout() << n << '\n';
	String outdir = AppendFileName(output, n);
	DeleteFolderDeep(outdir);
	RealizeDirectory(outdir);
	FindFile ff(AppendFileName(AppendFileName(input, nest), "*.*"));
	bool first = true;
	while(ff) {
		if(ff.IsFolder()) {
			Cout() << "   Building " << ff.GetName();
			String c;
			c << "umk " << nest << ' ' << ff.GetName() << ' ' << bm << " -l" << flags;
			if(first)
				c << 'a';
			c << ' ' << outdir;
			if(system(c))
				Cout() << "*** FAILED *** !\n";
			else {
				Cout() << " ok\n";
				first = false;
			}
		}
		DeleteFile(AppendFileName(outdir, ff.GetName() + ".ilk"));
		DeleteFile(AppendFileName(outdir, ff.GetName() + ".pdb"));
		ff.Next();
	}
}

CONSOLE_APP_MAIN
{
	Build("examples", "MSC8", "b");
	Build("examples", "MSC8", "r");
	Build("reference", "MSC8", "b");
	Build("reference", "MSC8", "r");
	Build("examples", "MINGW32", "b");
	Build("examples", "MINGW32", "r");
	Build("reference", "MINGW32", "b");
	Build("reference", "MINGW32", "r");
}
