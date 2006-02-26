#include "TCore.h"
#pragma hdrstop
#include "dbf.h"

#define LLOG(x) // LOG(x)

#define DO_DBF_TIMING 0

#if DO_DBF_TIMING
	#define DBF_TIMING(x) RTIMING(x)
#else
	#define DBF_TIMING(x)
#endif

static int scan_int(const char *p, const char *e = 0)
{
	DBF_TIMING("scan_int");
	bool neg = false;
	if(p != e && (*p == '+' || *p == '-'))
		neg = (*p++ == '-');
	if(p == e || (byte)(*p - '0') >= 10)
		return Null;
	int val = *p++ - '0';
	while(p != e && (byte)(*p - '0') < 10)
		val = 10 * val + *p++ - '0';
	return neg ? -val : val;
}

static double scan_dbl(const char *p, const char *e = 0)
{
	DBF_TIMING("scan_dbl");
	bool neg = false;
	if(p != e && (*p == '+' || *p == '-'))
		neg = (*p++ == '-');
	if(p == e || (byte)(*p - '0') >= 10)
		return Null;
	double mantissa = 0;
	char c;
	int exp = 0;
	while(p != e && (byte)(*p - '0') < 10)
		if((c = *p++) != '0') {
			if(exp) { mantissa *= ipow10(exp); exp = 0; }
			mantissa = 10 * mantissa + c - '0';
		}
		else
			exp++;
	int raise = exp;
	if(p != e && *p == '.') // decimal part
		while(++p != e && (byte)((c = *p) - '0') < 10)
			if(c != '0') {
				if(raise) { mantissa *= ipow10(raise); exp -= raise; raise = 0; }
				exp--;
				mantissa = 10 * mantissa + c - '0';
			}
			else
				raise++;
	if(p != e && (*p == 'E' || *p == 'e')) { // exponent
		int vexp = scan_int(p + 1, e);
		if(!IsNull(vexp))
			exp += vexp;
	}
	if(exp) {
		double e = ipow10(tabs(exp));
		mantissa = (exp > 0 ? mantissa * e : mantissa / e);
	}
	return neg ? -mantissa : mantissa;
}

static int StrBool(char c)
{
	if(c == 'A' || c == 'a' || c == 'Y' || c == 'y' || c == 'T' || c == 't' || c == '1')
		return 1;
	if(c == 'N' || c == 'n' || c == 'F' || c == 'f' || c == '0')
		return 0;
	return (int)Null;
}

DbfStream::Field::Field(const char *_name, char type, byte width, byte decimal)
: type(type), width(width), decimal(decimal), offset(0)
{
	if(_name) {
		const char *p = reinterpret_cast<const char *>(memchr(_name, 0, 11));
		name = String(_name, p ? p - _name : 11);
	}
	name = ToUpper(name);
}

String DbfStream::Field::Format(Value value) const
{
	switch(type) {
	case 'C':
		return StdFormat(value);

	case 'N': {
			double v = value;
			if(IsNull(v))
				return Null;
			String out = ::Format("%.*f", decimal, v);
			int delta = width - out.GetLength();
			if(delta > 0)
				out = String(' ', delta) + out;
			else if(delta < 0) { // too long - convert to virtual infinity
				int whole = width - decimal - (decimal > 0);
				out = Null;
				if(v < 0) {
					out << '-';
					whole--;
				}
				if(whole > 0)
					out.Cat('9', whole);
				if(decimal) {
					out.Cat('.');
					out.Cat('9', decimal);
				}
			}
			return out;
		}

	case 'D': {
			::Date dt = value;
			if(IsNull(dt))
				return Null;
			char p[9];
			p[8] = 0;
			p[7] = dt.day % 10 + '0';
			p[6] = dt.day / 10 + '0';
			p[5] = dt.month % 10 + '0';
			p[4] = dt.month / 10 + '0';
			int y = dt.year;
			p[3] = y % 10 + '0'; y /= 10;
			p[2] = y % 10 + '0'; y /= 10;
			p[1] = y % 10 + '0';
			p[0] = y / 10 + '0';
			return String(p, 8);
		}

	case 'L': {
			int cond = value;
			return cond && !IsNull(cond) ? "A" : "N";
		}

	default:
		NEVER();
		return Null;
	}
}

