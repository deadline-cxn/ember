// EMBER

#include "ember_game_server_socket.h"

//////////////////////////////////////////////////////
CServer::CServer()
{
    initSocket();
    pFirstPlayer=0;
    pFirstDelete=0;
    CPacket     Send(8192);
    pLog=new CLog();
    pLog->SetName("ember_game_server_socket.log");
    pLog->On();
    pLog->Restart();
    bMadepLog=true;
    Initialize();
}

//////////////////////////////////////////////////////
CServer::CServer(CLog *pInLog)
{
    pLog=pInLog;
    bMadepLog=false;
    Initialize();
    initSocket();
    if(Listen(port, true)==-1)
        printf("ERROR LISTENING ON PORT %d\n",port);

    pFirstPlayer=0;
    pFirstDelete=0;
    CPacket     Send(8192);

    pLog->_Add("Listening on port [%d]", iGetLocalPort());
}


//////////////////////////////////////////////////////
CServer::~CServer()
{
    savedata();
    Destroy();
    pFirstDelete=pFirstPlayer;
    while(pFirstDelete)
    {
        pFirstPlayer=pFirstDelete;
        pFirstDelete=pFirstDelete->pNext;
        DEL(pFirstPlayer);
    }
    if(bMadepLog)
    DEL(pLog);


}
//////////////////////////////////////////////////////

void CServer::Initialize(void)
{
    b_new_accounts=0;

    loaddata();

    ///////// SET UP MASTER SERVER CONNECTION
    // pFMMS_Connection = new C_FMMS;


    LoadWorld();

}

void CServer::Destroy(void)
{
    GetSerialNumber(1);

    KickAll("Server shutting down");

//    RemoveAllDeadPlayers();

    C_GSC * pClient=pFirstPlayer;
    while(pClient)
    {
        pFirstPlayer=pClient;
        pClient=pClient->pNext;
        DEL(pFirstPlayer);
    }


    ////////////// MAP STUFF

    SaveWorld();

    /*
    DEL(pMap);
    DEL(pMapBuffer);

    pFragMap=pFirstFragMap;
    while(pFragMap)
    {
        pDeleteFragMap=pFragMap;
        pFragMap=pFragMap->pNext;
        DEL(pDeleteFragMap);
    }
    */


    /*
    ////////////// DATABASE STUFF
	if(dlcs_strcasecmp(SysVar("save_db_on_exit"),"yes") )
	{
		pLog->_Add("Saving database");
		// db->Save("database.txt");
	}
	// DEL(db);
	*/
}

void CServer::Cycle(void)
{
    Accept();
    ReportToMaster();
    RemoveDeadPlayers();
    DoWorld();
    DoClient();

}

