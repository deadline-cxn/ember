/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "s_win32.h"
#include "fm_util.h"
#include "ember_game_server.h"
#include "global.h"

#ifdef _WIN32
BOOL WINAPI HandleCTRL_C(DWORD dwCtrlType)
{
    switch(dwCtrlType)
    {

        case CTRL_BREAK_EVENT:
            bQuit=true;
            Log("Break event killed server!");
            return true;

        case CTRL_SHUTDOWN_EVENT:
            bQuit=true;
            Log("Shutdown event killed server!");
            return true;

        case CTRL_LOGOFF_EVENT:
            bQuit=true;
            Log("Logoff event killed server!");
            return true;
        
        case CTRL_CLOSE_EVENT:
            bQuit=true;
            Log("Mouse [X] killed server!");
            return true;
            
        case CTRL_C_EVENT:
            bQuit=true;
            Log("CTRL-C killed server!");
            return true;

        default:
            break;
    }
    return false;
}


void ConsoleSetup(void)
{
    // Change window title for windows version, and setup ctrl handler
    SetConsoleCtrlHandler(HandleCTRL_C,TRUE);
    SetConsoleTitle(va("Ember Game Server %s(%s) Net Revision(%s) %s",VERSION,CPUSTRING,NET_REVISION,COPYRIGHT));
}

void ConsoleShutDown(void)
{

}

#endif
