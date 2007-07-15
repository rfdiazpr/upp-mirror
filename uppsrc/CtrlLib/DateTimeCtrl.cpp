#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

const Point Calendar::nullday = Point(-1, -1);

Calendar::Calendar()
{
	SetFrame(BlackFrame());
	style = NULL;

	Add(mleft);
	Add(mright);
	Add(yleft);
	Add(yright);

	mleft.Tip(t_("Previous month"));
	mright.Tip(t_("Next month"));
	yleft.Tip(t_("Previous year"));
	yright.Tip(t_("Next year"));

	mleft  <<= THISBACK(OnMonthLeft);
	mright <<= THISBACK(OnMonthRight);
	yleft  <<= THISBACK(OnYearLeft);
	yright <<= THISBACK(OnYearRight);

	selall     = true;
	isheader   = false;
	istoday    = false;
	washeader  = false;
	wastoday   = false;

	bs = 5;
	first_day = MONDAY;

	Reset();
}

void Calendar::Reset()
{
	today = GetSysDate();
	view = today;
	view.day = 0;
	sel = today;

	isheader = false;
	istoday = false;
	washeader = false;
	wastoday = false;

	newday = oldday = nullday;
	stoday = Format("%s: %s", t_("Today"), Format(today));
}

Calendar& Calendar::SetStyle(const Style& s)
{
	style = &s;
	Refresh();
	return *this;
}

void Calendar::OnMonthLeft()
{
	if(--view.month < 1)
	{
		view.month = 12;
		if(view.year > 0)
			view.year--;
	}
	RefreshAll();
}

void Calendar::OnMonthRight()
{
	if(++view.month > 12)
	{
		view.month = 1;
		view.year++;
	}
	RefreshAll();
}

void Calendar::OnYearLeft()
{
	if(view.year > 0) view.year--;
	RefreshAll();
}

void Calendar::OnYearRight()
{
	view.year++;
	RefreshAll();
}

int	Calendar::DayOfWeek(int day, int month, int year, int zelleroffset)
{
	if(month < 3)
	{
		month += 12 ;
		year -= 1 ;
    }

	int n1 = (26 * (month + 1)) / 10;
	int n2 = (125 * year) / 100;

	return ((day + n1 + n2 - (year / 100) + (year / 400) - zelleroffset) % 7);
}