DbfStream::Field DbfStream::Field::Text(const char *name, int width)
{
	ASSERT(width >= 1 && width <= 255);
	return Field(name, 'C', width);
}

DbfStream::Field DbfStream::Field::Number(const char *name, int width, int decimals)
{
	ASSERT(width >= 1 && width <= 31);
	ASSERT(decimals >= 0 && decimals <= width - 1);
	return Field(name, 'N', width, decimals);
}

DbfStream::Field DbfStream::Field::Date(const char *name)
{
	return Field(name, 'D', 8);
}

DbfStream::Field DbfStream::Field::Logical(const char *name)
{
	return Field(name, 'L', 1);
}

int DbfStream::Field::GetType() const
{
	switch(type) {
	case 'N': return (width <= 9 && decimal == 0 ? INT_V : DOUBLE_V);
	case 'M':
	case 'C': return STRING_V;
	case 'D': return DATE_V;
	case 'L': return INT_V;
	default:  return VOID_V;
	}
}

void DbfStream::Field::Serialize(Stream& stream)
{
	char buffer[12];
	ZeroArray(buffer);
	if(stream.IsStoring())
		memcpy(buffer, name, min(name.GetLength(), 11));
	stream.SerializeRaw((byte *)buffer, 11);
	if(stream.IsLoading()) {
		name = buffer;
		name = ToUpper(name);
	}
	stream % type;
	StreamIL(stream, offset);
	stream % width % decimal;
	if(stream.IsStoring())
		stream.Put(0, 14);
	else
		stream.SeekCur(14);
}

DbfStream::DbfStream()
{
}

DbfStream::DbfStream(const char *_file, bool write, byte _charset, bool _delete_share)
{
	Open(_file, write, _charset, _delete_share);
}

bool DbfStream::Check(const char *filename, bool write)
{
	return DbfStream().Open(filename, write, CHARSET_DEFAULT);
}

void DbfStream::SetCharset(byte cs)
{
	charset = cs;
	byte dest = GetDefaultCharset();
	codepage_cv = false;
	for(int i = 0; i < 256; i++) {
		codepage_uni[i] = ToUnicode(i, charset);
		codepage_map[i] = FromUnicode(codepage_uni[i], dest);
		codepage_rev[i] = FromUnicode(ToUnicode(i, dest), charset);
		if(codepage_map[i] != i)
			codepage_cv = true;
	}
}

bool DbfStream::Open(const char *file, bool write, byte _charset, bool _delete_share)
{
	Close();
	LLOG("DbfStream::Open(" << file << ", write = " << write << ", charset = " << (int)_charset << ")");
	if(!dbf.Open(file, (write ? FileStream::READWRITE : FileStream::READ)
	| (_delete_share ? FileStream::DELETESHARE : 0))) {
		LLOG("-> open error");
		return false;
	}
	if(dbt.Open(ForceExt(file, ".dbt"),
		FileStream::READ | (_delete_share ? FileStream::DELETESHARE : 0))) // writing MEMO's is not supported yet
		dbt.SetLoading();
	dbf.SetLoading();
	if(DoOpen(_charset))
		return true;
	LLOG("-> DoOpen error");
	Close();
	return false;
}

bool DbfStream::Create(const char *filename, const Array<Field>& _fields, byte _charset, bool _delete_share)
{
	Close();
	if(!dbf.Open(filename, FileStream::CREATE | (_delete_share ? FileStream::DELETESHARE : 0)))
		return false;
	if(DoCreate(_fields, _charset))
		return true;
	Close();
	return false;
}

static inline bool IsValidType(char t)
{
	return t == 'C' || t == 'N' || t == 'D' || t == 'L' || t == 'M';
}

