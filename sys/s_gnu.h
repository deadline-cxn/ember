/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "global.h"

#ifndef _WIN32
extern "C" long GetTickCount(void);
extern "C" void Sleep(int ms);
extern "C" int _kbhit(void);
extern "C" int _getch(void);
extern "C" void close_keyboard(void);
#endif
