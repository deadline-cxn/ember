
#include "s_linux.h"

#ifdef __linux__

void ConsoleSetup(void)
{
    _kbhit();
}

void ConsoleShutDown(void)
{
    close_keyboard();

}

void LoadPlugIns(void)
{

}


void UnLoadPlugIns(void)
{

}

#endif
