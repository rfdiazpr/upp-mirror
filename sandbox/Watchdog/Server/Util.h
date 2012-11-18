#ifndef _Watchdog_Server_Util_h_
#define _Watchdog_Server_Util_h_

void UpdateLogs();
void CleanResults();
void CleanAuth();
VectorMap<String,int>  Paging(Http& http);
bool CheckLocal(Http& http);
int& lastrev();
bool CheckAuth(Http& http, Sql& sql);
bool CheckAuth2(Http& http, Sql& sql, int client, const String& action);
void SendEmail(const String& to, const String& subject, const String& text);
ValueArray ParseFilter(const String& Filter);
SqlVal SqlEmptyString();

namespace Upp { namespace Ini {
	extern IniString output_dir;
}}

#endif
