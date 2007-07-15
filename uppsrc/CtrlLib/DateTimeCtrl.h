enum
{
	SUNDAY    = 0,
	MONDAY    = 1,
	TUESDAY   = 2,
	WEDNESDAY = 3,
	THURSDAY  = 4,
	FRIDAY    = 5,
	SATURDAY  = 6
};

class FlatButton : public Pusher {
public:
	Image img;
	Color fg, bg, hl;
	bool left;
	bool drawedge;

	FlatButton();

	void DrawFrame(Draw &w, const Rect &r, Color lc, Color tc, Color rc, Color bc);
	virtual void Paint(Draw &w);
	virtual void MouseEnter(Point p, dword kflags) {
		Refresh();
		Pusher::MouseEnter(p, kflags);
	}
	virtual void MouseLeave() {
		Refresh();
		Pusher::MouseLeave();
	}

	FlatButton& SetImage(const Image &_img) {
		img = _img;
		Refresh();
		return *this;
	}

	FlatButton& SetLeft()             { left = true; return *this;}
	FlatButton& SetRight() 			{left = false;	return *this;}
	FlatButton& DrawEdge(bool b) 	{drawedge = b;	return *this;}
};

class Calendar : public Ctrl
{
public:
	struct Style : ChStyle<Style> {
		Color header;
		Color background[3]; // main area, today, selection
		Color foreground[3]; // main area, today, selection
		Color outofmonth;
		Color curdate;
		Color today;
		Color selecttoday;
		Color cursorday;
		Color selectday;
		Color line;
		Color dayname;
		Color week;
		Font font;
	};

protected:
	const Style *style;
	const Style *St() const;

private:
	typedef Calendar CLASSNAME;

	FlatButton mleft;
	FlatButton mright;
	FlatButton yleft;
	FlatButton yright;

	static const int cols = 7;
	static const int rows = 6;

	static const Point nullday;
	int bs; //border size
	int hs; //header vertical size
	int ts; //today vertical size
	float colw;
	float rowh;
	int cw, rh;
	int btnw;
	int btnh;

	int col;
	int row;
	int lastrow;

	int fh;

	int days[rows][cols];

	Point newday;
	Point oldday;
	Point prevday;
	Point curday, firstday;

	String stoday;
	Size sztoday;
	String curdate;
	Size szcurdate;

	bool selall;
	bool istoday;
	bool wastoday;
	bool isheader;
	bool washeader;

	Date view;
	Date today;
	Date sel;

	int first_day;

	void OnMonthLeft();
	void OnMonthRight();
	void OnYearLeft();
	void OnYearRight();

	bool MouseOnToday(Point p);
	bool MouseOnHeader(Point p);

	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void Paint(Draw &w);
	virtual void Deactivate();
	virtual bool Key(dword key, int count);
	virtual void MouseLeave();
	virtual void Layout();
	virtual Image CursorImage(Point p, dword keyflags);

	int& Day(int x, int y) { return days[y][x]; }
	int& Day(Point p) 	   { return days[p.y][p.x]; }
	Point GetDay(Point p);

	void RefreshDay(Point p);
	void RefreshToday();
	void RefreshHeader();
	void RefreshAll();
	Size ComputeSize();

public:
	Calendar();

	Callback 			WhenPopDown;
	static const Style& StyleDefault();

	void Reset();

	bool IsLeapYear(int year) { return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0); }
	int	 DayOfWeek(int day, int month, int year, int zelleroffset = 2);
	int  WeekOfYear(int day, int month, int year);

	virtual Value GetData() const			   { return sel;          }
	virtual void  SetData(const Value& data)   { SetDate((Date)data); }

	Date GetDate() const			   { return sel;                  }
	void SetDate(int y, int m, int d)  { SetDate(Date(y, m, d));	  }
	void SetDate(const Date &dt);

	Date GetCursor() const			   { return view; 				  }
	bool HasCursor() const			   { return view.day != 0;        }

	Date GetView() const			   { return Date(view.year, view.month, view.day ? view.day : 1); }
	void SetView(const Date &v);

	Calendar& SetStyle(const Style& s);
	Calendar& SelectAll(bool b = true) { selall = b; return *this;     }
	Calendar& NoSelectAll()            { selall = false; return *this; }
	Calendar& FirstDay(int n = MONDAY) { first_day = n; return *this;  }

	void PopUp(Ctrl *owner, Rect &rt);
	Size GetCalendarSize()			   { return ComputeSize();   	   }
};


class DateTimeCtrl : public EditDate {
	FrameRight<Button> drop;
	Calendar calendar;

	void OnCalChoice();
	void OnCalClose();
	void OnDrop();

public:
	typedef DateTimeCtrl CLASSNAME;

	DateTimeCtrl();

	DateTimeCtrl& SetDate(int y, int m, int d);
	DateTimeCtrl& SetCalendarStyle(Calendar::Style& style)   { calendar.SetStyle(style); return *this;  }
	DateTimeCtrl& SetButtonStyle(Button::Style& style)       { drop.SetStyle(style); return *this; }
	DateTimeCtrl& SetButtonImage(const Image& image)         { drop.SetImage(image); return *this; }
	DateTimeCtrl& SetButtonMonoImage(const Image& image)     { drop.SetMonoImage(image); return *this; }
	DateTimeCtrl& SetButtonWidth(int cx)                     { drop.Width(cx); return *this; }
	DateTimeCtrl& SelectAll(bool b) 						 { calendar.SelectAll(b); return *this;     }
	DateTimeCtrl& NoSelectAll()     						 { calendar.SelectAll(false); return *this; }
};
