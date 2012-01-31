#include "HomeBudget.h"

#define IMAGEFILE <HomeBudget/HomeBudget.iml>
#define IMAGECLASS Images
#include <Draw/iml_source.h>

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

#define TOPICFILE <HomeBudget/src.tpp/all.i>
#include <Core/topic_group.h>
#define TOPICFILE <HomeBudget/help.tpp/all.i>
#include <Core/topic_group.h>


HomeBudget::HomeBudget(): lang(0)
{
	inst_id = Random();
	expenses.categories = &categories;
	categories.expenses = &expenses;
}

void HomeBudget::Setup()
{
	CtrlLayout(*this, String(t_("Home budget")) + " 1.1");

	Icon(Images::SmallIcon());
	LargeIcon(Images::LargeIcon());

	expenses.Setup();
	categories.Setup();

	tabs.Add(expenses.SizePos(), t_("Expenses"));
	tabs.Add(categories.SizePos(), t_("Categories"));

	Sizeable().Zoomable();

	exit <<= THISBACK(Close);
	options <<= THISBACK(Options);
	about <<= THISBACK(About);
	help <<= THISBACK(Help);
	imp <<= THISBACK(Import);
	exp <<= THISBACK(Export);
}

void HomeBudget::Serialize(Stream &s)
{
	int version = 2;
	s / version;
	SerializePlacement(s);
	s % categories.spl;
	s % lang;
	//s % categories;
	//s % money;
	if (version > 1){
		s / inst_id;
	} else {
		try {
			SQL.Execute("alter table MONEY add INST integer;");
			SQL * ::Update(MONEY)(INST,inst_id);
		}
		catch(SqlExc &e) {
			Exclamation("[ Failed to update database:&& ][* " + DeQtfLf(e) + "]");
		}
	}
	expenses.inst_id = inst_id;
}

void HomeBudget::Options()
{
	WithOptionsLayout<TopWindow> dlg;
	CtrlLayoutOK(dlg, t_("Options"));
	dlg.lang.Add(0, "English")
	        .Add(1, "Česky")
	        .Add(2, "Polski")
	        .SetIndex(lang);
	dlg.clear <<= THISBACK(ClearAll);
	if(dlg.Execute() != IDOK)
		return;
	lang = ~dlg.lang;
}

void HomeBudget::ClearAll()
{
	try
	{
		SQL & Delete(CATEGORIES);
		SQL & Delete(GROUPS);
		SQL & Delete(MONEY);
		SQL & Delete(DATES);
		//SQL.ExecuteX("VACUUM");

		expenses.LoadDates();
		categories.LoadGroups();
		categories.UpdateCategories();
		expenses.money.Clear();
		categories.categories.Clear();
		expenses.EnableMoney();

		PromptOK(t_("Database was cleared"));
	}
	catch(SqlExc &e)
	{
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::About()
{
	WithAboutLayout<TopWindow> dlg;
	CtrlLayoutCancel(dlg, t_("About"));
	dlg.info.NoSb(false).AutoHideSb();
	Size sz = dlg.info.GetSize();
	String lng("en-us");
	switch(lang) {
	case 1: lng = "cs-cz"; break;
	case 2: lng = "pl-pl"; break;
	default: break;
	}
	dlg.info.SetQTF(GetTopic(String("HomeBudget/src/About$") + lng), Zoom(150, 1400));
	dlg.info.SetZoom(Zoom(1, 1));
	dlg.Execute();
}

void HomeBudget::Help()
{
	PromptOK(t_("No help available yet"));
/*	HomeBudgetHelp dlg;
	dlg.AddTree(0, Images::Plus(), "HomeBudget/help/Introduction$pl-pl", "Wprowadzenie");
	dlg.AddTree(0, Images::Plus(), "HomeBudget/help/T0$pl-pl", "Instrukcja");
	dlg.AddTree(0, Images::Plus(), "HomeBudget/src/About$pl-pl", "ala");
	dlg.Execute();
*/
}

HomeBudgetHelp::HomeBudgetHelp()
{
}

Topic HomeBudgetHelp::AcquireTopic(const String& topic)
{
	Topic t;
	t = GetTopic(topic);
	t.text = (const char *) GetTopic(topic);
	//String path = topic;
	//if(FileExists(path))
	//	t.text = ReadTopic(LoadFile(path)) .text;
	return t;
}

GUI_APP_MAIN
{
	bool nodb = false;
	Sqlite3Session db;
	db.LogErrors(true);

	#ifdef flagDEBUG
	db.SetTrace();
	nodb = true;
	#endif

	FileIn fi("HomeBudget.db3");
	if(fi.IsError() || fi.GetSize() <= 0)
		nodb = true;
	fi.Close();

	if(!db.Open(ConfigFile("HomeBudget.db3")))
	{
		Exclamation(t_("Can't create or open database file"));
		return;
	}

	SQL = db;

	if(nodb)
	{
		SqlSchema sch(SQLITE3);
		StdStatementExecutor se(db);
		All_Tables(sch);
		if(sch.ScriptChanged(SqlSchema::UPGRADE))
			Sqlite3PerformScript(sch.Upgrade(), se);
		if(sch.ScriptChanged(SqlSchema::ATTRIBUTES))
		{
			Sqlite3PerformScript(sch.Attributes(), se);
		}
		if(sch.ScriptChanged(SqlSchema::CONFIG))
		{
			Sqlite3PerformScript(sch.ConfigDrop(), se);
			Sqlite3PerformScript(sch.Config(), se);
		}
		sch.SaveNormal();
	}

	HomeBudget hb;

	LoadFromFile(hb);

	switch(hb.lang) {
	case 1:
		SetLanguage(LNGC_('C','S','C','Z', CHARSET_UNICODE));
		break;
	case 2:
		SetLanguage(LNGC_('P','L','P','L', CHARSET_UNICODE));
		break;
	default:
		SetLanguage(LNGC_('E','N','U','S', CHARSET_UNICODE));
		break;
	}
	hb.Setup();
	hb.Run();
	StoreToFile(hb);
}