bool DbfStream::StreamHeader(bool full)
{
	ASSERT(dbf.IsOpen());
	data_offset = 32 + fields.GetCount() * 32 + 1;
	if(dbf.IsStoring()) {
		ASSERT(!IsReadOnly());
		dbf.Put(0x03); // #0: version number - 03 without a DBT file
		Date date = GetSysDate();
		dbf.Put(date.year - 1900); // #1 - 3: date of last update
		dbf.Put(date.month);
		dbf.Put(date.day);
	}
	else
		dbf.Get32le();
	StreamIL(dbf, rows);
	if(!full)
		return true;
	StreamIW(dbf, data_offset);
	int row_width = record.GetCount();
	if(dbf.IsStoring() && row_width == 0) { // calculate row width
		row_width = 1;
		for(int i = 0; i < fields.GetCount(); i++)
			row_width += fields[i].width;
	}
	StreamIW(dbf, row_width);
	if(dbf.IsStoring()) {
	//	dbf.PutIL(lang_code);
		dbf.Put(0, 20); // padding bytes
	}
	else
		dbf.SeekCur(20);
	int field_count = fields.GetCount();
	if(dbf.IsLoading()) {
		if(rows < 0)
			return false;
		if(data_offset <= 32) // || (data_offset & 0x1F) != 1)
			return false; // invalid header size
		if(dbf.GetSize() < (unsigned)data_offset)
			return false; // incomplete file header
		int rec_count = (int)((dbf.GetSize() - data_offset) / row_width);
		rows = min(rows, rec_count); // physical record #
		field_count = (data_offset >> 5) - 1;
		fields.Clear();
		has_memo = false;
	}
	int offset = 1;
	Index<String> used_names;
//	const byte *uascii = NlsUpperAscii();
	byte fld_charset = CHARSET_WIN1250; //!!
	for(int i = 0; i < field_count && offset < row_width; i++) {
		Field field;
		if(dbf.IsStoring()) {
			fields[i].offset = offset;
			field = fields[i];
		}
		field.Serialize(dbf);
		if(dbf.IsError())
			return false;
		if(dbf.IsLoading()) {
			if(field.type == 0)
				break;
			String outname;
			for(const char *p = field.name; *p; p++) {
				char c = ToUpper(ToAscii(*p, fld_charset));
				if(IsAlNum(c) || c == '_')
					outname.Cat(c);
			}
			if(outname.IsEmpty() || IsDigit(*outname))
				outname.Insert(0, 'F');
			if(used_names.Find(outname) >= 0) {
				int i = 0;
				while(used_names.Find(outname + '_' + IntStr(++i)) >= 0)
					;
				outname << '_' << i;
			}
			used_names.Add(field.name = outname);
		}
		if(dbf.IsLoading() && field.name != "_DBASELOCK" && IsValidType(field.type)) {
			field.offset = offset;
			fields.Add(field.name, field);
			if(field.type == 'M')
				has_memo = true;
		}
		offset += field.width;
	}
	byte b = 0x0D;
	dbf % b;
	ASSERT(dbf.GetPos() <= (unsigned)data_offset);
	record.SetCount(row_width);
	memset(record.Begin(), ' ', record.GetCount());
	if(dbf.IsError())
		return false;
	if(has_memo && dbt.IsOpen() && dbt.IsLoading()) { // read DBT header
		dbt.Seek(16);
		int b = dbt.Get();
		dbt_block_size = 512;
		if(b != 3) {
			dbt.Seek(20);
			dbt_block_size = (short)dbt.Get16le();
			if(dbt_block_size <= 0)
				has_memo = false;
		}
	}
	return true;
}

bool DbfStream::DoOpen(byte _charset)
{
	SetCharset(_charset);
	dirty = false;
	dirty_header = false;
	row_index = Null;
	next_row_index = 0;
	if(!StreamHeader(true))
		return false;
	DoFields();
	return true;
}

bool DbfStream::DoCreate(const Array<Field>& _fields, byte _charset)
{
	SetCharset(_charset);
	rows = 0;
	dirty = false;
	dirty_header = false;
	row_index = 0;
	next_row_index = 0;
	fields.Clear();
	for(int i = 0; i < _fields.GetCount(); i++)
		fields.Add(_fields[i].name, _fields[i]);
	if(!StreamHeader(true))
		return false;
	DoFields();
	return true;
}

void DbfStream::DoFields()
{
	has_memo = false;
	field_read.Clear();
	field_read.SetCount(fields.GetCount(), &DbfStream::GetItemEmpty);
	for(int i = 0; i < fields.GetCount(); i++) {
		const Field& fld = fields[i];
		switch(fld.type) {
		case 'C': field_read[i] = &DbfStream::GetItemString; break;
		case 'N': field_read[i] = (fld.width < 10 && fld.decimal == 0 ? &DbfStream::GetItemInt : &DbfStream::GetItemDouble); break;
		case 'D': field_read[i] = (fld.width < 10 ? &DbfStream::GetItemDateShort : &DbfStream::GetItemDateLong); break;
		case 'L': field_read[i] = &DbfStream::GetItemLogical; break;
		case 'M': field_read[i] = &DbfStream::GetItemMemo; break;
		}
	}
}

