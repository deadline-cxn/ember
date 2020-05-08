//////////////////////////////////////////////////////////////////////////////////
#ifndef _MANTRA_SERVER_H
#define _MANTRA_SERVER_H
//////////////////////////////////////////////////////////////////////////////////

#include "c_console.h"
#include "c_gui_chat_enum.h"
#include "c_log.h"
#include "c_net.h"
#include "c_var.h"
#include "dlcs.h"
#include "dlcs_db.h"
#include "manserver.client.h"
#include "mantra_common.h"
#include "mantra_version.h"

//////////////////////////////////////////////////////////////////////////////////

class C_GSC;

class CServer : public CCSocket {
   public:
    CServer(bool bIsQuiet);
    virtual ~CServer();

    CLog *     pLog;
    C_GSC *    pFirstPlayer;
    bool       bQuit;
    bool       bRestart;
    bool       bQuiet;
    long       dwStartTime;
    C_CONS *   pConsole;
    CVarSet *  pCVars;
    C_DLCS_DB *pDB;

    int    StartUp(void);
    bool   bNetStartUp(void);
    void   ShutDown(void);
    void   MainLoop(void);
    void   DoPlayers(void);
    void   DoKeyboard(void);
    void   DoWorld(void);
    bool   bCheckRestart(void);
    void   ExecuteConsoleCommand(char *szCommand);
    char * GetConsoleHistory(int x);
    void   SetDefaultCVars(void);
    bool   bSaveCVars(void);
    C_GSC *pGetClient(const char *szUserName);  // network / client stuff
    void   AcceptConnections(void);
    void   RejectConnection(int iReSocket, sockaddr *pReAddr, const char *szReason);
    void   SendAll(CPacket *pPacket, float fBlockTime);
    void   Disconnect(C_GSC *pClient, const char *szReason);
    int    iNumClients(void);
    void   PurgeZombieClients(void);
    void   SaveWorld(void);
    void   LoadWorld(void);
    void   Chat(C_GSC *pClient, const char *szFrom, const char *szMsg, int iChannel);    // by player pointer
    void   Chat(const char *szTo, const char *szFrom, const char *szMsg, int iChannel);  // by name
    void   ReportToMaster(void);

    // console commands
    void        kick_user(const char *szUserName, const char *szReason);
    void        kick_user(const char *szFormat, ...);
    void        kick_all(const char *szReason);
    void        create_guid(const char *szCin, char *szCout);
    void        add_user(const char *szUserName, int iAccess);
    void        delete_user(const char *szUserName);
    void        set_user_access(const char *szInUserName, int iAccess);
    const char *get_user_access(const char *szInUserName);
    void        ban_user(const char *szUserName);

    // void    SetupConsoleHistory(void);
    // void    RemoveConsoleHistory(void);
    // void    AddToConsoleHistory(char *s);
    // bool    LoadServerIni(const char *szFilename);
    // CPacket *pServerInfo;
    // void    SetServerInfo(CPacket *pPacket){ pServerInfo = pPacket; }
    // CPacket *pGetServerInfo(void){ return pServerInfo; } // serverinfo getter and setter
    // void    SetUpMessage(CPacket *pPacket); // configure the use of the master server
    // void    SetMaster(CMasterAddress *pMaster);
    // CMasterAddress *pGetMaster();
    // CMasterAddress *pMa;
    // char   szLastAddress[1024];
    // time_t  pServerTime;
    // struct  tm *tTime;
};
/////////////////////////////////////////
#endif  //_MANTRA_SERVER_H
/////////////////////////////////////////
