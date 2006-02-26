#include <CtrlLib/CtrlLib.h>

void AppMain()
{
	for(int i = 0; i < 1000000; i += 100000) {
		LOG("update osoba set prijm = prijm where osoba_seq >= " << i <<
		    " and osoba_seq < " << i + 100000 << ";");
		LOG("commit;");
	}

	FileIn in1("e:/src/osoby1.txt");
	FileIn in2("e:/src/osoby2.txt");
	
	while(!in1.IsEof() && !in2.IsEof()) {
		String ln1 = in1.GetLine();
		String ln2 = in2.GetLine();
		if(ln1 != ln2) {
			LOG(ln1);
			LOG(ln2);
			LOG("----------------------");
		}
	}
}