void CServer::Accept(void)
{
	C_GSC           *client;

    int             iLength,iFlags,iHelp;
    int             iAcceptSocket;
    char            cType;

    CCSocket        *pNewSocket=0;

    struct sockaddr_in ServerAddr;
    memset(&ServerAddr,0,sizeof(ServerAddr));

    CPacket Send(NET_DATAGRAMSIZE);
	CPacket Recv(NET_DATAGRAMSIZE);

	iAcceptSocket = CheckNewConnections();
    if(iAcceptSocket == -1) { return; }

    if((iLength = nRecv(iAcceptSocket,Recv.pGetPacketBuffer(),Recv.iGetMaxSize(),(struct sockaddr*)&ServerAddr))<=0)
    {
        pLog->_Add("WHAT!");
        return;
    }

    Recv.SetCurSize(iLength);

    iHelp   = ntohl(Recv.iRead());
    iFlags  = iHelp & (~NET_FLAG_LENGTH_MASK);
    iHelp  &= NET_FLAG_LENGTH_MASK;

    if (!(iFlags & NET_FLAG_CTL))    {  pLog->_Add("1");      return;    }
    if (iLength != iHelp)            {  pLog->_Add("2");      return;    }
    if (iLength < sizeof(int)*2)    {  pLog->_Add("3");      return;    }

    Recv.iRead();
    cType=Recv.cRead();

    switch(cType)
    {
        case CTL_CONNECT:

			// pLog->_Add("Connection attempt from %s",NET_pAddrToString((struct sockaddr*)&ServerAddr));

            if(strcmp(Recv.pRead(),"MANTRA"))
            {
                pLog->_Add("Fail attempt, not MANTRA packet");
                RejectConnection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"This is a Ember Game Server");
                return;
            }

            if(Recv.iRead() != atoi(NET_REVISION))
            {
                RejectConnection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"Server uses newer protocol! Update available.");
                return;
            }

            // TODO: Add reject (PRIVATE SERVER)

            pNewSocket = new CCSocket;
            if(!pNewSocket)
            {
                RejectConnection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"Server can't assign a new socket!");
                return;
            }

            memcpy(&pNewSocket->ToAddr,&ServerAddr,sizeof(ServerAddr));

			pNewSocket->iSocket=zOpenSocket(0);
            if(pNewSocket->iSocket==-1)
			{
				DEL(pNewSocket);
				RejectConnection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"Server can't assign new socket!");
				return;
			}

            pLog->_Add("Created new socket on port %d", pNewSocket->iGetLocalPort());


			Send.Write(0);
			Send.Write(0);
			Send.Write((char)CTL_ACCEPT);
			Send.Write(pNewSocket->iGetLocalPort());
			FinishCtlPacket(&Send);

            if(nSend(iAcceptSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),(struct sockaddr*)&ServerAddr)==-1)
            {
				DEL(pNewSocket);
				pLog->_Add("s_server.cpp -> CSocket::pAccept() -> error making a new connection");
				return;
			}

			//pLog->_Add("Sent OK! (%s)",NET_pAddrToString((struct sockaddr*)&pNewSocket->ToAddr));

			pNewSocket->bCanSend = true;
            pNewSocket->bConnected = true;

            break;

        case CTL_PING:

            Send.Write(0);
            Send.Write(0);
            Send.Write((char)CTL_PING);
            Send.Write((long)Recv.dwRead());
            FinishCtlPacket(&Send);

            if(nSend(iAcceptSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),(struct sockaddr*)&ServerAddr)==-1)
            {
                pLog->_Add("s_server.cpp -> respond to pingrequest failed");
            }
            return;

        case CTL_REJECT:
            pLog->_Add("CTL_REJECT ?");

            break;

        default:
            RejectConnection(iAcceptSocket,(struct sockaddr*)&ServerAddr,"This is a Ember gameserver");
            pLog->_Add("UNKNOWN CTL_PACKET (%d) received",cType);
            break;
    }

    client=new C_GSC(pLog);
	if(!client)	{        pLog->_Add("s_server.cpp -> Can't create client");        return;    }

    if(pFirstPlayer)
        client->pNext=pFirstPlayer;
    pFirstPlayer=client;

	memcpy(&client->FromAddr,&ServerAddr,sizeof(sockaddr_in));

    client->pSocket=pNewSocket;

}

////////////////////////////////////////////////////////

void CServer::RejectConnection(int iReSocket,sockaddr *pReAddr,char *pText)
{
    CPacket Send(NET_DATAGRAMSIZE);
    if(!pText) return;

    Send.Write(0);
    Send.Write(0);
    Send.Write((char)CTL_REJECT);
    Send.Write((char *)pText);
    FinishCtlPacket(&Send);

    if(nSend(iReSocket,Send.pGetPacketBuffer(),Send.iGetCurSize(),pReAddr)==-1)
    {
        pLog->_Add("Reject message failed (%s)",pText);
    }
}

////////////////////////////////////////////////////////

void CServer::SendAll(CPacket *pPacket,float fBlockTime)
{
    C_GSC *pClient;
    pClient=pFirstPlayer;

    while(pClient)
    {
        pClient->pSocket->SendUnreliableMessage(pPacket);
        pClient=pClient->pNext;
    }
}

////////////////////////////////////////////////////////

void CServer::Disconnect(C_GSC *client,bool bsave)
{
//    Logout(client,bsave);
}