void DbfStream::SetRowCount(int count)
{
	if(rows > count) // trim file
		dbf.SetSize(data_offset + count * record.GetCount());
	if(count > rows) { // extend file
		dbf.Seek(data_offset + rows * record.GetCount());
		dbf.Put(' ', (count - rows) * record.GetCount());
	}
	rows = count;
}

void DbfStream::Flush()
{
	FlushRow();
	FlushHeader();
}

void DbfStream::FlushRow()
{
	ASSERT(IsOpen());
	if(dirty) {
		ASSERT(!IsReadOnly());
		ASSERT(row_index >= 0);
		if(row_index > rows) {
			dbf.Seek(data_offset + rows * record.GetCount());
			dbf.Put(' ', (row_index - rows) * record.GetCount());
		}
		else
			dbf.Seek(data_offset + row_index * record.GetCount());
		dbf.Put(record.Begin(), record.GetCount());
		if(row_index >= rows) {
			rows = row_index + 1;
			dirty_header = true;
		}
		dirty = false;
	}
}

void DbfStream::FlushHeader()
{
	ASSERT(IsOpen());
	if(dirty_header) {
		ASSERT(!IsReadOnly());
		dbf.Seek(0);
		dbf.SetStoring();
		StreamHeader(false);
		dirty_header = false;
	}
}

bool DbfStream::Close()
{
	if(IsOpen())
		Flush();
	dbf.Close();
	dbt.Close();
	return !dbf.IsError() && (!has_memo || !dbt.IsError());
}

bool DbfStream::Fetch(int row)
{
	DBF_TIMING("DbfStream::Fetch");
	ASSERT(IsOpen());
	if(dirty)
		FlushRow();
	next_row_index = row + 1;
	if(row == row_index)
		return *record;
	row_index = row;
	if(row < 0 || row >= rows) {
		*record.Begin() = 0;
		return false;
	}

	dbf.Seek(data_offset + row * record.GetCount());
	byte *p = record.Begin(), *e = record.End();
	int count = dbf.Get(p, e - p);
	if(count < record.GetCount())
		memset(p + count, ' ', record.GetCount() - count);
	if(count <= 0 || (*p != ' ' && *p != 0)) {
		*p = 0;
		return false;
	}
	*p = ' ';
	if(codepage_cv)
		for(const char *page = codepage_map; ++p < e; *p = page[*p])
			;
	dirty = false;
	return true;
}

void DbfStream::Seek(int row)
{
	if(row != row_index) {
		FlushRow();
		next_row_index = row;
	}
}

bool DbfStream::Fetch()
{
	int r = next_row_index;
	for(; r < rows; r++)
		if(Fetch(r))
			return true;
	return false;
}

//	void            GetItem(int i, String& s) const;
//	void            GetItem(int i, int& n) const;
//	void            GetItem(int i, double& d) const;
//	void            GetItem(int i, Date& d) const;
//	void            GetItem(int i, Time& t) const;
//	void            GetItem(int i, Ref f) const;

/*
void DbfStream::GetItem(int i, String& s) const
{
	DBF_TIMING("DbfStream::GetItem(String)");
	const Field& fld = fields[i];
	s = Null;
	if(record.GetCount() > fld.offset) {
		const byte* p = record.Begin() + fld.offset;
		const byte* e = record.Begin() + min(fld.offset + fld.width, record.GetCount());
		while(p < e && *p == ' ')
			p++;
		while(e > p && e[-1] == ' ')
			e--;
		if(e > p)
			s = String(reinterpret_cast<const char *>(p), reinterpret_cast<const char *>(e));
	}
}
*/

/*
void DbfStream::GetItem(int i, int& n) const
{
	DBF_TIMING("DbfStream::GetItem(int)");
	n = GetItem(i);
}
*/

/*
void DbfStream::GetItem(int i, double& d) const
{
	DBF_TIMING("DbfStream::GetItem(double)");
	d = GetItem(i);
}
*/

/*
void DbfStream::GetItem(int i, Date& d) const
{
	DBF_TIMING("DbfStream::GetItem(Date)");
	d = GetItem(i);
}
*/

