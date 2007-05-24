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
		int pos, ps;
		int new_pos;
		int old_pos;
		int start_pos;
		int size;
		void UpdatePos(bool update = true);
		bool ready;
		Size sz;
		FrameLeft<StaticRect> ls;
		FrameRight<StaticRect> rs;
		
	public:
		TabScrollBar();
		
		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
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

class QuickTabs : public FrameCtrl<Ctrl>
{
	private:
		enum
		{
			MARGIN = 6,
			SPACE = 10,
			HEIGHT = 30
		};
			
		FrameBottom<TabScrollBar> sc;
		FrameRight<Button> list;
		void Scroll();
		
		struct Tab : Moveable<Tab>
		{
			String name;
			int x, cx;
			int y, cy;
			Size tsz;
			Tab() {}
			int Right() { return x + cx; }
			bool HasMouse(Point &p);
			bool HasMouseCross(Point &p);
		};
		
		Vector<Tab> tabs;
		int highlight;
		int active;
		int target;
		int cross;
		bool crosses;
		bool file_icons;
		Point mouse, oldp;

		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keys);
		virtual void LeftUp(Point p, dword keys);
		virtual void MouseMove(Point p, dword keys);
		virtual void MouseLeave();
		virtual void Layout();
		virtual void FrameLayout(Rect &r);
		virtual void FrameAddSize(Size& sz);
		virtual void FramePaint(Draw& w, const Rect& r);
		virtual void DragAndDrop(Point p, PasteClip& d);
		virtual void LeftDrag(Point p, dword keyflags);
		virtual void DragEnter(Point p, PasteClip& d);
		virtual void DragLeave();
		virtual void DragRepeat(Point p);
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		
		void DrawTab(Draw &w, int i);
		void Repos(bool update = false);
		int GetWidth(int n);
		int GetHeight(int n);
		int GetTotal();
		int GetTargetTab(Point p);
				
	public:
		QuickTabs();
		
		void Add(const char *name, bool make_active = false);
		void Close(int n);
		void Remove(int n);
		void Clear();
		
	 	QuickTabs& FileIcons(bool b = true) { file_icons = b; return *this; }
	 	QuickTabs& Crosses(bool b = true)   { crosses = b; return *this;    }
		
		typedef QuickTabs CLASSNAME;
		
		int    GetCount() const { return tabs.GetCount(); }
		int    GetCursor() const { return active; }
		String GetFile(int n) const { return tabs[n].name; }
		bool   FindSetFile(const String& fn);
		void   SetAddFile(const String& fn);
		void   RenameFile(const String& fn, const String& nn);
		void   Set(const QuickTabs& t);

		void SetCursor(int n);		
};

#endif

