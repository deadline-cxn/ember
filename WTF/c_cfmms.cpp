/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "c_cfmms.h"
#include "c_socket.h"
#ifdef __linux__
#include "s_gnu.h"
#endif
#include "c_cfmgs.h"
#include "c_serverinfo.h"

#ifdef _FMMS_WEB_
    extern bool bQuitFMMS;
    extern char szMasterIPAddress[1024];
    extern char szMasterPort[1024];
    extern char szBackground[1024];
    extern char szCSS[1024];
    extern char szMethod[1024];
    extern char szMoreInfo[1024];
    extern char szNetVersion[1024];
    static int  iTotalNumServers;
    extern "C" void FmmsOut(char *fmt, ...);
#endif

ServerData::ServerData(void)
{
#ifdef _EMBER_CLIENT_
    pConnection=NULL;
    pConnection=new C_FMGS;
    dwPing=9931;
    key=-1;

#endif
    pNext=NULL;
}

ServerData::~ServerData(void)
{
#ifdef _EMBER_CLIENT_
    DEL(pConnection);
#endif
}


//////////////////////////////////////////////////////

C_FMMS::C_FMMS(void)
{
    pSocket = NULL;
    bLoggedin = false;
    bSystemBusy = false;
    bHasRequestedList=false;
    bHasRecievedList=false;

    pServerList = NULL;
    pServerList = new ServerData[MAX_SERVERS+1];
    ClearServerList();
    //DLog("Created C_FMMS!");
}

//////////////////////////////////////////////////////

C_FMMS::~C_FMMS()
{
    if(pSocket) DEL(pSocket);

    if(pServerList)
    {
        ClearServerList();
        delete [] pServerList;
        pServerList=NULL;
    }
    //DLog("Deleted C_FMMS!");
}

//////////////////////////////////////////////////////

