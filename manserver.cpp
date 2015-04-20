///////////////////////////////////////////////
// Mantra Server
#include "manserver.h"
#include "mantra_player_character.h"
int main(int argc, char *argv[]){
    bool bquiet=false;
    char nargs;
    if(argc>1){ nargs=1;
        while(nargs<argc){
            if( (!strcmp(argv[nargs],"--help")) || (!strcmp(argv[nargs],"-h")) || (!strcmp(argv[nargs],"/?")) ){
                ShowHelp();
                exit(0);
            }

            if( (!strcmp(argv[nargs],"-q")) || (!strcmp(argv[nargs],"--quiet")) ||	(!strcmp(argv[nargs],"/q"))){
                bquiet=true;
            }
            nargs++;
        }
    }
    ConsoleSetup(); // (POSIX) system specific console setup
    pServer=new CServer(bquiet);
    if(!pServer) { ConsoleShutDown(); return 1; }
    while(!pServer->bQuit)
        pServer->cycle();
    DEL(pServer);
    ConsoleShutDown();
    return 1;
}
void ShowHelp(void){
    printf("Ember Game Server %s %s Help\n",VERSION,COPYRIGHT);
    printf("==============================================================\n");
    printf(" --help, -h, /? = this help\n");
    printf(" --quiet, -q, /q = quiet mode, no console output\n");
    printf("==============================================================\n");
}

#ifdef __linux__
#include "s_linux.h"
#include "s_gnu.h"
void ConsoleSetup(void) { _kbhit(); }
void ConsoleShutDown(void) { close_keyboard(); }
#endif

#ifdef _WIN32
BOOL WINAPI HandleCTRL_C(DWORD dwCtrlType){
    switch(dwCtrlType) {
        case CTRL_BREAK_EVENT:
            pServer->bQuit=true;
            pServer->Log("Break event killed server!");
            return true;
        case CTRL_SHUTDOWN_EVENT:
            pServer->bQuit=true;
            pServer->Log("Shutdown event killed server!");
            return true;
        case CTRL_LOGOFF_EVENT:
            pServer->bQuit=true;
            pServer->Log("Logoff event killed server!");
            return true;
        case CTRL_CLOSE_EVENT:
            pServer->bQuit=true;
            pServer->Log("Mouse [X] killed server!");
            return true;
        case CTRL_C_EVENT:
            pServer->bQuit=true;
            pServer->Log("CTRL-C killed server!");
            return true;
        default:
            break;
    }
    return false;
}
void ConsoleSetup(void){ // Change window title for windows version, and setup ctrl handler
    SetConsoleCtrlHandler(HandleCTRL_C,TRUE);
    SetConsoleTitle(va("Mantra Server %s(%s) Net(%d) Build(%d) %s",VERSION,CPUSTRING,NET_REVISION,MANTRA_S_BUILD,COPYRIGHT));
}
void ConsoleShutDown(void){ }
#endif
