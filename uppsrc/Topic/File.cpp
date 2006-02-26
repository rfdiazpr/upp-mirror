#include "Topic.h"

int CharFilterID(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

bool ParseTopicFileName(const String& fn, String& topic, int& lang)
{
	String q = GetFileTitle(fn);
	int w = q.Find('$');
	if(w < 0)
		return false;
	topic = q.Mid(0, w);
	lang = LNGFromText(q.Mid(w + 1));
	return lang;
}

Topic ReadTopic(const char *text)
{
	Topic topic;
	CParser p(text);
	try {
		while(!p.IsEof()) {
			if(p.Id("TITLE")) {
				p.PassChar('(');
				topic.title = p.ReadString();
				p.PassChar(')');
			}
			else
			if(p.Id("REF")) {
				p.PassChar('(');
				p.ReadString();
				p.PassChar(')');
			}
			else
			if(p.Id("TOPIC_TEXT")) {
				p.PassChar('(');
				topic.text << p.ReadString();
				p.PassChar(')');
			}
			else {
				topic.text << p.GetPtr();
				break;
			}
		}
	}
	catch(CParser::Error e) {
		topic.text = String::GetVoid();
		topic.title = e;
	}
	return topic;
}

Vector<String> GatherLabels(const RichText& text)
{
	Vector<RichValPos> p = text.GetValPos(TopicEditor::TopicPage(), RichText::LABELS);
	Index<String> ref;
	for(int i = 0; i < p.GetCount(); i++)
		if(!IsNull(p[i].data))
			ref.FindAdd(p[i].data.ToString());
	return ref.PickKeys();
}

String WriteTopic(const char *title, const RichText& text)
{
	String r;
	r << "TITLE(" << AsCString(title) << ")\r\n";
	Vector<String> ref = GatherLabels(text);
	for(int i = 0; i < ref.GetCount(); i++)
		r << "REF(" << AsCString(ref[i]) << ")\r\n";
	String qtf = AsQTF(text, CHARSET_UTF8, QTF_BODY|QTF_ALL_STYLES);
	const char *s = qtf;
	const char *e = qtf.End();
	while(s < e) {
		const char *ee = min(s + 400, e);
		r << "TOPIC_TEXT(\r\n" << AsCString(s, ee, 80, NULL, ASCSTRING_SMART|ASCSTRING_OCTALHI)
		  << ")\r\n";
		s = ee;
	}
	return r;
}
