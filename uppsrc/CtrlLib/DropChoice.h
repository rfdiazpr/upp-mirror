class PopUpTable : public ArrayCtrl {
public:
	virtual void Deactivate();
	virtual void LeftUp(Point p, dword keyflags);
	virtual bool Key(dword key, int);
	virtual void CancelMode();

protected:
	int          droplines;
	bool         open;

	void         DoClose();

public:
	void         PopUp(Ctrl *owner, int x, int top, int bottom, int width);
	void         PopUp(Ctrl *owner, int width);
	void         PopUp(Ctrl *owner);

	Callback     WhenCancel;
	Callback     WhenSelect;

	PopUpTable&  SetDropLines(int _droplines)          { droplines = _droplines; return *this; }

	void         Normal();

	PopUpTable();
	virtual ~PopUpTable();
};

class DropList : public Ctrl, public Convert {
public:
	virtual void  Paint(Draw& w);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  MouseMove(Point, dword style);
	virtual void  MouseLeave();
	virtual void  LeftUp(Point p, dword keyflags);
	virtual bool  Key(dword key, int);
	virtual void  SetData(const Value& data);
	virtual Value GetData() const;
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  State(int);

	virtual Value Format(const Value& q) const;

protected:
	FrameRight<Button> drop;
	PopUpTable         list;
	Index<Value>       key;
	Value              value;
	bool               displayall;
	bool               dropfocus;
	bool               push;
	const Display     *valuedisplay;

	void          Select();
	void          Cancel();
	void          Change(int q);

public:
	Callback      WhenDrop;
	Callback      WhenClick;

	DropList&     Add(const Value& key, const Value& text);
	DropList&     Add(const Value& value)       { return Add(value, value); }
	void          ClearList();
	void          Clear();

	void          Drop();

	const Value& operator=(const Value& v)      { SetData(v); return v; }
	operator Value() const                      { return GetData(); }

	DropList&     SetDropLines(int d)           { list.SetDropLines(d); return *this; }

	void          SetIndex(int i)               { SetData(GetKey(i)); }
	int           GetIndex()                    { return FindKey(value); }

	bool          HasKey(const Value& k) const  { return key.Find(k) >= 0; }
	int           FindKey(const Value& k) const { return key.Find(k); }

	int           GetCount() const              { return key.GetCount(); }
	void          Trim(int n);
	const Value&  GetKey(int i) const           { return key[i]; }

	Value         GetValue(int i) const         { return list.Get(i, 0); }
	Value         GetValue() const;
	void          SetValue(int i, const Value& v);
	void          SetValue(const Value& v);

	void          Adjust();
	void          Adjust(const Value& k);

	const PopUpTable& GetList() const           { return list; }

	DropList&     SetDisplay(int i, const Display& d)   { list.SetDisplay(i, 0, d); return *this; }
	DropList&     SetDisplay(const Display& d)          { list.ColumnAt(0).SetDisplay(d); return *this; }
	DropList&     SetLineCy(int lcy)                    { list.SetLineCy(lcy); return *this; }
	DropList&     SetDisplay(const Display& d, int lcy) { SetDisplay(d); SetLineCy(lcy); return *this; }
	DropList&     ValueDisplay(const Display& d)        { valuedisplay = &d; return *this; }
	DropList&     DisplayAll(bool b = true)             { displayall = b; return *this; }
	DropList&     NoDropFocus()                         { dropfocus = false; return *this; }

	DropList();
	virtual ~DropList();
};

void Add(DropList& list, const VectorMap<Value, Value>& values);
void Add(MapConvert& convert, const VectorMap<Value, Value>& values);
void Add(DropList& list, const MapConvert& convert);

class DropPusher : public DataPusher, public Convert {
public:
	virtual Value      Format(const Value& v) const;

private:
	FrameRight<Button> drop;
	PopUpTable         popup;
	Vector<Value>      keys;

	void               OnDrop();
	void               OnSelect();
	virtual void       DoAction();

public:
	Callback           WhenDrop;
	Callback           WhenClick;

	void               Clear();
	void               Add(Value value, Value display);
	void               Add(Value value)                 { Add(value, value); }

	Value              GetValue() const;

