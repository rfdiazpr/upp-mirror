class ProgressIndicator : public Ctrl {
public:
	virtual void   Paint(Draw& draw);
	virtual void   Layout();

protected:
	int   total, actual;
	int   pxp;
	bool  percent:1;

public:
	void  Set(int actual, int total);
	void  Set(int _actual)                     { Set(_actual, total); }

	void  operator=(int i)                     { Set(i); }
	int   operator++()                         { Set(actual + 1); return actual; }
	int   operator++(int)                      { int i = actual; Set(actual + 1); return i; }
	int   operator+=(int i)                    { Set(actual + i); return actual; }

	operator int()                             { return actual; }

	ProgressIndicator& SetTotal(int _total)    { Set(actual, _total); return *this; }
	ProgressIndicator& Percent(bool b = true)  { percent = b; return *this; }
	ProgressIndicator& NoPercent()             { return Percent(false); }

	ProgressIndicator();
	virtual ~ProgressIndicator();
};

class Progress : public WithProgressLayout<TopWindow> {
public:
	ProgressIndicator pi;
	virtual void Close();

protected:
	bool     cancel;
	int      total;
	int      pos;
	String   text;
	int      granularity;
	dword    set_time;
	dword    show_time;
	Ctrl    *owner;
	Modality modality;

	void     Setxt();
	void     Init();
	void     Process();

public:
	void     SetOwner(Ctrl *_owner)        { owner = _owner; }

	void     Create();
	void     Cancel();

	void     Set(int pos, int total);
	void     SetPos(int pos);
	void     SetText(const char *s)        { text = s; Setxt(); Sync(); }
	void     SetText(const String& s)      { text = s; Setxt(); Sync(); }
	void     SetTotal(int total);
	void     Step(int steps = 1);
	int      GetPos() const                { return pos; }
	int      GetTotal() const              { return total; }
	String   GetText() const               { return text; }

	bool     Canceled();
	bool     SetCanceled(int pos, int total);
	bool     SetPosCanceled(int pos);
	bool     StepCanceled(int steps = 1);

	void     Reset();

	void     Granularity(int ms)           { granularity = ms; }

	void     AlignText(int align)          { info.SetAlign(align); }

	operator Gate2<int, int>()             { return callback(this, &Progress::SetCanceled); }

	Progress();
	Progress(Ctrl *_owner);
	Progress(const char *txt, int total = 0);
	Progress(Ctrl *_owner, const char *txt, int total = 0);
	virtual ~Progress();
};