//////////////////////////////////////////////////////
void CServer::ReportToMaster(void)
{
/*    static bool starter=false; static long dwReportTimer=GetTickCount();
    // if(!dlcs_strcasecmp(gmvar("sys.master_report"),"yes")) return;
	// if(!pFMMS_Connection) return;
    if((GetTickCount()-dwReportTimer)<15000) { if(!starter) starter=true; else return; }
    dwReportTimer=GetTickCount(); //pLog->_Add("Reported to master");
    pFMMS_Connection->Connect("127.0.0.1","40404");
	*/
}

//////////////////////////////////////////////////////////////////////////////////////

void CServer::SaveWorld(void)
{

	static long dwSaveTimer=dlcs_get_tickcount();
	if(	(dlcs_get_tickcount()-dwSaveTimer) > 100000 )
	{
		dwSaveTimer=dlcs_get_tickcount();
		pLog->_Add("World saved...");
	}

}

//////////////////////////////////////////////////////////////////////////////////////

void CServer::LoadWorld(void)
{
	pLog->_Add("World loaded...");
}

//////////////////////////////////////////////////////////////////////////////////////

C_GSC *CServer::GetClient(char *user_name)
{
    C_GSC *find = pFirstPlayer;
    while(find)
    {
        if(dlcs_strcasecmp(find->data->username,user_name)) return find;
        find=find->pNext;
    }
    return 0;
}

void CServer::KickAll(char *reason)
{
    C_GSC *pClient;

    pClient = pFirstPlayer;
    if(pClient) pLog->_Add("Kicking online players");
    while(pClient)
    {

        pLog->_Add("       ....%s", pClient->data->username );

    //        if(pClient_looper->pSocket)
  //      {
            /*Send.Reset();
            Send.Write((char)NETMSG_CLIENTSHUTDOWN);
            Send.Write((int) pClient_looper->sid);
            Send.Write((char *)reason);//reason
            pClient_looper->pSocket->SendUnreliableMessage(&Send);
            */
    //    }

        Disconnect(pClient,1);
        pClient=pClient->pNext;
    }
    pLog->_Add("All players kicked!");
}

//////////////////////////////////////////////////////////////////////////////////////
void CServer::Kick(char *name,char *reason)
{
    C_GSC *client = pFirstPlayer;
    while(client)
    {
        if(dlcs_strcasecmp(client->data->username,name))
        {
        /*
            if(strlen(client->toon->t_name))
                pLog->_Add("%s(%s) kicked",name,client->toon->t_name);
            else
                pLog->_Add("%s kicked",name);

                */

/*
            Send.Reset();
            Send.Write((char)NETMSG_CLIENTSHUTDOWN);
            Send.Write((int)1);
            Send.Write((char *)reason); // reason
            client->pSocket->SendUnreliableMessage(&Send);
            */

            Disconnect(client,1); //Logout(client,1);

			/* client=pServer->pFirstPlayer;
            Send.Reset();
            Send.Write((char)NETMSG_VIS_REMOVE);
            Send.Write((int)i);
            while(client)
            {
                client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                client=client->pNext;
            }*/


            return;
        }
        client=client->pNext;
    }
    pLog->_Add("%s is not online!",name);
    return;
}