/*
void DbfStream::GetItem(int i, Time& t) const
{
	DBF_TIMING("DbfStream::GetItem(Time)");
	t = GetItem(i);
}
*/

/*
void DbfStream::GetItem(int i, Ref f) const
{
	switch(f.GetType()) {
	case STRING_V: GetItem(i, RefString(f)); break;
	case INT_V:    GetItem(i, RefInt(f));    break;
	case DOUBLE_V: GetItem(i, RefDouble(f)); break;
	case DATE_V:   GetItem(i, RefDate(f));   break;
	case TIME_V:   GetItem(i, RefTime(f));   break;
	default:       f.SetValue(GetItem(i));   break;
	}
}
*/

Value DbfStream::GetItem(int i) const
{
	DBF_TIMING("DbfStream::GetItem");
	ASSERT(i >= 0 && i < fields.GetCount());
	return (this ->* (field_read[i].proc))(i);
}

void DbfStream::PutItem(int i, Value value)
{
	const Field& fld = fields[i];
	dirty = true;
	byte *p = record.Begin() + fld.offset;
	if(IsNull(value)) // empty value is the same for all data types
		memset(p, ' ', fld.width);
	else {
		String s = fld.Format(value);
		int l = min<int>(s.GetLength(), fld.width);
		if(!codepage_cv)
			memcpy(p, s, l);
		else {
			byte *d = p;
			for(const byte *f = s, *e = f + l; f < e; *d++ = codepage_rev[*f++])
				;
		}
		if(l < fld.width)
			memset(p + l, ' ', fld.width - l);
	}
}

void DbfStream::PutItem(String n, Value value)
{
	int i = GetFieldIndex(n);
	if(i >= 0)
		PutItem(i, value);
	else
		NEVER(); // column missing
}

Value DbfStream::GetItem(String name) const
{
	int index = GetFieldIndex(name);
	if(index >= 0)
		return GetItem(index);
	else {
		NEVER();
		return ErrorValue(NFormat("column '%s' not found", name));
	}
}

Vector<Value> DbfStream::FetchRow()
{
	if(!Fetch())
		return Vector<Value>();
	return FetchRow(row_index);
}

Vector<Value> DbfStream::FetchRow(int row)
{
	DBF_TIMING("DbfStream::FetchRow");
	Vector<Value> values;
	if(!Fetch(row))
		return values;
	values.SetCount(fields.GetCount());
	for(int i = 0; i < fields.GetCount(); i++)
		values[i] = (this ->* (field_read[i].proc))(i);
	return values;
}

void DbfStream::WriteRow(int row, const Vector<Value>& values)
{
	ASSERT(row >= 0);
	FlushRow();
	if(values.GetCount() < fields.GetCount())
		Fetch(row);
	else {
		row_index = row;
		next_row_index = row_index + 1;
	}
	for(int i = 0, n = min(values.GetCount(), fields.GetCount()); i < n; i++)
		PutItem(i, values[i]);
}

void DbfStream::WriteRow(const Vector<Value>& values)
{
	WriteRow(next_row_index, values);
	Seek(row_index + 1);
}

String DbfStream::ToString() const
{
	String result;
	result
		<< fields.GetCount() << " columns\n"
		<< rows << " rows\n\nColumns:\n";

	for(int i = 0; i < GetFieldCount(); i++) {
		const Field& field = GetField(i);
		result = Format("%2i: %-12s '%c' %d", i, ~field.name, field.type, field.width);
		if(field.decimal)
			result << '.' << field.decimal;
		result << '\n';
	}
	return result << '\n';
}

void DbfStream::DumpData(Stream& stream)
{
	stream << *this; // dump layout

	// dump column headers
	stream << "ROWID ";
	int total = 6;
	Vector<int> fw;

	int i;
	for(i = 0; i < fields.GetCount(); i++) {
		const Field& field = GetField(i);
		int width = max<int>(field.width, strlen(field.name));
		switch(field.type) {
		case 'D': width = max(width, 10); break;
		}
		++width;
		stream << NFormat("%*<s", width, field.name);
		fw.Add() = width;
		total += width;
	}
	stream << "\n-";
	stream.Put('-', total - 2);
	stream << '\n';

	for(i = 0; i < rows; i++) {
		stream << NFormat("%5d ", i);
 		if(Fetch(i)) { // display individual columns
			for(int j = 0; j < fields.GetCount(); j++)
				stream << NFormat("%*<vt", fw[j], GetItem(j));
		}
		stream << '\n';
	}
	stream << "\n\n";
}

