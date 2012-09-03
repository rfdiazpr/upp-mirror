#ifndef _Watchdog_Server_Util_h_
#define _Watchdog_Server_Util_h_

void UpdateLogs();
void CleanResults();
VectorMap<String,int>  Paging(Http& http);
bool CheckLocal(Http& http);
int& lastrev();

namespace Upp { namespace Ini {
	extern IniString output_dir;
}}

#endif
