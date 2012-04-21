#include <Core/Core.h>
#include <NearMatchCompare/NearMatchCompare.hpp>

using namespace Upp;


#define TEST(A, B)  RLOG( String( "Compare ") << A << "  -  " << B << "\t==> " <<  (NearMatchCompare::isNearMatch(String(A), String(B))==true?"MATCH":"-----")\
                   << "    correlation=" << NearMatchCompare::correlation(String(A), String(B))\
                   <<  "        MAXa=" << NearMatchCompare::correlation(String(A), String(A))\
                   << "     MAXb=" << NearMatchCompare::correlation(String(B), String(B))\
                   );





void doCompare(Vector<String>& list)
{
	Vector<String>::Iterator iter = list.Begin();
	Vector<String>::Iterator iter2 = list.Begin();
	Vector<String>::Iterator endIter = list.End();
	while( iter != endIter )
	{
		iter2 = iter;
		//++iter2;
		while(iter2 != endIter)
		{
			if (*iter!=*iter2)
			if (NearMatchCompare::isNearMatch(*iter, *iter2))
			{
				RLOG(String("NEAR MATCH :   ") << *iter << " <=> " << *iter2 << "\t\tcorr = " << NearMatchCompare::correlation(*iter, *iter2)
							<<   "        MAXa=" << NearMatchCompare::correlation(*iter, *iter) << "     MAXb=" << NearMatchCompare::correlation(*iter2, *iter2) );
			}
			++iter2;
		}
		++iter;
	}
}

