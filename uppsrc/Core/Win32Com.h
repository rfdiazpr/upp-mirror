#ifdef PLATFORM_WIN32

#include <winreg.h>

inline bool IsWinNT() { return GetVersion() < 0x80000000; }

bool IsWinXP();

HINSTANCE AppGetHandle();
void      AppSetHandle(HINSTANCE dll_instance);

String AsString(const wchar_t *buffer);
String AsString(const wchar_t *buffer, int count);
String AsString(const wchar_t *buffer, const wchar_t *end);

String GetWinRegString(const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE);
int    GetWinRegInt(const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE);
bool   SetWinRegString(const String& string, const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE);
bool   SetWinRegInt(int data, const char *value, const char *path, HKEY base_key = HKEY_LOCAL_MACHINE);
void   DeleteWinReg(const String& key, HKEY base = HKEY_LOCAL_MACHINE);

String GetSystemDirectory();
String GetWindowsDirectory();
String GetModuleFileName(HINSTANCE instance = AppGetHandle());

#endif
