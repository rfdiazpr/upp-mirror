#include "Topic.h"

#ifdef _DEBUG
//#define REPAIR
#endif

bool IdeTopicEditor::Key(dword key, int c)
{
	if(key == K_CTRL_T) {
		WhenBack();
		return true;
	}
	return TopicEditor::Key(key, c);
}

void IdeTopicEditor::Label(String& label)
{
	Save();
	if(ref.browser.item.IsMulti())
		ref.browser.item.ClearSelection();
	ref.browser.item.Multi(false);
	ref.Title("Reference");
	ref.Set(label);
	ref.classlist.Hide();
	if(ref.Execute() != IDOK)
		return;
	label = ref.Get();
}

#ifdef REPAIR
void IdeTopicEditor::Repair()
{
	StartBrowserBase();
	RichText text = editor.CopyText(0, editor.GetLength());
	RichPara def;
	bool cont = false;
	for(int i = 0; i < text.GetPartCount(); i++)
		if(text.IsPara(i)) {
			RichPara para = text.Get(i);
			String l = para.format.label;
			if(l[0] == ':') {
				try {
					CParser p(l);
					Vector<String> nd;
					String r;
					String q = "::";
					nd.Add(q);
					for(;;) {
						p.PassChar2(':', ':');
						r << "::";
						if(!p.IsId())
							break;
						String x = p.ReadId();
						r << x;
						if(!p.IsChar2(':', ':'))
							break;
						q << x << "::";
						nd.Add(q);
					}
					Reverse(nd);
					while(!p.IsEof()) {
						if(p.IsId()) {
							String h = Qualify(BrowserBase(), nd, p.ReadId());
							if(iscid(*r.Last()) && iscib(*h))
								r << ' ';
							r << h;
						}
						else
							r.Cat(p.GetChar());
					}
					para.format.label = r;
					text.Set(i, para, text.GetStyles());
				}
				catch(CParser::Error)
				{}
				if(!cont)
					text.RichTxt::Insert(i++, def, text.GetStyles());
				cont = true;
			}
			else
				cont = false;
		}
	editor.Move(0);
	editor.RemoveText(editor.GetLength());
	editor.PasteText(text);
}
#endif

struct FindBrokenRefIterator : RichText::Iterator {
	int cursor;

	virtual bool operator()(int pos, const RichPara& para)
	{
		if(pos >= cursor && para.format.label[0] == ':' && para.format.label[1] == ':') {
			String nest;
			String key;
			if(para.GetLength() && SplitNestKey(para.format.label, nest, key)) {
				int q = BrowserBase().Find(nest);
				if(q >= 0 && BrowserBase()[q].key.Find(key) >= 0)
					return false;
			}
			cursor = pos;
			return true;
		}
		return false;
	}
};

void IdeTopicEditor::FindBrokenRef()
{
	StartBrowserBase();
	Progress pi;
	pi.SetTotal(topic.GetCount());
	for(;;) {
		if(IsNull(filepath))
			return;
		pi.SetText(GetFileTitle(filepath));
		pi.SetPos(topic.GetCursor());
		FindBrokenRefIterator fi;
		fi.cursor = editor.GetCursor();
		if(editor.Get().Iterate(fi)) {
			editor.Move(fi.cursor);
			break;
		}
		if(!topic.IsCursor())
			break;
		int c = topic.GetCursor() + 1;
		if(c >= topic.GetCount()) {
			PromptOK("No more broken references.");
			break;
		}
		topic.SetCursor(c);
		if(!IsNull(filepath))
			editor.Move(0);
	}
}

void IdeTopicEditor::Tools(Bar& bar)
{
	bar.Add("Insert code item..", IdeCommonImg::InsertItem(), THISBACK(InsertItem))
	   .Key(K_INSERT);
	bar.Add("Find broken references..", IdeCommonImg::FindBrokenRef(), THISBACK(FindBrokenRef))
	   .Key(K_CTRL_F3);
#ifdef REPAIR
	bar.Separator();
	bar.Add("Repair!", CtrlImg::Toggle(), THISBACK(Repair)).Key(K_ALT_F5);
	bar.Separator();
#endif
}

void IdeTopicEditor::MainTool(Bar& bar)
{
	editor.StyleTool(bar);
	bar.Gap();
	editor.FontTools(bar);
	bar.Gap();
	editor.InkTool(bar);
	editor.PaperTool(bar);
	bar.Gap();
	editor.LanguageTool(bar);
	editor.SpellCheckTool(bar);
	bar.Gap();
	editor.IndexEntryTool(bar);
	bar.Break();
	editor.HyperlinkTool(bar, 300);
	bar.Gap();
	editor.ParaTools(bar);
	bar.Gap();
	editor.EditTools(bar);
	bar.Gap();
	editor.PrintTool(bar);
	bar.GapRight();
	bar.Break();
	editor.LabelTool(bar, 500);
	bar.Add("Back", IdeCommonImg::Package(), THISBACK(GoBack))
	   .Key(K_CTRL_T);
	bar.Gap();
	Tools(bar);
	bar.Gap();
	editor.TableTools(bar);
}

String NaturalDeQtf(const char *s) {
	String r;
	r.Reserve(256);
	bool cm = false;
	while(*s) {
		if(*s == ' ')
			r.Cat(cm ? ' ' : '_');
		else {
			if((byte)*s > ' ' && !IsDigit(*s) && !IsAlpha(*s) && (byte)*s < 128)
				r.Cat('`');
			r.Cat(*s);
			if(*s == ',')
				cm = true;
			else
				cm = false;
		}
		s++;
	}
	return r;
}

