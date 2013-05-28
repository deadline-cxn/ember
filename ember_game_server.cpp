/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "ember_game_server.h"
#include "c_net.h"
#include "dlstorm.h"
#include "c_log.h"
#include "s_client.h"
#include "b_map.h"
#include "s_server.h"

#include "lualib.h"

#include "fm_util.h"
#include "c_gaf.h"

CGAF		*File;

CEmMap      *pMap;
CEmMap      *pMapBuffer;
CEmMap      *pFragMap;
CEmMap      *pFirstFragMap;
CEmMap      *pDeleteFragMap;

CServer     *fmgs;

CPacket     Send(8192);

C_FM_CLIENT *client;
C_FM_CLIENT *other_client;

char        szLastAddress[1024];
char        motd[1024];
time_t      fmgsTime;
struct      tm *tTime;
bool        bQuit;
bool        bQuiet;
bool        bRestart;

int				ConsoleHistoryPosition;
typedef struct	ConHistory { char text[1024]; };
ConHistory	   *ConsoleHistory;


#ifdef __linux__
#include "s_linux.h"
#include "s_gnu.h"
void ConsoleSetup(void) { _kbhit(); }
void ConsoleShutDown(void) { close_keyboard(); }
#endif

#ifdef _WIN32

BOOL WINAPI HandleCTRL_C(DWORD dwCtrlType)
{
    switch(dwCtrlType)
    {

        case CTRL_BREAK_EVENT:
            bQuit=true;
            Log("Break event killed server!");
            return true;

        case CTRL_SHUTDOWN_EVENT:
            bQuit=true;
            Log("Shutdown event killed server!");
            return true;

        case CTRL_LOGOFF_EVENT:
            bQuit=true;
            Log("Logoff event killed server!");
            return true;

        case CTRL_CLOSE_EVENT:
            bQuit=true;
            Log("Mouse [X] killed server!");
            return true;

        case CTRL_C_EVENT:
            bQuit=true;
            Log("CTRL-C killed server!");
            return true;

        default:
            break;
    }
    return false;
}


void ConsoleSetup(void)
{
    // Change window title for windows version, and setup ctrl handler
    SetConsoleCtrlHandler(HandleCTRL_C,TRUE);
    SetConsoleTitle(va("Ember Game Server %s(%s) Net Revision(%s) %s",VERSION,CPUSTRING,NET_REVISION,COPYRIGHT));
}
void ConsoleShutDown(void) { }

#endif

char *SysVar(char *v)
{
    return("nullz");
	//return db->parse(db->select(va("system var %s",v)),"value");
}

