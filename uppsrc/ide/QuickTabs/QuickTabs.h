#ifndef _QuickTabs_QuickTabs_h
#define _QuickTabs_QuickTabs_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS Img
#define IMAGEFILE <ide/QuickTabs/QuickTabs.iml>
#include <Draw/iml_header.h>

class TabScrollBar : public Ctrl
{
	private:
		int total;
		double pos, ps;
		int new_pos;
		int old_pos;
		double start_pos;
		double size;
		double cs, ics;
		void UpdatePos(bool update = true);
		bool ready;
		Size sz;

	public:
		TabScrollBar();

		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		virtual void Layout();

		int  GetPos() const;
		void SetPos(int p, bool dontscale = false);
		void AddPos(int p, bool dontscale = false);
		int  GetTotal() const;
		void AddTotal(int t);
		void SetTotal(int t);
		void GoEnd();
		void GoBegin();
		void Set(const TabScrollBar& t);
		Callback WhenScroll;
};

struct Tab : Moveable<Tab>
{
	String name;
	String group;
	bool visible;
	int x, cx;
	int y, cy;
	int id;
	Size tsz;
	Tab() : visible(true), id(-1)
	{}
	int Right() { return x + cx; }
	bool HasMouse(Point &p);
	bool HasMouseCross(Point &p);
};

struct Group : Moveable<Group>
{
	Group()	{}
	String name;
	String path;
	int active;
	int count;
	int first;
	int last;
};

class GroupButton : public Ctrl
{
	private:

		Vector<Group> groups;
		Vector<Tab> *tabs;
		int current;

	public:
		GroupButton();

		typedef GroupButton CLASSNAME;

		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void MouseEnter(Point p, dword keyflags);
		virtual void MouseLeave();

		void SetTabs(Vector<Tab> &t);
		void Make();
		int  Find(const String& g);
		const String& GetNext();
		void DoList(Bar &bar);
		void DoCloseGroupsList(Bar &bar);
		void DoGrouping(int n);
		void DoCloseGroup(int n);

		String GetName() const           { return current == 0 ? Null : groups[current].path; }
		int  SetCurrent(const String &s) { current = max(0, Find(s)); return current; }
		int  SetCurrent(int c)           { int t = current; current = c; return current; }
		int  GetCurrent() const          { return current;                }
		void SetActive(int id)           { groups[current].active = id;   }
		int  GetActive() const           { return groups[current].active; }
		int  GetFirst() const            { return groups[current].first;  }
		int  GetLast() const             { return groups[current].last;   }
		bool IsAll() const               { return current == 0;           }

		Callback WhenGrouping;
		Callback WhenCloseAll;
		Callback WhenCloseRest;
		Callback WhenCloseGroup;
};

class QuickTabs : public FrameCtrl<Ctrl>
{
	private:
		enum
		{
			MARGIN = 6,
			SPACE = 10,
			HEIGHT = 30,
			FILEICON = 16,
			SPACEICON = 5
		};

		int id;
		FrameBottom<TabScrollBar> sc;
		FrameLeft<GroupButton> groups;
		void Scroll();

		Vector<Tab> tabs;
		int highlight;
		int active;
		int target;
		int cross;
		bool crosses;
		bool file_icons;
		bool isctrl;
		bool isdrag;
		bool grouping;
		Point mouse, oldp;

		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keysflags);
		virtual void LeftUp(Point p, dword keysflags);
		virtual void RightDown(Point p, dword keyflags);
		virtual void MiddleDown(Point p, dword keyflags);
		virtual void MiddleUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keysflags);
		virtual void MouseLeave();
		virtual void Layout();
		virtual void FrameLayout(Rect &r);
		virtual void FrameAddSize(Size& sz);
		virtual void FramePaint(Draw& w, const Rect& r);
		virtual void DragAndDrop(Point p, PasteClip& d);
		virtual void LeftDrag(Point p, dword keyflags);
		virtual void DragEnter();
		virtual void DragLeave();
		virtual void DragRepeat(Point p);
		virtual void CancelMode();
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);

		void DrawTab(Draw &w, int i);
		void Repos();
		int  Find(int id);
		int  GetNext(int n);
		int  GetPrev(int n);

		int GetWidth(int n);
		int GetHeight(int n);
		int GetTotal();
		int GetTargetTab(Point p);

		void Group();
		void Menu(Bar& bar);

		void CloseAll();

	public:
		Callback WhenCloseRest;

		QuickTabs();

		void Add(const char *name, bool make_active = false);
		void Close(int n);
		void Clear();

		QuickTabs& FileIcons(bool b = true);
		QuickTabs& Crosses(bool b = true);
		QuickTabs& Grouping(bool b = true);

		typedef QuickTabs CLASSNAME;

		int    GetCount() const { return tabs.GetCount(); }
		int    GetCursor() const { return active; }
		String GetFile(int n) const { return tabs[n].name; }
		bool   FindSetFile(const String& fn);
		void   SetAddFile(const String& fn);
		void   RenameFile(const String& fn, const String& nn);
		void   Set(const QuickTabs& t);

		int    GetNextId();
		
		const Vector<Tab>& GetTabs() { return tabs; }
		int GetPos() { return sc.GetPos(); }

		void SetCursor(int n);
};

#endif
