/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_CONSOLE_SERVER
#define _EMBER_CONSOLE_SERVER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C_FMMS;
class CPacket;
class CServer;
class C_FM_CLIENT;
class CEmMap;
class CPacket;

class CDLDB;
class CDBVar;

extern CDLDB		*db;
extern CDBVar		*row;


extern CEmMap       *pMap;
extern CEmMap       *pMapBuffer;
extern CEmMap       *pFragMap;
extern CEmMap       *pFirstFragMap;
extern CEmMap       *pDeleteFragMap;
extern CPacket      Send;
extern CServer      *fmgs;

extern char         szLastAddress[1024];
extern bool         bQuit;




#ifdef _EMBER_SERVER_
extern bool bQuiet;
#endif


/*
///extern C_FMMS       *pFMMS_Connection;
extern C_FM_CLIENT  *pOtherPlayer;
extern C_FM_CLIENT  *pPlayer;
extern time_t       fmgsTime;
extern struct       tm *tTime;
extern bool         bQuiet;
extern bool         bRestart;
*/

#ifdef __cplusplus
extern "C"
{
#endif

    void AdminToPlayer(C_FM_CLIENT *pClient,char *msg,unsigned char r,unsigned char g, unsigned char b);
    void MovePlayer(C_FM_CLIENT *pClient,int x,int y,int z,int teleport);
    void AdminAnnounce(char *msg,unsigned char r,unsigned char g, unsigned char b);
    void Announce(C_FM_CLIENT *pClient,char *msg,unsigned char r,unsigned char g,unsigned char b);
    void GlobalAnnounce(C_FM_CLIENT *pClient,char *msg,unsigned char r,unsigned char g, unsigned char b);
    void Say(C_FM_CLIENT *pClient,char *msg,unsigned char r,unsigned char g,unsigned char b,char *font);
    void RemoveDeadPlayers(void);
    void RemoveAccount(char *szName);

    void DoClient(void);

    void DoWorld(void);
    void CheckRestart(void);
    void SaveWorld(void);
    void LoadWorld(void);
    void StartUp(void);
    void ShutDown(void);
    void ReportToMaster(void);
    void Msg(char *name,char *msg);
	void Announce(C_FM_CLIENT *pClient,char *msg,unsigned char r,unsigned char g,unsigned char b);

    void Kick(char *name,char *reason);
    void ShowHelp(void);

    void SetupConsoleHistory(void);
    void RemoveConsoleHistory(void);
    void AddToConsoleHistory(char *s);
    char *GetConsoleHistory(int x);

    unsigned long  GetSerialNumber(bool);
    void    RemoveAccount(char *szName);

	char *SysVar(char *);

    C_FM_CLIENT *GetClient(char *user_name);

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