int C_FMMS::Connect(char *pHost, char *pPort)
{
    static long fTryTime;
    int ret=0;
    int iLength;
    int iFlags;

    CInetAddress RetAddr;
    CPacket *pSend = new CPacket(16384);
    CPacket *pRec  = new CPacket(16384);

    if(!pSocket)
    {
        pSocket = new CCSocket();
        pSocket->bConnecting=false;
    }

    if(pSocket->bConnecting==false)
    {
        //DEL(pSocket);
        //pSocket=new CCSocket();

        pSocket->bConnecting=true;
        pSocket->bCanSend = true;
        pSocket->pAddr = CInetAddress::pGetByName(pHost);
        if(pSocket->pAddr == NULL)
        {
            //Log("FMMS connection can't resolve hostname!");
            DEL(pSend); DEL(pRec); return 0;
        }
        if(pSocket->iSocket == -1)
        pSocket->iSocket = NET_OpenSocket(0);
        if(pSocket->iSocket == -1)
        {
            //Log("FMMS connection can't open network socket!");
            DEL(pSend); DEL(pRec); return 0;
        }

        //Log("FMMS socket:[%d]",pSocket->iSocket);

        NET_SetSocketPort(&pSocket->pAddr->stAddr,atoi(pPort));

#ifdef _FMMS_WEB_

        FmmsOut("<p>FMMS (Web) %s by Seth Parson</p>\n",VERSION);
        FmmsOut("<hr>\n");

        pSend->Reset();
        pSend->Reset();
        pSend->Write(0);  // save header space which is filled in later
        pSend->Write(0);
        pSend->Write((char)MS_NETMSG_SERVERINFORMATION_GET);
        pSend->Write("EMBER");
        pSend->Write(atoi(szNetVersion));
        pSocket->FinishCtlPacket(pSend);
        pSocket->bConnected = false;
        pSocket->bCanSend   = false;
        pSocket->dConnectRetries = 0;

        if(NET_Write(pSocket->iSocket,pSend->pGetPacketBuffer(),pSend->iGetCurSize(),&pSocket->pAddr->stAddr)==-1)
        {
            FmmsOut("<p>Error communicating with FMMS, try again.</p>\n");
            DEL(pSend); DEL(pRec); return 0;
        }

		FmmsOut("<table>\n");


#endif

#ifdef _EMBER_CLIENT_

        pSend->Reset();
        pSend->Reset();
        pSend->Write(0);  // save header space which is filled in later
        pSend->Write(0);
        pSend->Write((char)MS_NETMSG_SERVERINFORMATION_GET);
        pSend->Write("EMBER");
        pSend->Write(atoi(NET_REVISION));
        pSocket->FinishCtlPacket(pSend);
        pSocket->bConnected = false;
        pSocket->bCanSend = false;
        pSocket->dConnectRetries = 0;
        if(NET_Write(pSocket->iSocket,pSend->pGetPacketBuffer(),pSend->iGetCurSize(),&pSocket->pAddr->stAddr)==-1)
        {
            DEL(pSend); DEL(pRec); return 0;
        }
#endif

#ifdef _EMBER_SERVER

        pSend->Reset();
        pSend->Write(0);  // save header space which is filled in later
        pSend->Write(0);
        pSend->Write((char)CTL_SERVERINFO_PUT);
        pSend->Write("EMBER");
        pSend->Write(atoi(NET_REVISION));

        pSend->Write((char *)l_sys("name")); //l_sys("name);
        pSend->Write((char *)l_sys("ip_address"));
        pSend->Write((char *)l_sys("server_id"));
        pSend->Write((char *)l_sys("resource_id"));
        pSend->Write((char *)l_sys("owner"));
        pSend->Write((char *)l_sys("owner_email"));
        pSend->Write((char *)l_sys("owner_website"));
        pSend->Write((char *)l_sys("port"));
        pSend->Write((char *)l_sys("maximum_players"));
        pSend->Write((char *)l_sys("players"));
        pSend->Write((char *)l_sys("character_slots"));
        pSend->Write((char *)l_sys("inactivity_timeout"));
        pSend->Write((char *)l_sys("world_save_timer"));
        pSend->Write((char *)l_sys("allow_new_users"));
        pSend->Write((char *)l_sys("allow_downloads"));
        pSend->Write((char *)"teleports?");
        pSend->Write((char *)"ftp_ip_address?");
        pSend->Write((char *)l_sys("minimum_char_age"));
        pSend->Write((char *)l_sys("start_time"));
        pSend->Write((char *)l_sys("operating_system"));
        pSend->Write((char *)VERSION);
        pSend->Write((char *)FMGS_REVISION);
        pSend->Write((char *)NET_REVISION);
        pSend->Write((char *)l_sys("locked"));
        pSend->Write((char *)"NULL");

        pSocket->bConnecting=false;
        pSocket->FinishCtlPacket(pSend);
        NET_Write(pSocket->iSocket,pSend->pGetPacketBuffer(),pSend->iGetCurSize(),&pSocket->pAddr->stAddr);
        //DEL(pSocket);
        DEL(pSend); DEL(pRec);
        return 0;
#endif

        fTryTime = dlcs_get_tickcount();
    }

    ret = NET_Read(pSocket->iSocket,pRec->pGetPacketBuffer(),pRec->iGetMaxSize(),&RetAddr.stAddr);

    if(ret==-1) { DEL(pSend); DEL(pRec); return 0; }
    if(ret > 0)   // we got something
    {
        pRec->SetCurSize(ret);
        iLength = ntohl(pRec->iRead());
        iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
        iLength &= NET_FLAG_LENGTH_MASK;
        if (!(iFlags & NET_FLAG_CTL)) { ret = 0; }
        if (iLength != ret) { ret = 0; }
        if (iLength < (int)(sizeof(int)*2+sizeof(char)*2)) { ret = 0; }
    }
    if (!ret) { DEL(pSend); DEL(pRec); return 0; }

    pRec->iRead();
    char ctltype = pRec->cRead();

    if (ctltype == CTL_REJECT)
    {
        DEL(pSend);
        DEL(pRec);
        return 0;
    }

    if (ctltype == CTL_SERVERINFO_RESET)
    {
        pSocket->bConnected=true;
        DEL(pSend);
        DEL(pRec);
        return 0;
    }

    if (ctltype == CTL_SERVERINFO)
    {
        if(iNumServers>MAX_SERVERS)
        {
            DEL(pSend);
            DEL(pRec);
            return 0;
        }
        strcpy(pServerList[iNumServers].name                ,pRec->pRead());
        strcpy(pServerList[iNumServers].ip_address          ,pRec->pRead());
        strcpy(pServerList[iNumServers].media               ,pRec->pRead());
        strcpy(pServerList[iNumServers].resource_id         ,pRec->pRead());
        strcpy(pServerList[iNumServers].owner               ,pRec->pRead());
        strcpy(pServerList[iNumServers].owner_email         ,pRec->pRead());
        strcpy(pServerList[iNumServers].owner_website       ,pRec->pRead());
        strcpy(pServerList[iNumServers].port                ,pRec->pRead());
        strcpy(pServerList[iNumServers].maximum_players     ,pRec->pRead());
        strcpy(pServerList[iNumServers].players             ,pRec->pRead());
        strcpy(pServerList[iNumServers].character_slots     ,pRec->pRead());
        strcpy(pServerList[iNumServers].inactivity_timeout  ,pRec->pRead());
        strcpy(pServerList[iNumServers].world_save_timer    ,pRec->pRead());
        strcpy(pServerList[iNumServers].allow_new_users     ,pRec->pRead());
        strcpy(pServerList[iNumServers].allow_downloads     ,pRec->pRead());
        strcpy(pServerList[iNumServers].teleports           ,pRec->pRead());
        strcpy(pServerList[iNumServers].ftp_ip_address      ,pRec->pRead());
        strcpy(pServerList[iNumServers].minimum_char_age    ,pRec->pRead());
        strcpy(pServerList[iNumServers].start_time          ,pRec->pRead());
        strcpy(pServerList[iNumServers].operating_system    ,pRec->pRead());
        strcpy(pServerList[iNumServers].version             ,pRec->pRead());
        strcpy(pServerList[iNumServers].revision            ,pRec->pRead());
        strcpy(pServerList[iNumServers].net_revision        ,pRec->pRead());
        strcpy(pServerList[iNumServers].locked              ,pRec->pRead());

#ifdef _FMMS_WEB_

        iTotalNumServers++;

        if(FM_StrCaseCmp("getmoreinfo",szMethod))
        {
            if(FM_StrCaseCmp(pServerList[iNumServers].name,szMoreInfo))
            {
                FmmsOut("More info for %s:<br>",pServerList[iNumServers].name);

                switch(pServerList[iNumServers].operating_system[0])
                {
                    case 'W':
                        FmmsOut("<img src=\"http://www.defectiveminds.com/images/win32.gif\" width=\"64\">");
                        break;
                    case 'L':
                        FmmsOut("<img src=\"http://www.defectiveminds.com/images/linux.gif\" width=\"64\">\n");
                        break;
                    case 'F':
                        FmmsOut("<img src=\"http://www.defectiveminds.com/images/freebsd.gif\" width=\"64\">\n");
                        break;
                    default:
                        break;
                }


                FmmsOut("<tr><td>IP Address      </td><td>%s</td></tr>\n",pServerList[iNumServers].ip_address);
                FmmsOut("<tr><td>Port            </td><td>%s</td></tr>\n",pServerList[iNumServers].port);
                FmmsOut("<tr><td>FTP IP Address  </td><td>%s</td></tr>\n",pServerList[iNumServers].ftp_ip_address);
                FmmsOut("<tr><td>Server Started  </td><td>%s</td></tr>\n",pServerList[iNumServers].start_time);
                FmmsOut("<tr><td>Operating System</td><td>%s</td></tr>\n",pServerList[iNumServers].operating_system);
                FmmsOut("<tr><td>Unique ID       </td><td>%s</tr>\n",pServerList[iNumServers].unique_id);
                FmmsOut("<tr><td>Resource ID     </td><td>%s</td></tr>\n",pServerList[iNumServers].resource_id);
                FmmsOut("<tr><td>Owner           </td><td>%s</td></tr>\n",pServerList[iNumServers].owner);
                FmmsOut("<tr><td>Owner Email     </td><td><a href=mailto:%s>%s</a></td></tr>\n",pServerList[iNumServers].owner_email,pServerList[iNumServers].owner_email);
                FmmsOut("<tr><td>Owner Website   </td><td><a href=http://%s>%s</a></td></tr>\n",pServerList[iNumServers].owner_website,pServerList[iNumServers].owner_website);
                FmmsOut("<tr><td>Maximum Players </td><td>%s</td></tr>\n",pServerList[iNumServers].maximum_players);
                FmmsOut("<tr><td>Current Players </td><td>%s</td></tr>\n",pServerList[iNumServers].players);
                FmmsOut("<tr><td>Character Slots </td><td>%s</td></tr>\n",pServerList[iNumServers].character_slots);
                FmmsOut("<tr><td>Inactivity Timer</td><td>%s</td></tr>\n",pServerList[iNumServers].inactivity_timeout);
                FmmsOut("<tr><td>World Save Timer</td><td>%s</td></tr>\n",pServerList[iNumServers].world_save_timer);
                FmmsOut("<tr><td>Allow New Users </td><td>%s</td></tr>\n",pServerList[iNumServers].allow_new_users);
                FmmsOut("<tr><td>Locked          </td><td>%s</td></tr>\n",pServerList[iNumServers].locked);
                FmmsOut("<tr><td>Allow Downloads </td><td>%s</td></tr>\n",pServerList[iNumServers].allow_downloads);
                FmmsOut("<tr><td>Obsolete Data   </td><td>%s</td></tr>\n",pServerList[iNumServers].teleports);
                FmmsOut("<tr><td>Minimum Character Age </td><td>%s</td></tr>\n",pServerList[iNumServers].minimum_char_age);
                FmmsOut("<tr><td>Version         </td><td>%s</td></tr>\n",pServerList[iNumServers].version);
                FmmsOut("<tr><td>Patch           </td><td>%s</td></tr>\n",pServerList[iNumServers].revision);
                FmmsOut("<tr><td>Net Revision    </td><td>%s</td></tr>\n",pServerList[iNumServers].net_revision);

            }
        }

        else
        {

            FmmsOut("<tr><td>");

            switch(pServerList[iNumServers].operating_system[0])
            {
                case 'W':
                    FmmsOut("<img src=\"http://www.defectiveminds.com/images/win32.gif\" width=\"16\">\n");
                    break;
                case 'L':
                    FmmsOut("<img src=\"http://www.defectiveminds.com/images/linux.gif\" width=\"16\">\n");
                    break;
                case 'F':
                    FmmsOut("<img src=\"http://www.defectiveminds.com/images/freebsd.gif\" width=\"16\">\n");
                    break;
                default:
                    break;
            }
		    FmmsOut("</td><td>");
		    FmmsOut("<a href=\"http://%s/\" target=\"_blank\">%s</a><br>\n",pServerList[iNumServers].owner_website,pServerList[iNumServers].name);
		    FmmsOut("</td><td>");
            FmmsOut("IP[%s:%s] ",pServerList[iNumServers].ip_address,pServerList[iNumServers].port);
            FmmsOut("Admin[<a href=mailto:%s>%s</a>] \n",pServerList[iNumServers].owner_email,pServerList[iNumServers].owner);
		    FmmsOut("</td><td>");
		    FmmsOut("Players: %s out of %s",pServerList[iNumServers].players,pServerList[iNumServers].maximum_players);
            FmmsOut("<td><a href=\"http://www.defetiveminds.com/cgi-bin/fmmsw.cgi?Submit=FMMS+WEB&method=getmoreinfo&getmoreinfo=%s&ip=%s&port=%s&netversion=%s\">More info</a></td>\n",pServerList[iNumServers].name,szMasterIPAddress,szMasterPort,szNetVersion);
            FmmsOut("</td></tr>");
        }

#endif
        iNumServers++;
        DEL(pSend); DEL(pRec); return 0;
    }

    if (ctltype == CTL_SERVERINFO_END)
    {
#ifdef _FMMS_WEB_
		FmmsOut("</table>\n");
        FmmsOut("<hr>\n");
        FmmsOut("<p>Total number of game servers reporting [%d]</p>\n",iTotalNumServers);
        FmmsOut("<p>FMMS and FMMS Web interface copyright %s</p><br>\n",COPYRIGHT);
        FmmsOut("<p><a href=\"http://www.defectiveminds.com/cgi-bin/fmmsw.cgi?Submit=FMMS+WEB&ip=%s&port=%s&netversion=%s\">Refresh search</a></p><p>&nbsp;</p>\n",szMasterIPAddress,szMasterPort,szNetVersion);

        bQuitFMMS=true;
#endif
        DEL(pSend); DEL(pRec); return 0;
    }
    DEL(pSend); DEL(pRec); return 0;
}

