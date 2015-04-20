/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMS_CONNECTION
#define _EMS_CONNECTION

#include "c_serverinfo.h"

#define MAX_SERVERS 5000

typedef enum {
    CON_NOTCONNECT,     // not connected to any host
    CON_LOGGINPROC,     // connected but not logged in
    CON_SYSBUSY,        // if some class internal things (filetransfer) are done
    CON_CONNECTED,      // ok normal connecting
} eConnectionState;

typedef enum{
    SI_GENERIC=0,
    SI_GET_SERVER_LIST,
    SI_PUT_SERVER,
} tMServerInfoUpdates;

class C_FMGS;
class CCSocket;

/*
class ServerData : public CServerInfo {
public:
    ServerData(void);
    ~ServerData(void);
    bool bPinging;
    long dwFuncTimer;
    long dwWaitTimer;
    long dwPing;
    int  key;

#ifdef _EMBER_CLIENT_
    C_FMGS *pConnection;
#endif
    ServerData *pNext;
};
*/

class C_FMMS {
public:
    C_FMMS();
    ~C_FMMS();
    int         Connect(char *pHost, char *pPort);
    void        Disconnect(void);
    void        DoUpdates(void);
    void        RequestServerList(void);
    void        RefreshServerList(void);
    void        ClearServerList(void);
    void        SetState(eConnectionState eNewState);   // state of the connection
    void        SetSocket(CCSocket *pNewSocket);        // sets the lowlevel netinterface
    CCSocket    *pSocket;                               // lowlevel network interface
    CCSocket    *pGetSocket(void);                      // gets the lowlevel netinterface
    bool        bLoggedin;                              // is the player logged in
    bool        bSystemBusy;                            // set if internal stuff to do
    int         iGetMessage(void);                      // tries to receive a message
    bool        bHasRequestedList;
    bool        bHasRecievedList;
    int         iNumServers;
//    ServerData  *pServerList;       // list of servers on the master server
    eConnectionState  eGetState(void);

#ifdef _EMBER_CLIENT_
    long        PingServer(int i);
//    long        PingServer(ServerData *pB);
#endif

};

#endif // _EMS_CONNECTION

