#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

const Point Calendar::nullday = Point(-1, -1);

Calendar::Calendar()
{
	SetFrame(BlackFrame());

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

	open       = false;
	userdate   = false;
	selall     = true;
	isheader   = false;
	istoday    = false;
	washeader  = false;
	wastoday   = false;
	aspopup    = false;

	fnt = StdFont();
	bs = 5;
	first_day = MONDAY;

	Reset();
}

void Calendar::Reset()
{
	Date tdate = GetSysDate();
	tday   = tdate.day;
	tmonth = tdate.month;
	tyear  = tdate.year;

	Date cdate = aspopup ? (Date) (*date) : tdate;
	if(cdate.IsValid())
	{
		cday   = cdate.day;
		cmonth = cdate.month;
		cyear  = cdate.year;
	}
	else
	{
		cday   = tday;
		cmonth = tmonth;
		cyear  = tyear;
	}

	if(!userdate)
	{
		day   = cday;
		month = cmonth;
		year  = cyear;
	}

	userdate = false;
	isheader = false;
	istoday = false;
	washeader = false;
	wastoday = false;

	selday = 0;

	newday = oldday = nullday;
	today = Format("%s: %s", t_("Today"), Format(tdate));
}

void Calendar::SetColors()
{
	colHeader	  = SColorHighlight;
	colCurDate	  = White;
	colSelCurDate = SColorText;
	colToday	  = SColorText;
	colSelToday   = SColorMark;
	colBg		  = SColorPaper;
	colFg		  = SColorText;
	colCurDayBg	  = Color(200, 250, 0);
	colCurDayFg	  = Black;
	colSelDayBg	  = Color(255, 254, 220);
	colSelDayFg	  = Black;
	colGrayDay	  = Color(180, 180, 180);
	colSelDay	  = SColorMark;
	colLine       = Gray;
	colDayNames	  = SColorText;
	colWeek		  = SColorText;
}

void Calendar::OnMonthLeft()
{
	if(--month < 1)
	{
		month = 12;
		if(year > 0)
			year--;
	}
	RefreshAll();
}

void Calendar::OnMonthRight()
{
	if(++month > 12)
	{
		month = 1;
		year++;
	}
	RefreshAll();
}

void Calendar::OnYearLeft()
{
	if(year > 0) year--;
	RefreshAll();
}