CONSOLE_APP_MAIN
{
	RLOG("\n---------------------------------------------------------------------");
	RLOG("\n--- 1 CARACTERE ---");
	TEST("1", "1");
	TEST("1", "2");

	RLOG("\n---------------------------------------------------------------------");
	RLOG("\n--- 2 CARACTERES ---");
	TEST("12", "12");
	TEST("12", "123");
	TEST("12", "11");
	TEST("12", "22");
	TEST("12", "33");
	TEST("12", "21");
	
	
	RLOG("\n---------------------------------------------------------------------");
	RLOG("\n--- 3 CARACTERES ---");
	TEST("123", "123");
	TEST("123", "122");
	TEST("123", "124");
	TEST("123", "143");
	TEST("123", "423");
	TEST("123", "223");
	TEST("123", "113");
	TEST("123", "133");
	TEST("123", "0123");
	TEST("123", "1023");
	TEST("123", "1203");
	TEST("123", "1230");
	TEST("123", "12345");
	TEST("123", "01234");
	TEST("123", "00123");
	TEST("123", "30012");
	TEST("123", "23001");
	TEST("123", "12300");

	RLOG("\n---------------------------------------------------------------------");
	RLOG("\n--- 4 CARACTERES ---");
	TEST("1234", "1234");
	TEST("1234", "0123");
	TEST("1234", "3012");
	TEST("1234", "2301");
	TEST("1234", "1032");
	TEST("1234", "1204");
	TEST("1234", "1224");
	TEST("1234", "1223");
	TEST("1234", "1244");
	TEST("1234", "1123");
	TEST("1234", "1134");
	TEST("1234", "4321");
	TEST("1234", "4432");
	TEST("1234", "4431");
	TEST("1234", "4324");


	RLOG("\n---------------------------------------------------------------------");
	RLOG("\n--- 5 CARACTERES ---");
	TEST("12345", "12345");
	TEST("12345", "12344");
	TEST("12345", "41234");
	TEST("12345", "12034");
	TEST("12345", "12045");
	TEST("12345", "12054");
	TEST("12345", "21045");
	TEST("12345", "21054");
	TEST("12345", "54021");
	
	Vector<String> listNoms;
	Vector<String> listPrenoms;

	listNoms << "GUILLOU"	;listPrenoms << "GHISLAIN";
	listNoms << "MEFORT"	;listPrenoms << "LOIC";
	listNoms << "JEZEQUEL"	;listPrenoms << "NICOLAS";
	listNoms << "LE	CAM"	;listPrenoms << "RAPHAEL";
	listNoms << "HERPE"		;listPrenoms << "LUDOVIC";
	listNoms << "RIGAULT"	;listPrenoms << "JEAN-PIERRE";
	listNoms << "VALLEE"	;listPrenoms << "FABRICE";
	listNoms << "HERVEIC"	;listPrenoms << "ERWAN";
	listNoms << "FROMONT"	;listPrenoms << "NICOLAS";
	listNoms << "GERAULT"	;listPrenoms << "ERIC";
	listNoms << "TALARMIN"	;listPrenoms << "FRANCOIS";
	listNoms << "BUNEL"		;listPrenoms << "PHILIPPE";
	listNoms << "LESCOP"	;listPrenoms << "LAURENT";
	listNoms << "LE PIMPEC"	;listPrenoms << "SYLVIANNE";
	listNoms << "CONGOS"	;listPrenoms << "JEAN-PAUL";
	listNoms << "JUGES"		;listPrenoms << "JEAN-PIERRE";
	listNoms << "LASSELLETTE";listPrenoms << "JEAN-PHILIPPE";
	listNoms << "BARONE"	;listPrenoms << "PATRICK";
	listNoms << "GOUILLEUX"	;listPrenoms << "BENOIT";
	listNoms << "RONDONNEAU";listPrenoms << "PATRICK";
	listNoms << "BARONNE"	;listPrenoms << "PATRICK";
	listNoms << "BARRONE"	;listPrenoms << "PATRICK";
	listNoms << "VUGNON"	;listPrenoms << "CEDRIC";
	listNoms << "BOURGUIGNON";listPrenoms << "FLORENT";
	listNoms << "FOUILLON"	;listPrenoms << "LAURENT";
	listNoms << "CRUCHELAR"	;listPrenoms << "GERARD";
	listNoms << "RIGAL"		;listPrenoms << "EMANUEL";
	listNoms << "MOREAU"	;listPrenoms << "STEPHANE";
	listNoms << "NICOLIN"	;listPrenoms << "IVAN";
	listNoms << "DEVANTOY"	;listPrenoms << "FRANCK";
	listNoms << "BUISSON"	;listPrenoms << "STEPHANE";
	listNoms << "SCHOPFER"	;listPrenoms << "MICHEL";
	listNoms << "HERNANDEZ"	;listPrenoms << "ALEXIS";
	listNoms << "ROLDAN"	;listPrenoms << "ANTOINE";
	listNoms << "VELLA"		;listPrenoms << "CLAUDE";
	listNoms << "GUSTOT"	;listPrenoms << "KEVIN";
	listNoms << "GALLIERO"	;listPrenoms << "EMMANUEL";
	listNoms << "GRAVIER"	;listPrenoms << "LAURENT";
	listNoms << "VIGOT"		;listPrenoms << "ROMAIN";
	listNoms << "DROMARD"	;listPrenoms << "CHRISTOPHE";
	listNoms << "HAMARD"	;listPrenoms << "FLORIANT";
	listNoms << "FLEURY"	;listPrenoms << "ANDRE";
	listNoms << "DENDRAEL "	;listPrenoms << "GUILLAUME";
	listNoms << "LLARDEN"	;listPrenoms << "RICARDO";
	listNoms << "PAPOLLA"	;listPrenoms << "PIERRE";
	listNoms << "DAVIAUD"	;listPrenoms << "FRANCOIS";
	listNoms << "EUGEUNE"	;listPrenoms << "ANDRE";
	listNoms << "TOUREILLE"	;listPrenoms << "REMI";
	listNoms << "VEILLERAND";listPrenoms << "CHRITOPHE";
	listNoms << "ABDELLAOUI";listPrenoms << "SAMY";
	listNoms << "GOUREL"	;listPrenoms << "FABRICE";
	listNoms << "VANDEZANDE";listPrenoms << "THOMAS";
	listNoms << "FERRAINOLO";listPrenoms << "ROBERT";
	listNoms << "FOYER"		;listPrenoms << "MARC";
	listNoms << "BOUZID"	;listPrenoms << "CYRIL";
	listNoms << "GARDAREIN"	;listPrenoms << "JEAN-LAURENT";
	listNoms << "PANUELA"	;listPrenoms << "BENOIT";
	listNoms << "CARRENO"	;listPrenoms << "CHRISTOPHE";
	listNoms << "CANEL"		;listPrenoms << "JEAN-PIERRE";
	listNoms << "MALET"		;listPrenoms << "AUGUSTIN";
	listNoms << "DELPHIN"	;listPrenoms << "GUILLAUME";
	listNoms << "BARNABE"	;listPrenoms << "ERIC";
	listNoms << "FROMENT"	;listPrenoms << "GUILLAUME";
	listNoms << "MARQUIS"	;listPrenoms << "FREDERIC";
	listNoms << "BONAMY"	;listPrenoms << "SEBASTIEN";
	listNoms << "AUNE"		;listPrenoms << "LAURENT";
	listNoms << "MOUCHEL"	;listPrenoms << "MATHIEU";
	listNoms << "SUCRE"		;listPrenoms << "ELLIOT";
	listNoms << "CALLUELA"	;listPrenoms << "JEAN-FRANCOIS";
	listNoms << "GRASMUCK"	;listPrenoms << "PIERRE";
	listNoms << "ORSINI"	;listPrenoms << "PHILIPPE";
	listNoms << "GIMESEZ"	;listPrenoms << "GERALD";
	listNoms << "BEAL"		;listPrenoms << "EDOUARD";
	listNoms << "BOULPIQUANTE";listPrenoms << "LYLIAN";
	listNoms << "BOLLOTTE"	;listPrenoms << "CYRIL";
	listNoms << "MOUTON"	;listPrenoms << "FABRICE";
	listNoms << "HUYNH"		;listPrenoms << "PHUONG";
	listNoms << "JOULIA"	;listPrenoms << "FABRICE";
	listNoms << "PIERESCHI"	;listPrenoms << "RICHARD";
	listNoms << "BOUZON"	;listPrenoms << "AURELIEN";
	listNoms << "COTTE"		;listPrenoms << "STEPHANE";
	listNoms << "CERBONI"	;listPrenoms << "CHRISTIAN";
	listNoms << "GUIGUES"	;listPrenoms << "GREGOIRE";
	listNoms << "TRIAGE"	;listPrenoms << "BAC	DE";
	listNoms << "MARTIN"	;listPrenoms << "DIDIER";
	listNoms << "QUENOUILLERE";listPrenoms << "DIDIER";
	listNoms << "QUENOUILERE";listPrenoms << "DIDER";
	listNoms << "QUENOUILER";listPrenoms << "DIDYER";
	listNoms << "QUENOULLERE";
	listNoms << "QUENOUILLER";
	listNoms << "QUENOULER";
	
	

	RLOG("\n---------------------------------------------------------------------");
	RLOG("---------------------------------------------------------------------");
	RLOG("---                        NOMS                                   ---");
	RLOG("---------------------------------------------------------------------");
	doCompare(listNoms);


	RLOG("\n---------------------------------------------------------------------");
	RLOG("---------------------------------------------------------------------");
	RLOG("---                        PRENOMS                                ---");
	RLOG("---------------------------------------------------------------------");
	doCompare(listPrenoms);
	
	RLOG("\n---------------------------------------------------------------------");
	RLOG("---------------------------------------------------------------------");
	RLOG("---                        CLUBS                                  ---");
	RLOG("---------------------------------------------------------------------");
	
	Vector<String> listClubs;
	listClubs << "FILS DU GOEMON SAUVAGE";
	listClubs << "ROYAL CLUB SUB ETEL";
	listClubs << "LES EPERLANS FOUGUEUX";
	listClubs << "FEDERATION NAUTIQUE DE PECHE SPORTIVE EN APNEE";
	listClubs << "CENTRE SUBAQUATIC NANTAIS";
	listClubs << "CHERBOURG PECHE APNEE CLUB";
	listClubs << "SAINT NAZAIRE SUB CLUB";
	listClubs << "HARPON CLUB AGATHOIS";
	listClubs << "APNEA";
	listClubs << "AGACHON TRANQUILLE";
	listClubs << "INCONNU";
	listClubs << "ABALONE CHASSE BORDEAUX";
	listClubs << "TRITON CLUB DE CARQUEIRANNE";
	listClubs << "ANTIBES SPORT AVENTURE";
	listClubs << "LICHE CLUB BAIE DE CAVALAIRE";
	listClubs << "LES AMIS DE NEPTUNE";
	listClubs << "APPA";
	listClubs << "ASPTT NICE";
	listClubs << "RABA";
	listClubs << "HARPON CLUB DE LA COTE DE NACRE";
	listClubs << "SUBAQUA CLUB DE COLOMBES";
	listClubs << "LIGUE NORMANDIE";
	listClubs << "HOMPALMUS CLUB MARTIGUES";
	listClubs << "AILES SPORTIVES EUROCOPTER";
	listClubs << "GASM TOULOUSE";
	listClubs << "ALPANA CORSE IMERSION";
	listClubs << "CLUB GRANDE CHASSE SOUS-MARINE";
	listClubs << "CLUB SCOLARI";
	listClubs << "ASPPT MARSEILLE";
	listClubs << "CORSICA APNEE AJACCIO";
	listClubs << "TRITON CLUB HOUESVILLAIS";
	listClubs << "HENDAYE AGACHON CLUB";
	listClubs << "LIGUE DE BRETAGNE ET PAYS DE LOIRE";

	doCompare(listClubs);
}