//////////////////////////////////////////////////////

void C_FMMS::Disconnect(void)
{
    if(pSocket)
    {
        pSocket->Disconnect();
        DEL(pSocket);
    }
}

//////////////////////////////////////////////////////

void C_FMMS::RequestServerList(void)
{
    CPacket *pSend=new CPacket(NET_DATAGRAMSIZE);
    if(eGetState()==MS_CTRL_CONNECTED)
    {
        if(!bHasRequestedList)
        {
            pSend->Reset();
            pSend->Write((char)MS_NETMSG_GET_SERVER_LIST); // MS_NETMSG_GET_SERVER_LIST
            pSend->Write((char)SI_GET_SERVER_LIST);
            pSocket->SendUnreliableMessage(pSend);
            bHasRequestedList=1;
        }
    }
    DEL(pSend);
}

//////////////////////////////////////////////////////////////////////////

int C_FMMS::iGetMessage()
{
    if(!pSocket)
    {
        return 0;
    }
    int i;
    if (eGetState() == MS_CTRL_NOTCONNECT)
        return 0;

    i = pSocket->iGetMessage();
    if(i==-1) i=0;
    return i;
}

//////////////////////////////////////////////////////////////////////////

eFMMSConnectionState C_FMMS::eGetState()
{
    if(!this) return MS_CTRL_NOTCONNECT;
    if(!pSocket)
        return MS_CTRL_NOTCONNECT;
    //if(!bLoggedin)
        //return CON_LOGGINPROC;
    //if(bSystemBusy)
        //return CON_SYSBUSY;
    return MS_CTRL_CONNECTED;
}

