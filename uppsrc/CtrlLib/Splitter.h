class Splitter : public Ctrl {
public:
	virtual void   Layout();
	virtual void   Paint(Draw& draw);
	virtual void   MouseMove(Point p, dword keyflags);
	virtual void   LeftDown(Point p, dword keyflags);
	virtual void   LeftUp(Point p, dword keyflags);
	virtual Image  CursorImage(Point p, dword keyflags);
	virtual void   Serialize(Stream& s);

protected:
	Vector<int> pos;
	Vector<int> mins;
	int         width;
	int         style;
	int         mouseindex;
	bool        vert;
	int         inset;

	int       ClientToPos(Point client) const;
	int       PosToClient(int pos) const;
	int       FindIndex(Point client) const;
	int       GetChildCount() const;

public:
	void      Set(Ctrl& l, Ctrl& r);

	Splitter& SetPos(int newpos, int index = 0);
	int       GetPos(int index = 0) const          { return index < pos.GetCount() ? pos[index] : 10000; }

	int       GetCount() const                     { return GetChildCount(); }

	void      Zoom(int i);
	void      NoZoom()                             { Zoom(-1); }
	int       GetZoom() const                      { return style; }

	void      SetMin(int i, int w)                 { mins.At(i, 0) = w; }

	void      Add(Ctrl& pane);
	Splitter& operator<<(Ctrl& pane)               { Add(pane); return *this; }

	Splitter& Vert(Ctrl& top, Ctrl& bottom);
	Splitter& Horz(Ctrl& left, Ctrl& right);
	Splitter& Vert()                               { vert = true; Layout(); return *this; }
	Splitter& Horz()                               { vert = false; Layout(); return *this; }
	Splitter& BarWidth(int w);
	bool      IsHorz() const                       { return !vert; }
	bool      IsVert() const                       { return vert; }

	void      Reset();

	Splitter();
	virtual ~Splitter();
};
