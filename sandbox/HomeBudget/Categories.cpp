#include "HomeBudget.h"

HomeBudgetCategories::HomeBudgetCategories() {
	Add(spl.Horz(groups, categories));
	spl.SetPos(2000);

	groups.AddIndex(ID);
	groups.AddColumn(NAME, t_("Name")).Edit(eg);
	groups.Appending().Removing().Editing().Accepting().Canceling();
	groups.RejectNullRow();
	groups.SetToolBar();
	groups.WhenInsertRow = THISBACK(InsertGroup);
	groups.WhenUpdateRow = THISBACK(UpdateGroup);
	groups.WhenRemoveRow = THISBACK(RemoveGroup);
	groups.WhenChangeRow = THISBACK(ChangeGroup);

	categories.AddIndex(ID);
	categories.AddColumn(NAME, t_("Name")).Edit(ec);
	categories.AddColumn(DEFVALUE, t_("Default value")).Edit(defval).SetConvert(Single<ConvDouble>());
	categories.AddColumn(PM, t_("Plus / Minus")).Edit(dlpm).SetConvert(dlpm).Default(-1);
	categories.AddColumn(INNEWMONTH, t_("Default for a new month")).Edit(yesno).SetConvert(yesno).Default(0);
	categories.WhenInsertRow = THISBACK(InsertCategory);
	categories.WhenUpdateRow = THISBACK(UpdateCategory);
	categories.WhenRemoveRow = THISBACK(RemoveCategory);
	categories.WhenAcceptedRow = THISBACK(UpdateCategories);
	categories.Appending().Removing().Editing();
	categories.RejectNullRow();
	categories.SetToolBar();

	dlpm.Add(-1, t_("Minus")).Add(1, t_("Plus"));
	yesno.Add(0, t_("No")).Add(1, t_("Yes"));
}

void HomeBudgetCategories::Setup() {
	LoadGroups();
	UpdateCategories();
	category <<= callback(expenses, &HomeBudgetExpenses::UpdateValue);
	category.Resizeable(false).Header(false);
	category.AddPlus(THISBACK(NewCategory));
}

void HomeBudgetCategories::LoadGroups() {
	groups.Clear();
	SQL * Select(ID, NAME).From(GROUPS);
	while (SQL.Fetch())
		groups.Add(SQL);

	if (!groups.IsEmpty())
		groups.SetCursor(0);
	else
		categories.Disable();
}

void HomeBudgetCategories::LoadCategories(int group_id) {
	categories.Clear();
	SQL * Select(ID, NAME, DEFVALUE, PM, INNEWMONTH).From(CATEGORIES).Where(GR_ID == group_id);
	while (SQL.Fetch())
		categories.Add(SQL);
}

void HomeBudgetCategories::UpdateCategories() {
	category.Clear();
	SQL & Select(ID.Of(CATEGORIES), NAME.Of(CATEGORIES), NAME.Of(GROUPS))
	.From(CATEGORIES, GROUPS)
	.Where(GR_ID == ID.Of(GROUPS))
	.OrderBy(NAME.Of(GROUPS), NAME.Of(CATEGORIES));

	while (SQL.Fetch()) {
		category.Add(SQL[0], Format("%s - %s", SQL[2], SQL[1]));
		expenses->category.Add(SQL[0], Format("%s - %s", SQL[2], SQL[1]));
	}
}

void HomeBudgetCategories::InsertGroup() {
	try {
		SQL & Insert(GROUPS)
		(NAME, groups(1));
		groups(0) = SQL.GetInsertedId();

		categories.Clear();
		categories.Enable();
	} catch (SqlExc &e) {
		groups.CancelInsert();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}

}

