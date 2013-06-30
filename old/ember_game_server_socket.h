// EMBER

#ifndef _EMBER_SERVER_H
#define _EMBER_SERVER_H

#include "c_net.h"
#include "c_log.h"
#include "ember_version.h"
#include "ember_network_common.h"
#include "ember_game_server_enum.h"
#include "manserver.client.h"
#include <vector>

////////////////////////////////////////////////////

class CServer : public CCSocket
{
public:
    CServer();
    CServer(CLog *pLogIn);
    ~CServer();

    bool            loaddata(void);
    bool            savedata(void);

    int             port;
    bool            b_new_accounts;

    CLog            *pLog;
    bool            bMadepLog;

    CPacket         Send;
    CPacket         *pServerInfo;

    C_GSC           *pFirstPlayer;
    C_GSC           *pFirstDelete;

    void Initialize(void);
    void Destroy(void);

    void Cycle(void);

    void Accept(void);
    void SendAll(CPacket *pPacket,float fBlockTime);
    void Disconnect(C_GSC *pClient,bool bsave);
    void ReportToMaster(void);
    void AdminToPlayer(C_GSC *pClient,char *msg,unsigned char r,unsigned char g, unsigned char b);
    void MovePlayer(C_GSC *pClient,int x,int y,int z,int teleport);
    void AdminAnnounce(char *msg,unsigned char r,unsigned char g, unsigned char b);
    void Announce(C_GSC *pClient,char *msg,unsigned char r,unsigned char g,unsigned char b);
    void GlobalAnnounce(C_GSC *pClient,char *msg,unsigned char r,unsigned char g, unsigned char b);
    void Say(C_GSC *pClient,char *msg,unsigned char r,unsigned char g,unsigned char b,char *font);
    // void RemoveAllDeadPlayers(void);
    void RemoveDeadPlayers(void);
    void RemoveAccount(char *szName);
    void DoClient(void);
    void DoWorld(void);
    void SaveWorld(void);
    void LoadWorld(void);
    void Msg(char *name,char *msg);
    void Kick(char *name,char *reason);
    void KickAll(char *reason);
    void ShowHelp(void);
    char *SysVar(char *);
    unsigned long  GetSerialNumber(bool);
    void RejectConnection(int iReSocket,sockaddr *pReAddr,char *pText);
    void SetServerInfo(CPacket *pPacket){ pServerInfo = pPacket; }
    CPacket *pGetServerInfo(void){ return pServerInfo; } // serverinfo getter and setter
    void SetUpMessage(CPacket *pPacket); // configure the use of the master server

    C_GSC *GetClient(char *user_name);

	//CCSocket *pGetFirstSocket();
    //CCSocket *pGetNextSocket();
    //void NetStat(); // prints the net statistics
    //void SetMaster(CMasterAddress *pMaster);
    //CMasterAddress *pGetMaster();
    //CMasterAddress *pMa;
};

#endif//_EMBER_SERVER_H