//////////////////////////////////////////////////////////////////////////

void C_FMMS::SetState(eFMMSConnectionState eNewState)
{
    if((pSocket)&&(eNewState==MS_CTRL_CONNECTED))
        bLoggedin = true;
    if(eNewState == MS_CTRL_NOTCONNECT)
    {
        bLoggedin = false;
        if (pSocket)
        {
            DEL(pSocket);
        }
    }
    if(eNewState == MS_CTRL_SYSBUSY)
        bSystemBusy = true;
}

///////////////////////////////////////////////////////////////////////////

void C_FMMS::ClearServerList(void)
{
    int i;

    for(i=0;i<MAX_SERVERS;i++)
    {
        memset(pServerList[i].name                ,0,255);
        memset(pServerList[i].ip_address          ,0,255);
        memset(pServerList[i].media               ,0,255);
        memset(pServerList[i].resource_id         ,0,255);
        memset(pServerList[i].owner               ,0,255);
        memset(pServerList[i].owner_email         ,0,255);
        memset(pServerList[i].owner_website       ,0,255);
        memset(pServerList[i].port                ,0,255);
        memset(pServerList[i].maximum_players     ,0,255);
        memset(pServerList[i].players             ,0,255);
        memset(pServerList[i].character_slots     ,0,255);
        memset(pServerList[i].inactivity_timeout  ,0,255);
        memset(pServerList[i].world_save_timer    ,0,255);
        memset(pServerList[i].allow_new_users     ,0,255);
        memset(pServerList[i].allow_downloads     ,0,255);
        memset(pServerList[i].teleports           ,0,255);
        memset(pServerList[i].ftp_ip_address      ,0,255);
        memset(pServerList[i].minimum_char_age    ,0,255);
        memset(pServerList[i].start_time          ,0,255);
        memset(pServerList[i].operating_system    ,0,255);
        memset(pServerList[i].version             ,0,255);
        memset(pServerList[i].revision            ,0,255);
        memset(pServerList[i].net_revision        ,0,255);
        memset(pServerList[i].locked              ,0,255);
        memset(pServerList[i].user                ,0,255);
        memset(pServerList[i].password            ,0,255);
#ifdef _FMCLIENT_
        DEL(pServerList[i].pConnection);
#endif
    }

    iNumServers=0;
}

#ifdef _EMBER_CLIENT_
long C_FMMS::PingServer(int i)
{
    if(i>MAX_SERVERS) return 9996;
    if(!pServerList[i].pConnection) pServerList[i].pConnection=new C_FMGS;
    if(!pServerList[i].pConnection) return 9998;
    pServerList[i].dwPing=pServerList[i].pConnection->Ping(pServerList[i].ip_address,pServerList[i].port,pServerList[i].dwPing,pServerList[i].bPinging,pServerList[i].dwFuncTimer,pServerList[i].dwWaitTimer);
    //Log("%s:%s->%d",pServerList[i].ip_address,pServerList[i].port,pServerList[i].dwPing);
    return pServerList[i].dwPing;
}

long C_FMMS::PingServer(ServerData *pB)
{
    if(!pB) return 9992;
    if(!pB->pConnection) pB->pConnection=new C_FMGS;
    if(!pB->pConnection) return 9997;
    pB->dwPing=pB->pConnection->Ping(pB->ip_address,pB->port,pB->dwPing,pB->bPinging,pB->dwFuncTimer,pB->dwWaitTimer);
    return pB->dwPing;
}

#endif