//////////////////////////////////////////////////////////////////////////////////////
void CServer::MovePlayer(C_GSC *client,int x,int y,int z,int teleport)
{

	if(!client) return;
}
//////////////////////////////////////////////////////////////////////////////////////
void CServer::Say(C_GSC *client,char *msg,u_char r,u_char g,u_char b,char *font)
{

}
//////////////////////////////////////////////////////////////////////////////////////
void CServer::Msg(char *name,char *msg)
{
	C_GSC *pClient;
	pClient = pFirstPlayer;
	while(pClient)
	{
		if(dlcs_strcasecmp(pClient->data->username,name))
		{
			Announce(pClient,msg,255,255,255);
			return;
		}
		pClient=pClient->pNext;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void CServer::Announce(C_GSC *pClient,char *msg,u_char r,u_char g,u_char b)
{
	if(pClient)
	{
		Send.Reset();
		Send.Write((char)NETMSG_SYSTEMMSG);
		Send.Write((char *)msg);
		Send.Write((char)0);
		pClient->pSocket->SendUnreliableMessage(&Send);
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void CServer::GlobalAnnounce(C_GSC *pClient,char *msg,u_char r,u_char g, u_char b)
{
    C_GSC  *pClient_looper;
    if(!pClient) return;
	pClient_looper = pFirstPlayer;
	while(pClient_looper)
	{
		Announce(pClient_looper,msg,255,255,255);
		pClient_looper=pClient_looper->pNext;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void CServer::AdminAnnounce(char *msg,u_char r,u_char g, u_char b)
{
    C_GSC  *pClient_looper;
	Send.Reset();
	Send.Write((char)NETMSG_SYSTEMMSG);
	Send.Write((char *)msg);
	Send.Write((char)0);

	pClient_looper = pFirstPlayer;
	while(pClient_looper)
	{
		//pClient_looper->pSocket->SendUnreliableMessage((CPacket *)&Send);
		pClient_looper=pClient_looper->pNext;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void CServer::AdminToPlayer(C_GSC *pClient,char *msg,u_char r,u_char g, u_char b)
{
	if(!pClient) return;
	Send.Reset();
	Send.Write((char)NETMSG_SYSTEMMSG);
	Send.Write((char *)msg);
	Send.Write((char)0);
	//pClient->pSocket->SendUnreliableMessage((CPacket *)&Send);
}
//////////////////////////////////////////////////////////////////////////////////////
void CServer::DoClient(void)
{
    static C_GSC *pClient=0;
    if(!pClient) { pClient = pFirstPlayer; }
    if(pClient)
    {
        // pLog->_Add("DoClient(%s)(%d)",pClient->username, pClient->pSocket->iGetLocalPort());
        pClient->do_net();
        pClient=pClient->pNext;
    }
}
//////////////////////////////////////////////////////////////////////////////////////
void CServer::RemoveDeadPlayers(void)
{
	C_GSC *pClient=0;
	C_GSC *pDelete=0;

	pClient = pFirstPlayer;
	if(!pClient) return;

	while(pClient->pNext)
	{
        if(pClient->pNext->bDelete)
        {
            pDelete=pClient->pNext;

            if(pClient->pNext->pNext)
                pClient->pNext=pClient->pNext->pNext;

                pLog->_Add("Removed %s:%d (username[%s])",
                    inet_ntoa(pDelete->pSocket->ToAddr.sin_addr),
                    ntohs(pDelete->pSocket->ToAddr.sin_port),
                    pDelete->data->username
                );

                DEL(pDelete);


        }
        if(pClient->pNext)
            pClient = pClient->pNext;
	}
	if(pClient==pFirstPlayer)
	{
        if(pClient)
        {
            if(pClient->bDelete)
            {
                pLog->_Add("Removed %s:%d (username[%s])",
                    inet_ntoa(pFirstPlayer->pSocket->ToAddr.sin_addr),
                    ntohs(pFirstPlayer->pSocket->ToAddr.sin_port),
                    pFirstPlayer->data->username
                );

                DEL(pFirstPlayer);
            }
        }
	}
}
//////////////////////////////////////////////////////////////////////////////////////

void CServer::DoWorld(void)
{
	static long dwWorldSaveTimer=dlcs_get_tickcount();

	SaveWorld();
}

bool CServer::loaddata(void){

    FILE* fp;
    char In[256];
    vector <string> v;
    fp=fopen("server.ini","rt");
    if(!fp) return false;
    while(1)
	{
		if(!fgets(In,255,fp)) break;
        v = explode("=",In);
        if(v.size()==2){

            if(dscc(v[0].c_str(),"b_new_accounts")){
                b_new_accounts=atoi(v[1].c_str());
                continue;
            }
            if(dscc(v[0].c_str(),"port")){
                port=atoi(v[1].c_str());
                continue;
            }
        }

    }


    fclose(fp);
    return true;
}

bool CServer::savedata(void){

    FILE *fp;
    fp=fopen("server.ini","wt");
    if(!fp) return false;
    fputs("** Mantra Server Configuration\n",fp);
    fputs(va("b_new_accounts=%d\n",b_new_accounts),fp);
    fputs(va("port=%d\n",port),fp);
    fclose(fp);
    return true;
}


/////////////////////////////////////////////////////////
void CServer::RemoveAccount(char *szName)
{
    char szTemp[1024];
    FILE *fp1;
    FILE *fp2;
    char In[1024];
    char *Tok;
    char Save[1024];
    int i;

    sprintf(szTemp,"users%c%s",PATH_SEP,szName);
    strcpy(Save,szTemp);
    sprintf(Save,"%s%c%s.fcd",szTemp,PATH_SEP,szName);
    remove(Save);

    for(i=0;i<9;i++)
    {
        sprintf(Save,"%s%c%s.cs%d",szTemp,PATH_SEP,szName,i);
        remove(Save);
        sprintf(Save,"%s%c%s.it%d.can",szTemp,PATH_SEP,szName,i);
        remove(Save);
    }
    rmdir(szTemp);

    sprintf(szTemp,"users%cpassword.dat",PATH_SEP);
    fp1=fopen(szTemp,"rt");
    sprintf(szTemp,"users%cpassword.tmp",PATH_SEP);
    fp2=fopen(szTemp,"wt");
    if(fp1)
    {
        while(fgets(In,255,fp1))
        {
            strcpy(Save,In);
            Tok=strtok(In,"(,)");
            if(!strcmp(Tok,"Data"))
            {
                Tok=strtok(NULL,"(,)");
                if(!strcmp(Tok,szName))
                {
                    continue;
                }
            }
            fputs(Save,fp2);
        }
        fclose(fp1);
        fclose(fp2);
    }
    sprintf(szTemp,"users%cpassword.dat",PATH_SEP);
    remove(szTemp);
    sprintf(In,"users%cpassword.tmp",PATH_SEP);
    rename(In,szTemp);

}
/******************************************************************
 ** Increment the global serial number counter and return it     **
 ******************************************************************/
u_long CServer::GetSerialNumber(bool bSave)
{
    static u_long iSerialNumber=0;
    char szSerialNumber[256];
    FILE *fp;

    if(iSerialNumber==0)  // only to save the serial number the for the first time
    {
        fp=fopen("serial.dat","rt");
        if(!fp)
        {
            // not found, create a new item.id file and put a 1
            fp=fopen("serial.dat","wt");
            if(!fp)
            {
                pLog->_Add("Can't create serial.dat! #1");
                return 0;
            }
            iSerialNumber=1;
            pLog->_Add("Serial number set to %d",iSerialNumber);
            sprintf(szSerialNumber,"%d",iSerialNumber);
            fputs(szSerialNumber,fp);
            fputs("=================================================\n",fp);
            fputs("Serial number tracking file. DO NOT erase or modify.\n",fp);

            fclose(fp);
            return iSerialNumber;
        }
        fgets(szSerialNumber, 100, fp);
        fclose(fp);
        iSerialNumber=atoi(szSerialNumber);
    }

    if(bSave) // this is called at the end of the program to save the state of iserialnumber
    {
        fp=fopen("serial.dat","wt");
        if(!fp)
        {
            pLog->_Add("Can't create serial.dat! #2");
            return 0;
        }
        sprintf(szSerialNumber,"%d\n",iSerialNumber);
        fputs(szSerialNumber,fp);
        fputs("=================================================\n",fp);
        fputs("Serial number tracking file. DO NOT erase or modify.\n",fp);
        fclose(fp);
    }

    iSerialNumber++;
    // pLog->_Add("Serial number incremented to %d",iSerialNumber);
    return iSerialNumber;
}


char *CServer::SysVar(char *v)
{
    return("nullz");
	//return db->parse(db->select(va("system var %s",v)),"value");
}

