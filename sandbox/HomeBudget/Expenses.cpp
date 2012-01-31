#include "HomeBudget.h"

struct DispPM : GridDisplay {
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
					   Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
	{
		if (!val.IsNull()) {
			int t = int(val) < 0 ? 0 : 1;
			SetCenterImage(t ? Images::Plus : Images::Minus);
		} else
			SetCenterImage(Null);
		GridDisplay::Paint(w, x, y, cx, cy, Value(""), style, fg, bg, fnt, found, fs, fe);
	}
};

HomeBudgetExpenses::HomeBudgetExpenses(): inst_id(0) {
	money.AddIndex(ID);
	money.AddColumn(CAT_ID, t_("Category")).Edit(category).SetConvert(category);
	money.AddColumn(PM, t_("Plus / Minus")).SetDisplay(Single<DispPM>()).Edit(plusminus);
	money.AddColumn(VALUE, t_("Value")).Edit(val).Default(0).SetConvert(Single<ConvDouble>());
	money.AddColumn(DT, t_("When")).Edit(dt).Default(GetSysDate());
	money.AddColumn(DESC, t_("Describe")).Edit(es);
	money.Appending().Removing().Editing().Accepting().Canceling();
	money.RejectNullRow();
	money.WhenInsertRow = THISBACK(InsertMoney);
	money.WhenUpdateRow = THISBACK(UpdateMoney);
	money.WhenRemoveRow = THISBACK(RemoveMoney);
	money.SetToolBar();

	Date dt = GetSysDate();
	dt.day = 1;

	month.AddIndex(ID);
	month.AddColumn(DT, t_("Month")).Edit(months).SetConvert(Single<ConvMonth>()).Default(dt);
	month.Appending().Removing().Editing().Accepting().Canceling();
	month.WhenInsertRow = THISBACK(InsertDate);
	month.WhenUpdateRow = THISBACK(UpdateDate);
	month.WhenRemoveRow = THISBACK(RemoveDate);
	month.WhenChangeRow = THISBACK(ChangeDate);
	month.WhenAcceptedRow = THISBACK(AcceptedDate);
	month.WhenNewRow    = THISBACK(NewDate);
	month.SetToolBar();

	dlpm.Add(-1, t_("Minus")).Add(1, t_("Plus"));
	plusminus.Add(-1, t_("Minus")).Add(1, t_("Plus"));
	yesno.Add(0, t_("No")).Add(1, t_("Yes"));

	GenMonthList(dt.year);

	SQL * Delete(MONEY).Where(IsNull(CAT_ID));

	LoadDates();
	EnableMoney();

	mostpr.AddColumn(t_("Name"));
	mostpr.AddColumn(t_("Amount")).SetConvert(Single<ConvDouble>()).AlignCenter();
	mostcat.AddColumn(t_("Name"));
	mostcat.AddColumn(t_("Amount")).SetConvert(Single<ConvDouble>()).AlignCenter();

	tabmost.Add(mostpr.SizePos(), t_("Product"));
	tabmost.Add(mostcat.SizePos(), t_("Category"));

	if (month.GetCount() > 0) {
		month.GoEnd();
		UpdateSummary();
	}
}

void HomeBudgetExpenses::Setup() {
	category <<= THISBACK(UpdateValue);
	category.Resizeable(false).Header(false);
	category.AddPlus(callback(categories,&HomeBudgetCategories::NewCategory));
	CtrlLayout(*this);
}

void HomeBudgetExpenses::LoadMoney(int dtid) {
	money.Clear();
	SQL * Select(ID, CAT_ID, PM, VALUE, DT, DESC).From(MONEY).Where(DT_ID == dtid);
	while (SQL.Fetch()) {
		money.Add();
		money(ID) = SQL[ID];
		money(CAT_ID) = SQL[CAT_ID];
		money(PM) = SQL[PM];
		money(VALUE) = SQL[VALUE];
		money(DT) = SQL[DT];
		money(DESC) = SQL[DESC];
	}
}

