/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_EGS_CONNECTION_H
#define _EMBER_EGS_CONNECTION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "c_socket.h"

#define NET_REVISION "254"

typedef enum
{
    FMGS_CON_NOTCONNECT,     // not connected to any host
    FMGS_CON_LOGGINPROC,     // connected but not logged in
    FMGS_CON_SYSBUSY,        // if some class internal things (filetransfer) are done
    FMGS_CON_CONNECTED,      // ok normal connecting
} eFMGSConnectionState;

typedef enum{
    GS_NETMSG_NOMESSAGE = 1,
    GS_NETMSG_NOP,
    GS_NETMSG_LOGIN_REQUEST,
    GS_NETMSG_LOGIN_REQUEST_REPLY,
    GS_NETMSG_LOGOUT,
    GS_NETMSG_SYSTEMMSG,
    GS_NETMSG_LOCALCHAT,
    GS_NETMSG_VIS_UPDATE,
    GS_NETMSG_VIS_REMOVE,
    GS_NETMSG_MOVEPLAYER,
    GS_NETMSG_CREATE_CHARACTER,
    GS_NETMSG_MODIFY_CHARACTER,
    GS_NETMSG_RETRIEVECHARS,
    GS_NETMSG_RETRIEVECHARINFO,
    GS_NETMSG_GETINVENTORY,
    GS_NETMSG_CHANGETILE,
    GS_NETMSG_CHANGEOBJECT,
    GS_NETMSG_CLIENTSHUTDOWN,
    GS_NETMSG_DELETE_CHARACTER,
    GS_NETMSG_GENERIC_MSG,
    GS_NETMSG_SET_TILE,
    GS_NETMSG_SET_OBJECT,
    GS_NETMSG_CHANGE_MODE,
    GS_NETMSG_MODIFY_MAP,
    GS_NETMSG_SET_VERTEX,
    GS_NETMSG_HEARTBEAT,
    GS_NETMSG_HEARTBEAT_REQUEST,
    GS_NETMSG_CHAR_STATUS,
    GS_NETMSG_EMBER__DOWNLOAD,
    GS_NETMSG_EMBER__UPLOAD,
    GS_NETMSG_PING,
    GS_NETMSG_PROMPT,
    GS_NETMSG_FVM,
    GS_NETMSG_SERVER_INFO,
    GS_NETMSG_PUT_SERVER_INFO,
    GS_NETMSG_GET_SERVER_LIST,
    GS_NETMSG_MASTER_TO_GAME,
    GS_NETMSG_GUI,
	GS_NETMSG_GUI_COMMAND,
    GS_NETMSG_GET_LOGIN_PROGRAM,
	GS_NETMSG_SERVERINFORMATION,
	GS_NETMSG_FILE_XFER,


} tNetmsg;

class C_FMGS
{
public:
    C_FMGS();
    ~C_FMGS();
    long        Ping(char *pHost,char *pPort,long &dwPing,bool &bPinging,long &dwFuncTimer,long &dwWaitTimer);//
    int         Connect(char *pHost, char *pPort,char *pUser,char *pPasswd);
    void        Disconnect(void);
    void        SetState(eFMGSConnectionState eNewState);       // state of the connection
    void        SetSocket(CCSocket *pNewSocket);                // sets the lowlevel netinterface
    CCSocket    *pSocket;                                   // lowlevel network interface
    CCSocket    *pGetSocket(void);                          // gets the lowlevel netinterface
    bool        bLoggedin;                                  // is the player logged in
    bool        bSystemBusy;                                // set if internal stuff to do
    int         iGetMessage(void);                          // tries to receive a message
    CCSocket    *pPingSocket;                               // lowlevel network interface
    long        dwPing;
    C_FMGS      *pNext;
    C_FMGS      *pPrevious;
    eFMGSConnectionState  eGetState(void);

    void SendNetMessage(CPacket *packet,char cMethod);

    //long            dwPing;
    long            dMsgTime;
    char            cLastNetMessage;

};


#endif // _EMBER_EGS_CONNECTION_H
