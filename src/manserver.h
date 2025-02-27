// Mantra

#ifndef _MANTRA_CONSOLE_SERVER
#define _MANTRA_CONSOLE_SERVER

#include "dlcs.h"
#include "manserver.server.h"

#ifdef __cplusplus
extern "C" {
#endif
void ShowHelp(void);

#ifdef _WIN32
void SuspendPowerManagement(void);
void LogWindowsVersion(void);
void ConsoleSetup(void);
void ConsoleShutDown(void);
#else
/*
long GetTickCount(void);
int  _kbhit(void);
int  _getch(void);
void close_keyboard(void);
void Sleep(int ms);
*/
#endif

#ifdef __cplusplus
}
#endif

#endif  // _MANTRA_CONSOLE_SERVER