	typedef DropPusher CLASSNAME;

	DropPusher();
	virtual ~DropPusher();
};

class DropChoice : public Pte<DropChoice> {
protected:
	FrameRight<Button> drop;
	PopUpTable         list;
	Ctrl              *owner;
	bool               appending;
	bool               dropfocus;
	bool               always_enabled;

	void        Select();
	void        Drop();

public:
	Callback    WhenDrop;
	Callback    WhenSelect;

	bool        DoKey(dword key);

	void        Clear();
	void        Add(const Value& data);
	void        Serialize(Stream& s);

	void        AddHistory(const Value& data, int max = 12);

	void        AddTo(Ctrl& _owner)                   { _owner.AddFrame(drop); owner = &_owner; }
	bool        IsActive() const                      { return drop.IsOpen(); }

	void        Show(bool b = true)                   { drop.Show(b); }
	void        Hide()                                { Show(false); }

	Value       Get() const;
	int         GetIndex() const;

	DropChoice& SetDisplay(int i, const Display& d)   { list.SetDisplay(i, 0, d); return *this; }
	DropChoice& SetDisplay(const Display& d)          { list.ColumnAt(0).SetDisplay(d); return *this; }
	DropChoice& SetLineCy(int lcy)                    { list.SetLineCy(lcy); return *this; }
	DropChoice& SetDisplay(const Display& d, int lcy) { SetDisplay(d); SetLineCy(lcy); return *this; }
	DropChoice& SetDropLines(int n)                   { list.SetDropLines(n); return *this; }
	DropChoice& Appending()                           { appending = true; return *this; }
	DropChoice& AlwaysEnabled(bool e = true);
	DropChoice& NoDropFocus()                         { dropfocus = false; return *this; }

	DropChoice();

	static bool DataSelect(Ctrl& owner, DropChoice& drop, const String& appends);
};

template <class T>
class WithDropChoice : public T {
public:
	virtual bool    Key(dword key, int repcnt);

protected:
	DropChoice      select;
	String          appends;

	void            DoWhenSelect();
	void            DoWhenDrop()                          { WhenDrop(); }

public:
	Callback        WhenDrop;
	Callback        WhenSelect;

	void            ClearList()                           { select.Clear(); }
	void            AddList(const Value& data)            { select.Add(data); }
	void            SerializeList(Stream& s)              { select.Serialize(s); }

	void            AddHistory(int max = 12)              { select.AddHistory(this->GetData(), max); }

	WithDropChoice& Dropping(bool b = true)               { select.Show(b); return *this; }
	WithDropChoice& NoDropping()                          { return Dropping(false); }
	WithDropChoice& NoDropFocus()                         { select.NoDropFocus(); return *this; }
	WithDropChoice& Appending(const String& s = ", ")     { appends = s; select.Appending(); return *this; }
	WithDropChoice& SetDropLines(int n)                   { select.SetDropLines(n); return *this; }
	WithDropChoice& SetDisplay(int i, const Display& d)   { select.SetDisplay(i, d); return *this; }
	WithDropChoice& SetDisplay(const Display& d)          { select.SetDisplay(d); return *this; }
	WithDropChoice& SetLineCy(int lcy)                    { select.SetLineCy(lcy); return *this; }
	WithDropChoice& SetDisplay(const Display& d, int lcy) { select.SetDisplay(d, lcy); return *this; }
	WithDropChoice& AlwaysEnabled(bool b = true)          { select.AlwaysEnabled(b); return *this; }

	WithDropChoice();
};

template <class T>
WithDropChoice<T>::WithDropChoice() {
	select.AddTo(*this);
	select.WhenDrop = callback(this, &WithDropChoice::DoWhenDrop);
	select.WhenSelect = callback(this, &WithDropChoice::DoWhenSelect);
	appends = String::GetVoid();
}

template <class T>
bool WithDropChoice<T>::Key(dword key, int repcnt) {
	return select.DoKey(key) || T::Key(key, repcnt);
}

template <class T>
void WithDropChoice<T>::DoWhenSelect() {
	if(DropChoice::DataSelect(*this, select, appends)) {
		this->SetFocus();
		WhenSelect();
	}
}