void HomeBudgetCategories::UpdateGroup() {
	try {
		SQL & ::Update(GROUPS)
		(NAME, groups(1))
		.Where(ID == groups(0));
	} catch (SqlExc &e) {
		groups.CancelUpdate();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudgetCategories::RemoveGroup() {
	if (categories.IsEmpty()) {
		SQL & Delete(GROUPS).Where(ID == groups(ID));

		if (groups.GetCount() == 1)
			categories.Disable();
	} else {
		PromptOK(t_("You can't remove this group. It is not empty."));
		groups.CancelRemove();
	}
}

struct AddNewCat : WithNewCategoryLayout<TopWindow> {
	typedef AddNewCat CLASSNAME;
	bool newgroup;

	AddNewCat() {
		newgroup = false;
		CtrlLayoutOKCancel(*this, t_("New item"));
		addgroup.SetImage(Images::SmallPlus());
		addgroup <<= THISBACK(NewGroup);
		groups <<= THISBACK(SelGroup);
		addgroup.NoWantFocus();

		int cnt = 0;
		SQL * Select(ID, NAME).From(GROUPS);
		while (SQL.Fetch()) {
			groups.Add(SQL[ID], SQL[NAME]);
			cnt++;
		}

		bool isgroup = cnt > 0;
		if (isgroup)
			groups.SetIndex(0);
		else
			ok.Disable();

		pm = 1;
	}

	void SelGroup() {
		ok.Enable();
	}

	void NewGroup() {
		WithNewGroupLayout<TopWindow> dlg;
		CtrlLayoutOKCancel(dlg, t_("New category"));
		if (dlg.Execute() == IDOK) {
			try {
				SQL & Insert(GROUPS)(NAME, ~dlg.name);
				int64 id = SQL.GetInsertedId();
				groups.Add(id, ~dlg.name);
				groups <<= id;
				ok.Enable();
				newgroup = true;
			} catch (SqlExc &e) {
				Exclamation("[* " + DeQtfLf(e) + "]");
			}
		}
	}
	bool IsNewGroup() {
		return newgroup;
	}
};

void HomeBudgetCategories::NewCategory() {
	AddNewCat dlg;

	if (dlg.Execute() == IDOK) {
		try {
			SQL & Insert(CATEGORIES)
			(GR_ID, ~dlg.groups)
			(NAME, ~dlg.name)
			(DEFVALUE, 0)
			(PM, dlg.pm == 0 ? 1 : -1)
			(INNEWMONTH, 0);

			int64 id = SQL.GetInsertedId();
			UpdateCategories();
			expenses->money.Set(CAT_ID, id);
			expenses->UpdateValue();

			int gid = ~dlg.groups;

			if (dlg.IsNewGroup()) {
				groups.Add(Value(gid), dlg.groups.GetValue());
				groups.GoEnd();
				categories.Enable();
			}
			int c = groups.GetCursor();
			if (c >= 0 && gid == groups(c, 0))
				LoadCategories(gid);
		} catch (SqlExc &e) {
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
}

void HomeBudgetCategories::InsertCategory() {
	try {
		SQL & Insert(CATEGORIES)
		(GR_ID, groups(ID))
		(NAME, categories(NAME))
		(DEFVALUE, categories(DEFVALUE))
		(PM, categories(PM))
		(INNEWMONTH, categories(INNEWMONTH));

		categories(0) = SQL.GetInsertedId();
	} catch (SqlExc &e) {
		categories.CancelInsert();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}

}

void HomeBudgetCategories::UpdateCategory() {
	try {
		SQL & ::Update(CATEGORIES)
		(NAME, categories(NAME))
		(DEFVALUE, categories(DEFVALUE))
		(PM, categories(PM))
		(INNEWMONTH, categories(INNEWMONTH))
		.Where(ID == categories(ID));

		expenses->UpdateSummary();
	} catch (SqlExc &e) {
		categories.CancelUpdate();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudgetCategories::ChangeGroup() {
	LoadCategories(groups(ID));
}

void HomeBudgetCategories::RemoveCategory() {
	try {
		SQL * Select(CAT_ID, NAME)
		.From(MONEY, CATEGORIES)
		.Where(CAT_ID == categories(ID) && ID.Of(CATEGORIES) == categories(ID))
		.Limit(1);

		if (SQL.Fetch()) {
			PromptOK(Format(t_("Item '%s' can't be removed. It is used in calculations."), AsString(SQL[1])));
			categories.CancelRemove();
		} else {
			SQL & Delete(CATEGORIES).Where(ID == categories(ID));
			UpdateCategories();
		}
	} catch (SqlExc &e) {
		categories.CancelRemove();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}
