#include <Core/Core.h>

using namespace Upp;

#include <plugin/lzma/lzma.h>


void ProgressFunc(int a, int b){
	Cout() << "Progress: in: " << a << ", out: " << b << '\n';
}

void OutputFunc(const void* buf, int size){
	Cout() << String((const char*)buf, size);
}

CONSOLE_APP_MAIN {
	String fn; 
	if(CommandLine().IsEmpty())
		fn = GetExeFilePath();
	else	
		fn = CommandLine()[0];
	
	String tmp = LoadFile(fn);
	Cout() << Format("          Name%s     Size      MD5\n", String(' ',fn.GetCount()-4));
	Cout() << Format("Original: %s     %9<i %s\n", fn, tmp.GetCount(), MD5String(tmp));

	Lzma lzma;
	//lzma.WhenProgress = callback(&ProgressFunc);
	//lzma.WhenOut = callback(&OutputFunc); // NOTE: if you uncomment this, you should comment out any calls to lzma.Get() below
	
	/* Compression */
	{
		FileIn fi(fn);
		FileOut fo(fn + ".enc");
		lzma.Compress();
		while (!fi.IsEof() && !fi.IsError()){
			lzma.Put(fi.Get(10240));
			fo.Put(lzma.Get());
		}
		lzma.End();
		fo.Put(lzma.Get());
	}

	tmp = LoadFile(fn+".enc");
	Cout() << Format("Encoded:  %s %9<i %s\n", fn+".enc", tmp.GetCount(), MD5String(tmp));
	
	/* Decompression */
	{
		FileIn fi(fn + ".enc");
		FileOut fo(fn + ".dec");
		lzma.Decompress();
		while (!fi.IsEof() && !fi.IsError()){
			lzma.Put(fi.Get(10240));
			//fo.Put(lzma.Get());
		}
		lzma.End();
		fo.Put(lzma.Get());
	}
	
	tmp = LoadFile(fn+".dec");
	Cout() << Format("Decoded:  %s %9<i %s\n", fn+".dec", tmp.GetCount(), MD5String(tmp));
}