String DecoratedItem(const CppItemInfo& m, const char *natural)
{
	String qtf = "[%00-00 ";
	Vector<ItemTextPart> n = ParseItemNatural(m, natural);
	if(m.virt)
		qtf << "virtual ";
	if(m.kind == CLASSFUNCTION || m.kind == CLASSFUNCTIONTEMPLATE)
		qtf << "static ";
	for(int i = 0; i < n.GetCount(); i++) {
		ItemTextPart& p = n[i];
		qtf << "[";
		switch(p.type) {
		case ITEM_PNAME:
			qtf << '*';
		case ITEM_NUMBER:
			qtf << "@3";
			break;
		case ITEM_TNAME:
			qtf << "@4";
		case ITEM_NAME:
			qtf << '*';
			break;
		}
		qtf << ' ';
		qtf << NaturalDeQtf(String(~m.natural + p.pos, p.len));
		qtf << ']';
	}
	return qtf + "]";
}

void IdeTopicEditor::CreateQtf(const String& item, const CppItemInfo& m, String& p1, String& p2)
{
	static char styles[] =
		"[ $$0,0#00000000000000000000000000000000:Default]"
		"[t4167;C+117 $$1,0#37138531426314131251341829483380:class]"
		"[ $$2,0#83433469410354161042741608181528:base]"
		"[b167;a42;C $$3,0#40027414424643823182269349404212:item]"
		"[l288;i704;a17;O9;~~~.992; $$4,0#10431211400427159095818037425705:param]"
		"[i416;b42;a42;O9;~~~.416; $$5,0#10566046415157235020018451313112:classparam]"
		"[l288;a17; $$6,6#27521748481378242620020725143825:desc]";
	String qtf = styles;
	bool str = m.kind == STRUCT || m.kind == STRUCTTEMPLATE;
	qtf << (str ? "[s1;:" : "[s3;:") << DeQtf(item) << ": ";
	if(m.IsTemplate()) {
		int q = 0;
		int w = 0;
		while(q < m.natural.GetLength()) {
			if(m.natural[q] == '<')
				w++;
			if(m.natural[q] == '>') {
				w--;
				if(w == 0) {
					q++;
					break;
				}
			}
			q++;
		}
		qtf << DecoratedItem(m, m.natural.Mid(0, q));
		if(q < m.natural.GetLength()) {
			while(m.natural[q] == ' ')
				q++;
			qtf << '&' << DecoratedItem(m, m.natural.Mid(q));
		}
	}
	else
		qtf << DecoratedItem(m, m.natural);

	qtf << "&]";
	p1 = qtf;
	qtf = styles;
	qtf << "[s6 &]";
	Vector<String> t = Split(m.tname, ';');
	for(int i = 0; i < t.GetCount(); i++)
		qtf << (str ? "[s5 [%00-00C*@4 " : "[s4 [C*@4 ") << DeQtf(t[i]) << "]-|&]";
	Vector<String> p = Split(m.pname, ';');
	if(!str)
		for(int i = 0; i < p.GetCount(); i++)
			qtf << "[s4 [%00-00C*@3 " << DeQtf(p[i]) << "]-|&]";
	if(str && p.GetCount()) {
		qtf << "[s2 [*/ Derived from] [%00-00 ";
		for(int i = 0; i < p.GetCount(); i++) {
			if(i)
				qtf << ", ";
			qtf << "[*@(0.0.255) " << DeQtf(p[i]) << "]";
		}
		qtf << "]&]";
	}
	if(m.kind >= FUNCTION && m.kind <= INLINEFRIEND) {
		CParser p(m.natural);
		if(!p.Id("void") || !p.IsId())
			qtf << "[s4 [*/ Return value]-|&]";
	}
	qtf << "[s0 &]";
	p2 = qtf;
}

void IdeTopicEditor::InsertItem()
{
	if(IsNull(filepath))
		return;
	Save();
	ref.Title("Insert");
	if(ref.browser.item.IsCursor())
		ref.browser.item.SetFocus();
	ref.browser.item.Multi();
	ref.classlist.Show();
	int c = ref.Execute();
	if(c == IDCANCEL)
		return;
	if(c == IDYES) {
		String qtf = "&{{1 ";
		for(int i = 0; i < ref.browser.nesting.GetCount(); i++) {
			const CppNestingInfo& f = ValueTo<CppNestingInfo>(ref.browser.nesting.Get(i, 2));
			if(i)
				qtf << ":: ";
			qtf << DeQtf(f.nesting);
		}
		qtf << "}}&";
		editor.PasteText(ParseQTF(qtf));
		return;
	}
	String p1, p2;
	if(ref.browser.item.IsSelection()) {
		for(int i = 0; i < ref.browser.item.GetCount(); i++)
			if(ref.browser.item.IsSelected(i)) {
				String a1, a2;
				CreateQtf(ref.browser.GetItem(i), ref.browser.GetItemInfo(i), a1, a2);
				p1 << p2 << a1;
				p2 = a2;
			}
	}
	else
	if(ref.browser.item.IsCursor())
		CreateQtf(ref.browser.GetItem(), ref.browser.GetItemInfo(), p1, p2);
	else
		return;
	editor.BeginOp();
	int a = editor.GetCursor();
	editor.PasteText(ParseQTF(p1));
	c = editor.GetCursor();
	editor.PasteText(ParseQTF(p2));
	editor.Move(a);
	editor.Move(c);
}

void IdeTopicEditor::SerializeWspc(Stream& s)
{
	int version = 0;
	s / version;
	TopicEditor::SerializeWspc(s);
	ref.Serialize(s);
}

void IdeTopicEditor::SyncFile(const String& path, const String& link)
{
	SyncRefsFile(path, link);
	WhenSync();
}

IdeTopicEditor::IdeTopicEditor()
{
	editor.WhenLabel = THISBACK(Label);
}

IdeTopicEditor::~IdeTopicEditor()
{
}
