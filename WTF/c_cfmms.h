/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMS_CONNECTION
#define _EMS_CONNECTION

#include "ember_network_common.h"
#include "c_serverinfo.h"

#define MAX_SERVERS 5000

typedef enum{
    MS_NETMSG_NOMESSAGE = 1,
    MS_NETMSG_NOP,
    MS_NETMSG_LOGIN_REQUEST,
    MS_NETMSG_LOGIN_REQUEST_REPLY,
    MS_NETMSG_LOGOUT,
    MS_NETMSG_SYSTEMMSG,
    MS_NETMSG_LOCALCHAT,
    MS_NETMSG_VIS_UPDATE,
    MS_NETMSG_VIS_REMOVE,
    MS_NETMSG_MOVEPLAYER,
    MS_NETMSG_CREATE_CHARACTER,
    MS_NETMSG_MODIFY_CHARACTER,
    MS_NETMSG_RETRIEVECHARS,
    MS_NETMSG_RETRIEVECHARINFO,
    MS_NETMSG_GETINVENTORY,
    MS_NETMSG_CHANGETILE,
    MS_NETMSG_CHANGEOBJECT,
    MS_NETMSG_CLIENTSHUTDOWN,
    MS_NETMSG_DELETE_CHARACTER,
    MS_NETMSG_GENERIC_MSG,
    MS_NETMSG_SET_TILE,
    MS_NETMSG_SET_OBJECT,
    MS_NETMSG_CHANGE_MODE,
    MS_NETMSG_MODIFY_MAP,
    MS_NETMSG_SET_VERTEX,
    MS_NETMSG_HEARTBEAT,
    MS_NETMSG_HEARTBEAT_REQUEST,
    MS_NETMSG_CHAR_STATUS,
    MS_NETMSG_EMBER__DOWNLOAD,
    MS_NETMSG_EMBER__UPLOAD,
    MS_NETMSG_PING,
    MS_NETMSG_PROMPT,
    MS_NETMSG_FVM,
    MS_NETMSG_SERVER_INFO,
    MS_NETMSG_PUT_SERVER_INFO,
    MS_NETMSG_GET_SERVER_LIST,
    MS_NETMSG_MASTER_TO_GAME,
    MS_NETMSG_GUI,
	MS_NETMSG_GUI_COMMAND,
    MS_NETMSG_GET_LOGIN_PROGRAM,
	MS_NETMSG_SERVERINFORMATION,
	MS_NETMSG_SERVERINFORMATION_GET,
	MS_NETMSG_SERVERINFORMATION_RESET,
	MS_NETMSG_SERVERINFORMATION_END,
	MS_NETMSG_FILE_XFER,


} tFMMSMsg;

typedef enum{
    SI_GENERIC=0,
    SI_GET_SERVER_LIST,
    SI_PUT_SERVER,
} tMServerInfoUpdates;

class C_FMGS;
class CCSocket;

/*

class ServerData : public CServerInfo
{
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

class C_FMMS
{
public:
    C_FMMS();
    ~C_FMMS();
    int         Connect(char *pHost, char *pPort);
    void        Disconnect(void);
    void        DoUpdates(void);
    void        RequestServerList(void);
    void        RefreshServerList(void);
    void        ClearServerList(void);
    void        SetState(eFMMSConnectionState eNewState);   // state of the connection
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
    eFMMSConnectionState  eGetState(void);

#ifdef _EMBER_CLIENT_
    long        PingServer(int i);
//    long        PingServer(ServerData *pB);
#endif

};

#endif // _EMS_CONNECTION

