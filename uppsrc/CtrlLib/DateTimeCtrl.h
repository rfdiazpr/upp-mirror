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

	int width;
	int height;
	int fh;

	bool selall;
	bool aspopup;

	Color colHeader;
	Color colBg;
	Color colFg;
	Color colCurDayBg;
	Color colCurDayFg;
	Color colSelDayBg;
	Color colSelDayFg;
	Color colGrayDay;
	Color colCurDate;
	Color colToday;
	Color colSelToday;
	Color colSelCurDate;
	Color colSelDay;
	Color colLine;
	Color colDayNames;
	Color colWeek;

	Font fnt;

	int days[rows][cols];

	Point newday;
	Point oldday;
	Point prevday;
	Point curday, firstday;

	String today;
	Size sztoday;
	String curdate;
	Size szcurdate;
	bool istoday;
	bool wastoday;
	bool isheader;
	bool washeader;

	EditDate *date;

	int day, month, year;
	int tday, tmonth, tyear;
	int cday, cmonth, cyear;
	int selday;
	int selmonth;

	int first_day;

	bool userdate;
	bool open;
	bool clickall;

	void OnMonthLeft();
	void OnMonthRight();
	void OnYearLeft();
	void OnYearRight();

	void SetColors();

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
	void ComputeSize();

public:

	Calendar();

	void Reset();

	bool IsLeapYear(int year) { return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0); }
	int	 DayOfWeek(int day, int month, int year, int zelleroffset = 2);
	int  WeekOfYear(int day, int month, int year);

	Date GetDate();

	Calendar& SetFont(Font f)          { fnt = f; return *this;        }
	Calendar& SelectAll(bool b = true) { selall = b; return *this;     }
	Calendar& NoSelectAll()            { selall = false; return *this; }
	Calendar& AsPopUp(bool b = true)   { aspopup = b; return *this;    }
	Calendar& FirstDay(int n = MONDAY) { first_day = n; return *this; }

	void SetParent(EditDate &ctrl) { date = &ctrl; }
	void SetDate(int y, int m, int d);
	void SetDate(Date &dt);
	void PopUp(Ctrl *owner, Rect &rt);
	Size GetCalendarSize();
};


class DateTimeCtrl : public EditDate {
	FrameRight<Button> drop;
	Calendar calendar;
	void OnDrop();

	public:
		typedef DateTimeCtrl CLASSNAME;

	    DateTimeCtrl();

		DateTimeCtrl& SetDate(int y, int m, int d);
	    DateTimeCtrl& SetFont(Font f)   { calendar.SetFont(f); return *this;       }
		DateTimeCtrl& SelectAll(bool b) { calendar.SelectAll(b); return *this;     }
		DateTimeCtrl& NoSelectAll()     { calendar.SelectAll(false); return *this; }
};
