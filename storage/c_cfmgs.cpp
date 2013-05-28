/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "c_cfmgs.h"
#include "dlstorm.h"

//////////////////////////////////////////////////////
C_FMGS::C_FMGS(void) { pSocket=NULL; pPingSocket=NULL; bLoggedin=false; bSystemBusy=false; pNext=NULL; }
//////////////////////////////////////////////////////
C_FMGS::~C_FMGS() { DEL(pSocket); DEL(pPingSocket); }
//////////////////////////////////////////////////////
int C_FMGS::Connect(char *pHost,char *pPort,char *pUser,char *pPasswd)
{
    long fTryTime;
    int iTry;
    int ret=0;
    int iLength;
    int iFlags;
    int iT;
    CInetAddress RetAddr;
    CPacket Send(NET_DATAGRAMSIZE);
    CPacket Rec(NET_DATAGRAMSIZE);
    CPacket Packet(1024);
    bLoggedin = false; DEL(pSocket); pSocket = new CCSocket();
    if(!pSocket)
    {
        // Log("C_FMGS::Connect() can not create a new pSocket!");
        return 0;
    }
    if(eGetState()==FMGS_CON_NOTCONNECT) return 0;
    pSocket->bCanSend = true;
    pSocket->pAddr = CInetAddress::pGetByName(pHost);
    if(pSocket->pAddr == NULL)
    {
        //Log("C_FMGS::Connect() Can't resolve hostname!");
        return 0;
    }
    pSocket->iSocket = NET_OpenSocket(0);
    if(pSocket->iSocket == -1)
    {
        // Log("C_FMGS::Connect() Can't open network socket!");
        return 0;
    }
    NET_SetSocketPort(&pSocket->pAddr->stAddr,atoi(pPort));
    pSocket->bConnected = false;
    pSocket->bCanSend = false;
    Send.Write(0);  // save header space which is filled in later
    Send.Write(0);
    Send.Write((char)CTL_CONNECT);
    Send.Write("EMBER");
    Send.Write(atoi(NET_REVISION));
    pSocket->FinishCtlPacket(&Send);

    for(iTry = 0; iTry < (int)pSocket->dConnectRetry ;iTry++)
    {
        if(NET_Write(pSocket->iSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),&pSocket->pAddr->stAddr)==-1) return 0;
        fTryTime = dlcs_get_tickcount();
        do
        {
            ret = NET_Read(pSocket->iSocket,Rec.pGetPacketBuffer(),Rec.iGetMaxSize(),&RetAddr.stAddr);
            if (ret == -1)
            {
                // Log("C_FMGS::Connect() -> connection read failed");
                return 0;
            }
            if (ret > 0)
            {
                Rec.SetCurSize(ret);
                iLength = ntohl(Rec.iRead());
                iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
                iLength &= NET_FLAG_LENGTH_MASK;
                if (!(iFlags & NET_FLAG_CTL))               { ret=0; continue; }
                if (iLength != ret)                         { ret=0; continue; }
                if (iLength < (int)(sizeof(int)*2+sizeof(char)*2)) { ret=0; continue; }
                break;
            }
        } while((ret == 0) && ((dlcs_get_tickcount() - fTryTime) < pSocket->dConnectTimeOut));
        if(ret) break;
    }
    if(!ret) return 0;
    Rec.iRead();
    char ctltype = Rec.cRead();
    if(ctltype == CTL_REJECT) return 0;
    if(ctltype != CTL_ACCEPT) return 0;
	// Log("Got a CTL_ACCEPT here");
    pSocket->pAddr->stAddr=RetAddr.stAddr;
    iT = Rec.iRead();
    NET_SetSocketPort(&pSocket->pAddr->stAddr,iT);
    pSocket->bConnected = true;
    pSocket->bCanSend = true;
    SetState(FMGS_CON_CONNECTED);
    Packet.Reset();
    Packet.Write((char)GS_NETMSG_LOGIN_REQUEST);
    Packet.Write((char *)pUser);
    Packet.Write((char *)pPasswd);
    pSocket->SendReliableMessage(&Packet); // Log in
    return 1;
}

/**************************************************
 ** SendNetMessage                               **
 **************************************************/

void C_FMGS::SendNetMessage(CPacket *packet,char cMethod)
{
    if(!pSocket) return;

        if(eGetState()==FMGS_CON_NOTCONNECT) return;
        switch(cMethod)
        {
            case 1: // Unreliable
                pSocket->SendUnreliableMessage(packet); // (CPacket *)&SendData);
                dMsgTime=dlcs_get_tickcount();
                break;

            case 0: // Reliable

                // while(!pSocket->bCanSendMessage()) DoNetwork();

                pSocket->SendReliableMessage(packet); // (CPacket *)&SendData);
                dMsgTime=dlcs_get_tickcount();
                break;

            default:
                pSocket->SendUnreliableMessage(packet); // (CPacket *)&SendData);
                dMsgTime=dlcs_get_tickcount();
                break;
        }

}



