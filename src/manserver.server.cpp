/////////////////////////////////////////
#include "manserver.server.h"

#include "c_var.h"

//////////////////////////////////////////////////////////////////////////////////

CServer::CServer(bool bIsQuiet) {
    pLog         = 0;
    bQuiet       = bIsQuiet;
    bQuit        = false;
    bRestart     = false;
    pFirstPlayer = 0;
    pConsole     = 0;
    pCVars       = 0;
    pDB          = 0;
    randomize();
    pLog = new CLog("server.log", bQuiet);  // LOG
    if (!pLog) {
        printf("\nNo memory for log!\n");
        exit(0);
    }
    pLog->On();
    pLog->Restart();
    StartUp();  // STARTUP/SHUTDOWN
}

//////////////////////////////////////////////////////////////////////////////////

CServer::~CServer() {
    kick_all("Server shutting down");
    C_GSC *c = pFirstPlayer;
    while (c) {
        pFirstPlayer = c;
        c            = c->pNext;
        DEL(pFirstPlayer);
    }
    ShutDown();  // STARTUP/SHUTDOWN
    NET_Shutdown();
    DEL(pLog);
}

//////////////////////////////////////////////////////////////////////////////////

bool CServer::bCheckRestart(void) {
    if (bRestart) {
        LogEntry("[Initiating server restart]=====================================");
        ShutDown();
        StartUp();
        LogEntry("==============================================[Server restarted]");
        bRestart = 0;
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::SetDefaultCVars() {
    LogEntry("Setting default CVar values...");  // Todo: move these default CVar definitions into ember_server.h
    pCVars->Set("b_new_accounts", "0");
    pCVars->Set("b_require_website", "0");
    pCVars->Set("i_db_mysql_port", "3036");
    pCVars->Set("i_max_clients", "2000");
    pCVars->Set("i_port", "7000");
    pCVars->Set("i_world_save_timer", "300000");  // 5 minutes tick time
    pCVars->Set("s_admin_email", "admin@url");
    pCVars->Set("s_db_lmdb_location", "lmdb_location");
    pCVars->Set("s_db_mysql_db", "mysql_db");
    pCVars->Set("s_db_mysql_host", "192.168.1.81");
    pCVars->Set("s_db_mysql_pass", "mysql_pass");
    pCVars->Set("s_db_mysql_user", "mysql_user");
    pCVars->Set("s_db_sqlite_file", "data.sqlite");
    pCVars->Set("s_db_type", "sqlite");
    pCVars->Set("s_motd", "mantra");
    pCVars->Set("s_name", "mantra");
    pCVars->Set("s_website_link", "serverurl");
}

//////////////////////////////////////////////////////////////////////////////////

int CServer::StartUp(void) {
    ////////////////////////////////////////////////////////////////////////////
    // Initialize some variables
    dwStartTime  = dlcs_get_tickcount();
    int  iRetVal = false;
    bool bCheck  = false;
    dlcsm_make_str(szTemp);

    ////////////////////////////////////////////////////////////////////////////
    // Log title stuff
    LogEntry("////////////////////////////////////////////////////////////////////////////");
    LogEntry("Mantra Server %s(%s - %s) Net(%s) Build(%d) %s\n",  //
             MANTRA_VERSION,                                      //
             DLCS_CPU_STRING,                                     //
             DLCS_OS_STRING,                                      //
             MANTRA_NET_REVISION,                                 //
             MANTRA_S_BUILD,                                      //
             MANTRA_COPYRIGHT);
    LogEntry("BUILD: %s %s \n", __DATE__, __TIME__);
    LogEntry("////////////////////////////////////////////////////////////////////////////");

    ////////////////////////////////////////////////////////////////////////////
    // Log start time... Todo: Store proper timestamp for server start time
    // LogEntry("STARTTIME:%s", dlcs_timestamp(dwStartTime));

    ////////////////////////////////////////////////////////////////////////////
    // Setup CVars
    LogEntry("Initializing CVars...");
    pCVars = new CVarSet(pLog);
    // Define default variables
    SetDefaultCVars();
    // Overwrite defaults from server.ini
    LogEntry("Loading server.ini CVar values...");
    pCVars->bLoad("server.ini");
    LogEntry("CVars initialized...");
    /*  LogEntry("s_name         = [%s]", (char *)pCVars->szGet("s_name"));
        LogEntry("s_admin_email  = [%s]", (char *)pCVars->szGet("s_admin_email"));
        LogEntry("s_website_link = [%s]", (char *)pCVars->szGet("s_website_link"));
        LogEntry("s_motd         = [%s]", (char *)pCVars->szGet("s_motd"));         */
    LogEntry("////////////////////////////////////////////////////////////////////////////");

    ////////////////////////////////////////////////////////////////////////////
    // Initialize Console
    LogEntry("Initializing Console...");
    pConsole = new C_CONS(pLog, pCVars);  // Initialize Console for it has cvars built in
    // Todo: SetupConsoleHistory
    // SetupConsoleHistory();
    // Todo: Register Server Functions
    // pCVars->bRegisterFunction("kick_all", kick_all);

    ////////////////////////////////////////////////////////////////////////////
    // Attempt to disable power management
    // Todo: make this a configurable option under CVars
    LogEntry("Disabling power management...");
    dlcs_suspend_power_management();

    ////////////////////////////////////////////////////////////////////////////
    // Attempt to Initialize the Network
    LogEntry("Initializing Network...");
    iRetVal = bNetStartUp();
    if (!iRetVal) {
        LogEntry("Network initialization FAILURE!");
        return iRetVal;
    } else {
        LogEntry("Network initialized...");
    }

    ////////////////////////////////////////////////////////////////////////////
    // Initialize Chat Channels
    LogEntry("Initializing Default Chat Channels...");
    dfGUI_CHAT();
    LogEntry("Default chat channels initialized...");

    ////////////////////////////////////////////////////////////////////////////
    // Initialize Database Todo: finish this section
    LogEntry("Initializing Database...");

    bCheck = true;
    pDB    = new C_DLCS_DB(pLog);
    if (pDB) {
        strcpy(szTemp, pCVars->szGet("s_db_type"));
        if (dlcs_strcasecmp(szTemp, "sqlite")) {
            if (pDB->OpenSQLiteDB(pCVars->szGet("s_db_sqlite_file"))) {
                LogEntry("Can not initialize database...");
                bCheck = false;
            }
        }

        if (dlcs_strcasecmp(szTemp, "mysql")) {
            bCheck = false;
        }
        if (dlcs_strcasecmp(szTemp, "lmdb")) {
            bCheck = false;
        }
    } else {
        LogEntry("Can not allocate memory for pDB! FATAL!");
        bCheck = false;
    }
    if (!bCheck) {
        bQuit = true;
        return;
    }

    /*  pSQLite = new C_SQLite();  // Open User Database
        if (pSQLite) {
            LogEntry("SQLite started");
            pSQLite->pLog = pLog;
            pSQLite->OpenDB("data_new.sqlite");
            LogEntry("SQLite opened DB data_new.sqlite");
        } */

    ////////////////////////////////////////////////////////////////////////////
    // Initialize DB Tables if DB opens but finds no data
    // pSQLite->db_query("update users set chat_channels = 'SYSTEM'",0);
    // pSQLite->db_query("alter table users add column NAME char(256) default 'SYSTEM'");
    // if (!pSQLite->db_query("select * from users")) {
    // LogEntry("=====================[WARNING! ERROR!]======================");
    // LogEntry("Database [data.sqlite] empty or corrupt");
    // LogEntry("=====================[WARNING! ERROR!]======================");
    // bQuit = true;
    // where username='seth'
    // pSQLite->db_query("CREATE TABLE users (username varchar(32), password varchar(32), access smallint)",0);
    // pSQLite->db_query("CREATE TABLE users(id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT default 'user',password TEXT default 'none',access smallint default '0',chat_channels TEXT default 'SYSTEM')",0);
    // pSQLite->db_query((char *)va("INSERT INTO users VALUES (1, 'seth', '%s', 255)",md5_digest("123")),0);
    // pSQLite->db_query((char *)va("INSERT INTO users (username) VALUES ('seth_also')",0));
    // pSQLite->db_query((char *)va("INSERT INTO users VALUES ('seth','%s', 255)",md5_digest("123")),0);
    // pSQLite->db_query((char *)va("INSERT INTO users VALUES ('test','%s', 5)",  md5_digest("226fi3")),0);
    // pSQLite->db_query((char *)va("INSERT INTO users VALUES ('test2','%s', 6)", md5_digest("2326df3")),0);
    // pSQLite->db_query((char *)va("INSERT INTO users VALUES ('test4','%s', 7)", md5_digest("223k6gf3")),0);
    // pSQLite->db_query((char *)va("INSERT INTO users VALUES ('zany','%s', 8)",  md5_digest("22lg63f3")),0);
    //  } else {
    // LogEntry("seth.password=[%s]",       (char *)  db_getvalue("username","seth","password").c_str());
    // LogEntry("seth.access=[%d]",     atoi((char *) db_getvalue("username","seth","access").c_str()));
    //}
    //

    LogEntry("Database initialized... (TODO PLACEHOLDER TEXT)");

    ////////////////////////////////////////////////////////////////////////////
    // Todo: finish load_world function
    LogEntry("Initializing World...");
    LoadWorld();
    int t1, t2, t3, t4;
    t1 = pCVars->iGet("i_world_save_timer");
    t2 = t1 / 60000;
    t3 = t1 % 60000;
    t4 = t3 / 1000;
    LogEntry("World save timer set to [%d] (%d minutes, %d seconds)", t1, t2, t4);
    LogEntry("World loaded...");

    ////////////////////////////////////////////////////////////////////////////
    // Todo: Add in character loading for NPC's and such here
    LogEntry("Initializing NPCs...");
    /*  C_GSC *c_x = 0;
        c_x        = new C_GSC(pLog, this);
        strcpy(c_x->username, "seth");
        c_x->load();
        CMPCharacter *pc = new CMPCharacter(c_x);
        // pc->load(pSQLite);  // add dlcs_db features
        pc->gender                = 1;
        pc->health->current_value = 2;
        DEL(c_x);
        DEL(pc); */
    LogEntry("NPC's initialized... (TODO PLACEHOLDER TEXT)");

    LogEntry("////////////////////////////////////////////////////////////////////////////");
    LogEntry("CServer::StartUp... FINISHED!            (Type '/help' for help on commands)");
    LogEntry("////////////////////////////////////////////////////////////////////////////");
    return iRetVal;
}

//////////////////////////////////////////////////////////////////////////////////

bool CServer::bNetStartUp() {
    int iPort;
    iPort = pConsole->pCVars->iGet("i_port");
    NET_Init();
    initSocket();
    int iListenRet = Listen(iPort, true);
    if (iListenRet == -1) {
        LogEntry("**** ERROR Listening on port [%d][%d][%s] ****\n",  //
                 iPort,                                               //
                 iListenRet,                                          //
                 NET_pGetLastError());
        return false;
    } else {
        LogEntry("Listening on port [%d]",  //
                 iGetLocalPort());          //
                                            // iListenRet,                       //
        // NET_pGetLastError());
        return true;
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::ShutDown(void) {
    LogEntry("////////////////////////////////////////////////////////////////////////////");
    LogEntry("Shutting down the server...");
    LogEntry("////////////////////////////////////////////////////////////////////////////");
    LogEntry("CServer::ShutDown: Saving World");
    SaveWorld();
    LogEntry("CServer::ShutDown: Saving CVars");
    bSaveCVars();
    LogEntry("CServer::ShutDown: Cleaning up Console");
    DEL(pConsole);
    // RemoveConsoleHistory();
    LogEntry("CServer::ShutDown: Cleaning up CVars");
    DEL(pCVars);
    LogEntry("CServer::ShutDown: Cleaning up Players");
    DEL(pFirstPlayer);
    LogEntry("CServer::ShutDown: Cleaning up Database");
    DEL(pDB);

    //////////////////////////////////////////////////////////////////////////////////

    /*
        LogEntry("==================================================================================================");
        LogEntry("                                                   ░░░   ░░░                        ░▒▒░ ");
        LogEntry("         ▒▓█████████▓▒     ░░    ▒▒    ░░░  ▓▓▓  ░██▓                      ▒██▒");
        LogEntry("         ▒▓█████████▓▒     ▓█░   ▓█    ▓██░ ███  ▒███                     ▓██▓");
        LogEntry("       ░▓█████████████▓▒  ▓██ ░███   ▒██░ ███  ▒███                     ▓██▓");
        LogEntry("       ▒███░  ░██▓  ░██▓▒  ▓██████▒    ░░  ███  ░███░                   ░███▓");
        LogEntry("       ▒█████████▓  ▒██▒   ▓████▓░   ░▒▒░ ███   ███▒  ░▓▓▓███▒ ░▒██████▓");
        LogEntry("       ▓███████████████░  ▓█████▒   ▒██▒ ███   ███▓  ▒▓██░ █▓░███░▒███▓");
        LogEntry("       ░▓▓███████████▓▒   ▓██▓▒██    ██   ███   ███▓  ▓█▓  ▓██▒██░  ▒██▓");
        LogEntry("          ▓█▒▒ ░░▒▒▒█▒       ▓██ ░▒██▓ ██▓  ███▒  ████░▓████▓▒  ██   ░██▓");
        LogEntry("          ▓█▒▒ ░░▒▒▒▒        ▒██   ░██░▒██▒  ████░ ████▓▓██▓    ▒███▒░▒███░");
        LogEntry("             ▒░░   ░░ ░        ░▓▒    ▒█░░██▒   ▒███░ ▒███▓▒▒███▒ ░▓████████▓░");
        LogEntry("          ░▒▓▓▓▓▓▓▓▒░       ░▒▒▒░░░▓█▓▒░  ░░░▒▓▓░░  ░▒▒░ ░▓████▓░ ░▒██▓▓▒░");
        LogEntry(" ");
        LogEntry("==================================================================================================");
        */
    LogEntry("////////////////////////////////////////////////////////////////////////////");
    LogEntry("Server Killed!");
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::DoKeyboard() {
    u_char ch = 0;
    if (_kbhit()) {
        ch = _getch();
        switch (ch) {
            case 0:
                ch = _getch();
                switch (ch) {
                    default: break;
                }
                break;

            case 13:
            case 10:
                /* TODO: Re Add History
                    AddToConsoleHistory(szConsoleInput);
                    iConsoleHistoryPosition = 0;                    */

                LogEntry(">%s", pConsole->szConsoleInput);
                if (pConsole->szConsoleInput[0] == 0)
                    break;
                else {
                    ExecuteConsoleCommand(pConsole->szConsoleInput);
                    memset(pConsole->szConsoleInput, 0, _DLCS_CONSOLE_ENTRY_SIZE);
                }
                break;

            case 27: break;

            case 8:  // backspace
                pConsole->szConsoleInput[strlen(pConsole->szConsoleInput) - 1] = 0;
                printf(">                                                  \r>%s \r", pConsole->szConsoleInput);
                break;

            case 21:  // ctrl-U
                printf(">                                                  \r>");
                memset(pConsole->szConsoleInput, 0, _DLCS_CONSOLE_ENTRY_SIZE);
                break;

            case 0xE0:  // Arrow keys...
                ch = _getch();
                switch (ch) {
                    case 72:  // UP
                              /*
                                           strcpy(szConsoleInput, GetConsoleHistory(iConsoleHistoryPosition));
                                           iConsoleHistoryPosition++;
                                           if (iConsoleHistoryPosition > MAX_CONSOLE_INPUT_BUFFER) iConsoleHistoryPosition = 0;
                                           printf(">                                                  \r>%s \r", szConsoleInput);
                                           */
                        break;

                    case 80:  // DOWN
                              /*
                                           strcpy(szConsoleInput, GetConsoleHistory(iConsoleHistoryPosition));
                                           iConsoleHistoryPosition--;
                                           if (iConsoleHistoryPosition < 0) iConsoleHistoryPosition = MAX_CONSOLE_INPUT_BUFFER;
                                           printf(">                                                  \r>%s \r", szConsoleInput);
                                           */

                        break;

                    case 75:  // LEFT (TODO: build cursor into console command input)
                        break;

                    case 77:  // RIGHT (TODO: build cursor into console command input)
                        break;

                    default: break;
                }
                break;

            default:
                int i                           = strlen(pConsole->szConsoleInput);
                pConsole->szConsoleInput[i]     = ch;
                pConsole->szConsoleInput[i + 1] = 0;
                printf(">                                                  \r>%s \r", pConsole->szConsoleInput);
                break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::MainLoop(void) {
    while (!bQuit) {
        AcceptConnections();
        DoPlayers();
        DoKeyboard();
        DoWorld();
        SaveWorld();
        PurgeZombieClients();
        bCheckRestart();
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::DoPlayers(void) {
    static C_GSC *pClient = 0;
    if (!pClient) pClient = pFirstPlayer;
    if (pClient) {
        pClient->do_net();
        pClient = pClient->pNext;
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::AcceptConnections(void) {
    C_GSC *            pClient;
    int                iLength;
    int                iFlags, iHelp;
    int                iAcceptSocket;
    char               cType;
    struct sockaddr_in ServerAddr;
    memset(&ServerAddr, 0, sizeof(ServerAddr));
    CPacket Send(NET_DATAGRAMSIZE);
    CPacket Recv(NET_DATAGRAMSIZE);
    iAcceptSocket = CheckNewConnections();
    if (iAcceptSocket == -1) return;
    if ((iLength = nRecv(iAcceptSocket, Recv.pGetPacketBuffer(), Recv.iGetMaxSize(), (struct sockaddr *)&ServerAddr)) <= 0) return;
    Recv.SetCurSize(iLength);
    iHelp  = ntohl(Recv.iRead());
    iFlags = iHelp & (~NET_FLAG_LENGTH_MASK);
    iHelp &= NET_FLAG_LENGTH_MASK;
    if (!(iFlags & NET_FLAG_CTL)) return;
    if (iLength != iHelp) return;
    if (iLength < (int)sizeof(int) * 2) return;
    Recv.iRead();
    cType = Recv.cRead();
    switch (cType) {
        //////////////////////////////////////////////////////////////////////////////////
        case CTL_CONNECT:
            if (strcmp(Recv.pRead(), "MANTRA")) {
                LogEntry("Fail attempt, not MANTRA packet");
                RejectConnection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "This is an Ember Game Server");
                return;
            }
            if (Recv.iRead() != atoi(MANTRA_NET_REVISION)) {
                RejectConnection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "Server uses newer protocol! Update available.");
                return;
            }

            // TODO: Add reject (PRIVATE SERVER)
            // TODO: Add BAN IP Rejection

            pClient = new C_GSC(pLog, this);
            if (!pClient) {
                LogEntry("manserver.server.cpp -> Can't create client");
                return;
            }
            if (!pClient) {
                RejectConnection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "Server can't assign a new socket!");
                return;
            }
            memcpy(&pClient->ToAddr, &ServerAddr, sizeof(ServerAddr));
            pClient->iSocket = zOpenSocket(0);
            if (pClient->iSocket == -1) {
                DEL(pClient);
                RejectConnection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "Server can't assign new socket!");
                return;
            }
            Send.Reset();
            Send.Write(0);
            Send.Write(0);
            Send.Write((char)CTL_ACCEPT);
            Send.Write(pClient->iGetLocalPort());
            FinishCtlPacket(&Send);
            if (nSend(iAcceptSocket, Send.pGetPacketBuffer(), Send.iGetCurSize(), (struct sockaddr *)&ServerAddr) == -1) {
                DEL(pClient);
                LogEntry("s_server.cpp -> CSocket::pAccept() -> error making a new connection");
                return;
            }
            pClient->bCanSend   = true;
            pClient->bConnected = true;
            break;
        //////////////////////////////////////////////////////////////////////////////////
        case CTL_PING:
            Send.Reset();
            Send.Write(0);
            Send.Write(0);
            Send.Write((char)CTL_PING);
            Send.Write((long)Recv.dwRead());
            FinishCtlPacket(&Send);
            if (nSend(iAcceptSocket, Send.pGetPacketBuffer(), Send.iGetCurSize(), (struct sockaddr *)&ServerAddr) == -1) {
                LogEntry("s_server.cpp -> respond to pingrequest failed");
            }
            return;
        //////////////////////////////////////////////////////////////////////////////////
        case CTL_REJECT:
            LogEntry("CTL_REJECT ?");  //
            return;
        //////////////////////////////////////////////////////////////////////////////////
        default:
            RejectConnection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "This is a Mantra gameserver");
            LogEntry("UNKNOWN CTL_PACKET (%d) received", cType);
            return;
    }
    // Move client to the front of the list (can get last user logged in information from pFirstPlayer)
    if (pFirstPlayer) {
        pClient->pNext = pFirstPlayer;  //
    }
    pFirstPlayer = pClient;
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::RejectConnection(int iReSocket, sockaddr *pReAddr, const char *szReason) {
    CPacket pSend(NET_DATAGRAMSIZE);
    if (!szReason) return;
    pSend.Reset();
    pSend.Write(0);
    pSend.Write(0);
    pSend.Write((char)CTL_REJECT);
    pSend.Write((char *)szReason);
    FinishCtlPacket(&pSend);
    if (nSend(iReSocket, pSend.pGetPacketBuffer(), pSend.iGetCurSize(), pReAddr) == -1) {
        LogEntry("Reject message failed (%s)", szReason);
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::SendAll(CPacket *pPacket, float fBlockTime) {
    C_GSC *pClient;
    pClient = pFirstPlayer;
    while (pClient) {
        if (!pClient->bDelete) pClient->SendUnreliableMessage(pPacket);
        pClient = pClient->pNext;
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::Disconnect(C_GSC *pClient, const char *szReason) {
    CPacket pSend(NET_DATAGRAMSIZE);
    if (!pClient) return;
    if (pClient) {
        pSend.Reset();
        pSend.Write((char)NM_CLIENT_SHUTDOWN);
        pSend.Write((char *)pClient->session_id);
        pSend.Write((char *)szReason);
        pClient->SendUnreliableMessage(&pSend);
    }
    pSend.Reset();
    pSend.Write((char)NM_OTHER_CLIENT_SHUTDOWN);
    pSend.Write((char *)pClient->session_id);
    SendAll(&pSend, 0);
    pClient->bDelete = true;
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::SaveWorld(void) {
    int         iWorldSaveTimer = pCVars->iGet("i_world_save_timer");
    static long dwSaveTimer     = dlcs_get_tickcount();
    if ((dlcs_get_tickcount() - dwSaveTimer) > iWorldSaveTimer) {
        dwSaveTimer = dlcs_get_tickcount();
        LogEntry("World data saved i_world_save_timer=[%d]...", iWorldSaveTimer);
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::LoadWorld(void) {
    LogEntry("World data loaded...");  //
}

//////////////////////////////////////////////////////////////////////////////////

C_GSC *CServer::pGetClient(const char *szUserName) {
    C_GSC *pClient = pFirstPlayer;
    while (pClient) {
        if (dlcs_strcasecmp(pClient->username, szUserName)) return pClient;
        pClient = pClient->pNext;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::kick_all(const char *szReason) {
    C_GSC *pClient;
    pClient = pFirstPlayer;
    if (!pClient) return;
    LogEntry("Kicking all online players");
    while (pClient) {
        LogEntry("       ....%s", pClient->username);
        Disconnect(pClient, szReason);
        pClient = pClient->pNext;
    }
    LogEntry("All players kicked!");
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::kick_user(const char *szFormat, ...) {
    dlcsm_make_sizestr(buffer, _DLCS_CONSOLE_ENTRY_SIZE);
    dlcsm_make_textstr(szUserName);
    dlcsm_make_textstr(szReason);
    va_list args;
    va_start(args, szFormat);
    vsprintf(buffer, szFormat, args);
    // perror(buffer);
    va_end(args);

    // LogEntry("CServer::kick_user(const char *format, ...) %s", buffer);

    dlcsm_explode_list(explode_list);
    explode_list = dlcs_explode(" ", buffer);

    vector<string>::iterator ex_data = explode_list.begin();

    if (explode_list.size() == 1) {
        strcpy(szUserName, ex_data->c_str());
        strcpy(szReason, "Server admin kicked you.");
    } else {
        strcpy(szUserName, ex_data->c_str());
        for (vector<string>::iterator ex_data_2 = explode_list.begin() + 1; ex_data_2 != explode_list.end(); ++ex_data_2) {
            strcat(szReason, va("%s ", ex_data_2->c_str()));
        }
    }

    dlcsm_rtrim(' ', szReason);

    LogEntry("CServer::kick_user: szUserName[%s] szReason[%s]", szUserName, szReason);

    C_GSC *pUser = pFirstPlayer;
    while (pUser) {
        if (dlcs_strcasecmp(pUser->username, szUserName)) {
            Disconnect(pUser, szReason);
            LogEntry("CServer::kick_user: Admin kicked user [%s]", szReason);
            return;
        }
        pUser = pUser->pNext;
    }

    LogEntry("CServer::kick_user: %s is not online!", szUserName);
    return;
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::add_user(const char *szUserName, int iAccess) {
    // add dlcs_db add player query
    // pSQLite->db_query(va("insert into users (username,access) values ('%s',%d)", in, ia));
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::delete_user(const char *szUserName) {
    // add dlcs_db delete user query
    // pSQLite->db_query("delete * from users where usersname = '%s'", szName);
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::set_user_access(const char *szUserName, int iAccess) {
    if (iAccess > 255) return;  // Todo: look at setting up access groups
    if (iAccess < 0) return;
    // Database change
    // pSQLite->db_query("update users set access=%d where username='%s'", in_access, in_user_name);
    // Online change
    C_GSC *pClient = pGetClient(szUserName);
    if (pClient) pClient->access = iAccess;
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::Chat(const char *szTo, const char *szFrom, const char *szMsg, int iChannel) {
    C_GSC *pPlayer = pGetClient(szTo);
    if (pPlayer) {
        Chat(pPlayer, szFrom, szMsg, iChannel);
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::Chat(C_GSC *pClient, const char *szFrom, const char *szMsg, int iChannel) {
    CPacket Send(NET_DATAGRAMSIZE);
    Send.Reset();
    Send.Write((char)NM_MESSAGE);
    Send.Write((char *)szMsg);
    Send.Write((char *)szFrom);
    Send.Write((int)iChannel);  // CHANNEL
    if (pClient) Send.Write((char *)pClient->session_id);
    switch (iChannel) {
        case CHANNEL_CLAN: break;
        case CHANNEL_GENERAL: break;
        case CHANNEL_TRADE: break;
        case CHANNEL_SAY: break;
        case CHANNEL_YELL: break;
        case CHANNEL_LOCAL: break;
        case CHANNEL_PARTY: break;
        case CHANNEL_RAID: break;
        case CHANNEL_SYSTEM: SendAll((CPacket *)&Send, 0); break;
        case CHANNEL_WHISPER: pClient->SendUnreliableMessage((CPacket *)&Send); break;
        case CHANNEL_CUSTOM: break;
        default: break;
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::PurgeZombieClients(void) {
    C_GSC *pClient = 0;
    C_GSC *pDelete = 0;
    pClient        = pFirstPlayer;
    if (!pClient) return;
    while (pClient->pNext) {
        if (pClient->pNext->bDelete) {
            pDelete = pClient->pNext;
            if (pClient->pNext->pNext) pClient->pNext = pClient->pNext->pNext;
            DEL(pDelete);
        }
        if (pClient->pNext) pClient = pClient->pNext;
    }
    if (pClient == pFirstPlayer) {
        if (pClient) {
            if (pClient->bDelete) {
                DEL(pFirstPlayer);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::DoWorld(void) {
    //
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::ExecuteConsoleCommand(char *szCommand) {
    vector<string> v = dlcs_explode(" ", szCommand);

    dlcsm_make_sizestr(szTempConsoleString1, _DLCS_CONSOLE_ENTRY_SIZE);
    dlcsm_make_sizestr(szTempConsoleString2, _DLCS_CONSOLE_ENTRY_SIZE);
    dlcsm_make_sizestr(szTargs, _DLCS_CONSOLE_ENTRY_SIZE);

    if (v.size() > 0) {
        strcpy(szTargs, " ");
        if (v.size() > 1) {
            for (unsigned int i = 1; i < v.size(); i++) {
                strcat(szTargs, v[i].c_str());
                strcat(szTargs, " ");
            }
        }

        if (dlcs_strcasecmp(v[0].c_str(), "/help")) {
            LogEntry("[List of available commands]====================================");
            LogEntry("/db..............................Database management");
            LogEntry("/nuy.............................Allow new user accounts");
            LogEntry("/nun.............................Do not allow new user accounts");
            LogEntry("/ann <message>...................Announce a message to the entire server");
            LogEntry("/msg <name> <message>............Send message to specific player");
            LogEntry("/motd [new motd].................View, or change the message of the day");
            LogEntry("/who.............................List online players");
            LogEntry("/users...........................List users from database");
            LogEntry("/add_user <name> [access]........Add new user to database");
            LogEntry("/remove_user <name>..............Remove a user account");
            LogEntry("/kick_user <name> [message]......Kick player with optional message");
            LogEntry("/access <name> <access>..........Change player's access level");
            LogEntry("/kill [minutes]..................Kill the server [minutes] (CTRL-C immediate)");
            LogEntry("/unkill..........................Stop kill countdown");
            LogEntry("/restart [minutes]...............Restart the server [minutes]");
            LogEntry("/unrestart.......................Stop restart countdown");
            LogEntry("/time............................See time / timestamp");
            LogEntry("/ban <user>......................Ban player (will ban user)");
            LogEntry("/banip <user>....................Ban player (will ban user's ip address)");
            LogEntry("/unban <user>....................Unban player (will unban user)");
            LogEntry("/unbanip <ipaddress>.............Unban player (will unban user's ip address)");
            LogEntry("/bandomain <domain>..............Ban entire domain");
            LogEntry("/unbandomain <domain>............Unban entire domain");
            LogEntry("/banlist.........................List all banned ip's");
            LogEntry("======================================================[End List]");
            return;
        }

        ///////////////////////////////////////////////////////////////////////////

        if ((dlcs_strcasecmp(v[0].c_str(), "/db"))) {
            // call dlcs_db functions to manipulate database configuration
            // pSQLite->db_queryl(targs);
        }

        //////////////////////////////////////////

        if ((dlcs_strcasecmp(v[0].c_str(), "/users"))) {
            //  call new CServer class function to return user list
            // pSQLite->db_queryl("select username,access from users");
        }

        //////////////////////////////////////////

        if ((dlcs_strcasecmp(v[0].c_str(), "/add_user"))) {
            if (v.size() > 1) {
                if (v.size() > 2) {
                    add_user((char *)v[1].c_str(), atoi((char *)v[2].c_str()));
                } else {
                    add_user((char *)v[1].c_str(), 0);
                }
            }
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/delete_user")) {
            if (v.size() > 1) delete_user((char *)v[1].c_str());
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/set_user_access")) {
            if (v.size() > 2) {
                int dx = atoi(v[2].c_str());  // dx is access
                if ((dx < 0) || (dx > 255)) dx = 0;
                set_user_access((char *)v[1].c_str(), dx);
            }
        }

        //////////////////////////////////////////

        if ((dlcs_strcasecmp(v[0].c_str(), "/time"))) {
            strcpy(szTempConsoleString1, dlcs_get_time());
            LogEntry(szTempConsoleString1);
            dlcs_timestamp(szTempConsoleString1);
            LogEntry("timestamp: %s", szTempConsoleString1);
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/ann")) {
            if (v.size() > 1) {
                Chat((C_GSC *)0, "Admin", szTargs, CHANNEL_SYSTEM);
                LogEntry(szTargs);
            }
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/msg")) {
            if (v.size() > 2) {
                strcpy(szTargs, " ");
                if (v.size() > 2) {
                    for (unsigned int i = 2; i < v.size(); i++) {
                        strcat(szTargs, v[i].c_str());
                        strcat(szTargs, " ");
                    }
                }
                Chat(v[1].c_str(), "Admin", szTargs, CHANNEL_WHISPER);
                // chat(pGetClient((char *)v[1].c_str()), "Admin", szTargs, CHANNEL_WHISPER);
                // Todo add CVar to change the text Admin when /msg sent from console
                // and/or add option to console command
            }
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/kick_user")) {
            if (v.size() > 2) {
                strcpy(szTempConsoleString1, "");
                for (int i = 2; i < v.size(); i++) {
                    strcat(szTempConsoleString1, va("%s ", (char *)v[i].c_str()));
                }
                kick_user("%s %s", (char *)v[1].c_str(), szTempConsoleString1);
            }  //
            else if (v.size() > 1) {
                kick_user("%s %s", (char *)v[1].c_str(), "Kicked by admin");
            }
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/kill")) bQuit = true;

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/restart")) bRestart = 1;

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/nuy")) {
            pConsole->pCVars->Set("b_new_accounts", true);
            LogEntry("Now accepting new users");
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/nun")) {
            pConsole->pCVars->Set("b_new_accounts", false);
            LogEntry("New users will not be accepted");
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/who")) {
            C_GSC *c = pFirstPlayer;
            while (c) {
                LogEntry("%s", c->username);
                c = c->pNext;
            }
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/motd")) {
        }

        //////////////////////////////////////////
    }
}

//////////////////////////////////////////////////////////////////////////////////
// char *CServer::GetConsoleHistory(int x) {
// if (SConsoleHistory) return (SConsoleHistory[x].text);
// return (char *)(va("con history error![%d]", x));
//}

//////////////////////////////////////////////////////////////////////////////////
// void CServer::SetupConsoleHistory(void) {
// SConsoleHistory = 0;
// SConsoleHistory = new stConHistory[MAX_CONSOLE_INPUT_BUFFER + 1];
// for (int i = 0; i < MAX_CONSOLE_INPUT_BUFFER; i++) memset(SConsoleHistory[i].text, 0, _DLCS_CONSOLE_ENTRY_SIZE);
// iConsoleHistoryPosition = 0;
//}

//////////////////////////////////////////////////////////////////////////////////
// void CServer::AddToConsoleHistory(char *s) {
// TODO: Access pConsole (add history)
// for (int i = MAX_CONSOLE_INPUT_BUFFER; i > 0; i--) strcpy(SConsoleHistory[i].text, SConsoleHistory[i - 1].text);
// strcpy(SConsoleHistory[0].text, s);
//}

/////////////////////////////////////////
// void CServer::RemoveConsoleHistory(void) {
// delete[] SConsoleHistory;
// SConsoleHistory = 0;
//}

//////////////////////////////////////////////////////////////////////////////////

bool CServer::bSaveCVars(void) { return pCVars->bSave("server.ini"); }

//////////////////////////////////////////////////////////////////////////////////

int CServer::iNumClients() {
    int    x = 0;
    C_GSC *c = pFirstPlayer;
    while (c) {
        x++;
        c = c->pNext;
    }
    return x;
}

//////////////////////////////////////////////////////////////////////////////////

void CServer::create_guid(const char *szCin, char *szCout) {
    char t[1024];
    memset(t, 0, 1024);
    char t2[1024];
    memset(t2, 0, 1024);
    strcpy(t, va("%s%d", szCin, rand() % 99999));
    strcpy(t2, dlcs_md5_digest(t).c_str());
    dlcs_timestamp(t);
    strcpy(szCout, va("%s-%s-%05d", t2, t, rand() % 99999));
}

//////////////////////////////////////////////////////////////////////////////////
void CServer::ReportToMaster(void) {
    static bool starter       = false;
    static long dwReportTimer = GetTickCount();
    // if(!dlcs_strcasecmp(gmvar("sys.master_report"),"yes")) return;
    //     if(!pFMMS_Connection) return;
    // if((GetTickCount()-dwReportTimer)<15000) {
    //    if(!starter) starter=true; else return;
    //}
    // dwReportTimer=GetTickCount();
    // LogEntry("Reported to master");
    // pFMMS_Connection->Connect("127.0.0.1","40404");
}
