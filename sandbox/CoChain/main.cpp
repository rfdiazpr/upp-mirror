#include "CoChain.h"

int CharFilterID(int c)
{
	return IsAlNum(c) ? ToLower(c) : ' ';
}

struct SortFileLines {
	FileIn  in;
	FileOut out;
	CoChain cc;
	
	typedef SortFileLines CLASSNAME;
	
	void SaveLine(Vector<String>& w)
	{
		out.PutLine(Join(w, " "));
	}
	
	void SortLine(Vector<String>& w)
	{
		Sort(w);
		cc(THISBACK1(SaveLine, w));
	}
	
	void SplitLine(const String& line)
	{
		DDUMP(line);
		cc(THISBACK1(SortLine, Split(line, ' ')));
	}
	
	void FilterLine(const String& line)
	{
		cc(THISBACK1(SplitLine, Filter(line, CharFilterID)));
	}
	
	void Perform(const char *fin, const char *fout)
	{
		if(!in.Open(fin) || !out.Open(fout)) {
			Cout() << "Failed";
			return;
		}
		for(int i = 0; i < 10; i++) {
			while(!in.IsEof())
				cc(THISBACK1(FilterLine, in.GetLine()));
			in.Seek(0);
		}
		cc.Finish();
	}
};

CONSOLE_APP_MAIN
{
	SortFileLines().Perform("/home/cxl/upp.src/uppsrc/Core/Format.cpp", GetHomeDirFile("Sorted.txt"));
}
