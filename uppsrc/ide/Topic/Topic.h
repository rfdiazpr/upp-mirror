#ifndef _ide_Topic_Topic_h_
#define _ide_Topic_Topic_h_

#include <Topic/Topic.h>
#include <ide/Browser/Browser.h>

#define LAYOUTFILE <ide/Topic/Topic.lay>
#include <CtrlCore/lay.h>

struct ReferenceDlg : WithReferenceDlgLayout<TopWindow> {
	void   EnterItem();
	void   EnterItemOk();
	void   Set(const String& s);
	String Get() const            { return ~reference; }

	void   Serialize(Stream& s)   { browser.SerializeWspc(s); SerializePlacement(s); }

	typedef ReferenceDlg CLASSNAME;

	ReferenceDlg();
};

class IdeTopicEditor : public TopicEditor {
public:
	virtual void SyncFile(const String& path, const String& link);
	virtual bool Key(dword key, int c);

protected:
	bool         autosave;
	ReferenceDlg ref;

	virtual void MainTool(Bar& bar);

	void Tools(Bar& bar);
	void Label(String&);
	void CreateQtf(const String& item, const CppItemInfo& m, String& p1, String& p2);
	void InsertItem();

	void GoBack()                  { WhenBack(); }
	void FindBrokenRef();
	
	void Repair();

public:
	Callback WhenSync;
	Callback WhenBack;

	typedef IdeTopicEditor CLASSNAME;

	void SerializeWspc(Stream& s);

	IdeTopicEditor();
	virtual ~IdeTopicEditor();
};

#endif
