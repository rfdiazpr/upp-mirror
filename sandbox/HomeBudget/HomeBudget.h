#ifndef _HomeBudget_HomeBudget_h
#define _HomeBudget_HomeBudget_h

#include <CtrlLib/CtrlLib.h>
#include <DropGrid/DropGrid.h>
#include <GridCtrl/GridCtrl.h>
#include <plugin/sqlite3/lib/sqlite3.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define IMAGEFILE <HomeBudget/HomeBudget.iml>
#define IMAGECLASS Images
#include <Draw/iml_header.h>

#define LAYOUTFILE <HomeBudget/HomeBudget.lay>
#include <CtrlCore/lay.h>

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <HomeBudget/HomeBudget.sch>

#include <Sql/sch_header.h>

#define TFILE <HomeBudget/HomeBudget.t>
#include <Core/t.h>

struct ConvMonth : Convert
{
	Value Format(const Value &q) const
	{
		Date dt = (Date) q;
		return q.IsNull() ? "" : UPP::Format("%Month %.4d", dt.month, dt.year);
	}
};

struct ConvDouble : Convert
{
	Value Format(const Value &q) const
	{
		return q.IsNull() ? Null : UPP::Format("%.2f", q);
	}
};

class HomeBudgetHelp : public HelpWindow
{
	public:
		HomeBudgetHelp();
		virtual Topic AcquireTopic(const String& topic);
};

class HomeBudget;

class HomeBudgetExport : public WithExportLayout<TopWindow> {
	typedef HomeBudgetExport CLASSNAME;
	public:
		HomeBudgetExport(const HomeBudget& hb);
		void OnSwitch();
};

class HomeBudgetCategories;

class HomeBudgetExpenses : public WithExpensesLayout<ParentCtrl> {
	typedef HomeBudgetExpenses CLASSNAME;
	public:
		GridCtrl mostpr;
		GridCtrl mostcat;
		DropDate dt;
		DropGrid dlpm, category, months, yesno, plusminus;
		EditDouble defval, val;
		EditStringNotNull eg, ec;
		EditString es;
		int dtid;
		int inst_id;
		HomeBudgetCategories* categories;
		
		HomeBudgetExpenses();
		void Setup();

		void InsertMoney();
		void UpdateMoney();
		void RemoveMoney();

		void InsertDate();
		void UpdateDate();
		void RemoveDate();
		void ChangeDate();
		void NewDate();
		void AcceptedDate();

		void UpdateSummary();
		void ClearSummary();
		void UpdateValue();

		void LoadDates();
		void GenMonthList(int year);

		int GetCategorySign();

		void SetRest(StaticText &rest, float r);
		void LoadMoney(int did);
		void EnableMoney(int cnt = -1);
};

class HomeBudgetCategories : public ParentCtrl {
	typedef HomeBudgetCategories CLASSNAME;
	public:
		Splitter spl;
		GridCtrl groups;
		GridCtrl categories;

		DropGrid dlpm, category, months, yesno;
		EditDouble defval;
		EditStringNotNull eg, ec;
		
		HomeBudgetExpenses* expenses;

		HomeBudgetCategories();
		void Setup();
	 
		void NewGroup();
		void NewCategory();

		void InsertGroup();
		void UpdateGroup();
		void RemoveGroup();
		void ChangeGroup();

		void InsertCategory();
		void UpdateCategory();
		void RemoveCategory();
		void LoadCategories(int group_id);
		void UpdateCategories();

		void LoadGroups();	
};

class HomeBudget : public WithHomeBudgetLayout<TopWindow>
{
	public:
		typedef HomeBudget CLASSNAME;

		HomeBudgetExpenses expenses;
		HomeBudgetCategories categories;

		int lang;
		int inst_id;

		HomeBudget();

		void Serialize(Stream &s);
		void Options();
		void About();
		void Setup();
		void Export();
		void Import();

		void Help();

		void ClearAll();
};

#endif
