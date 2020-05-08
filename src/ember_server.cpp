///////////////////////////////////////////////
// Mantra Server
#include "manserver.h"
#include "manserver.server.h"
#include "mantra_player_character.h"

CServer *pServer;

int main(int argc, char *argv[]) {
    bool          bquiet = false;
    unsigned char nargs;

    if (argc > 1) {
        nargs = 1;
        while (nargs < argc) {
            if ((!strcmp((const char *)argv[nargs], "--help")) || (!strcmp((const char *)argv[nargs], "-h")) || (!strcmp((const char *)argv[nargs], "/?"))) {
                ShowHelp();
                exit(0);
            }

            if ((!strcmp((const char *)argv[nargs], "-q")) || (!strcmp((const char *)argv[nargs], "--quiet")) || (!strcmp((const char *)argv[nargs], "/q"))) {
                bquiet = true;
            }

            nargs++;
        }
    }

    ConsoleSetup();  // (POSIX) system specific console setup

    pServer = new CServer(bquiet);
    if (!pServer) {
        printf("Could not allocate memory to start server.\n");
        ConsoleShutDown();
        return 1;
    }

    pServer->MainLoop();  // Main Server Loop

    DEL(pServer);
    ConsoleShutDown();

    return 1;
}

void ShowHelp(void) {
    printf("Ember Game Server %s %s Help\n", MANTRA_VERSION, MANTRA_COPYRIGHT);
    printf("==============================================================\n");
    printf(" --help, -h, /? = this help\n");
    printf(" --quiet, -q, /q = quiet mode, no console output\n");
    printf("==============================================================\n");
}

#ifdef _WIN32
BOOL WINAPI HandleCTRL_C(DWORD dwCtrlType) {
    CLog *pLog;
    pLog = pServer->pLog;
    switch (dwCtrlType) {
        case CTRL_BREAK_EVENT:
            pServer->bQuit = true;
            LogEntry("Break event killed server!");
            return true;
        case CTRL_SHUTDOWN_EVENT:
            pServer->bQuit = true;
            LogEntry("Shutdown event killed server!");
            return true;
        case CTRL_LOGOFF_EVENT:
            pServer->bQuit = true;
            LogEntry("Logoff event killed server!");
            return true;
        case CTRL_CLOSE_EVENT:
            pServer->bQuit = true;
            LogEntry("Mouse [X] killed server!");
            return true;
        case CTRL_C_EVENT:
            pServer->bQuit = true;
            LogEntry("CTRL-C killed server!");
            return true;
        default: break;
    }
    return false;
}

void ConsoleSetup(void) {  // Change window title for windows version, and setup ctrl handler
    SetConsoleCtrlHandler(HandleCTRL_C, TRUE);
    SetConsoleTitle(va("Mantra Server %s(%s - %s) Net(%d) Build(%d) %s", MANTRA_VERSION, DLCS_CPU_STRING, DLCS_OS_STRING, MANTRA_NET_REVISION, MANTRA_S_BUILD, MANTRA_COPYRIGHT));
}

void ConsoleShutDown(void) {}

#endif