void Calendar::OnYearRight()
{
	year++;
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
			day = -day;
			if(lastrow < 3)
				month--;
			else
				month++;

			if(month < 1)
			{
				month = 12;
				year--;
			}
			if(month > 12)
			{
				month = 1;
				year++;
			}
			refall = true;
		}
	}
	else if(!istoday && !isheader)
		return;

	Date dt;

	if(istoday)
		dt = Date(tyear, tmonth, tday);
	else if(isheader)
		dt = Date(year, month, 1);
	else
		dt = Date(year, month, day);

	if(aspopup)
	{
		GetParent()->IgnoreMouseClick();
		Close();
		*date <<= dt;
		date->SetFocus();
		date->WhenAction();
	}
	else
	{
		cday = dt.day;
		cmonth = dt.month;
		cyear = dt.year;

		WhenAction();

		if(refall)
		{
			selday = 0;
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
				selday = 0;
				RefreshDay(oldday);
			}
		}
		if(newday != nullday)
		{
			bool b = (selall == false ? Day(newday) > 0 : true);
			if(b)
			{
				selday = Day(newday);
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

Date Calendar::GetDate()
{
	return Date(cyear, cmonth, cday);
}

void Calendar::ComputeSize()
{
	Size sz = aspopup ? Size(-1, -1) : GetSize();
	Size tsz = GetTextSize("WW", fnt.NoBold().NoUnderline());

	fh = tsz.cy;

	colw = (float)(tsz.cx + 6);
	rowh = (float)(tsz.cy + 4);

	width = bs * 2 + 2 + (int)(colw * (cols + 1));
	height = 4 + (int)(rowh * (rows + 3));

	if(sz.cx > width)
	{
		colw = (sz.cx - bs * 2 + 2) / (float) (cols + 1);
		width = sz.cx;
	}
	if(sz.cy > height)
	{
		rowh = (sz.cy - 4) / (float) (rows + 3);
		height = sz.cy;
	}

	cw = (int)(colw);
	rh = (int)(rowh);

	hs = rh;
	ts = rh;

	btnh = rh - 4 - 1;
	btnw = rh - 4;
}

Size Calendar::GetCalendarSize()
{
	ComputeSize();
	return Size(width, height);
}

void Calendar::Paint(Draw &w)
{
	SetColors(); // to draw in proper colors when theme is changed

	Size sz = GetSize();
	Size tsz;
	String str;
	int d = 1;
	int m = month;
	int y = year;

	if(w.IsPainting(0, 0, sz.cx, hs))
	{
		w.DrawRect(0, 0, sz.cx, hs, colHeader);
		curdate = Format("%s %d", MonthName(month - 1), year);
		szcurdate = GetTextSize(curdate, fnt.Bold());
		w.DrawText((sz.cx - szcurdate.cx) / 2, (hs - szcurdate.cy) / 2, curdate, fnt.Underline(isheader), colCurDate);
	}
	w.DrawRect(0, hs, sz.cx, sz.cy - ts - hs, colBg);

	if(w.IsPainting(0, hs, sz.cx, rh))
	{
		int y = (int) (rowh + (rowh - fh) / 2.0);
		fnt.NoBold().NoUnderline();
		tsz = GetTextSize(t_("Wk"), fnt);
		w.DrawText(bs + (cw - tsz.cx) / 2, y, t_("Wk"), fnt, colWeek);

		for(int i = 0; i < cols; i++)
		{
			const String &s = DyName((i + first_day) % 7);
			tsz = GetTextSize(s, fnt);
			w.DrawText(bs  + 2 + (int)((i + 1) * colw) + (cw - tsz.cx) / 2, y, s, fnt, colDayNames);
		}
	}

	if(w.IsPainting(0, sz.cy - ts, sz.cx, ts))
	{
		sztoday = GetTextSize(today, fnt.Bold().Underline(istoday));
		w.DrawRect(0, sz.cy - ts, sz.cx, ts, colBg);
		w.DrawText((sz.cx - sztoday.cx) / 2, sz.cy - (ts + sztoday.cy) / 2, today, fnt, istoday ? colSelToday : colToday);
	}

	int fd = DayOfWeek(1, month, year);

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
		w.DrawText(bs + (cw - GetTextSize(str, fnt).cx) / 2, yp + yc, str, fnt.NoBold().NoUnderline(), colWeek);

		for(int j = 0; j < cols; j++)
		{
			int xp = bs + 2 + (int)((j + 1) * colw);

			if(w.IsPainting(xp, yp, cw, rh))
			{
				Color fg = colFg;
				Color bg = colBg;

				fnt.NoBold().NoUnderline();
				bool special = false;
				int sd = d;

				if(m == month)
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
					if(d == tday && m == tmonth && y == tyear)
					{
						fg = colCurDayFg;
						bg = colCurDayBg;
						fnt.Bold();
						special = true;

						curday.x = j;
						curday.y = i;
					}
					if(d == cday && m == cmonth && y == cyear)
					{
						fg = colSelDayFg;
						bg = colSelDayBg;
						fnt.Bold();
						special = true;

						prevday.x = j;
						prevday.y = i;
					}
				}
				else
				{
					fg = colGrayDay;
					sd = d > 0 ? -d : d;
					Day(j, i) = sd;
				}


				if(sd == selday)
				{
					if(sd < 0 && selall)
					{
						fg = colGrayDay;
						fnt.Bold().Underline(!special);
					}
					if(sd > 0 && month == m)
					{
						fg = colSelDay;
						fnt.Bold().Underline(!special);
					}
					selmonth = m;
				}

				w.DrawRect(xp, yp, cw, rh, bg);
				if(special)
				{
					DrawFrame(w, xp + 1, yp + 1, cw - 2, rh - 2, Black);
					DrawFrame(w, xp, yp, cw, rh, colBg);
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

	w.DrawRect(bs, (int) (rowh * 2) + 1, sz.cx - bs * 2, 1, colLine);
	w.DrawRect(bs + cw + 1, hs + bs, 1, sz.cy - hs - ts - bs * 1, colLine);

	lastrow = row;
}

void Calendar::Deactivate()
{
	if(open)
	{
		open = false;
		Close();
		IgnoreMouseClick();
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
	open = true;
}

void Calendar::SetDate(int y, int m, int d)
{
	day = d;
	month = m;
	year = y;
	userdate = true;
	RefreshAll();
}

void Calendar::SetDate(Date &dt)
{
	SetDate(dt.year, dt.month, dt.day);
}

DateTimeCtrl::DateTimeCtrl()
{
	AddFrame(drop);
	drop.SetMonoImage(CtrlsImg::DA());
	//drop.NoWantFocus();
	calendar.SetParent(*this);
	calendar.AsPopUp();
	drop <<= THISBACK(OnDrop);
}

DateTimeCtrl& DateTimeCtrl::SetDate(int y, int m, int d)
{
	calendar.SetDate(y, m, d);
	SetData(Date(y, m, d));

	return *this;
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
	calendar.PopUp(this, r);
}

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