int Calendar::WeekOfYear(int day, int month, int year) /* ISO-8601 */
{
	const static int mnth[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	int weeknum;

	bool cleapyear = IsLeapYear(year);
	bool bleapyear = IsLeapYear(year - 1);

	int daynum = day + mnth[month - 1];
	if(cleapyear && month > 2)
		daynum += 1;

	int yy = (year - 1) % 100;
	int c = (year - 1) - yy;
	int g = yy + yy / 4;
	int jan1weekday = 1 + (((((c / 100) % 4) * 5) + g) % 7);
	int h = daynum + (jan1weekday - 1);
	int weekday = 1 + ((h -1) % 7);

	int yearnum;

	if(daynum <= (8 - jan1weekday) && jan1weekday > 4)
	{
		yearnum = year - 1;
		if(jan1weekday == 5 || (jan1weekday == 6 && bleapyear))
			weeknum = 53;
		else
			weeknum = 52;
	}
	else
		yearnum = year;

	if(yearnum == year)
	{
		int yeardays = cleapyear ? 366 : 365;
		if((yeardays - daynum) < (4 - weekday))
		{
			yearnum = year + 1;
			weeknum = 1;
		}
	}
	if(yearnum == year)
	{
		int fday = daynum + (7 - weekday) + (jan1weekday -1);
		weeknum = fday / 7;
		if(jan1weekday > 4)
			weeknum -= 1;
	}

	return weeknum;
}

void Calendar::LeftDown(Point p, dword keyflags)
{
}

void Calendar::LeftUp(Point p, dword keyflags)
{
	bool isnewday = newday != nullday;
	int day = 1;
	bool refall = false;
	if(isnewday)
	{
		day = Day(newday);
		if(!selall && day < 0) return;

		if(day < 0)
		{
			view.day = -day;
			if(lastrow < 3)
				view.month--;
			else
				view.month++;

			if(view.month < 1)
			{
				view.month = 12;
				view.year--;
			}
			if(view.month > 12)
			{
				view.month = 1;
				view.year++;
			}
			refall = true;
		}
	}
	else if(!istoday && !isheader)
		return;

	Date dt;

	if(istoday)
		dt = today;
	else if(isheader)
		dt = Date(view.year, view.month, 1);
	else
		dt = Date(view.year, view.month, view.day);

	if(IsPopUp())
	{
		sel = dt;
		Deactivate();
		WhenAction();
	}
	else
	{
		sel = dt;
		WhenAction();

		if(refall)
		{
			view.day = 0;
			RefreshAll();
			Sync();
			oldday = nullday;
			MouseMove(p, 0);
			return;
		}

		RefreshDay(prevday);
		if(istoday)
			SetDate(dt);
		else if(isheader)
			RefreshDay(firstday);
		else
			RefreshDay(newday);
	}
}

void Calendar::MouseMove(Point p, dword keyflags)
{
	newday = GetDay(p);
	if(newday != oldday)
	{
		if(oldday != nullday)
		{
			bool b = (selall == false ? Day(oldday) > 0 : true);
			if(b)
			{
				view.day = 0;
				RefreshDay(oldday);
			}
		}
		if(newday != nullday)
		{
			bool b = (selall == false ? Day(newday) > 0 : true);
			if(b)
			{
				view.day = Day(newday);
				RefreshDay(newday);
			}
		}
		oldday = newday;
	}
	istoday = MouseOnToday(p);
	if(istoday != wastoday)
	{
		wastoday = istoday;
		RefreshToday();
	}
	isheader = MouseOnHeader(p);
	if(isheader != washeader)
	{
		washeader = isheader;
		RefreshHeader();
	}
}

Image Calendar::CursorImage(Point p, dword keyflags)
{
	bool b = (selall == false ? Day(newday) > 0 : true);
	if((newday != nullday && b) || istoday || isheader)
		return CtrlImg::HandCursor();
	else
		return Image::Arrow();
}

bool Calendar::MouseOnToday(Point p)
{
	Size sz = GetSize();
	int x0 = (sz.cx - sztoday.cx) / 2;
	int x1 = x0 + sztoday.cx;
	int y0 = sz.cy - (rh + sztoday.cy) / 2;
	int y1 = y0 + sztoday.cy;
	return (p.x >= x0 && p.x <= x1 && p.y >= y0 && p.y <= y1);
}

bool Calendar::MouseOnHeader(Point p)
{
	Size sz = GetSize();
	int x0 = (sz.cx - szcurdate.cx) / 2;
	int x1 = x0 + szcurdate.cx;
	int y0 = (hs - szcurdate.cy) / 2;
	int y1 = y0 + szcurdate.cy;
	return (p.x >= x0 && p.x <= x1 && p.y >= y0 && p.y <= y1);
}

void Calendar::RefreshDay(Point p)
{
	col = p.x;
	row = p.y;

	int y0 = 2 + (int)((p.y + 2) * rowh);
	int x0 = bs + 2 + (int)((p.x + 1) * colw);

	Refresh(x0, y0, cw, rh);
}

void Calendar::RefreshToday()
{
	Size sz = GetSize();
	Refresh(0, sz.cy - ts, sz.cx, ts);
}

void Calendar::RefreshHeader()
{
	Size sz = GetSize();
	int bw = btnw * 2 + 4;
	Refresh(bw, 0, sz.cx - bw * 2, hs);
}

void Calendar::RefreshAll()
{
	Refresh();
}

Point Calendar::GetDay(Point p)
{
	for(int i = 0; i < rows; i++)
	{
		int y0 = 2 + (int)((i + 2) * rowh);
		int y1 = y0 + rh;

		if(p.y >= y0 && p.y < y1)
		for(int j = 0; j < cols; j++)
		{
			int x0 = bs + 2 + (int)((j + 1) * colw);
			int x1 = x0 + cw;
			if(p.x >= x0 && p.x < x1)
				return Point(j, i);
		}
	}
	return Point(-1, -1);
}

Size Calendar::ComputeSize()
{
	const Style &st = style ? *style : StyleDefault();
	Font fnt = st.font;

	Size sz = IsPopUp() ? Size(-1, -1) : GetSize();
	Size tsz = GetTextSize("WW", fnt.NoBold().NoUnderline());
	Size rsz;

	fh = tsz.cy;

	colw = (float)(tsz.cx + 6);
	rowh = (float)(tsz.cy + 4);

	rsz.cx = bs * 2 + 2 + (int)(colw * (cols + 1));
	rsz.cy = 4 + (int)(rowh * (rows + 3));

	if(sz.cx > rsz.cx)
	{
		colw = (sz.cx - bs * 2 + 2) / (float) (cols + 1);
		rsz.cx = sz.cx;
	}
	if(sz.cy > rsz.cy)
	{
		rowh = (sz.cy - 4) / (float) (rows + 3);
		rsz.cy = sz.cy;
	}

	cw = (int)(colw);
	rh = (int)(rowh);

	hs = rh;
	ts = rh;

	btnh = rh - 4 - 1;
	btnw = rh - 4;

	return rsz;
}

void Calendar::Paint(Draw &w)
{
	const Style &st = style ? *style : StyleDefault();
	Font fnt = st.font;

	Size sz = GetSize();
	Size tsz;
	String str;
	int d = 1;
	int m = view.month;
	int y = view.year;

	if(w.IsPainting(0, 0, sz.cx, hs))
	{
		w.DrawRect(0, 0, sz.cx, hs, st.header);
		curdate = Format("%s %d", MonthName(view.month - 1), view.year);
		szcurdate = GetTextSize(curdate, fnt.Bold());
		w.DrawText((sz.cx - szcurdate.cx) / 2, (hs - szcurdate.cy) / 2, curdate, fnt.Underline(isheader), st.curdate);
	}
	w.DrawRect(0, hs, sz.cx, sz.cy - ts - hs, st.background[0]);

	if(w.IsPainting(0, hs, sz.cx, rh))
	{
		int y = (int) (rowh + (rowh - fh) / 2.0);
		fnt.NoBold().NoUnderline();
		tsz = GetTextSize(t_("Wk"), fnt);
		w.DrawText(bs + (cw - tsz.cx) / 2, y, t_("Wk"), fnt, st.week);

		for(int i = 0; i < cols; i++)
		{
			const String &s = DyName((i + first_day) % 7);
			tsz = GetTextSize(s, fnt);
			w.DrawText(bs  + 2 + (int)((i + 1) * colw) + (cw - tsz.cx) / 2, y, s, fnt, st.dayname);
		}
	}

	if(w.IsPainting(0, sz.cy - ts, sz.cx, ts))
	{
		sztoday = GetTextSize(stoday, fnt.Bold().Underline(istoday));
		w.DrawRect(0, sz.cy - ts, sz.cx, ts, st.background[0]);
		w.DrawText((sz.cx - sztoday.cx) / 2, sz.cy - (ts + sztoday.cy) / 2, stoday, fnt, istoday ? st.selecttoday : st.today);
	}

	int fd = DayOfWeek(1, view.month, view.year);

	if(fd > 0)
	{
		if(--m < 1)
		{
			m = 12;
			y -= 1;
		}
		d = GetDaysOfMonth(m, y) - fd + 1;
	}

	for(int i = 0; i < rows; i++)
	{
		int yp = 2 + (int) ((i + 2) * rowh);
		int yc = (rh - fh) / 2;

		str = AsString(WeekOfYear(d, m, y));
		w.DrawText(bs + (cw - GetTextSize(str, fnt).cx) / 2, yp + yc, str, fnt.NoBold().NoUnderline(), st.week);

		for(int j = 0; j < cols; j++)
		{
			int xp = bs + 2 + (int)((j + 1) * colw);

			if(w.IsPainting(xp, yp, cw, rh))
			{
				Color fg = st.foreground[0];
				Color bg = st.background[0];

				fnt.NoBold().NoUnderline();
				bool special = false;
				int sd = d;

				if(m == view.month)
				{
					Day(j, i) = d;

					fg = Black;

					if(j == 6)
					{
						fg = Red;
					}
					if(d == 1)
					{
						firstday.x = j;
						firstday.y = i;
					}
					if(d == today.day && m == today.month && y == today.year)
					{
						fg = st.foreground[1];
						bg = st.background[1];
						fnt.Bold();
						special = true;

						curday.x = j;
						curday.y = i;
					}
					if(d == sel.day && m == sel.month && y == sel.year)
					{
						fg = st.foreground[2];
						bg = st.background[2];
						fnt.Bold();
						special = true;

						prevday.x = j;
						prevday.y = i;
					}
				}
				else
				{
					fg = st.outofmonth;
					sd = d > 0 ? -d : d;
					Day(j, i) = sd;
				}


				if(sd == view.day)
				{
					if(sd < 0 && selall)
					{
						fg = st.outofmonth;
						fnt.Bold().Underline(!special);
					}
					if(sd > 0 && view.month == m)
					{
						fg = st.selectday;
						fnt.Bold().Underline(!special);
					}
				}

				w.DrawRect(xp, yp, cw, rh, bg);
				if(special)
				{
					DrawFrame(w, xp + 1, yp + 1, cw - 2, rh - 2, Black);
					DrawFrame(w, xp, yp, cw, rh, st.background[0]);
				}

				str = AsString(abs(d));
				w.DrawText(xp + (cw - GetTextSize(str, fnt).cx) / 2, yp + yc , str, fnt, fg);
			}

			if(++d > GetDaysOfMonth(m, y))
			{
				d = 1;
				if(++m > 12)
				{
					m = 1;
					y += 1;
				}
			}
		}
	}

	w.DrawRect(bs, (int) (rowh * 2) + 1, sz.cx - bs * 2, 1, st.line);
	w.DrawRect(bs + cw + 1, hs + bs, 1, sz.cy - hs - ts - bs * 1, st.line);

	lastrow = row;
}

void Calendar::Deactivate()
{
	if(IsOpen() && IsPopUp())
	{
		WhenPopDown();
		IgnoreMouseClick();
		Close();
	}
}

void Calendar::Layout()
{
	ComputeSize();

	mleft.LeftPos(2, btnw).TopPos(2, btnh);
	mleft.SetLeft().DrawEdge(0).SetImage(CtrlImg::smallleft());
	mright.LeftPos(btnw + 2, btnw).TopPos(2, btnh);
	mright.SetRight().DrawEdge(0).SetImage(CtrlImg::smallright());

	yright.RightPos(2, btnw).TopPos(2, btnh);
	yright.SetRight().DrawEdge(0).SetImage(CtrlImg::smallright());
	yleft.RightPos(btnw + 2, btnw).TopPos(2, btnh);
	yleft.SetLeft().DrawEdge(0).SetImage(CtrlImg::smallleft());
}

bool Calendar::Key(dword key, int count)
{
	if(key == K_ESCAPE)
	{
		Deactivate();
		return true;
	}

	return false;
}

void Calendar::MouseLeave()
{
	if(istoday)
	{
		istoday = false;
		wastoday = false;
		RefreshToday();
	}
	else if(isheader)
	{
		isheader = false;
		washeader = false;
		RefreshHeader();
	}
	else
		MouseMove(Point(-1, -1), 0);
}

void Calendar::PopUp(Ctrl *owner, Rect &rt)
{
	Close();
	Reset();
	SetRect(rt);
	Ctrl::PopUp(owner, true, true, GUI_DropShadows());
}

void Calendar::SetDate(const Date &dt)
{
	sel = !IsNull(dt) && dt.IsValid() ? dt : today;
	view = sel;
	view.day = 0;
	RefreshAll();
}

void Calendar::SetView(const Date &v)
{
	view = v;
	view.day = 0;
	RefreshAll();
}

CH_STYLE(Calendar, Style, StyleDefault)
{
	header  = SColorHighlight;

	background[0] = SColorPaper; 		// main area
	background[1] = Color(200, 250, 0); 	// today
	background[2] = Color(255, 254, 220);// selection

	foreground[0] = SColorText; 	// main area
	foreground[1] = Black;		// today
	foreground[2] = Black;		// selection

	outofmonth 		= Color(180, 180, 180);
	curdate 		= White;
	today 			= SColorText;
	selecttoday 	= SColorMark;
	cursorday		= SColorText;
	selectday		= SColorMark;
	line			= Gray;
	dayname			= SColorText;
	week			= SColorText;
	Font font		= StdFont();
}

// DateTimeCtrl

DateTimeCtrl::DateTimeCtrl()
{
	AddFrame(drop);
	drop.SetMonoImage(CtrlsImg::DA());
	//drop.NoWantFocus();
	drop 				<<= THISBACK(OnDrop);
	calendar 			<<= THISBACK(OnCalChoice);
	calendar.WhenPopDown  = THISBACK(OnCalClose);
}

DateTimeCtrl& DateTimeCtrl::SetDate(int y, int m, int d)
{
	SetData(Date(y, m, d));
	return *this;
}

void DateTimeCtrl::OnCalChoice()
{
	SetData(~calendar);
}

void DateTimeCtrl::OnCalClose()
{
	IgnoreMouse(false);
	SetFocus();
}

void DateTimeCtrl::OnDrop()
{
	if(!IsEditable())
		return;

	Size sz = calendar.GetCalendarSize();
	int width = sz.cx;
	int height = sz.cy;

	Rect rw = Ctrl::GetWorkArea();
	Rect rs = GetScreenRect();
	Rect r;
	r.left   = rs.left;
	r.right  = rs.left + width;
	r.top    = rs.bottom;
	r.bottom = rs.bottom + height;

	if(r.bottom > rw.bottom)
	{
		r.top = rs.top - height;
		r.bottom = rs.top;
	}
	if(r.right > rw.right)
	{
		int diff;
		if(rs.right <= rw.right)
			diff = rs.right - r.right;
		else
			diff = rw.right - r.right;

		r.left += diff;
		r.right += diff;
	}
	if(r.left < rw.left)
	{
		int diff = rw.left - r.left;
		r.left += diff;
		r.right += diff;

	}
	IgnoreMouse(true);
	calendar.PopUp(this, r);
	calendar <<= GetData();
}

// FlatButton

FlatButton::FlatButton()
{
	hl = Blend(SColorHighlight, White, 200);//Color(235, 240, 243);
	bg = Blend(SColorHighlight, White, 128);//Color(179, 220, 253);
	fg = SColorPaper;//White;
	left = true;
}

void FlatButton::DrawFrame(Draw &w, const Rect &r, Color lc, Color tc, Color rc, Color bc)
{
	w.DrawRect(r.left, r.top, r.left + 1, r.bottom, lc);
	w.DrawRect(r.right - 1, r.top, r.right, r.bottom, rc);
	w.DrawRect(r.left, r.top, r.right, r.top + 1, tc);
	w.DrawRect(r.left, r.bottom - 1, r.right, r.bottom, bc);
}

void FlatButton::Paint(Draw &w)
{
	Size sz = GetSize();
	Size isz = img.GetSize();

	w.DrawRect(sz, HasMouse() ? hl : bg);

	if(drawedge)
		UPP::DrawFrame(w, sz, fg);
	else
	{
		if(!left)
			DrawFrame(w, sz, bg, fg, fg, fg);
		else
			DrawFrame(w, sz, fg, fg, bg, fg);
	}

	int dx = IsPush() * (left ? -1 : 1);
	int dy = IsPush();
	w.DrawImage((sz.cx - isz.cx) / 2 + dx, (sz.cy - isz.cy) / 2 + dy, img);
}

END_UPP_NAMESPACE