//////////////////////////////////////////////////////////////////////////////////////
void ShowHelp(void)
{
    printf("Ember Game Server %s %s Help\n",VERSION,COPYRIGHT);
    printf("==============================================================\n");
    printf(" --help, -h, /? = this help\n");
    printf(" --quiet, -q, /q = quiet mode, no console output\n");
    printf("==============================================================\n");
}
//////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{

	std::vector <std::string> vstr;

    bQuiet=false;
    char chr;
    if(argc>1)
    {
        chr=1;

        while(chr<argc)
        {
            if( (!strcmp(argv[chr],"--help")) ||
                (!strcmp(argv[chr],"-h")) ||
                (!strcmp(argv[chr],"/?")) )
            {
                ShowHelp();
                exit(0);
            }

            if( (!strcmp(argv[chr],"-q")) ||
                (!strcmp(argv[chr],"--quiet")) ||
				(!strcmp(argv[chr],"/q")))
            {
                bQuiet=true;
            }
            chr++;
        }
    }

	int threadId=1;
    int i,dx;
    FILE *fp;
    char szTemp[8192];
    char szTemp2[1024];
    char szTemp3[1024];
    char szTemp4[1024];
    char szTemp5[1024];
    char *szToken;
    u_char ch=0;
    memset(szTemp,0,8192);
    char szConsoleInput[1024];
    memset(szConsoleInput,0,1024);
    memset(motd,0,1024);
    sprintf(motd,"motd.txt");
    ConsoleSetup();             // system specific console setup
    randomize();                // set the random seed

    bQuit=false;

    StartUp();

    while(!bQuit) // game loop
    {
        if(fmgs)
			fmgs->Accept();

        // ReportToMaster();

        RemoveDeadPlayers();

		CheckRestart();

        DoClient();

        DoWorld();

        Sleep(2); // sleep 2 msecs so it don't hog cpu time

        if(_kbhit())
        {
            ch=_getch();
            if(ch)
            {
                switch(ch)
                {
                    case 13:
                    case 10:

                        AddToConsoleHistory(szConsoleInput);
                        ConsoleHistoryPosition=0;
                        strcpy(szTemp5,szConsoleInput);
						strcpy(szTemp4,szConsoleInput);
                        memset(szConsoleInput,0,1024);

                        Log(">%s",szTemp5);

						if(szTemp5[0]==0) break;
						// if(szTemp5[0]!='/') { ExecString(szTemp5); break; }
						else
						{
							szToken=strtok(szTemp5," \n\r");
							if(szToken)
							{
								///////////////////////////////////////////////////////////////////////////
								// Local /help

								if( (dlcs_strcasecmp(szToken,"/help")) ||
									(dlcs_strcasecmp(szToken,"?")) )
								{
									Log("[List of available commands]====================================");
									Log("/help............................Show this help");
									Log("/db_tables.......................List tables in database");
									Log("/db_add_table <table name>.......Add new table to database");
									Log("/dbs.............................Saves database");

								//	Log("command(s).......................Execute GM script on command line");
								//	Log("/gmf.............................Execute GM script file");
								//	Log("/who.............................List online players");
								//	Log("/msg <user> <message>............Send message to specific player");
								//	Log("/access <user> <access level #>..Change player's access level");
								//	Log("/kick <user> [message]...........Kick player with optional message");
							//		Log("/ban <user>......................Ban player (will ban user)");
							//		Log("/banip <user>....................Ban player (will ban user's ip address)");
							//		Log("/unban <user>....................Unban player (will unban user)");
							//		Log("/unbanip <ipaddress>.............Unban player (will unban user's ip address)");
							//		Log("/bandomain <domain>..............Ban entire domain");
							//		Log("/unbandomain <domain>............Unban entire domain");
							//		Log("/ipbanlist.......................List all banned ip's");
							//		Log("/domainbanlist...................List all banned domains");
							//		Log("/userbanlist.....................List all banned users");
							//		Log("/nuy.............................Allow new user accounts");
							//		Log("/nun.............................Do not allow new user accounts");
							//		Log("/ann <message>...................Announce a message to the entire server");
							//		Log("/motd [new motd].................View, or change the message of the day");
							//		Log("/users...........................List user accounts");
							//		Log("/deleteuser <username>...........Remove a user account");
									Log("/kill [minutes]..................Kill the server [minutes until kill] (CTRL-C immediate)");
							//		Log("/restart [minutes]...............Restart the server [minutes until restart]");
									Log("======================================================[End List]");
									break;
								}

								///////////////////////////////////////////////////////////////////////////

								if( (dlcs_strcasecmp(szToken,"/time")) )
								{
									Log(dlcs_get_time());
									Log("timestamp: %s",dlcs_timestamp());
								}

								//////////////////////////////////////////

								if( (dlcs_strcasecmp(szToken,"/db_save")) ||
									(dlcs_strcasecmp(szToken,"/dbs")) )
								{

									// db->Save("database.txt");
								}

								//////////////////////////////////////////

								if( (dlcs_strcasecmp(szToken,"/db_add_table_column")) ||
									(dlcs_strcasecmp(szToken,"/dbatc")) )
								{
								    /*
									szToken=strtok(NULL," \n\r");
									if(szToken)
									{
										t=db->GetTable(szToken);
										if(t!=NULL)
										{
											szToken=strtok(NULL," \n\r");
											if(szToken)
											{
												t->AddColumn(szToken);
											}
										}
									}
									*/
								}

								//////////////////////////////////////////

								if( (dlcs_strcasecmp(szToken,"/db_add_table")) ||
									(dlcs_strcasecmp(szToken,"/dbat")) )
								{
									szToken=strtok(NULL," \n\r");
									if(szToken)
									{
										// db->AddTable(szToken);
									}
								}

								//////////////////////////////////////////

								if(dlcs_strcasecmp(szToken,"/db_tables"))
								{

									Log("db tables:");

									// t=db->FirstTable();
									//while(t)
									//{
									//	Log("%s",t->name);
									//	t=db->NextTable();
									//}
								}

								//////////////////////////////////////////

								if( (dlcs_strcasecmp(szToken,"/dbupdate")) ||
									(dlcs_strcasecmp(szToken,"/dbup")) )
								{

									vstr=explode(" ",szTemp4);

									Log(" vstr size = %d",vstr.size() );

									if(vstr.size()<2)
									{
										Log("Usage examples:");
										Log("/dbup t usrs users ");
										Log("    (table USRS renamed to USERS)");

										Log("/dbup c users aged age");
										Log("    (column AGED renamed to AGE in table USERS)");

										Log("/dbup v users age 19 name seth");
										Log("    (AGE updated to 19 in table USERS where NAME=seth)");
									}
									else
									{

										for(int i=0;i<vstr.size();i++)
										{
											Log("[%d][%s]",i,vstr[i].c_str());
										}

										if( (vstr[1]=="table") ||
											(vstr[1]=="t") )
										{
											if(vstr.size()>3)
											{
												Log("old table name[%s] -> new table name[%s]",
												vstr[2].c_str(),	// old table name
												vstr[3].c_str()		// new table name
												);
												//t=db->GetTable((char *)vstr[2].c_str());
												//if(t)
													//strcpy(t->name,vstr[3].c_str());
											}

										}
										if( (vstr[1]=="column") ||
											(vstr[1]=="c") )
										{

										}

										if( (vstr[1]=="value") ||
											(vstr[1]=="v") )
										{
											if(vstr.size()>6)
											{
												Log("table[%s] column[%s] value[%s]",
												vstr[2].c_str(), // table
												vstr[3].c_str(), // op column
												vstr[4].c_str(), // op value
												vstr[5].c_str(), // comp column
												vstr[6].c_str()  // comp value
												);

												/*

												t=db->GetTable((char *)vstr[2].c_str());
												if(t)
												{

													row=db->select(
														va("%s %s %s",
														(char *)vstr[2].c_str(),
														(char *)vstr[5].c_str(),
														(char *)vstr[6].c_str()
														));
													db->row_update(row,
														(char *)vstr[3].c_str(),
														(char *)vstr[4].c_str()
														);
												}
												*/
											}
										}
									}
								}

								//////////////////////////////////////////

								if(dlcs_strcasecmp(szToken,"/deleteuser"))
								{
									szToken = strtok(NULL," \n\r");
									if(szToken) RemoveAccount(szToken);
									break;
								}

								//////////////////////////////////////////

								if(dlcs_strcasecmp(szToken,"/ann"))
								{
									szToken=strtok(NULL,"\n\r");
									if(szToken)
									{
										AdminAnnounce(szToken,0,255,0);
										Log(szToken);
									}
									break;
								}

								//////////////////////////////////////////

								if(dlcs_strcasecmp(szToken,"/kick"))
								{
									szToken=strtok(NULL," \n\r");
									if(szToken!=NULL)
									{
										strcpy(szTemp,szToken);
										szToken=strtok(NULL,"\n\r");
										if(szToken)
										{
											Kick(szTemp,szToken);
										}
										else
										{
											Kick(szTemp,"Kicked by admin");
										}
									}
									break;
								}

								//////////////////////////////////////////

								if(dlcs_strcasecmp(szToken,"/kill"))
								{
									bQuit=true;
									break;
								}

								//////////////////////////////////////////

								if(dlcs_strcasecmp(szToken,"/users"))
								{
									Log("[USERS]");
									/*
									row=db->select("users id 0");
									row=row->next_key;
									while(row)
									{
										Log("%s %s",

												db->parse(row,"name"),
												db->parse(row,"last_login")

											);
										row=row->next_key;
									}
									*/
									break;
								}

								//////////////////////////////////////////

								if(dlcs_strcasecmp(szToken,"/restart"))
								{
									bRestart=1;
									break;
								}

//								if(dlcs_strcasecmp(szToken,"/gmf"))
//								{ szToken=strtok(NULL," \n\r"); if(szToken) ExecScript(szToken,1); break; }
								//if(dlcs_strcasecmp(szToken,"/nuy"))
//								{ ExecString("sys.new_users=\"yes\";"); Log("Now accepting new user accounts."); break; }
								//if(dlcs_strcasecmp(szToken,"/nun"))
//								{ ExecString("sys.new_users=\"no\";");  Log("Not accepting new user accounts."); break; }
								//if(dlcs_strcasecmp(szToken,"/who")) { ExecString("sys.who();"); break; }


								///////////////////////////////////////////////////////////////////////////
								// Local /motd

								if(dlcs_strcasecmp(szToken,"/motd"))
								{
									szToken=strtok(NULL,"\n\r");
									fp=fopen(motd,"rt");
									if(!fp)
									{
										fp=fopen(motd,"wt");
										if(fp)
										{
											fputs("Message of the day reset by server\n\r",fp);
											fclose(fp);
										}
										fp=fopen(motd,"rt");
										if(!fp)
										{
											Log("motd.txt can not be open for reading!");
											break;
										}
									}
									while(fgets(szTemp4,255,fp))
									{
										Log(szTemp4);
									}
									fclose(fp);

									if(szToken)
									{
										fp=fopen(motd,"wt");
										if(!fp)
										{
											Log("motd.txt can't be written, check permissions");
											break;
										}
										fputs(szToken,fp);
										fputs("\n\r",fp);
										fclose(fp);
										Log("Message of the day changed to: %s",szToken);
									}
									break;
								}

								///////////////////////////////////////////////////////////////////////////
								// Local /msg

								if(dlcs_strcasecmp(szToken,"/msg"))
								{
									szToken = strtok(NULL," \n\r");
									if(szToken!=NULL)
									{
										strcpy(szTemp,szToken);
										szToken=strtok(NULL,"\n\r");
										if(szToken)
											//l_interpret(va("sendmessage(\"%s\",\"MSG(Admin): %s\");",szTemp,szToken));
										break;
									}
									break;
								}

								///////////////////////////////////////////////////////////////////////////
								// Local /access

								if(dlcs_strcasecmp(szToken,"/access"))
								{
									szToken = strtok(NULL," \n\r");
									if(szToken!=NULL)
									{
										strcpy(szTemp2,szToken);
										szToken = strtok(NULL,"\n\r");
										if(!szToken)
										{
											Log("/access usage: /access username,access level");
											break;
										}
										strcpy(szTemp3,szToken);
										dx = atoi(szTemp3); // dx is access
										if( (dx<0) || (dx>255) ) dx=0;

										//l_interpret(va("setaccess(\"%s\",%d)",szTemp2,dx));
									}
									break;
								}

							}
						}
                        break;

                    case 27:
                        break;

                    case 8: // backspace
                        i=strlen(szConsoleInput);
                        szConsoleInput[i-1]=0;
                        printf(">                                                  \r>%s \r",szConsoleInput);

                        break;

                    case 21: // ctrl-U
                        printf(">                                                  \r>");
                        memset(szConsoleInput,0,1024);
                        break;

                    case 0xE0: // Arrow keys...
                        ch=_getch();
                        switch(ch)
                        {
                            case 72:    // UP
                                strcpy(szConsoleInput,GetConsoleHistory(ConsoleHistoryPosition));
                                ConsoleHistoryPosition++;
                                if(ConsoleHistoryPosition>MAX_CONSOLE_INPUT_BUFFER)
                                    ConsoleHistoryPosition=0;
                                printf(">                                                  \r>%s \r",szConsoleInput);
                                break;

                            case 80:    // DOWN
                                strcpy(szConsoleInput,GetConsoleHistory(ConsoleHistoryPosition));
                                ConsoleHistoryPosition--;
                                if(ConsoleHistoryPosition<0)
                                    ConsoleHistoryPosition=MAX_CONSOLE_INPUT_BUFFER;
                                printf(">                                                  \r>%s \r",szConsoleInput);
                                break;

                            case 75:    // LEFT (TODO: build cursor into console command input)
                                break;

                            case 77:    // RIGHT (TODO: build cursor into console command input)
                                break;

                            default:
                                break;
                        }
                        break;

                    default:
                        i=strlen(szConsoleInput);
                        szConsoleInput[i]=ch;
                        szConsoleInput[i+1]=0;
                        printf(">                                                  \r>%s \r",szConsoleInput);
                        break;
                }
            }
        }
    }
    ShutDown();
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////
void MovePlayer(C_FM_CLIENT *client,int x,int y,int z,int teleport)
{
	if(!fmgs) return;
	if(!client) return;
}
//////////////////////////////////////////////////////////////////////////////////////
void Say(C_FM_CLIENT *client,char *msg,u_char r,u_char g,u_char b,char *font)
{

}
//////////////////////////////////////////////////////////////////////////////////////
void Msg(char *name,char *msg)
{
	other_client=fmgs->pFirstPlayer;
	while(other_client)
	{
		if(dlcs_strcasecmp(other_client->name,name))
		{
			Announce(other_client,msg,255,255,255);
			return;
		}
		other_client=other_client->pNext;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void Announce(C_FM_CLIENT *client,char *msg,u_char r,u_char g,u_char b)
{
	if(!fmgs) return;
	if(client)
	{
		Send.Reset();
		Send.Write((char)NETMSG_SYSTEMMSG);
		Send.Write((char *)msg);
		Send.Write((char)0);
		client->pSocket->SendUnreliableMessage(&Send);
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void GlobalAnnounce(C_FM_CLIENT *client,char *msg,u_char r,u_char g, u_char b)
{
    if(!fmgs)   return;
    if(!client) return;
	other_client=fmgs->pFirstPlayer;
	while(other_client)
	{
		Announce(other_client,msg,255,255,255);
		other_client=other_client->pNext;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void AdminAnnounce(char *msg,u_char r,u_char g, u_char b)
{
	if(!fmgs) return;
	Send.Reset();
	Send.Write((char)NETMSG_SYSTEMMSG);
	Send.Write((char *)msg);
	Send.Write((char)0);
	other_client=fmgs->pFirstPlayer;
	while(other_client)
	{
		other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
		other_client=other_client->pNext;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void AdminToPlayer(C_FM_CLIENT *client,char *msg,u_char r,u_char g, u_char b)
{
	if(!fmgs)   return;
	if(!client) return;
	Send.Reset();
	Send.Write((char)NETMSG_SYSTEMMSG);
	Send.Write((char *)msg);
	Send.Write((char)0);
	client->pSocket->SendUnreliableMessage((CPacket *)&Send);
}
//////////////////////////////////////////////////////////////////////////////////////
void DoClient(void)
{
    static C_FM_CLIENT *client=0;
    if(bQuit) return;
    if(!fmgs) return;
    if(!client) client=fmgs->pFirstPlayer;
    if(client)
    {
        client->do_net();
        client=client->pNext;
    }
}
//////////////////////////////////////////////////////////////////////////////////////
void RemoveDeadPlayers(void)
{
	if(!fmgs) return;
	C_FM_CLIENT *other_client=0;
	other_client=fmgs->pFirstDelete;
	if(other_client)
	{
		fmgs->pFirstDelete=other_client->pNext;
		DEL(other_client);
	}
}
//////////////////////////////////////////////////////////////////////////////////////
void RemoveAllDeadPlayers(void)
{
	if(!fmgs) return;
	while(fmgs->pFirstDelete) RemoveDeadPlayers();
}
//////////////////////////////////////////////////////////////////////////////////////
void DoWorld(void)
{
	static long dwWorldSaveTimer=dlcs_get_tickcount();
    if(bQuit) return;

	SaveWorld();


}
//////////////////////////////////////////////////////////////////////////////////////
// START UP
//////////////////////////////////////////////////////////////////////////////////////
void StartUp(void)
{

    ////////////////////////////////////////////////////////////////////////////
    // Setup server variables

    pLog = new CLog;
    if(!pLog) { printf("\nNo memory for log!\n"); exit(0); }
    pLog->SetName("server.log");
    pLog->On();
    pLog->Restart();

	File=new CGAF;
	File->Open("ember_data.dlc");
	File->CreateDir("data");
	File->AddDir("data","data",1);
	File->Close();
	DEL(File);

    int i=0;
    int j=0;
    int k=0;

    Log("            ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»");
    Log("            º.  °°°°°° °°   °° °°°°°. °°°°°° °°°°°    º");
    Log("            º   °°     °°° °°° °°  °° °° .   °° .°°   º");
    Log("            º   ±±±±   ±±°°°±± °°°±±  ±°°°±  °°  ±°.  º");
    Log("            º   ²±    .±± ± ±± ±±  ²± ±±     ±±²²±    º");
    Log("            º  .²²     ²².  ²² ±²  ²² ²²     ²²  ²² . º");
    Log("            º   ²ÛÛ²²Û ÛÛ   ²Û ²ÛÛ²Û  Û²²Û²² Û²  Û²   º");
    Log("            ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼");
    Log("EGS Version (%s) Patch (%s) Net Revision (%s) %s",VERSION,EGS_REVISION,NET_REVISION,COPYRIGHT);
    Log("°±²ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛßßßßÛßßßßßßßßÛÛÛÛßßßßÛÛÛÛÛÛÛÛÛÛÛßßßßÛÛÛÛÛÛÛÛÛ²±°");
    Log("                  ÜÛÛÜ ÜÛÛÛÛÛß     ÜÛÛ             ÛÛ ");
    Log("                ÜÛß      ÞÛ  ÜÛÛ  Ûß ßÛ  ÜÛÛÛÛÛß  ÞÛ  ");
    Log("                ßÛÜÜÜ    ÛÝ Ûß Û ÛÝ ÜÛ     ÞÛ     Û   ");
    Log("                  ßßÛÛ  ÞÛ ÞÛ ÜÛ ÛÛßÛÛ     ÛÝ    ÞÝ   ");
    Log("                ÜÜ  ÞÛ  ÛÝ ÛÛßÛÛ ÛÛ  ÞÛ   ÞÛ          ");
    Log("                 ßßßß      ÛÝ ÛÝ Û    Û   ÛÝ    Û     ");
    Log("°±²ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÜÜÜÜÜÜÜÜÜÛÛÜÜÜÛÜÜÜÛÛÛÜÜÜÛÛÛÛÛÛÛÛÛÛÛÛÛ²±°");

    Log(dlcs_get_os_version());
    dlcs_suspend_power_management();

    SetupConsoleHistory();

	//db=new CDLDB;

    //db->Load("database.txt");

	// examples:
	/*
	row=db->select("system id 1");
	CDBVar *t; t=row;
	while(t)
	{
		Log("%s=%s",t->name,t->value);
		t=t->next_var;
	}


	row=db->select("users name seth");

	Log("seth is %s %s ( age = %s )",row->name,row->value,db->parse(row,"age"));
	Log("%s",db->parse(row,"email"));



	db->row_update(db->select("users name seth"),"age","19");

	Log("%s is %s %s ( age = %s )",
		db->parse(row,"name"),
		row->name,
		row->value,
		db->parse(row,"age")
		);
		*/


    ////////////////////////////////////////////////////////////////////////////
    // Map!

    pMap = new CEmMap;
    if(pMap==NULL)
    {
        Log("Low memory can't create pMap!");
        bQuit=1;
        ShutDown();
    }
    pMap->Initialize(GMP_MAPSIZE,GMP_MAPSIZE,0,5);
    pMapBuffer = NULL;
    pMapBuffer = new CEmMap;
    if(pMapBuffer==NULL)
    {
        Log("Low memory can't create pMapBuffer!");
        bQuit=1;
        ShutDown();
    }
    if(!pMapBuffer->Initialize(GMP_MAPSIZE*3,GMP_MAPSIZE*3,0,5))
    {
        Log("Low memory can't initialize BigMap data!");
        bQuit=1;
        ShutDown();
    }
    pFragMap = new CEmMap;
    if(pFragMap==NULL)
    {
        Log("Low memory can't create pFragMap!");
        bQuit=1;
        ShutDown();
    }
    pFirstFragMap=pFragMap;
    for(i=0;i<10;i++)
    {
        if(!pFragMap)
        {
            Log("Low memory can't create pFragMap sectors!");
            bQuit=1;
            ShutDown();
        }
        if(!pFragMap->Initialize(GMP_MAPSIZE,GMP_MAPSIZE,0,5))
        {
            Log("Low memory can't create pFragMap sectors!");
            bQuit=1;
            ShutDown();
        }
        pFragMap->iKey=i;
        pFragMap->pNext=new CEmMap;
        pFragMap=pFragMap->pNext;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Setup server

    NET_Init();

    if(!fmgs)
    {
        fmgs=new CServer();
        if(fmgs==NULL)
        {
            Log("Low memory can't create server...");
            bQuit=1;
            ShutDown();
        }

        client=fmgs->pFirstPlayer;
        Log("Listening on port [%s]", SysVar("port") );
    }

//    pFMMS_Connection = new C_FMMS;

	LoadWorld();

    ////////////////////////////////////////////////////////////////////////////
    // Setup console scroll back buffer

    Log("(Type '/help' for help on commands)");
    Log("°±²ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ²±°");
}

////////////////////////////////////////////////////////////////////////////
// SHUT DOWN

void ShutDown(void)
{

    ConsoleShutDown();
    RemoveConsoleHistory();

    GetSerialNumber(1);

    char szTemp[1024];
    memset(szTemp,0,1024);

    int i=0;
    C_FM_CLIENT *client=NULL;
    C_FM_CLIENT *pDelPlayer=NULL;

    ///////////////////////////////////////////

    if(fmgs)
    {
        client=fmgs->pFirstPlayer;
        if(client) Log("Kicking online players");
        while(client)
        {
			pDelPlayer=client;
            Log("       ....%s", pDelPlayer->name );
            Send.Reset();
            Send.Write((char)NETMSG_CLIENTSHUTDOWN);
            Send.Write((int) pDelPlayer->sid);
            Send.Write((char *)"Server shutdown by admin");//reason
            pDelPlayer->pSocket->SendUnreliableMessage(&Send);
            fmgs->Disconnect(pDelPlayer,1);
			client=client->pNext;
			DEL(pDelPlayer);
        }
    }

    ///////////////////////////////////////////

    RemoveAllDeadPlayers();

    ///////////////////////////////////////////

    SaveWorld();

    ///////////////////////////////////////////

    DEL(pMap);
    DEL(pMapBuffer);

    pFragMap=pFirstFragMap;
    while(pFragMap)
    {
        pDeleteFragMap=pFragMap;
        pFragMap=pFragMap->pNext;
        DEL(pDeleteFragMap);
    }

    DEL(fmgs);

    NET_Shutdown();

	if(dlcs_strcasecmp(SysVar("save_db_on_exit"),"yes") )
	{
		Log("Saving database");
		// db->Save("database.txt");
	}


	// DEL(db);

    ///////////////////////////////////////////

    Log("°±²ÛÛÛÛÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛÛÛÛÛ²±°");
    Log("        ÜÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛÜ ");
    Log("        Û        ÜÛÛÛÜ                      ÜÛÛÛÜ        Û ");
    Log("        Û        ÛúÛÜÛ    ßÛÜß ß Û   Û      ÛÜÛúÛ        Û ");
    Log("        Û        ßÛÛÛß     Û Û Û Û   Û      ßÛÛÛß        Û ");
    Log("        Û         ÝÝÞ      ß Û ß ßßß ßßß     ÝÞÞ         Û ");
    Log("        Û         þÜþ     Server Killed!     þÜþ         Û ");
    Log("        ßÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛß ");
    Log("°±²ÛÛÛÛÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛÛÛÛÛ²±°");
    Log("    °±²ÛÛÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛÛÛ²±°");
    Log("         ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß");

    DEL(pLog);

}

//////////////////////////////////////////////////////////////////////////////////////
void CheckRestart(void)
{
    C_FM_CLIENT *other_client;
    C_FM_CLIENT *clientStump;

    if(bRestart)
    {
        Log("[Initiating server restart]=====================================");
        other_client=fmgs->pFirstPlayer;
        while(other_client)
        {
            Send.Reset();
            Send.Write((char)NETMSG_CLIENTSHUTDOWN);
            Send.Write((int) other_client->sid);
            Send.Write((char *)"Manual server reset, try reconnecting.");//reason
            other_client->pSocket->SendUnreliableMessage(&Send);
            clientStump=other_client;
            other_client=other_client->pNext;
            fmgs->Disconnect(clientStump,1);
        }
        ShutDown();
        StartUp();
        Log("==============================================[Server restarted]");
        bRestart=0;
    }
}
//////////////////////////////////////////////////////////////////////////////////////
void ReportToMaster(void)
{
	/*
    static bool starter=false; static long dwReportTimer=GetTickCount();
    // if(!dlcs_strcasecmp(gmvar("sys.master_report"),"yes")) return;
	// if(!pFMMS_Connection) return;
    if((GetTickCount()-dwReportTimer)<15000) { if(!starter) starter=true; else return; }
    dwReportTimer=GetTickCount(); //Log("Reported to master");
    pFMMS_Connection->Connect("127.0.0.1","40404");
	*/
}

//////////////////////////////////////////////////////////////////////////////////////

extern "C" void SaveWorld(void)
{

	static long dwSaveTimer=dlcs_get_tickcount();
	if(	(dlcs_get_tickcount()-dwSaveTimer) > 50000 ) //((long)(atoi( SysVar("world_save_timer") ) *1000)) )
	{
		dwSaveTimer=dlcs_get_tickcount();
		Log("World saved...");
	}

}

//////////////////////////////////////////////////////////////////////////////////////

extern "C" void LoadWorld(void)
{
	Log("World loaded...");
}

//////////////////////////////////////////////////////////////////////////////////////

extern "C" C_FM_CLIENT *GetClient(char *user_name)
{
    if(!fmgs) return 0;
    C_FM_CLIENT *find=fmgs->pFirstPlayer;
    while(find)
    {
        if(dlcs_strcasecmp(find->name,user_name)) return find;
        find=find->pNext;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
void Kick(char *name,char *reason)
{
    C_FM_CLIENT *client=fmgs->pFirstPlayer;
    while(client)
    {
        if(dlcs_strcasecmp(client->name,name))
        {
            if(strlen(client->toon->t_name))
                Log("%s(%s) kicked",name,client->toon->t_name);
            else
                Log("%s kicked",name);

            Send.Reset();
            Send.Write((char)NETMSG_CLIENTSHUTDOWN);
            Send.Write((int)1);
            Send.Write((char *)reason); // reason
            client->pSocket->SendUnreliableMessage(&Send);

            fmgs->Disconnect(client,1); //Logout(client,1);

			/* client=fmgs->pFirstPlayer;
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
    Log("%s is not online!",name);
    return;
}
//////////////////////////////////////////////////////////////////////////////////////
char *GetConsoleHistory(int x) { if(ConsoleHistory) return strdup(ConsoleHistory[x].text); return strdup(va("con history error![%d]",x)); }
//////////////////////////////////////////////////////////////////////////////////////
void SetupConsoleHistory(void) { ConsoleHistory=0; ConsoleHistory=new ConHistory[MAX_CONSOLE_INPUT_BUFFER+1]; for(int i=0;i<MAX_CONSOLE_INPUT_BUFFER;i++) memset(ConsoleHistory[i].text,0,1024); ConsoleHistoryPosition=0; }
//////////////////////////////////////////////////////////////////////////////////////
void AddToConsoleHistory(char *s) { for(int i=MAX_CONSOLE_INPUT_BUFFER;i>0;i--) strcpy(ConsoleHistory[i].text,ConsoleHistory[i-1].text); strcpy(ConsoleHistory[0].text,s); }
//////////////////////////////////////////////////////////////////////////////////////
void RemoveConsoleHistory(void) { delete [] ConsoleHistory; ConsoleHistory=0; }









#ifdef _EMBER_SERVER_
/////////////////////////////////////////////////////////
void RemoveAccount(char *szName)
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
u_long GetSerialNumber(bool bSave)
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
                Log("Can't create serial.dat! #1");
                return 0;
            }
            iSerialNumber=1;
            Log("Serial number set to %d",iSerialNumber);
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
            Log("Can't create serial.dat! #2");
            return 0;
        }
        sprintf(szSerialNumber,"%d\n",iSerialNumber);
        fputs(szSerialNumber,fp);
        fputs("=================================================\n",fp);
        fputs("Serial number tracking file. DO NOT erase or modify.\n",fp);
        fclose(fp);
    }

    iSerialNumber++;
    // Log("Serial number incremented to %d",iSerialNumber);
    return iSerialNumber;
}
#endif
