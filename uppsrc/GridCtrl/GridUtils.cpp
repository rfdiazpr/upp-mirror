#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

LineEdit *dlog = NULL;
DropList *dlev = NULL;

static int pos = 0;

//void Log(const char *fmt, ...)
//{
//	char buffer[1024];
//	va_list argptr;
//	va_start(argptr, fmt);
//	int l = vsprintf(buffer, fmt, argptr);
//	va_end(argptr);
//	LOG(buffer);
//}
//
//void Log(int level, const char *fmt, ...)
//{
//	char buffer[1024];
//	va_list argptr;
//	va_start(argptr, fmt);
//	int l = vsprintf(buffer, fmt, argptr);
//	va_end(argptr);
//	LOG(buffer);
//}


void Log(const char *fmt, ...)
{
	if(!dlog || !dlev || int(~(*dlev)) > 0)
		return;

	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsprintf(buffer, fmt, argptr);
	va_end(argptr);

	dlog->Insert(pos, buffer);
	pos += l;
	dlog->Insert(pos, "\n");
	pos++;
	dlog->SetCursor(pos);
}

void Log(int level, const char *fmt, ...)
{
	if(!dlog || !dlev || level < int(~(*dlev)))
		return;

	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsprintf(buffer, fmt, argptr);
	va_end(argptr);

	dlog->Insert(pos, buffer);
	pos += l;
	dlog->Insert(pos, "\n");
	pos++;
	dlog->SetCursor(pos);
}

END_UPP_NAMESPACE
