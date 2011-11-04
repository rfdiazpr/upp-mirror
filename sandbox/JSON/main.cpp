#include "JSON.h"

CONSOLE_APP_MAIN
{
	String x = LoadFile(GetDataFile("test.json"));
	JSONParser p(x);
	Value json = ParseJSON(p);
	DDUMP(json);
	Json js(json);
	DDUMP(js["age"]);
	Json phone_number = js["phoneNumber"];
	for(int i = 0; i < phone_number.GetCount(); i++) {
		DDUMP(phone_number[i]["number"]);
		DDUMP(phone_number[i]["type"]);
	}
//	js["age"] = 10;
	DDUMP(EncodeJson(js, true));
	DDUMP(EncodeJson(json, false));

	DLOG("=========================================================================");
	CParser p2(x);
	p2.PassChar('[');
	if(!p2.Char(']')) {
		do {
			LOG("---------------");
			DDUMP(ParseJSON2(p2));
		}
		while(p2.Char(','));
		p2.PassChar(']');
	}
}
