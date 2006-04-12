class EditField : public Ctrl {
public:
	virtual void  Layout();
	virtual void  Paint(Draw& draw);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword keyflags);
	virtual void  LeftDouble(Point p, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual bool  Key(dword key, int rep);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  SetData(const Value& data);
	virtual Value GetData() const;

protected:
	WString    text;
	int        sc;
	int        cursor, anchor;

	WString    undotext;
	int        undocursor, undoanchor;

	CharFilter      filter;
	const Convert  *convert;
	Font            font;

	WString         nulltext;
	Color           nullink;
	int             maxlen;
	int             autosize;

	bool       password:1;
	bool       autoformat:1;
	bool       initcaps:1;
	bool       clickselect;

	int     GetTextCx(const wchar *text, int n, bool password);
	void    Paints(Draw& w, int& x, int cy, const wchar *&txt,
		           Color ink, Color paper, int n, bool pwd);
	int     GetStringCx(const wchar *text, int n);
	int     GetCaret(int cursor);
	int     GetCursor(int posx);
	void    Finish(bool refresh = true);
	void    SaveUndo();
	void    DoAutoFormat();
	bool    HasBorder();

public:
	static  int   GetStdHeight(Font font = StdFont());

	int     Insert(int pos, const WString& text);
	int     Insert(int pos, const String& text)         { return Insert(pos, text.ToWString()); }
	int     Insert(int pos, const char *text)           { return Insert(pos, WString(text)); }
	void    Remove(int pos, int n);

	void    Insert(const WString& text);
	void    Insert(const String& text)                  { return Insert(text.ToWString()); }
	void    Insert(const char *text)                    { return Insert(WString(text)); }
	void    Insert(int chr);

	void    Move(int newpos, bool select);

	void    SetSelection(int l = 0, int h = INT_MAX);
	bool    GetSelection(int& l, int& h) const;
	bool    RemoveSelection();
	void    Copy();

	void           SetText(const WString& text);
	void           SetText(const String& t)  { SetText(t.ToWString()); }
	void           SetText(const char *t)    { SetText(WString(t)); }
	const WString& GetText() const           { return text; }

	operator const WString&() const          { return text; }
	operator String() const                  { return text.ToString(); }
	void operator=(const WString& s)         { SetText(s); }
	int     GetLength() const                { return text.GetLength(); }

	void    Clear();
	void    Reset();

	EditField& Password(bool pwd = true)     { password = pwd; Finish(); return *this; }
	EditField& SetFilter(int (*f)(int))      { filter = f; return *this; }
	EditField& SetConvert(const Convert& c)  { convert = &c; return *this; }
	EditField& AutoFormat(bool b = true)     { autoformat = b; return *this; }
	EditField& NoAutoFormat()                { return AutoFormat(false); }
	EditField& SetFont(Font _font);
	EditField& ClickSelect(bool b = true)    { clickselect = b; return *this; }
	EditField& InitCaps(bool b = true)       { initcaps = b; return *this; }
	EditField& NullText(const char *text = t_("(default)"), Color ink = Brown);
	EditField& MaxChars(int mc)              { maxlen = mc; return *this; }
	EditField& AutoSize(int maxcx = INT_MAX) { autosize = maxcx; Finish(); return *this; }

	CharFilter     GetFilter() const         { return filter; }
	const Convert& GetConvert() const        { return *convert; }
	Font           GetFont() const           { return font; }

	EditField();
	virtual ~EditField();
};

template <class DataType, class Cv>
class EditValue : public EditField, public Cv {
public:
	EditValue& operator=(const DataType& t)  { SetData(t); return *this; }
	operator DataType() const                { return GetData(); }

	EditValue()                              { SetConvert(*this); }
};

template <class DataType, class Cv>
class EditMinMax : public EditValue<DataType, Cv> {
public:
	EditMinMax& operator=(const DataType& t)          { SetData(t); return *this; }

	EditMinMax() {}
	EditMinMax(DataType min, DataType max)            { Cv::MinMax(min, max); }

