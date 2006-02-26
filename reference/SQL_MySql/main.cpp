#include <MySql/MySql.h>

CONSOLE_APP_MAIN
{
	MySqlSession session;
	if(!session.Connect()) {
		printf("Can't connect with MySql\n");
		return;
	}
	Sql sql(session);
	sql.Execute("use mysql");
	sql.Execute("show tables");
	while(sql.Fetch())
		Cout() << (String)sql[0] << '\n';
}