void HomeBudgetExpenses::LoadDates() {
	month.Clear();
	SQL * Select(ID, DT).From(DATES);
	while (SQL.Fetch())
		month.Add(SQL);
}


void HomeBudgetExpenses::UpdateSummary() {
	if (!month.IsCursor())
		return;

	float p = 0, tp = 0;
	float m = 0, tm = 0;
	float r = 0, tr = 0;

	mostcat.Clear();
	mostpr.Clear();

	try {
		SQL & Select(PM, SqlSum(VALUE))
		.From(MONEY, DATES)
		.Where(DT.Of(DATES) < month(DT) && DT_ID == ID.Of(DATES) && NotNull(VALUE))
		.GroupBy(PM);

		while (SQL.Fetch()) {
			int pm = SQL[PM];
			float v = (float) int(SQL[1]);
			if (pm < 0)
				tm = v;
			else
				tp = v;
		}

		tr = tp - tm;

		SQL & Select(PM, SqlSum(VALUE))
		.From(MONEY)
		.Where(DT_ID == dtid && NotNull(VALUE))
		.GroupBy(PM);

		while (SQL.Fetch()) {
			int pm = SQL[PM];
			float v = (float) int(SQL[1]);
			if (pm < 0)
				m = v;
			else
				p = v;
		}

		SQL & Select(NAME, SqlId("sum(value) as val"))
		.From(MONEY, CATEGORIES)
		.Where(DT_ID == dtid && CAT_ID == ID.Of(CATEGORIES) && PM.Of(MONEY) < 0)
		.GroupBy(CAT_ID)
		.OrderBy(Descending(SqlId("val")));

		while (SQL.Fetch())
			mostpr.Add(SQL);

		SQL & Select(NAME.Of(GROUPS), SqlId("sum(value) as val"))
		.From(MONEY, GROUPS, CATEGORIES)
		.Where(DT_ID == dtid && PM.Of(MONEY) < 0 && CAT_ID == ID.Of(CATEGORIES) && GR_ID == ID.Of(GROUPS))
		.GroupBy(GR_ID)
		.OrderBy(Descending(SqlId("val")));

		while (SQL.Fetch())
			mostcat.Add(SQL);
	} catch (SqlExc &e) {
		Exclamation("[* " + DeQtfLf(e) + "]");
	}

	r = p - m;
	plus.SetText(Format("%.2f", p));
	minus.SetText(Format("%.2f", m));
	SetRest(rest, r);
	SetRest(prev_month, tr);
	SetRest(real_rest, r + tr);
}

void HomeBudgetExpenses::ClearSummary() {
	plus.SetText("");
	minus.SetText("");
	rest.SetText("");
	prev_month.SetText("");
	real_rest.SetText("");
	mostpr.Clear();
	mostcat.Clear();
}

void HomeBudgetExpenses::SetRest(StaticText &rest, float r) {
	if (r == 0) {
		rest.SetText("0.00");
		rest.SetInk(Black);
	} else
		if (r > 0) {
			rest.SetText(Format("+%.2f", r));
			rest.SetInk(Green);
		} else {
			rest.SetText(Format("%.2f", r));
			rest.SetInk(LtRed);
		}
}