	EditMinMax& Min(DataType min)                     { Cv::Min(min); return *this; }
	EditMinMax& Max(DataType max)                     { Cv::Max(max); return *this; }
	EditMinMax& NotNull(bool nn = true)               { Cv::NotNull(nn); return *this; }
};

template <class DataType, class Cv>
class EditMinMaxNotNull : public EditValue<DataType, Cv> {
public:
	EditMinMaxNotNull& operator=(const DataType& t)   { SetData(t); return *this; }

	EditMinMaxNotNull()                               { Cv::NotNull(); }
	EditMinMaxNotNull(DataType min, DataType max)     { Cv::NotNull(); Cv::MinMax(min, max); }

	EditMinMaxNotNull& Min(DataType min)              { Cv::Min(min); return *this; }
	EditMinMaxNotNull& Max(DataType max)              { Cv::Max(max); return *this; }
	EditMinMaxNotNull& NotNull(bool nn = true)        { Cv::NotNull(nn); return *this; }
};

typedef EditMinMax<int, ConvertInt>              EditInt;
typedef EditMinMax<double, ConvertDouble>        EditDouble;
typedef EditMinMax<Date, ConvertDate>            EditDate;
typedef EditMinMax<Time, ConvertTime>            EditTime;
typedef EditMinMaxNotNull<int, ConvertInt>       EditIntNotNull;
typedef EditMinMaxNotNull<double, ConvertDouble> EditDoubleNotNull;
typedef EditMinMaxNotNull<Date, ConvertDate>     EditDateNotNull;
typedef EditMinMaxNotNull<Time, ConvertTime>     EditTimeNotNull;

class EditString : public EditValue<WString, ConvertString> {
public:
	operator const WString&() const                  { return GetText(); }

	EditString& operator=(const WString& data)       { SetData(data); return *this; }
	EditString& operator=(const String& data)        { SetData(data); return *this; }

	EditString() {}
	EditString(int maxlen)                           { MaxLen(maxlen); }

	EditString& MaxLen(int maxlen)                   { ConvertString::MaxLen(maxlen); return *this; }
	EditString& NotNull(bool nn = true)              { ConvertString::NotNull(nn); return *this; }
};

class EditStringNotNull : public EditString {
public:
	EditStringNotNull& operator=(const WString& data) { SetData(data); return *this; }
	EditStringNotNull& operator=(const String& data)  { SetData(data); return *this; }

	EditStringNotNull()                               { NotNull(); }
	EditStringNotNull(int maxlen)                     { NotNull(); MaxLen(maxlen); }
};

class EditIntSpin : public EditInt {
protected:
	SpinButtons sb;

	void   Inc();
	void   Dec();
	void   Init();

public:
	EditIntSpin();
	EditIntSpin(int min, int max);
	virtual ~EditIntSpin();

	virtual bool    Key(dword key, int repcnt);

	EditIntSpin&    ShowSpin(bool s = true)   { sb.Show(s); return *this; }
};

class EditDoubleSpin : public EditDouble
{
public:
	typedef EditDoubleSpin CLASSNAME;
	EditDoubleSpin(double inc = 0.1);
	EditDoubleSpin(double min, double max, double inc = 0.1);
	virtual ~EditDoubleSpin();

	EditDoubleSpin& SetInc(double _inc = 0.1) { inc = _inc; return *this; }
	double          GetInc() const            { return inc; }

	EditDoubleSpin& ShowSpin(bool s = true)   { spin.Show(s); return *this; }

	virtual bool    Key(dword key, int repcnt);

protected:
	void            Inc();
	void            Dec();
	void            Init();

private:
	SpinButtons     spin;
	double          inc;
};

class EditDoubleNotNullSpin : public EditDoubleSpin
{
public:
	EditDoubleNotNullSpin(double inc = 0.1) : EditDoubleSpin(inc) { NotNull(); }
	EditDoubleNotNullSpin(double min, double max, double inc = 0.1) : EditDoubleSpin(min, max, inc) { NotNull(); }
};
