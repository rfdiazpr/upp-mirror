class ToolWin : public ParentCtrl {
public:
	virtual void MoveBegin();
	virtual void Moving();
	virtual void MoveEnd();

private:		
	struct ToolWinFrame : public TopWindow {
		virtual void  Paint(Draw& w);
		virtual void  LeftDown(Point p, dword keyflags);
		virtual void  MouseMove(Point p, dword keyflags);
		virtual Image CursorImage(Point p, dword keyflags);
		virtual void  LeftUp(Point p, dword keyflags);
		virtual void  Layout();
		virtual Size  GetMinSize() const;
		virtual Size  GetMaxSize() const;
	
		Point      p0;
		Rect       rect0;
		Point      dragdir;
		ToolButton close;
	
		void DoClose();
		
		Point GetDir(Point p) const;
		int   GetTitleCy() const;
		int   GetBorder() const;
		Rect  GetMargins() const;
		Size  AddMargins(Size sz) const;
		
		void StartMouseDrag0();
		
		ToolWin *client;
	
		typedef ToolWinFrame CLASSNAME;
	
		void StartMouseDrag();
		void SetClientRect(Rect r);
	
		ToolWinFrame();
	};

	ToolWinFrame win;

public:
	void StartMouseDrag()             { win.StartMouseDrag(); }
	void SetClientRect(const Rect& r) { win.SetClientRect(r); }
	void PlaceClientRect(Rect r);
	
//	TopWindow& Win()                  { return win; }
	
	ToolWin();
};