void HomeBudgetExpenses::UpdateValue() {
	if (money.IsEmpty())
		return;

	try {
		SQL & Select(DEFVALUE, PM).From(CATEGORIES).Where(ID == money.Get(CAT_ID));
		Value v = SQL.Fetch() ? SQL[0] : Value(0);
		Value s = SQL.Fetch() ? SQL[1] : Value(-1);

		if (IsNull(money.Get(VALUE)))
			money.Set(VALUE, v);

		if (IsNull(money.Get(PM)))
			money.Set(PM, s);
	} catch (SqlExc &e) {
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudgetExpenses::GenMonthList(int year) {
	months.Clear();

	int curr_year = GetSysDate().year;
	int max_year = max(year,curr_year);

	for (int scan_year = min(year, curr_year) - 1; scan_year <= max_year; scan_year++)
		for (int i = 0; i < 12; i++)
			months.Add(Date(scan_year, i + 1, 1), Format("%Month %.4d", i + 1, scan_year));
}

int HomeBudgetExpenses::GetCategorySign() {
	SQL * Select(PM).From(CATEGORIES).Where(ID == money.Get(CAT_ID));
	Value v = SQL.Fetch() ? SQL[0] : Value(0);
	return v;
}

void HomeBudgetExpenses::InsertMoney() {
	try {
		SQL & Insert(MONEY)
		(DT_ID,  dtid)
		(CAT_ID, money(CAT_ID))
		(PM,     money(PM))
		(VALUE,  money(VALUE))
		(DT,     money(DT))
		(DESC,   money(DESC))
		(INST,   inst_id);

		money(ID) = SQL.GetInsertedId();

		UpdateSummary();
	} catch (SqlExc &e) {
		money.CancelInsert();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudgetExpenses::UpdateMoney() {
	try {
		SQL & ::Update(MONEY)
		(DT_ID,  dtid)
		(CAT_ID, money(CAT_ID))
		(PM,     money(PM))
		(VALUE,  money(VALUE))
		(DT,     money(DT))
		(DESC,   money(DESC))
		(INST,   inst_id)
		.Where(ID == money(ID));

		UpdateSummary();
	} catch (SqlExc &e) {
		money.CancelUpdate();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudgetExpenses::RemoveMoney() {
	try {
		SQL & Delete(MONEY).Where(ID == money(ID));
		UpdateSummary();
	} catch (SqlExc &e) {
		money.CancelRemove();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudgetExpenses::InsertDate() {
	try {
		SQL & Insert(DATES)(DT, month(DT));
		month(ID) = dtid = (int) SQL.GetInsertedId();
		EnableMoney();
	} catch (SqlExc &e) {
		month.CancelInsert();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudgetExpenses::EnableMoney(int cnt) {
	money.Enable(cnt < 0 ? month.GetCount() > 0 : cnt);
}

void HomeBudgetExpenses::UpdateDate() {
	try {
		SQL & ::Update(DATES)(DT, month(DT)).Where(ID == month(ID));
	} catch (SqlExc &e) {
		month.CancelUpdate();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudgetExpenses::RemoveDate() {
	try {
		SQL.Begin();
		SQL & Delete(MONEY).Where(DT_ID == month(ID));
		SQL & Delete(DATES).Where(ID == month(ID));
		SQL.Commit();
		bool en = month.GetCount() <= 1 ? false : true;
		EnableMoney(en);
		if (!en)
			money.Clear();
	} catch (SqlExc &e) {
		SQL.Rollback();
		month.CancelRemove();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}

}

void HomeBudgetExpenses::ChangeDate() {
	dtid = month(ID);
	LoadMoney(dtid);
	EnableMoney();
	UpdateSummary();
	GenMonthList(((Date) month(DT)).year);
}

void HomeBudgetExpenses::NewDate() {
	money.Clear();
	money.Disable();
	UpdateSummary();
}

void HomeBudgetExpenses::AcceptedDate() {
	if (!month.IsNewRow())
		return;

	Sql q;
	SQL.Begin();
	SQL * Select(ID, DEFVALUE, PM).From(CATEGORIES).Where(INNEWMONTH == 1);
	while (SQL.Fetch()) {
		money.Add();
		money(CAT_ID) = SQL[ID];
		money(VALUE) = SQL[DEFVALUE];
		money(PM) = SQL[PM];

		q * Insert(MONEY)
		(DT_ID,  dtid)
		(CAT_ID, money(CAT_ID))
		(PM,     money(PM))
		(VALUE,  money(VALUE))
		(DT,     money(DT))
		(DESC,   money(DESC));

		money(ID) = SQL.GetInsertedId();

		money.RefreshNewRow();
	}
	SQL.Commit();
	UpdateSummary();
}