Value DbfStream::GetItemInt(int i) const
{
	const byte *p = record.Begin() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	return scan_int((const char *)p, (const char *)e);
}

Value DbfStream::GetItemDouble(int i) const
{
	const byte *p = record.Begin() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	return scan_dbl((const char *)p, (const char *)e);
}

Value DbfStream::GetItemString(int i) const
{
	const byte *p = record.Begin() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	while(e > p && e[-1] == ' ')
		e--;
	return String(p, e - p);
}

Value DbfStream::GetItemDateShort(int i) const
{
	const byte *p = record.Begin() + fields[i].offset;
	Date date;
	if(IsDigit(*p))
		if(p[2] == '.') {
			date.day = 10 * p[0] + p[1] - 11 * '0';
			date.month = 10 * p[3] + p[4] - 11 * '0';
			date.year = 10 * p[6] + p[7] + 1900 - 11 * '0';
		}
		else {
			date.year = 1000 * p[0] + 100 * p[1] + 10 * p[2] + p[3] - 1111 * '0';
			date.month = 10 * p[4] + p[5] - 11 * '0';
			date.day = 10 * p[6] + p[7] - 11 * '0';
		}
	return date;
}

Value DbfStream::GetItemDateLong(int i) const
{
	const byte *p = record.Begin() + fields[i].offset;
	if(IsDigit(*p) && !IsDigit(p[2])) {
		Date date;
		date.day = 10 * p[0] + p[1] - 11 * '0';
		date.month = 10 * p[3] + p[4] - 11 * '0';
		date.year = 1000 * p[6] + 100 * p[7] + 10 * p[8] + p[9] - 1111 * '0';
		return date;
	}
	else
		return GetItemDateShort(i);
}

Value DbfStream::GetItemLogical(int i) const
{
	return StrBool(record[fields[i].offset]);
}

Value DbfStream::GetItemMemo(int i) const
{
	if(!has_memo)
		return false;
	const byte *p = record.Begin() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	int block = scan_int((const char *)p, (const char *)e);
	if(dbt.IsOpen() && !IsNull(block) && block > 0) {
		unsigned pos = block * dbt_block_size;
		if(pos >= dbt.GetSize())
			return Value();
		dbt.Seek(pos);
		if(dbt.GetIL() == 0x8FFFF) { // dBASE IV memo with explicit length
			unsigned len = dbt.GetIL();
			if(len <= 0 || len > dbt.GetLeft())
				return Value();
			Buffer<byte> buffer(len);
			if(!dbt.GetAll(buffer, len))
				return Value();
			byte *p;
			if(p = (byte *)memchr(buffer, '\0', len)) len = p - buffer;
			if(p = (byte *)memchr(buffer, '\x1A', len)) len = p - buffer;
			if(codepage_cv) {
				byte *p = buffer, *e = p + len;
				for(const char *page = codepage_map; p != e; p++)
					*p = page[*p];
			}
			return String(buffer, len);
		}
		else { // dBASE III memo with 1A1A separator
			String out;
			Buffer<byte> buffer(dbt_block_size + 1);
			bool eof = false;
			for(int b; !eof && (b = dbt.Get(buffer, dbt_block_size + 1)) > 0;) {
				byte *p = buffer, *e = p + b;
				if(codepage_cv) {
					for(const char *page = codepage_map; p <= e; p++)
						if((*p = page[*p]) == '\x1A') {
							if(++p == e)
								break;
							if((*p = page[*p]) == '\x1A') {
								e = p - 1;
								eof = true;
								break;
							}
						}
				}
				else {
					while((p = (byte *)memchr(p, '\x1A', e - p)) != 0)
						if(++p < e && *p++ == '\x1A') { // double EOF found
							e = p - 2;
							eof = true;
							break;
						}
				}
				int l = e - buffer;
				if(!eof && l > dbt_block_size) {
					dbt.SeekCur(dbt_block_size - l);
					e = buffer + l;
				}
				if(l > 0)
					out.Cat(buffer, l);
			}

			return out;
		}
	}
	return Value();
}

Value DbfStream::GetItemEmpty(int i) const
{
	return Value();
}