//////////////////////////////////////////////////////////////////////////
int C_FMGS::iGetMessage()
{
    int i;
    if(!pSocket)
    {
        //Log("C_FMGS::iGetMessage() tried to operate on a null socket...");
        return 0;
    }
    if (eGetState() == FMGS_CON_NOTCONNECT) return 0;
    i = pSocket->iGetMessage(); if(i==-1) i=0; return i;
}
//////////////////////////////////////////////////////////////////////////
eFMGSConnectionState C_FMGS::eGetState()
{
    if(!this)       return FMGS_CON_NOTCONNECT;
    if(!pSocket)    return FMGS_CON_NOTCONNECT;
    if(!bLoggedin)  return FMGS_CON_LOGGINPROC;
    if(bSystemBusy) return FMGS_CON_SYSBUSY;
                    return FMGS_CON_CONNECTED;
}
//////////////////////////////////////////////////////////////////////////
void C_FMGS::SetState(eFMGSConnectionState eNewState)
{
    if((pSocket)&&(eNewState==FMGS_CON_CONNECTED)) bLoggedin=true;
    if(eNewState == FMGS_CON_NOTCONNECT) { bLoggedin=false; if(pSocket) DEL(pSocket); }
    if(eNewState==FMGS_CON_SYSBUSY) bSystemBusy=true;
}
//////////////////////////////////////////////////////////////////////////

void C_FMGS::Disconnect()
{
    if(pSocket)
    {
        pSocket->Disconnect();
        DEL(pSocket);
    }
    else
    {
        //Log("C_FMGS::Disconnect() tried to operate on a null socket.");
    }
}

//////////////////////////////////////////////////////////
CCSocket *C_FMGS::pGetSocket() { return pSocket; }
//////////////////////////////////////////////////////////
void C_FMGS::SetSocket(CCSocket *pNewSocket) { DEL(pSocket); pSocket=pNewSocket; }
//////////////////////////////////////////////////////////
long C_FMGS::Ping(char *pHost,char *pPort,long &dwPing,bool &bPinging,long &dwFuncTimer,long &dwWaitTimer)
{
    //static bool bPinging=false;
    //static long dwFuncTimer=dlcs_get_tickcount();
    //static long dwWaitTimer=dlcs_get_tickcount();

    int ret,iLength,iFlags;
    CPacket Send(NET_DATAGRAMSIZE);
    CPacket Recv(NET_DATAGRAMSIZE);
    CInetAddress RetAddr;
    if(!pPingSocket) { pPingSocket=new CCSocket(); bPinging=false; }
    if(!pPingSocket) { bPinging=false; return dwPing; }
    if(!bPinging)
    {
        if(dlcs_get_tickcount()-dwFuncTimer<5000) { bPinging=false; return dwPing; }
        dwFuncTimer=dlcs_get_tickcount();
        pPingSocket->bCanSend=true;
        pPingSocket->pAddr=CInetAddress::pGetByName(pHost);
        if(pPingSocket->pAddr)
        {
            pPingSocket->iSocket=NET_OpenSocket(0);
            if(pPingSocket->iSocket >= 0)
            {
                NET_SetSocketPort(&pPingSocket->pAddr->stAddr,atoi(pPort));
                pPingSocket->bConnected=false;
                pPingSocket->bCanSend=false;
                Send.Reset();
                Send.Write(0);  // save header space which is filled in later
                Send.Write(0);
                Send.Write((char)CTL_PING);
                Send.Write((long)dlcs_get_tickcount());
                Send.Write(atoi(NET_REVISION));
                pPingSocket->FinishCtlPacket(&Send);
                NET_Write(pPingSocket->iSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),&pPingSocket->pAddr->stAddr);
                bPinging=true;
                dwWaitTimer=dlcs_get_tickcount();
            }
        }
    }
    else
    {
        ret = NET_Read(pPingSocket->iSocket,Recv.pGetPacketBuffer(),Recv.iGetMaxSize(),&RetAddr.stAddr);
        if(ret==-1) ret=0;
        if(ret>0)
        {
            Recv.SetCurSize(ret);
            iLength = ntohl(Recv.iRead());
            iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
            iLength &= NET_FLAG_LENGTH_MASK;
            if(!(iFlags & NET_FLAG_CTL)) ret=0;
            if(iLength != ret)           ret=0;
            if(iLength < (int) (sizeof(int)*2+sizeof(char)*2)) ret=0;
        }
        if(ret==0) { if(dlcs_get_tickcount()-dwWaitTimer>5000) { dwPing=9994; bPinging=false; } }
        else { Recv.iRead(); char ctltype = Recv.cRead(); if(ctltype == CTL_PING) { dwPing=(dlcs_get_tickcount()-Recv.dwRead()); bPinging=false; } }
    }
    return dwPing;
}
