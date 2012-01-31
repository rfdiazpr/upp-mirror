#include "HomeBudget.h"

HomeBudgetExport::HomeBudgetExport(const HomeBudget& hb) {
	what = 0;
	what <<= THISBACK(OnSwitch);
	OnSwitch();

	months.AddIndex();
	months.AddColumn(t_("Month")).SetConvert(Single<ConvMonth>());
	for (int i = 0; i < hb.expenses.month.GetRowCount(); i++) {
		months.AddRow(hb.expenses.month.Get(i, ID), hb.expenses.month.Get(i, DT));
	}
	months.Select(0, months.GetRowCount());

	groups.AddIndex();
	groups.AddColumn(t_("Product"));
	for (int i = 0; i < hb.categories.groups.GetRowCount(); i++) {
		groups.AddRow(hb.categories.groups.Get(i, ID), hb.categories.groups.Get(i, NAME));
	}
	groups.Select(0, groups.GetRowCount());

	CtrlLayoutOKCancel(*this, "Export");
}

void HomeBudgetExport::OnSwitch() {
	bool b = ~what;
	months.Enable(b);
	groups.Enable(b);
}

void HomeBudget::Export() {
	HomeBudgetExport dlg(*this);
	dlg.file <<= GetHomeDirFile("export.csv");
	if (dlg.ExecuteOK()) {
		SqlSet mlist, glist;
		bool all = int(~dlg.what) == 0;
		for (int i = 0; i < dlg.months.GetRowCount(); i++) {
			if (all || dlg.months.IsSelected(i))
				mlist.Cat(SqlVal(IntStr(dlg.months.Get(i, 0))));
		}
		for (int i = 0; i < dlg.groups.GetRowCount(); i++) {
			if (all || dlg.groups.IsSelected(i))
				glist.Cat(SqlVal(IntStr(dlg.groups.Get(i, 0))));
		}
		FileOut f(String(~dlg.file));
		SQL & Select(DT).From(DATES).Where(In(ID, mlist));
		while (SQL.Fetch())
			f << "D\t"
			<< Date(SQL[DT]).Get() << '\n';
		SQL & Select(NAME).From(GROUPS).Where(In(ID, glist));
		while (SQL.Fetch())
			f << "G\t"
			<< SQL[NAME] << '\n';
		SQL & Select(NAME.Of(GROUPS), NAME.Of(CATEGORIES), DEFVALUE, PM, INNEWMONTH)
		.From(CATEGORIES)
		.InnerJoin(GROUPS).On(GR_ID.Of(CATEGORIES) == ID.Of(GROUPS))
		.Where(In(GR_ID, glist));
		while (SQL.Fetch())
			f << "C\t"
			<< SQL[0] << '\t'
			<< SQL[1] << '\t'
			<< SQL[2] << '\t'
			<< SQL[3] << '\t'
			<< SQL[4] << '\n';
		SQL & Select(DT.Of(DATES), NAME.Of(GROUPS), NAME.Of(CATEGORIES),
					 PM.Of(MONEY), VALUE.Of(MONEY), DT.Of(MONEY), DESC, INST)
		.From(MONEY)
		.InnerJoin(CATEGORIES).On(CAT_ID == ID.Of(CATEGORIES))
		.InnerJoin(DATES).On(ID.Of(DATES) == DT_ID.Of(MONEY))
		.InnerJoin(GROUPS).On(ID.Of(GROUPS) == GR_ID.Of(CATEGORIES))
		.Where(In(ID.Of(DATES), mlist) && In(ID.Of(GROUPS), glist));
		while (SQL.Fetch())
			f << "M\t"
			<< Date(SQL[0]).Get() << '\t'
			<< SQL[1] << '\t'
			<< SQL[2] << '\t'
			<< SQL[3] << '\t'
			<< SQL[4] << '\t'
			<< Date(SQL[5]).Get() << '\t'
			<< SQL[6] << '\t'
			<< SQL[7] << '\n';
		f.Close();
	}
}

void HomeBudget::Import() {
	FileSelector fs;
	fs.Set(GetHomeDirectory() + "/");
	if (!fs.ExecuteOpen())
		return;
	FileIn f(fs.Get());
	try {
		while (!f.IsEof()) {
			Vector<String> line = Split(f.GetLine(), '\t');
			Date d, dd;
			switch (line[0][0]) {
				case 'D':
					d.Set(StrInt(line[1]));
					SQL & Select(ID).From(DATES).Where(DT == d);
					if (!SQL.Fetch())
						SQL & Insert(DATES)(DT, d);
					break;
				case 'G':
					SQL.Execute("SELECT GROUPS.ID FROM GROUPS WHERE GROUPS.NAME='" + line[1] + "'");
					if (!SQL.Fetch())
						SQL & Insert(GROUPS)(NAME, line[1]);
					break;
				case 'C':
					SQL.Execute("SELECT GROUPS.ID FROM CATEGORIES JOIN GROUPS ON CATEGORIES.GR_ID=GROUPS.ID WHERE GROUPS.NAME='" + line[1] + "' AND CATEGORIES.NAME='" + line[2] + "'");
					if (!SQL.Fetch()) {
						SQL & Select(ID).From(GROUPS).Where(NAME == line[1]);
						if (!SQL.Fetch())
							NEVER();
						int gid = SQL[0];
						SQL & Insert(CATEGORIES)(GR_ID, gid)
						(NAME, line[2])
						(DEFVALUE, line[3])
						(PM, line[4])
						(INNEWMONTH, line[5]);
					}
					break;
				case 'M':
					d.Set(StrInt(line[1]));
					dd.Set(StrInt(line[6]));
					SQL.Execute("SELECT MONEY.ID FROM MONEY "
								"JOIN DATES ON (MONEY.DT_ID=DATES.ID) "
								"JOIN CATEGORIES ON (CAT_ID=CATEGORIES.ID) "
								"JOIN GROUPS ON (GROUPS.ID=CATEGORIES.GR_ID) "
								"WHERE (DATES.DT=" + Format("'%i-%02i-01'", d.year, d.month) + " AND GROUPS.NAME='" + line[2] +
								"' AND CATEGORIES.NAME='" + line[3] + "' AND MONEY.PM=" + line[4] + " AND VALUE=" + line[5] + " AND DESC='" + line[7] +
								"' AND INST='" + line[8] + "' AND MONEY.DT=" + Format("'%i-%02i-%02i'", dd.year, dd.month, dd.day) + ")");
					if (!SQL.Fetch()) {
						SQL & Select(ID.Of(CATEGORIES)).From(CATEGORIES).
						InnerJoin(GROUPS).On(ID.Of(GROUPS) == GR_ID.Of(CATEGORIES)).
						Where(NAME.Of(CATEGORIES) == line[3] && NAME.Of(GROUPS) == line[2]);
						if (!SQL.Fetch())
							NEVER();
						int cid = SQL[0];
						SQL & Select(ID).From(DATES).Where(DT == d);
						if (!SQL.Fetch())
							NEVER();
						int did = SQL[0];
						SQL & Insert(MONEY)(DT_ID, did)
						(CAT_ID, cid)
						(PM, line[4])
						(VALUE, line[5])
						(DT, dd)
						(DESC, line[7])
						(INST, line[8]);
					}
					break;
				default:
					NEVER();
			}
			//reload everything
			expenses.LoadDates();
			categories.LoadGroups();
			categories.groups.GoBegin();
			categories.LoadCategories(categories.groups(ID));
			expenses.month.GoEnd();
			expenses.LoadMoney(expenses.month(ID));
		}
	} catch (SqlExc &e) {
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}
