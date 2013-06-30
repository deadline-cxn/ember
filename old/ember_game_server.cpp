// Mantra Server

#include "ember_game_server.h"
#include "ember_version.h"
#include "ember_game_server_enum.h"
#include "c_serverinfo.h"
#include "ember_game_server_socket.h"
#include "ember_game_server_client_socket.h"
#include "dlstorm.h"
#include "c_net.h"
#include "c_log.h"
#include "c_gaf.h"
#include "sqlite3.h"
#include <map>

CServer     *pServer;

CGAF		*File;
CLog        *pLog;      // Log file

sqlite3     *pUserDB;
map <string, string> dbresult;

char        szLastAddress[1024];
char        motd[1024];
time_t      pServerTime;
struct      tm *tTime;
bool        bQuit;
bool        bQuiet;
bool        bRestart;

int				ConsoleHistoryPosition;
typedef struct	ConHistory { char text[1024]; };
ConHistory	   *ConsoleHistory;

void CLog::_Add(char *fmt, ...){
    char ach[1024];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
	if(ach[strlen(ach)-1]!='\n')
    strcat(ach,"\n");
    AddEntry(ach);
    if(!bQuiet)
        printf(ach);
}
void ShowHelp(void){
    printf("Ember Game Server %s %s Help\n",VERSION,COPYRIGHT);
    printf("==============================================================\n");
    printf(" --help, -h, /? = this help\n");
    printf(" --quiet, -q, /q = quiet mode, no console output\n");
    printf("==============================================================\n");
}
int main(int argc, char *argv[]){
	std::vector <std::string> vstr;
    char chr;
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

    bQuiet=false;
    bQuit=false;

    randomize();                // set the random seed

    if(argc>1){
        chr=1;

        while(chr<argc){
            if( (!strcmp(argv[chr],"--help")) || (!strcmp(argv[chr],"-h")) || (!strcmp(argv[chr],"/?")) ){
                ShowHelp();
                exit(0);
            }

            if( (!strcmp(argv[chr],"-q")) || (!strcmp(argv[chr],"--quiet")) ||	(!strcmp(argv[chr],"/q"))){
                bQuiet=true;
            }
            chr++;
        }
    }

    ConsoleSetup();             // system specific console setup
    StartUp();

    if(!pServer) bQuit=true;

    while(!bQuit){ // game loop
        CheckRestart();

        pServer->Cycle();

        Sleep(2); // sleep 2 msecs so it don't hog cpu time

        if(_kbhit()){
            ch=_getch();
            switch(ch){
                case 0: break;
                case 13:
                case 10:

                    AddToConsoleHistory(szConsoleInput);
                    ConsoleHistoryPosition=0;
                    strcpy(szTemp5,szConsoleInput);
                    strcpy(szTemp4,szConsoleInput);
                    memset(szConsoleInput,0,1024);

                    pLog->_Add(">%s",szTemp5);

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
                                pLog->_Add("[List of available commands]====================================");
                                pLog->_Add("/help............................Show this help");
                                pLog->_Add("/db_tables.......................List tables in database");
                                pLog->_Add("/db_add_table <table name>.......Add new table to database");
                                pLog->_Add("/dbs.............................Saves database");

                            //	pLog->_Add("command(s).......................Execute GM script on command line");
                            //	pLog->_Add("/gmf.............................Execute GM script file");
                            //	pLog->_Add("/who.............................List online players");
                            //	pLog->_Add("/msg <user> <message>............Send message to specific player");
                            //	pLog->_Add("/access <user> <access level #>..Change player's access level");
                            //	pLog->_Add("/kick <user> [message]...........Kick player with optional message");
                        //		pLog->_Add("/ban <user>......................Ban player (will ban user)");
                        //		pLog->_Add("/banip <user>....................Ban player (will ban user's ip address)");
                        //		pLog->_Add("/unban <user>....................Unban player (will unban user)");
                        //		pLog->_Add("/unbanip <ipaddress>.............Unban player (will unban user's ip address)");
                        //		pLog->_Add("/bandomain <domain>..............Ban entire domain");
                        //		pLog->_Add("/unbandomain <domain>............Unban entire domain");
                        //		pLog->_Add("/ipbanlist.......................List all banned ip's");
                        //		pLog->_Add("/domainbanlist...................List all banned domains");
                        //		pLog->_Add("/userbanlist.....................List all banned users");
                        //		pLog->_Add("/nuy.............................Allow new user accounts");
                        //		pLog->_Add("/nun.............................Do not allow new user accounts");
                        //		pLog->_Add("/ann <message>...................Announce a message to the entire server");
                        //		pLog->_Add("/motd [new motd].................View, or change the message of the day");
                        //		pLog->_Add("/users...........................List user accounts");
                        //		pLog->_Add("/deleteuser <username>...........Remove a user account");
                                pLog->_Add("/kill [minutes]..................Kill the server [minutes until kill] (CTRL-C immediate)");
                        //		pLog->_Add("/restart [minutes]...............Restart the server [minutes until restart]");
                                pLog->_Add("======================================================[End List]");
                                break;
                            }

                            ///////////////////////////////////////////////////////////////////////////

                            if( (dlcs_strcasecmp(szToken,"/time")) )
                            {
                                pLog->_Add(dlcs_get_time());
                                pLog->_Add("timestamp: %s",dlcs_timestamp());
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

                                pLog->_Add("db tables:");

                                // t=db->FirstTable();
                                //while(t)
                                //{
                                //	pLog->_Add("%s",t->name);
                                //	t=db->NextTable();
                                //}
                            }

                            //////////////////////////////////////////

                            if( (dlcs_strcasecmp(szToken,"/dbupdate")) ||
                                (dlcs_strcasecmp(szToken,"/dbup")) )
                            {

                                vstr=explode(" ",szTemp4);

                                pLog->_Add(" vstr size = %d",vstr.size() );

                                if(vstr.size()<2)
                                {
                                    pLog->_Add("Usage examples:");
                                    pLog->_Add("/dbup t usrs users ");
                                    pLog->_Add("    (table USRS renamed to USERS)");

                                    pLog->_Add("/dbup c users aged age");
                                    pLog->_Add("    (column AGED renamed to AGE in table USERS)");

                                    pLog->_Add("/dbup v users age 19 name seth");
                                    pLog->_Add("    (AGE updated to 19 in table USERS where NAME=seth)");
                                }
                                else
                                {

                                    for(int i=0;i<vstr.size();i++)
                                    {
                                        pLog->_Add("[%d][%s]",i,vstr[i].c_str());
                                    }

                                    if( (vstr[1]=="table") ||
                                        (vstr[1]=="t") )
                                    {
                                        if(vstr.size()>3)
                                        {
                                            pLog->_Add("old table name[%s] -> new table name[%s]",
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
                                            pLog->_Add("table[%s] column[%s] value[%s]",
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
                                if(szToken) pServer->RemoveAccount(szToken);
                                break;
                            }

                            //////////////////////////////////////////

                            if(dlcs_strcasecmp(szToken,"/ann"))
                            {
                                szToken=strtok(NULL,"\n\r");
                                if(szToken)
                                {
                                    pServer->AdminAnnounce(szToken,0,255,0);
                                    pLog->_Add(szToken);
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
                                        pServer->Kick(szTemp,szToken);
                                    }
                                    else
                                    {
                                        pServer->Kick(szTemp,"Kicked by admin");
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
                                pLog->_Add("[USERS]");
                                /*
                                row=db->select("users id 0");
                                row=row->next_key;
                                while(row)
                                {
                                    pLog->_Add("%s %s",

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
//								{ ExecString("sys.new_users=\"yes\";"); pLog->_Add("Now accepting new user accounts."); break; }
                            //if(dlcs_strcasecmp(szToken,"/nun"))
//								{ ExecString("sys.new_users=\"no\";");  pLog->_Add("Not accepting new user accounts."); break; }
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
                                        pLog->_Add("motd.txt can not be open for reading!");
                                        break;
                                    }
                                }
                                while(fgets(szTemp4,255,fp))
                                {
                                    pLog->_Add(szTemp4);
                                }
                                fclose(fp);

                                if(szToken)
                                {
                                    fp=fopen(motd,"wt");
                                    if(!fp)
                                    {
                                        pLog->_Add("motd.txt can't be written, check permissions");
                                        break;
                                    }
                                    fputs(szToken,fp);
                                    fputs("\n\r",fp);
                                    fclose(fp);
                                    pLog->_Add("Message of the day changed to: %s",szToken);
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
                                        pLog->_Add("/access usage: /access username,access level");
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
    ShutDown();
    return 1;
}
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;

    char t1[1024]; memset(t1,0,1024);
    char t2[1024]; memset(t2,0,1024);

    for(i=0; i<argc; i++){
        strcpy(t1,azColName[i]);
        strcpy(t2,argv[i]);
        pLog->_Add("%s = %s\n", t1,t2);// azColName[i], argv[i] ? argv[i] : "NULL");
        dbresult.insert(pair<string, string>(t1,t2));
    }
    return 0;
  }

int dbquery(char *q)
{
    dbresult.clear();
    char *zErrMsg=0;
    int qerr;
    //pLog->_Add("%s!",dbresult["username"].c_str());

    if(qerr!=SQLITE_OK)
    {
        pLog->_Add("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    qerr=sqlite3_exec(pUserDB, q, callback, 0, &zErrMsg);
    return qerr;

}

void StartUp(void){ // START UP

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

    pLog->_Add("            �����������������������������������������ͻ");
    pLog->_Add("            �.  ������ ��   �� �����. ������ �����    �");
    pLog->_Add("            �   ��     ��� ��� ��  �� �� .   �� .��   �");
    pLog->_Add("            �   ����   ������� �����  �����  ��  ��.  �");
    pLog->_Add("            �   ��    .�� � �� ��  �� ��     �����    �");
    pLog->_Add("            �  .��     ��.  �� ��  �� ��     ��  �� . �");
    pLog->_Add("            �   ��۲�� ��   �� ��۲�  ۲�۲� ۲  ۲   �");
    pLog->_Add("            �����������������������������������������ͼ");
    pLog->_Add("EGS Version (%s) Patch (%s) Net Revision (%s) %s",VERSION,EGS_REVISION,NET_REVISION,COPYRIGHT);
    pLog->_Add("��������������������������������������������������������������۲��");
    pLog->_Add("                  ���� �������     ���             �� ");
    pLog->_Add("                ���      ��  ���  �� ��  �������  ��  ");
    pLog->_Add("                �����    �� �� � �� ��     ��     �   ");
    pLog->_Add("                  ����  �� �� �� �����     ��    ��   ");
    pLog->_Add("                ��  ��  �� ����� ��  ��   ��          ");
    pLog->_Add("                 ����      �� �� �    �   ��    �     ");
    pLog->_Add("��������������������������������������������������������������۲��");

    pLog->_Add(dlcs_get_os_version());
    dlcs_suspend_power_management();

    SetupConsoleHistory();


    ////////////////////////////////////////////////////////////////////////////
    // Open User Database

    i = sqlite3_open("data/users/users.db",&pUserDB);
    if(i){
        pLog->_Add("Can't open user database");
        sqlite3_close(pUserDB);
    }else{
        pLog->_Add("Opened user database");
    }
    if(dbquery("select * from users")!=SQLITE_OK){
        dbquery("CREATE TABLE users (username varchar(32), password varchar(32), access smallint)");
        dbquery((char *)va("INSERT INTO users VALUES ('seth','%s', 255)",md5_digest("123")));
    }

    ////////////////////////////////////////////////////////////////////////////
    // Setup server

    NET_Init();

    if(!pServer)
    {
        pServer=new CServer(40404,pLog);
        if(pServer==NULL)
        {
            pLog->_Add("Low memory can't create server...");
            bQuit=1;
            ShutDown();
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Setup console scroll back buffer

    pLog->_Add("(Type '/help' for help on commands)");
    pLog->_Add("��������������������������������������������������������������۲��");
}
void ShutDown(void){ // SHUT DOWN

    ConsoleShutDown();
    RemoveConsoleHistory();

    DEL(pServer);

    sqlite3_close(pUserDB);
    pLog->_Add("Closed user database");

    NET_Shutdown();

    ///////////////////////////////////////////

    pLog->_Add("��������������������������������������������������������������۲��");
    pLog->_Add("        �������������������������������������������������� ");
    pLog->_Add("        �        �����                      �����        � ");
    pLog->_Add("        �        �����    ���� � �   �      �����        � ");
    pLog->_Add("        �        �����     � � � �   �      �����        � ");
    pLog->_Add("        �         ���      � � � ��� ���     ���         � ");
    pLog->_Add("        �         ���     Server Killed!     ���         � ");
    pLog->_Add("        �������������������������������������������������� ");
    pLog->_Add("��������������������������������������������������������������۲��");
    pLog->_Add("    ������������������������������������������������������۲��");
    pLog->_Add("         ������������������������������������������������");

    DEL(pLog);

}
void CheckRestart(void) {
    if(bRestart)
    {
        pLog->_Add("[Initiating server restart]=====================================");

        // pServer->KickAll("Server reset, try reconnecting.");

        ShutDown();
        StartUp();
        pLog->_Add("==============================================[Server restarted]");
        bRestart=0;
    }
}
char *GetConsoleHistory(int x){
    if(ConsoleHistory) return strdup(ConsoleHistory[x].text);
    return strdup(va("con history error![%d]",x));
}
void SetupConsoleHistory(void){
    ConsoleHistory=0;
    ConsoleHistory=new ConHistory[MAX_CONSOLE_INPUT_BUFFER+1];
    for(int i=0;i<MAX_CONSOLE_INPUT_BUFFER;i++)
        memset(ConsoleHistory[i].text,0,1024);
    ConsoleHistoryPosition=0;
}
void AddToConsoleHistory(char *s){
    for(int i=MAX_CONSOLE_INPUT_BUFFER;i>0;i--)
        strcpy(ConsoleHistory[i].text,ConsoleHistory[i-1].text);
    strcpy(ConsoleHistory[0].text,s);
}
void RemoveConsoleHistory(void){
    delete [] ConsoleHistory;
    ConsoleHistory=0;
}

void C_GSC::do_net(void){
    if(!pSocket) return;
    CPacket Send(8192);
	CPacket *Recv=NULL;
	map <string,string> pairdata;
	pairdata.clear();
    char szTemp[1024];     memset(szTemp,0,1024);
    char szTemp2[1024];    memset(szTemp2,0,1024);
    char szTemp3[1024];    memset(szTemp3,0,1024);
    char szTemp4[2048];    memset(szTemp4,0,2048);
    int iMsg,i,j,k,x,y,z,ax,ay,az,bx,by,bz,cx,cy,cz,dx,dy,dz,ex,ey,ez,fx,fy,fz;
    iMsg=i=j=k=x=y=z=ax=ay=az=bx=by=bz=cx=cy=cz=dx=dy=dz=ex=ey=ez=fx=fy=fz=0;
    double gx,gy,gz; gx=gy=gz=0; unsigned char vx,vy,vz; vx=vy=vz=0;
    char cMsgType;
    C_GSC *other_client;

    iMsg=pSocket->iGetMessage();

    if(iMsg>0){
        fold_block {
        pLog->_Add("Incoming message from (%s)(%s)",username,NET_pAddrToString((struct sockaddr*)&pSocket->FromAddr));
        Recv=pSocket->pGetMessage();

        if(!Recv) return;

        cMsgType=Recv->cRead();

        pLog->_Add("Got a message %d from %s",cMsgType,        NET_pAddrToString((struct sockaddr*)&pSocket->FromAddr)        );

        //pLog->_Add("iGetMessage=%d",i);
        Recv=pSocket->pGetMessage();

        if(FromAddr.sin_addr.s_addr!=pSocket->FromAddr.sin_addr.s_addr)
        {
            pLog->_Add("Recv a net message from unknown");
            return;
        }

        memcpy(&pSocket->ToAddr,&FromAddr,sizeof(sockaddr));
        //pLog->_Add("Recv a net message from client=%s",NET_pAddrToString((struct sockaddr*)&FromAddr));

        cMsgType=Recv->cRead();

        } // end fold block

        switch(cMsgType)
        {
            case NETMSG_LOGIN_REQUEST: fold_block {

                // set ax to default login result, copy username, password
                ax=BAD_LOGIN;
                strcpy(szTemp,"null");
                strcpy( username , Recv->pRead());
                strcpy( password , Recv->pRead());

                // check for NULL result
                if((username==NULL) || (password==NULL) || (!strlen(username))){
                    strcpy(szTemp,"Bad network message");
                    bDelete=true;
                    break;
                }

                // temporary log for visibility
                pLog->_Add("Login request from %s:%d (username[%s][%s])",inet_ntoa(pSocket->ToAddr.sin_addr),ntohs(pSocket->ToAddr.sin_port),username,password);

                // check for maximum players logged in
                // if(atoi(gmvar("sys.players"))>=atoi(gmvar("sys.maxplayers"))) ax=TOO_MANY_PLAYERS;

                // check to see if player is already logged in
                // SYSTEM is reserved, add any other reserved usernames here
                if(dlcs_strcasecmp(username,"SYSTEM")) ax=ALREADY_LOGGED_IN;
				other_client=pServer->pFirstPlayer;
                while(other_client){
                    if(dlcs_strcasecmp(other_client->username,username))
                        ax=ALREADY_LOGGED_IN;
                    other_client=other_client->pNext;
                }

                // temporary test check, remove later
                if( (dscc(username,"seth")) &&
                    (dscc(password,md5_digest("123"))) )
                    ax=GOOD_LOGIN;



                // compare passwords
                if(ax==BAD_LOGIN){
                    if(dbquery((char*)va("select * from users where username=%s",username))==SQLITE_OK){
                        if((dscc(dbresult["password"].c_str(),password)))
                            ax=GOOD_LOGIN;
                        else
                            ax=BAD_LOGIN;
                    }else{
                        ax=NEW_ACCOUNT;
                    }
                }


                switch(ax){
                    case BAD_LOGIN:         strcpy(szTemp,"Incorrect password...");            break;
                    case ALREADY_LOGGED_IN: strcpy(szTemp,"This user is already logged in from another machine...");       break;
                    case ACCOUNT_EXPIRED:   strcpy(szTemp,"Your account has expired. Contact system administrator.");      break;
                    case TOO_MANY_PLAYERS:  strcpy(szTemp,"Server is full. Too many       players already logged in...");  break;
                    case GOOD_LOGIN: fold_block {
                        if(!strlen(username)){
                            ax=BAD_LOGIN;
                            strcpy(szTemp,"Bad network try again.");
                            break; }

                        break; }
                    case NEW_ACCOUNT: fold_block {
                        if(0){ //dlcs_strcasecmp("yes",gmvar("sys.new_users"))){
                            ax=BAD_LOGIN;
                            // strcpy(szTemp,va("Sign up for a new account on the webpage %s",gmvar("sys.website")));
                            strcpy(szTemp,"New Account");
                            break;
                        } else {
                            ax=BAD_LOGIN;
                            strcpy(szTemp,"Sorry, we're not accepting new accounts at this time. Try again later.");
                            break;
                        }
                        break;
                    default:
                        break;}
                }

                if(ax==GOOD_LOGIN){
                    VortexInsert();
                    // ExecString(va("player[%d].name=\"%s\";",client->sid,client->name));
                    // result=emquery(va("select * from rpg_users where name='%s'",client->name));
                    // if(result) { row=mysql_fetch_row(result); Log("User [%s] logged in (last login %s)",row[0],row[10]); mysql_free_result(result); }
                    // emquery(va("UPDATE `rpg_users` SET `last_login` = NOW( ) WHERE `name`='%s'",client->name));
                    // emquery(va("UPDATE `rpg_users` SET `this_ip` = '%s' WHERE `name`='%s'",gmvar(va("player[%d].ip_address",client->sid)),gmvar(va("player[%d].name",client->sid))));
                    // client->ecsSetState(NET_CONNECTED);
                    strcpy(szTemp,"Welcome to the server.");
                    //sid=client->sid;
                    // access=client->access;
                }

                // client->LoginReply(ax,szTemp);

                pLog->_Add("LoginReply(%s)",szTemp);

                Send.Reset();
                Send.Write((char) NETMSG_LOGIN_REQUEST_REPLY);
                Send.Write((char)   ax);
                Send.Write((char *) szTemp);
                Send.Write((char *) VERSION);
                Send.Write((int)    sid);
                Send.Write((char *) "sys.media");
                Send.Write((char *) "sys.name");
                Send.Write((char *) "sys.owner");
                Send.Write((int)    255);
                Send.Write((char *) "access what");
                Send.Write((char)   10); // character slots
                pSocket->SendUnreliableMessage(&Send);
                if(ax!=GOOD_LOGIN) bDelete=true;

                break; }
            case NETMSG_LOGOUT: fold_block {

                bDelete=true;
                pLog->_Add("%s logged out",username);
                    //Logout(client,1);

                break; }
            case NETMSG_GUI_CALLBACK: fold_block {

                strcpy(szTemp3,Recv->pRead());
                ax=Recv->cRead(); // number of data to get
                for(j=0;j<ax;j++)
                {
                    strcpy(szTemp ,Recv->pRead());
                    strcpy(szTemp2,Recv->pRead());
                    pairdata[szTemp]=szTemp2;
                    // for now, only pull out console function
                    if(dscc(szTemp,"console"))
                    {
                        pLog->_Add("%s GUI_CALLBACK (%s)",username,szTemp2);
                    }
                }


                break;}
            case NETMSG_GET_LOGIN_PROGRAM: fold_block {
                /* Log("Hi");
                Send.Reset();
                Send.Write((char)NETMSG_GUI);
                Send.Write((char)0);
                Send.Write((char)0);
                client->pSocket->SendUnreliableMessage((CPacket *)&Send); */

                break; }
            case NETMSG_SERVER_INFO: fold_block {
                switch(Recv->cRead())
                {
                    case SI_GENERIC:
                        break;
                    default:
                        break;
                }

                break; }
            case NETMSG_RETRIEVECHARS: fold_block {
                /* inactivity_timer->Reset();
                client->avatar->wipe();
                for(i=0;i<atoi(l_sys("character_slots"));i++)
                {
                    client->avatar->load(i);
                    if(strlen(client->avatar->name))
                    {
                        Send.Reset();
                        Send.Write((char)NETMSG_RETRIEVECHARS);
                        Send.Write((char)i);
                        Send.Write((char *)client->avatar->name);
                        client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                        client->avatar->wipe();
                    }
                }*/

                break; }
            case NETMSG_RETRIEVECHARINFO: fold_block {
                /*inactivity_timer->Reset();
                cx=Recv->cRead();
                if(client->in_limbo==false)
                {
                    client->avatar->load(cx);
                    client->in_limbo=true;
                    Send.Reset();
                    Send.Write((char)NETMSG_RETRIEVECHARINFO);
                    Send.Write((char *)client->avatar->name);
                    Send.Write((int)client->sid);
                    Send.Write((char)client->avatar->direction);
                    Send.Write((int)15015);
                    Send.Write((int)15015);
                    Send.Write((int)15015);
                    client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    MovePlayer(client,client->avatar->x,client->avatar->y,client->avatar->z,1);
                }*/

                break; }
            case NETMSG_LOCALCHAT: fold_block {
                inactivity_timer->Reset();
                strcpy(szTemp,Recv->pRead());
                //if((atoi(l_sys("log_local_chat"))?0:1)) Log("%s(%s)L\"%s\"",client->avatar->name,client->avatar->name,szTemp);
                Send.Reset();
                Send.Write((char)NETMSG_LOCALCHAT);
                Send.Write((int)sid);
                Send.Write((char *)szTemp);
                Send.Write((char) 0); // R
                Send.Write((char) 255); // G
                Send.Write((char) 0); // B
                /*client->avatar->chat_r=Recv->cRead();
                Send.Write((char) client->avatar->chat_r);   // Red element
                client->avatar->chat_g=Recv->cRead();
                Send.Write((char) client->avatar->chat_g);   // Green element
                client->avatar->chat_b=Recv->cRead();
                Send.Write((char) client->avatar->chat_b);   // Blue element
                client->avatar->set_chat_font(Recv->pRead());
                Send.Write((char *) client->avatar->chat_font); // Font name */
                other_client=pServer->pFirstPlayer;
                while(other_client){
                    /*if( ( other_client->avatar->x >= client->avatar->x-GMP_MAPSIZE ) &&
                        ( other_client->avatar->x <= client->avatar->x+GMP_MAPSIZE ) &&
                        ( other_client->avatar->y >= client->avatar->y-GMP_MAPSIZE ) &&
                        ( other_client->avatar->y <= client->avatar->y+GMP_MAPSIZE ) &&
                        ( other_client->avatar->z == client->avatar->z    ) )
                        */

                    other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                        //
                    other_client=other_client->pNext;
                }
                break; }
            case NETMSG_SYSTEMMSG:  fold_block { // Global Chat / System Message
                inactivity_timer->Reset();
                strcpy(szTemp,Recv->pRead());
                pServer->GlobalAnnounce(this,(char *)va("%s:%s",username,szTemp),ax,ay,az);
                break;}
            case NETMSG_GENERIC_MSG: fold_block {
                inactivity_timer->Reset();
                strcpy(szTemp,Recv->pRead());
                memset(szTemp2,0,1024);
                j=0;
                for(i=0;i<strlen(szTemp);i++)
                {
                    if(szTemp[i]=='\'')
                    {
                        szTemp2[j]='\\';
                        j++;
                        szTemp2[j]='\'';
                        j++;
                    }
                    else
                    {
                        szTemp2[j]=szTemp[i];
                        j++;
                    }
                }
                for(i=0;i<strlen(szTemp2);i++) szTemp2[i]=szTemp2[i+1]; // should have a / in front, remove it
                //l_interpret(va("world.client[%d]:con_parse(\"%s\")",windex,szTemp2));
                break; }
            case NETMSG_MOVEPLAYER: fold_block {

                // given a starting location A
                // and   an ending  location B
                // the server will generate a path ((x,y) waypoints)
                // and return it to the client
                // if there is no path to the destination,
                // the function will return 0
                // location A and location B must be visible on the client's screen

                /*
                // Request for movement
                // Store current client location here.

                inactivity_timer->Reset();

                ax = client->avatar->x;
                ay = client->avatar->y;
                az = client->avatar->z;

                // Read requested move from data packet.

                dx = Recv->cRead();

                switch(dx)
                {
                    case FM_NORTH:
                        client->avatar->y=client->avatar->y-1;
                        client->avatar->direction=FM_NORTH;
                        break;

                    case FM_SOUTH:
                        client->avatar->y=client->avatar->y+1;
                        client->avatar->direction=FM_SOUTH;
                        break;

                    case FM_EAST:
                        client->avatar->x=client->avatar->x+1;
                        client->avatar->direction=FM_EAST;
                        break;

                    case FM_WEST:
                        client->avatar->x=client->avatar->x-1;
                        client->avatar->direction=FM_WEST;
                        break;

                    case FM_NORTHWEST:
                        client->avatar->y=client->avatar->y-1;
                        client->avatar->x=client->avatar->x-1;
                        client->avatar->direction=FM_NORTHWEST;
                        break;

                    case FM_SOUTHWEST:
                        client->avatar->x=client->avatar->x-1;
                        client->avatar->y=client->avatar->y+1;
                        client->avatar->direction=FM_SOUTHWEST;
                        break;

                    case FM_NORTHEAST:
                        client->avatar->x=client->avatar->x+1;
                        client->avatar->y=client->avatar->y-1;
                        client->avatar->direction=FM_NORTHEAST;
                        break;

                    case FM_SOUTHEAST:
                        client->avatar->x=client->avatar->x+1;
                        client->avatar->y=client->avatar->y+1;
                        client->avatar->direction=FM_SOUTHEAST;
                        break;

                    default:
                        Log("%s(%s) Move Error! %d",client->avatar->name,client->avatar->name,dx);
                        break;
                }

                // Get map sector for checking stuff

                dy = pMap->LoadSector3D("map",  MapCoord(client->avatar->x),
                                                MapCoord(client->avatar->y),
                                                MapCoord(client->avatar->z));

                dx=0; // teleport disable for now

                // Check if tile is blocked
                if( (client->game_state==PLAY) &&
                    (pMap->bIsBlocked(CamCoord(client->avatar->x),CamCoord(client->avatar->y))) )
                {
                        // restore previous client location
                        client->avatar->x=ax;
                        client->avatar->y=ay;
                        client->avatar->z=az;
                        break;
                }
                else
                {
                    MovePlayer(client,client->avatar->x,client->avatar->y,client->avatar->z,dx);
                }

                /*
                dx = 0; // no teleports allowed in this server!
                if(root->pSystem->iGetProperty("teleports"))
                {
                    dx = pMap->bIsTeleport( client->avatar->x,
                                            client->avatar->y); // Check if on a teleport location here

                    if(dx) // reference teleport list to get the destination teleport location
                    {
                        pTeleport=pFirstTeleport;
                        while(pTeleport)
                        {
                            if( (pTeleport->iSourceX) == (client->pObjectRef->iGetProperty("x")) &&
                                (pTeleport->iSourceY) == (client->pObjectRef->iGetProperty("y")) &&
                                (pTeleport->iSourceZ) == (client->pObjectRef->iGetProperty("z")) )
                                break;

                            pTeleport=pTeleport->pNext;
                        }

                        if(pTeleport)
                        {
                            client->pObjectRef->SetProperty("x",pTeleport->iDestinationX);
                            client->pObjectRef->SetProperty("y",pTeleport->iDestinationY);
                            client->pObjectRef->SetProperty("z",pTeleport->iDestinationZ);
                        }
                    }
                }*/


                break; }
            case NETMSG_MODIFY_CHARACTER: fold_block {
                inactivity_timer->Reset();
                /*
                if(!DoesHaveAccess(client->cGetAccess(),"modify_character"))
                {
                    // Player doesn't have access to change character information
                    // Send back a refuse request message
                    Announce(client,"Can't modify your character. You don't have the access.",255,0,0);
                    Log("%s(%s) tried to modify character!",client->avatar->name,client->avatar->name);
                    break;
                }

                Send.Reset();
                Send.Write((char)NETMSG_MODIFY_CHARACTER);
                Send.Write((int)Recv->iRead());

                switch(Recv->cRead())
                {
                    case MC_RACE:
                        client->SetRace(Recv->cRead());
                        Send.Write((char)MC_RACE);
                        Send.Write((int)client->GetRace());
                        break;

                    case MC_GENDER:
                        client->SetGender(Recv->cRead());
                        Send.Write((char)MC_GENDER);
                        Send.Write((char)client->GetGender());
                        break;

                    case MC_CLASS:
                        client->SetClass(Recv->cRead());
                        Send.Write((char)MC_CLASS);
                        Send.Write((int)client->GetClass());
                        break;

                    default:
                        break;
                }

                client->avatar->save("users");

                // Send only to on screen players
                // do really all players need this information?
                // This packet will be sent when your characters appearance
                // changes somehow, if there are players on screen, go ahead
                // and describe the appearance so the client can render it

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    if( ( other_client->GetMapX() >= ( client->GetMapX()-1) ) &&
                        ( other_client->GetMapX() <= ( client->GetMapX()+1) ) &&
                        ( other_client->GetMapY() >= ( client->GetMapY()-1) ) &&
                        ( other_client->GetMapY() <= ( client->GetMapY()+1) ) &&
                        ( other_client->GetMapZ() ==   client->GetMapZ()    ) )
                    {
                        other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    }
                    other_client=other_client->pNext;
                }
                */

                break; }
            case NETMSG_CLIENTSHUTDOWN: fold_block {

                other_client=pServer->pFirstPlayer;
                while(other_client)
                {
                    if(other_client != this)
                    {
                        Send.Reset();
                        Send.Write((char)NETMSG_VIS_REMOVE);
                        Send.Write((int)sid);
                        other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    }
                    other_client=other_client->pNext;
                }
                bDelete=true;
                return;

                break;}
            case NETMSG_CREATE_CHARACTER: fold_block {
                /* inactivity_timer->Reset();
                client->avatar->hail(Recv->cRead());
                strcpy(client->avatar->name,Recv->pRead());

                // call hook to do stuff with client here...

                client->avatar->x=15015;
                client->avatar->y=15015;
                client->avatar->z=15015;

                client->avatar->save(client->avatar->slot); // Save the character slot
                Send.Reset();
                Send.Write((char)NETMSG_CREATE_CHARACTER);
                Send.Write((char)client->avatar->slot);
                client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                */

                break;}
            case NETMSG_DELETE_CHARACTER: fold_block {
                inactivity_timer->Reset();
                cx = Recv->cRead();
                fold_block { // !DoesHaveAccess(client->cGetAccess(),"fast_char_delete"))

                    //avatar->load(cx);
                    //tTime=localtime(&client->CL_Data->tCreationTime);
                    //ax=tTime->tm_year;
                    //ay=tTime->tm_yday;
                    //time(&fmgsTime);
                    //tTime=localtime(&fmgsTime);
                    //bx=tTime->tm_year;
                    //by=tTime->tm_yday;
                    //if(bx==ax)
                    //-
                    //    if( (by-ay) < l_sys("minimum_char_age)
                    //    -
                    //        sprintf(szTemp,"Character must age at least %s real days!",l_sys("minimum_char_age"));
                    //        Announce(client,szTemp,50,175,255);
                    //        break;
                    //    -
                    // -
                }
                //client->avatar->wipe();
                //client->avatar->save(cx);
                Send.Reset();
                Send.Write((char)NETMSG_DELETE_CHARACTER);
                Send.Write((char)cx);
                pSocket->SendUnreliableMessage((CPacket *)&Send);

                break;}
            case NETMSG_MODIFY_MAP: fold_block {

                inactivity_timer->Reset();
                /* if(pMap==NULL) break;

                cx=Recv->cRead();      // Type of modification
                cy=Recv->cRead();      // boolean

                ax=client->avatar->x;//Recv->iRead(); // x
                ay=client->avatar->y;//Recv->iRead(); // y
                az=client->avatar->z;//Recv->iRead(); // z

                pMap->LoadSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));

                Send.Reset();
                Send.Write((char)NETMSG_MODIFY_MAP);

                if(1) //!DoesHaveAccess(client->cGetAccess(),"map_build"))
                {
                    AdminToPlayer(client,"You don't have access to build the map.",255,0,0);
                    break;
                }

                switch(cx)
                {
                    case GMP_PROPERTY_LIQUID:

                        pMap->SetLiquid(CamCoord(ax),CamCoord(ay),(cy?true:false));  // keep microsoft happy
                        Send.Write((char)GMP_PROPERTY_LIQUID);
                        Send.Write((char)cy);

                        break;

                    case GMP_PROPERTY_BLOCKED:

                        pMap->SetBlocked(CamCoord(ax),CamCoord(ay),(cy?true:false));  // keep microsoft happy
                        Send.Write((char)GMP_PROPERTY_BLOCKED);
                        Send.Write((char)cy);

                        break;

                    case GMP_PROPERTY_INDOORS:

                        break;

                    case GMP_PROPERTY_TELEPORT:


                        Send.Write((char)GMP_PROPERTY_TELEPORT);
                        Send.Write((char)cy);

                        pTeleport=pFirstTeleport;
                        while(pTeleport)
                        {
                            if( (pTeleport->iSourceX  == ax) &&
                                (pTeleport->iSourceY  == ay) &&
                                (pTeleport->iSourceZ  == az) )
                                break;
                            pTeleport=pTeleport->pNext;
                        }

                        if(pTeleport)
                        {
                            if(cy)
                            {
                                strcpy(pTeleport->szName,Recv->pRead());
                                strcpy(pTeleport->szDestName,Recv->pRead());
                                pTeleport->iDestinationX=Recv->iRead();
                                pTeleport->iDestinationY=Recv->iRead();
                                pTeleport->iDestinationZ=Recv->iRead();
                                sprintf(szTemp,"teleports%c%d.cfg",PATH_SEP,pTeleport->iSourceX,pTeleport->iSourceY,pTeleport->iSourceZ);
                                pTeleport->bSave(szTemp);
                            }
                            else
                            {
                                sprintf(szTemp,"teleports%c%d-%d-%d.ini",PATH_SEP,pTeleport->iSourceX,pTeleport->iSourceY,pTeleport->iSourceZ);
                                pTeleport->bClear();
                                remove(szTemp);
                            }

                            pMap->SetTeleport(ax,ay,(cy?true:false));
                            break;
                        }

                        if(!cy)
                        {
                            pMap->SetTeleport(ax,ay,(cy?true:false));
                            break;
                        }

                        i=0;

                        pTeleport=pFirstTeleport;
                        while(pTeleport)
                        {
                            if(pTeleport->iKey==NOT_A_TELEPORT)
                                break;
                            i++;
                            pTeleport=pTeleport->pNext;
                        }

                        if(i>MAX_TELEPORTS)
                            break;

                        if(pTeleport)
                        {
                            pTeleport->iKey=i;

                            strcpy(pTeleport->szName,Recv->pRead());

                            pTeleport->iSourceX=ax;
                            pTeleport->iSourceY=ay;
                            pTeleport->iSourceZ=az;

                            strcpy(pTeleport->szDestName,Recv->pRead());

                            pTeleport->iDestinationX=Recv->iRead();
                            pTeleport->iDestinationY=Recv->iRead();
                            pTeleport->iDestinationZ=Recv->iRead();

                            sprintf(szTemp,"teleports%c%d-%d-%d.ini",PATH_SEP,pTeleport->iSourceX,pTeleport->iSourceY,pTeleport->iSourceZ);
                            pTeleport->bSave(szTemp);

                            pMap->SetTeleport(ax,ay,(cy?true:false));
                            pMap->SaveSector3D("map",bx,by,bz);

                            if(pTeleport->pNext)
                                break;

                            pTeleport->pNext = new CTeleport;
                        }
                        ////*
                        break;

                    default:
                        return;
                        break;
                }

                Send.Write((int)ax);
                Send.Write((int)ay);
                Send.Write((int)az);

                // Send to all players

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    other_client=other_client->pNext;
                }
                pMap->SaveSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(bz));
                */

                break;}
            case NETMSG_SET_TILE: fold_block {
                inactivity_timer->Reset();
                if(1)//!DoesHaveAccess(client->cGetAccess(),"map_build"))
                {
//                        AdminToPlayer(client,"You don't have access to build the map.",255,0,0);
                    // Log("%s(%s) tried to set map tile.",client->avatar->name,client->avatar->name);
                    break;
                }

                ax = Recv->iRead(); // x
                ay = Recv->iRead(); // y
                az = Recv->iRead(); // z
                i  = Recv->cRead(); // bank
                j  = Recv->cRead(); // tile

                // Log("%s(%s) set tile:%d %d %d %d %d %d [%d][%d]",client->avatar->name,client->avatar->name,ax,ay,az,bx,by,bz,i,j);

//                    pMap->LoadSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));
//                    pMap->SetTile(CamCoord(ax),CamCoord(ay),i,j);
//                    pMap->SaveSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));


/*
                Send.Reset();
                Send.Write((char)NETMSG_SET_TILE);
                Send.Write((int)ax);
                Send.Write((int)ay);
                Send.Write((int)az);
                Send.Write((char)i);
                Send.Write((char)j);

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    other_client=other_client->pNext;
                }
*/

                break;}
            case NETMSG_SET_OBJECT: fold_block {
                inactivity_timer->Reset();
                if(1) //!DoesHaveAccess(client->cGetAccess(),"map_build"))
                {
//                        AdminToPlayer(client,"You don't have access to build the map.",255,0,0);
                    break;
                }

                ax = Recv->iRead();  // x
                ay = Recv->iRead();  // y
                az = Recv->iRead();  // z
                i  = Recv->cRead(); // bank
                j  = Recv->cRead(); // object
                k  = Recv->cRead(); // layer

//                    pMap->LoadSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));
//                    pMap->SetObj(ax,ay,i,j,k);
//                    pMap->SaveSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));

/*
                Send.Reset();
                Send.Write((char)NETMSG_SET_OBJECT);
                Send.Write((int)ax);
                Send.Write((int)ay);
                Send.Write((int)az);
                Send.Write((char)i);
                Send.Write((char)j);
                Send.Write((char)k);

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    other_client=other_client->pNext;
                }
*/

                break; }
            case NETMSG_SET_VERTEX: fold_block {

                inactivity_timer->Reset();

                if(1) //!DoesHaveAccess(client->cGetAccess(),"map_build"))
                {
//                        AdminToPlayer(client,"You don't have access to build the map.",255,0,0);
                    break;
                }
/*
                if(!pMapBuffer)
                {
                    // Log("Attempt to set vertex on map that doesn't exist.");
                    break;
                }

                ax = Recv->iRead();  // x
                ay = Recv->iRead();  // y
                az = Recv->iRead();  // z

                cx = Recv->cRead(); // what type of change
                cy = Recv->cRead(); // which vertex
                vx = Recv->cRead(); // red element
                vy = Recv->cRead(); // green element
                vz = Recv->cRead(); // blue element

                LoadMap(ax,ay,az);

                switch(cx)
                {
                    case 0: // height
                        pMapBuffer->SetVertexHeight(CamCoord(ax),CamCoord(ay),cy,vx);
                        break;

                    case 1: // color/lighting
                        pMapBuffer->SetVertexColor(CamCoord(ax),CamCoord(ay),cy,vx,vy,vz);
                        break;

                    case 2: // width
                        pMapBuffer->SetVertexWidth(CamCoord(ax),CamCoord(ay),cy,vx);
                        break;

                    case 3: // relight the entire map sector with this color
                        pMapBuffer->ClearVertexColors(vx,vy,vz);
                        break;

                    default:
                        break;
                }

                SaveMap(ax,ay,az);

                Send.Reset();
                Send.Write((char)NETMSG_SET_VERTEX);
                Send.Write((int)ax);
                Send.Write((int)ay);
                Send.Write((int)az);
                Send.Write((char)cx); // what type of change
                Send.Write((char)cy); // which vertex
                Send.Write((char)vx);
                Send.Write((char)vy);
                Send.Write((char)vz);

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    other_client=other_client->pNext;
                }
*/

                break; }
            case NETMSG_HEARTBEAT: fold_block {
                inactivity_timer->Reset();
                bHeartBeatCheck=0; // OK, clear the heart beat check

                break;}
            case NETMSG_FVM: fold_block {
                inactivity_timer->Reset();
                ax=Recv->cRead();

                switch(ax)
                {
                    case FVM_GETTARGET:
//                                strcpy(client->CurrentTarget,Recv->pRead());
//                                strcpy(client->Script,Recv->pRead());
                        break;

                    case FVM_MOUSEINFO:
//                                client->MouseInfo.x=1; // Recv->iRead();
//                                client->MouseInfo.y=1; // Recv->iRead();
//                                client->MouseInfo.buttonstate=0; // Recv->cRead();
//                                client->SetScript("onclick.fvm");
                        //pFVM->ExecuteScript(client);
                        break;

                    case FVM_GUIBUTTONPRESSED:
                        break;

                    default:
                        break;
                }

                break;}
            case NETMSG_PING: fold_block {
                inactivity_timer->Reset();
                gx=Recv->dwRead();

                Send.Reset();
                Send.Write((char)NETMSG_PING);
                Send.Write((long)gx);

                //client->pSocket->SendUnreliableMessage((CPacket *)&Send);

                break; }
            case NETMSG_FILE_XFER: fold_block {
                inactivity_timer->Reset();
                char *pFileBuf;
                FILE *pFile;
                bool bFileFail;
                switch(Recv->cRead())
                {
                            // NET_FILE_RES_MEDIA
                            // NET_FILE_RES_SCRIPT
                    /*

                    case NET_FILE_NOP:
                        break;

                    case NET_FILE_START:
                        // filename
                        // filesize
                        // start file, put a temporary sequencer in temp folder for the file
                        // ie; hey.bmp will create temp/hey.bmp.sequence
                        strcpy(szTemp1,Recv->pRead());
                        cx=Recv->iRead();
                        sprintf(szTemp2,"%s%ctemp%c%s.sequence",pClientData->FMDir,_PS,_PS,szTemp1);
                        sprintf(szTemp3,"%s%ctemp%c%s",pClientData->FMDir,_PS,_PS,szTemp1);
                        bFileFail=false;
                        pFile=fopen(szTemp3,"wb");
                        if(pFile)
                        {
                            fclose(pFile);
                            pFile=fopen(szTemp2,"wt");
                            if(pFile)
                            {
                                bx=1;
                                fputs(va("%d\n%d\n",bx,cx),pFile);
                                fclose(pFile);
                                SendData.Reset();
                                SendData.Write((char)NETMSG_FILE_XFER);
                                SendData.Write((char *)szTemp1);
                                SendData.Write((char)NET_FILE_START_OK);
                                SendNetMessage(0);
                            }
                            else bFileFail=true;
                        }
                        else bFileFail=true;
                        if(bFileFail)
                        {
                            SendData.Reset();
                            SendData.Write((char)NETMSG_FILE_XFER);
                            SendData.Write((char *)szTemp1);
                            SendData.Write((char)NET_FILE_ERROR);
                            SendNetMessage(0);
                        }

                        break;
                        */

                    /*
                    case NET_FILE_DATA:

                        // sequence number
                        // filename
                        // data block (1024)

                        ax=Recv->iRead(); // sequence number
                        strcpy(szTemp1,Recv->pRead()); // filename
                        pFileBuf=Recv->pRead(NET_FILE_XFER_BLOCK_SIZE);
                        sprintf(szTemp2,"%s%ctemp%c%s.sequence",pClientData->FMDir,_PS,_PS,szTemp1);
                        sprintf(szTemp3,"%s%ctemp%c%s",pClientData->FMDir,_PS,_PS,szTemp1);
                        bFileFail=false;
                        pFile=fopen(szTemp2,"rb");
                        if(pFile)
                        {
                            fgets(szTemp4,256,pFile); bx=atoi(szTemp4);
                            fgets(szTemp4,256,pFile); cx=atoi(szTemp4);
                            fclose(pFile);
                        }
                        else bFileFail=true;
                        if(ax==bx)
                        {
                            pFile=fopen(szTemp3,"a");
                            if(pFile)
                            {
                                fwrite(pFileBuf,NET_FILE_XFER_BLOCK_SIZE,1,pFile);
                                fclose(pFile);
                                pFile=fopen(szTemp2,"wt");
                                if(pFile)
                                {
                                    bx++;
                                    fputs(va("%d\n%d\n",bx,cx),pFile);
                                    fclose(pFile);
                                }

                                SendData.Reset();
                                SendData.Write((char)NETMSG_FILE_XFER);
                                SendData.Write((char)NET_FILE_DATA_OK);
                                SendData.Write((char *)szTemp1);
                                SendData.Write((int)bx);
                            }
                            else
                            {
                                bFileFail=true;
                            }

                        }
                        if(bFileFail)
                        {
                            SendData.Reset();
                            SendData.Write((char)NETMSG_FILE_XFER);
                            SendData.Write((char)NET_FILE_DATA_RESEND);
                            SendData.Write((char *)szTemp1);
                            SendData.Write((int)bx);
                        }
                        break;

                    case NET_FILE_ACK:
                        break;

                    case NET_FILE_ABORT:
                        break;

                    case NET_FILE_EOF:

                        // sequence number
                        // filename
                        // size of data block
                        // data block

                        ax=Recv->iRead(); // sequence number
                        strcpy(szTemp1,Recv->pRead()); // filename
                        dx=Recv->iRead();
                        pFileBuf=Recv->pRead(dx);
                        sprintf(szTemp2,"%s%ctemp%c%s.sequence",pClientData->FMDir,_PS,_PS,szTemp1);
                        sprintf(szTemp3,"%s%ctemp%c%s",pClientData->FMDir,_PS,_PS,szTemp1);
                        bFileFail=false;
                        pFile=fopen(szTemp3,"a");
                        if(pFile)
                        {
                            fwrite(pFileBuf,dx,1,pFile);
                            fclose(pFile);
                            remove(szTemp2);
                        }
                        else
                        {
                            SendData.Reset();
                            SendData.Write((char)NETMSG_FILE_XFER);
                            SendData.Write((char)NET_FILE_EOF_RESEND);
                            SendData.Write((char *)szTemp1);
                        }
                        break;

                    case NET_FILE_RESUME:
                        break;
                        */

//						case NET_FILE_START_OK:
//							break;

                    default:
                        break;

                    }

                break; }
            case NETMSG_NOP: fold_block {
                if (strcmp(Recv->pRead(),"1234567890")==0)
                {
                    Send.Reset();
                    Send.Write((char)NETMSG_NOP);
                    Send.Write("0987654321");
                    //if(client->pSocket->bCanSendMessage())
                        //client->pSocket->SendUnreliableMessage(&Send);
                }

                break;}

            default:
                break;
        }
    }

    if(inactivity_timer->Up()){ // Check for inactivity
        //Log("duh...");

        if(0)//(client->in_limbo==false)
        {
            inactivity_timer->Reset();

            Send.Reset();
            Send.Write((char)NETMSG_HEARTBEAT);
            Send.Write((int)1);
            pSocket->SendUnreliableMessage((CPacket *)&Send);
            bHeartBeatCheck=1;
            //heartbeat_timer->Reset();


            if(0){ //!DoesHaveAccess(client->cGetAccess(),"timeout_override"))

                if(0){ //(strlen(client->avatar->name) && strlen(client->avatar->name))
                    // Log("%s(%s) being kicked due to inactivity.",client->avatar->name,client->avatar->name);
                    inactivity_timer->Reset(); //to prevent server from doing this twice (or more)
                    other_client=pServer->pFirstPlayer;
                    while(other_client){
                        // change to only send to onscreen players later...
                        if(other_client != this){
                            Send.Reset();
                            Send.Write((char)NETMSG_VIS_REMOVE);
                            Send.Write((int)sid);
                            other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                        }
                        other_client=other_client->pNext;
                    }
                    // send shutdown to client
                    Send.Reset();
                    Send.Write((char)NETMSG_CLIENTSHUTDOWN);
                    Send.Write((int)sid);
                    Send.Write((char *)"Inactivity"); // reason
                    pSocket->SendUnreliableMessage((CPacket *)&Send);
                    // Player disconnect here
                    other_client=pServer->pFirstPlayer;
                    while(other_client){
                        // change to only send to onscreen players later...
                        if(other_client != this){
                            Send.Reset();
                            Send.Write((char)NETMSG_VIS_REMOVE);
                            Send.Write((int)sid);
                            other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                        }
                        other_client=other_client->pNext;
                    }
                    bDelete=true; //pServer->Logout(client);
                }
            }
        }
    }

    if(heartbeat_timer->Up() && bHeartBeatCheck) { // Check for heartbeats and remove if not found

                // add in ability to check more than once, say three or four times before it logs out.
                // The client is no longer connected

                other_client=pServer->pFirstPlayer;
                while(other_client)
                {
                    // change to only send to onscreen players later...
                    if(other_client != this){
                        Send.Reset();
                        Send.Write((char)NETMSG_VIS_REMOVE);
                        Send.Write((int)sid);
                        other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    }
                    other_client=other_client->pNext;
                }
                bDelete=true;
    }

}

#ifdef __linux__
#include "s_linux.h"
#include "s_gnu.h"
void ConsoleSetup(void) { _kbhit(); }
void ConsoleShutDown(void) { close_keyboard(); }
#endif

#ifdef _WIN32
BOOL WINAPI HandleCTRL_C(DWORD dwCtrlType){
    switch(dwCtrlType)
    {

        case CTRL_BREAK_EVENT:
            bQuit=true;
            pLog->_Add("Break event killed server!");
            return true;

        case CTRL_SHUTDOWN_EVENT:
            bQuit=true;
            pLog->_Add("Shutdown event killed server!");
            return true;

        case CTRL_LOGOFF_EVENT:
            bQuit=true;
            pLog->_Add("Logoff event killed server!");
            return true;

        case CTRL_CLOSE_EVENT:
            bQuit=true;
            pLog->_Add("Mouse [X] killed server!");
            return true;

        case CTRL_C_EVENT:
            bQuit=true;
            pLog->_Add("CTRL-C killed server!");
            return true;

        default:
            break;
    }
    return false;
}
void ConsoleSetup(void){
    // Change window title for windows version, and setup ctrl handler
    SetConsoleCtrlHandler(HandleCTRL_C,TRUE);
    SetConsoleTitle(va("Ember Game Server %s(%s) Net Revision(%s) %s",VERSION,CPUSTRING,NET_REVISION,COPYRIGHT));
}
void ConsoleShutDown(void){
}

#endif
