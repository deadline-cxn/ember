/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_CONSOLE_SERVER
#define _EMBER_CONSOLE_SERVER

//class CServer;
//class CDLDB;
//class CDBVar;
//extern CDLDB		*db;
//extern CDBVar		*row;
//extern char         szLastAddress[1024];
//extern bool         bQuit;
//#ifdef _EMBER_SERVER_
//extern bool bQuiet;
//#endif


#define MAX_CONSOLE_INPUT_BUFFER    10		// Number of console commands to scroll back to execute again

/*
///extern C_FMMS       *pFMMS_Connection;
extern C_EMBER_ClientSocket  *pOtherPlayer;
extern C_EMBER_ClientSocket  *pPlayer;
extern time_t       fmgsTime;
extern struct       tm *tTime;
extern bool         bQuiet;
extern bool         bRestart;
*/

#ifdef __cplusplus
extern "C"
{
#endif

    void StartUp(void);
    void ShutDown(void);
    void CheckRestart(void);

    void SetupConsoleHistory(void);
    void RemoveConsoleHistory(void);
    void AddToConsoleHistory(char *s);
    char *GetConsoleHistory(int x);

    // void  do_net(void);

    void ConsoleSetup(void);
    void ConsoleShutDown(void);


#ifdef _WIN32
    void  SuspendPowerManagement(void);
    void  LogWindowsVersion(void);
#else
    long  GetTickCount(void);
    int   _kbhit(void);
    int   _getch(void);
    void  close_keyboard(void);
    void  Sleep(int ms);

#endif

#ifdef __cplusplus
}
#endif

#endif // _EMBER_CONSOLE_SERVER
