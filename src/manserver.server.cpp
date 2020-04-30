/////////////////////////////////////////
#include "manserver.server.h"
/////////////////////////////////////////
CServer::CServer(bool bIsQuiet) {
    pLog         = 0;
    bQuiet       = bIsQuiet;
    bQuit        = false;
    bRestart     = false;
    pSQLite      = 0;
    pFirstPlayer = 0;
    memset(szConsoleInput, 0, 1024);
    memset(&r_data, 0, sizeof(r_data));
    randomize();
    pLog = new CLog("server.log", bQuiet);  // LOG
    if (!pLog) {
        printf("\nNo memory for log!\n");
        exit(0);
    }
    pLog->On();
    pLog->Restart();
    start_up();  // STARTUP/SHUTDOWN
}

/////////////////////////////////////////
CServer::~CServer() {
    kick_all("Server shutting down");
    C_GSC *c = pFirstPlayer;
    while (c) {
        pFirstPlayer = c;
        c            = c->pNext;
        DEL(pFirstPlayer);
    }
    shut_down();  // STARTUP/SHUTDOWN
    NET_Shutdown();
    DEL(pLog);
}
/////////////////////////////////////////
bool CServer::check_restart(void) {
    if (bRestart) {
        LogEntry("[Initiating server restart]=====================================");
        shut_down();
        start_up();
        LogEntry("==============================================[Server restarted]");
        bRestart = 0;
        return true;
    }
    return false;
}
/////////////////////////////////////////
void CServer::start_up(void) {
    ////////////////////////////////////////////////////////////////////////////
    // Setup server variables

    dfGUI_CHAT();
    start_time = dlcs_get_tickcount();
    dlcsm_make_str(t);

    /*  LogEntry("            ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»");
        LogEntry("            º.  °°°°°° °°   °° °°°°°. °°°°°° °°°°°    º");
        LogEntry("            º   °°     °°° °°° °°  °° °° .   °° .°°   º");
        LogEntry("            º   ±±±±   ±±°°°±± °°°±±  ±°°°±  °°  ±°.  º");
        LogEntry("            º   ²±    .±± ± ±± ±±  ²± ±±     ±±²²±    º");
        LogEntry("            º  .²²     ²².  ²² ±²  ²² ²²     ²²  ²² . º");
        LogEntry("            º   ²ÛÛ²²Û ÛÛ   ²Û ²ÛÛ²Û  Û²²Û²² Û²  Û²   º");
        LogEntry("            ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼");
        LogEntry("EGS Version (%s) Patch (%s) Net Revision (%s) %s",VERSION,EGS_REVISION,NET_REVISION,COPYRIGHT);
        LogEntry("°±²ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛßßßßÛßßßßßßßßÛÛÛÛßßßßÛÛÛÛÛÛÛÛÛÛÛßßßßÛÛÛÛÛÛÛÛÛ²±°");
        LogEntry("                  ÜÛÛÜ ÜÛÛÛÛÛß     ÜÛÛ             ÛÛ ");
        LogEntry("                ÜÛß      ÞÛ  ÜÛÛ  Ûß ßÛ  ÜÛÛÛÛÛß  ÞÛ  ");
        LogEntry("                ßÛÜÜÜ    ÛÝ Ûß Û ÛÝ ÜÛ     ÞÛ     Û   ");
        LogEntry("                  ßßÛÛ  ÞÛ ÞÛ ÜÛ ÛÛßÛÛ     ÛÝ    ÞÝ   ");
        LogEntry("                ÜÜ  ÞÛ  ÛÝ ÛÛßÛÛ ÛÛ  ÞÛ   ÞÛ          ");
        LogEntry("                 ßßßß      ÛÝ ÛÝ Û    Û   ÛÝ    Û     ");
        LogEntry("°±²ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÜÜÜÜÜÜÜÜÜÛÛÜÜÜÛÜÜÜÛÛÛÜÜÜÛÛÛÛÛÛÛÛÛÛÛÛÛ²±°"); */

    LogEntry("Mantra Server %s(%s - %s) Net(%s) Build(%d) %s\n", MANTRA_VERSION, DLCS_CPU_STRING, DLCS_OS_STRING, MANTRA_NET_REVISION, MANTRA_S_BUILD, MANTRA_COPYRIGHT);
    LogEntry("BUILD: %s %s", __DATE__, __TIME__);

    dlcs_suspend_power_management();
    SetupConsoleHistory();

    /*
        pSQLite = new C_SQLite();  // Open User Database

        if (pSQLite) {
            LogEntry("SQLite started");
            pSQLite->pLog = pLog;
            pSQLite->OpenDB("data_new.sqlite");
            LogEntry("SQLite opened DB data_new.sqlite");
        }
    */

    // pSQLite->db_query("update users set chat_channels = 'SYSTEM'",0);
    // pSQLite->db_query("alter table users add column NAME char(256) default 'SYSTEM'");

    if (!pSQLite->db_query("select * from users")) {
        LogEntry("=====================[WARNING! ERROR!]======================");
        LogEntry("Database [data.sqlite] empty or corrupt");
        LogEntry("=====================[WARNING! ERROR!]======================");
        bQuit = true;

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

    } else {
        // LogEntry("seth.password=[%s]",       (char *)  db_getvalue("username","seth","password").c_str());
        // LogEntry("seth.access=[%d]",     atoi((char *) db_getvalue("username","seth","access").c_str()));
    }

    load_data();

    LogEntry("name  = [%s]", r_data.s_name);
    LogEntry("admin = [%s]", r_data.s_admin_email);
    LogEntry("web   = [%s]", r_data.s_website_link);
    LogEntry("MOTD  = [%s]", r_data.s_motd);

    load_world();

    LogEntry("(Type '/help' for help on commands)");
    // LogEntry("АБВллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллВБА");

    C_GSC *c_x = 0;
    c_x        = new C_GSC(pLog, this);
    strcpy(c_x->username, "seth");

    c_x->load();

    CMPCharacter *pc = new CMPCharacter(c_x);

    pc->load(pSQLite);

    pc->gender                = 1;
    pc->health->current_value = 2;
    DEL(c_x);
    DEL(pc);

    NET_Init();
    initSocket();
    if (Listen(r_data.i_port, true) == -1) {
        LogEntry("ERROR LISTENING ON PORT %d\n", r_data.i_port);
    } else {
        LogEntry("Listening on port [%d]", iGetLocalPort());
    }
}
/////////////////////////////////////////
void CServer::shut_down(void) {
    save_data();  ////////////// MAP STUFF
    save_world();
    RemoveConsoleHistory();
    DEL(pSQLite);

    ///////////////////////////////////////////
    /*
        LogEntry("°±²ÛÛÛÛÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛÛÛÛÛ²±°");
        LogEntry("        ÜÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛÜ ");
        LogEntry("        Û        ÜÛÛÛÜ                      ÜÛÛÛÜ        Û ");
        LogEntry("        Û        ÛúÛÜÛ    ßÛÜß ß Û   Û      ÛÜÛúÛ        Û ");
        LogEntry("        Û        ßÛÛÛß     Û Û Û Û   Û      ßÛÛÛß        Û ");
        LogEntry("        Û         ÝÝÞ      ß Û ß ßßß ßßß     ÝÞÞ         Û ");
        LogEntry("        Û         þÜþ     Server Killed!     þÜþ         Û ");
        LogEntry("        ßÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛß ");
        LogEntry("°±²ÛÛÛÛÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛÛÛÛÛ²±°");
        LogEntry("    °±²ÛÛÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛÛÛ²±°");
        LogEntry("         ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß");
        */
    LogEntry("Server killed");
}
/////////////////////////////////////////
void CServer::do_keyboard() {
    u_char ch = 0;
    int    i;

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

                AddToConsoleHistory(szConsoleInput);
                ConsoleHistoryPosition = 0;

                LogEntry(">%s", szConsoleInput);

                if (szConsoleInput[0] == 0)
                    break;
                else {
                    console_command(szConsoleInput);
                    memset(szConsoleInput, 0, 1024);
                }
                break;

            case 27: break;

            case 8:  // backspace
                i                     = strlen(szConsoleInput);
                szConsoleInput[i - 1] = 0;
                printf(">                                                  \r>%s \r", szConsoleInput);

                break;

            case 21:  // ctrl-U
                printf(">                                                  \r>");
                memset(szConsoleInput, 0, 1024);
                break;

            case 0xE0:  // Arrow keys...
                ch = _getch();
                switch (ch) {
                    case 72:  // UP
                        strcpy(szConsoleInput, GetConsoleHistory(ConsoleHistoryPosition));
                        ConsoleHistoryPosition++;
                        if (ConsoleHistoryPosition > MAX_CONSOLE_INPUT_BUFFER) ConsoleHistoryPosition = 0;
                        printf(">                                                  \r>%s \r", szConsoleInput);
                        break;

                    case 80:  // DOWN
                        strcpy(szConsoleInput, GetConsoleHistory(ConsoleHistoryPosition));
                        ConsoleHistoryPosition--;
                        if (ConsoleHistoryPosition < 0) ConsoleHistoryPosition = MAX_CONSOLE_INPUT_BUFFER;
                        printf(">                                                  \r>%s \r", szConsoleInput);
                        break;

                    case 75:  // LEFT (TODO: build cursor into console command input)
                        break;

                    case 77:  // RIGHT (TODO: build cursor into console command input)
                        break;

                    default: break;
                }
                break;

            default:
                i                     = strlen(szConsoleInput);
                szConsoleInput[i]     = ch;
                szConsoleInput[i + 1] = 0;
                printf(">                                                  \r>%s \r", szConsoleInput);
                break;
        }
    }
}
/////////////////////////////////////////
void CServer::cycle(void) {
    static C_GSC *pClient = 0;

    if (pClient == 0) pClient = pFirstPlayer;
    if (pClient) {
        pClient->do_net();
        pClient = pClient->pNext;
    }

    check_restart();
    do_keyboard();
    accept_connection();
    do_world();
    purge_clients();
}
/////////////////////////////////////////
void CServer::accept_connection(void) {
    C_GSC *client;

    int  iLength;
    int  iFlags, iHelp;
    int  iAcceptSocket;
    char cType;

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
        case CTL_CONNECT:

            if (strcmp(Recv.pRead(), "MANTRA")) {
                LogEntry("Fail attempt, not MANTRA packet");
                reject_connection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "This is a Ember Game Server");
                return;
            }

            if (Recv.iRead() != atoi(MANTRA_NET_REVISION)) {
                reject_connection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "Server uses newer protocol! Update available.");
                return;
            }

            // TODO: Add reject (PRIVATE SERVER)

            // create new client object
            client = new C_GSC(pLog, this);
            if (!client) {
                LogEntry("manserver.server.cpp -> Can't create client");
                return;
            }

            if (!client) {
                reject_connection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "Server can't assign a new socket!");
                return;
            }

            memcpy(&client->ToAddr, &ServerAddr, sizeof(ServerAddr));

            client->iSocket = zOpenSocket(0);
            if (client->iSocket == -1) {
                DEL(client);
                reject_connection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "Server can't assign new socket!");
                return;
            }

            Send.Reset();
            Send.Write(0);
            Send.Write(0);
            Send.Write((char)CTL_ACCEPT);
            Send.Write(client->iGetLocalPort());
            FinishCtlPacket(&Send);

            if (nSend(iAcceptSocket, Send.pGetPacketBuffer(), Send.iGetCurSize(), (struct sockaddr *)&ServerAddr) == -1) {
                DEL(client);
                LogEntry("s_server.cpp -> CSocket::pAccept() -> error making a new connection");
                return;
            }

            client->bCanSend   = true;
            client->bConnected = true;

            break;

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

        case CTL_REJECT: LogEntry("CTL_REJECT ?"); return;

        default:
            reject_connection(iAcceptSocket, (struct sockaddr *)&ServerAddr, "This is a Mantra gameserver");
            LogEntry("UNKNOWN CTL_PACKET (%d) received", cType);
            return;
    }

    // insert client into client list
    if (pFirstPlayer) client->pNext = pFirstPlayer;
    pFirstPlayer = client;
}
/////////////////////////////////////////
void CServer::reject_connection(int iReSocket, sockaddr *pReAddr, const char *reason) {
    CPacket Send(NET_DATAGRAMSIZE);
    if (!reason) return;
    Send.Reset();
    Send.Write(0);
    Send.Write(0);
    Send.Write((char)CTL_REJECT);
    Send.Write((char *)reason);
    FinishCtlPacket(&Send);
    if (nSend(iReSocket, Send.pGetPacketBuffer(), Send.iGetCurSize(), pReAddr) == -1) {
        LogEntry("Reject message failed (%s)", reason);
    }
}
/////////////////////////////////////////
void CServer::send_all(CPacket *pPacket, float fBlockTime) {
    C_GSC *pClient;
    pClient = pFirstPlayer;
    while (pClient) {
        if (!pClient->bDelete) pClient->SendUnreliableMessage(pPacket);
        pClient = pClient->pNext;
    }
}
/////////////////////////////////////////
void CServer::disconnect(C_GSC *client, const char *reason) {
    CPacket Send(NET_DATAGRAMSIZE);

    if (!client) return;
    if (client) {
        Send.Reset();
        Send.Write((char)NM_CLIENT_SHUTDOWN);
        Send.Write((char *)client->session_id);
        Send.Write((char *)reason);
        client->SendUnreliableMessage(&Send);
    }

    Send.Reset();
    Send.Write((char)NM_OTHER_CLIENT_SHUTDOWN);
    Send.Write((char *)client->session_id);
    send_all(&Send, 0);

    client->bDelete = true;
}
/////////////////////////////////////////
void CServer::save_world(void) {
    static long dwSaveTimer = dlcs_get_tickcount();
    if ((dlcs_get_tickcount() - dwSaveTimer) > 100000) {
        dwSaveTimer = dlcs_get_tickcount();
        LogEntry("World saved...");
    }
}
/////////////////////////////////////////
void CServer::load_world(void) { LogEntry("World loaded..."); }
/////////////////////////////////////////
C_GSC *CServer::get_client(const char *user_name) {
    C_GSC *find = pFirstPlayer;
    while (find) {
        if (dlcs_strcasecmp(find->username, user_name)) return find;
        find = find->pNext;
    }
    return 0;
}
/////////////////////////////////////////
void CServer::kick_all(const char *reason) {
    C_GSC *pClient;
    pClient = pFirstPlayer;
    if (!pClient) return;
    LogEntry("Kicking online players");
    while (pClient) {
        LogEntry("       ....%s", pClient->username);
        disconnect(pClient, reason);
        pClient = pClient->pNext;
    }
    LogEntry("All players kicked!");
}
/////////////////////////////////////////
void CServer::kick_user(const char *name, const char *reason) {
    C_GSC *client = pFirstPlayer;
    while (client) {
        if (dlcs_strcasecmp(client->username, name)) {
            disconnect(client, reason);
            return;
        }
        client = client->pNext;
    }
    LogEntry("%s is not online!", name);
    return;
}
/////////////////////////////////////////
void CServer::add_user(const char *in, u_char ia) { pSQLite->db_query(va("insert into users (username,access) values ('%s',%d)", in, ia)); }
/////////////////////////////////////////
void CServer::remove_user(const char *szName) { pSQLite->db_query("delete * from users where usersname = '%s'", szName); }
/////////////////////////////////////////
void CServer::user_access(const char *in_user_name, int in_access) {
    if (in_access > 255) return;
    if (in_access < 0) return;
    pSQLite->db_query("update users set access=%d where username='%s'", in_access, in_user_name);
    C_GSC *c = get_client(in_user_name);
    if (c) c->access = in_access;
}
/////////////////////////////////////////
void CServer::chat(C_GSC *pClient, const char *from, const char *msg, int channel) {
    CPacket Send(NET_DATAGRAMSIZE);

    Send.Reset();
    Send.Write((char)NM_MESSAGE);

    Send.Write((char *)msg);
    Send.Write((char *)from);
    Send.Write((int)channel);  // CHANNEL
    if (pClient) Send.Write((char *)pClient->session_id);

    switch (channel) {
        case CHANNEL_CLAN: break;

        case CHANNEL_GENERAL: break;

        case CHANNEL_TRADE: break;

        case CHANNEL_SAY:
        case CHANNEL_YELL:
        case CHANNEL_LOCAL: break;

        case CHANNEL_PARTY: break;

        case CHANNEL_RAID: break;

        case CHANNEL_SYSTEM: send_all((CPacket *)&Send, 0); break;

        case CHANNEL_WHISPER: pClient->SendUnreliableMessage((CPacket *)&Send); break;

        case CHANNEL_CUSTOM: break;

        default: break;
    }
}
/////////////////////////////////////////
void CServer::purge_clients(void) {
    C_GSC *pClient = 0;
    C_GSC *pDelete = 0;

    pClient = pFirstPlayer;
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
/////////////////////////////////////////
void CServer::do_world(void) { save_world(); }
/////////////////////////////////////////
void CServer::console_command(char *command) {
    vector<string> v = dlcs_explode(" ", command);
    char           temp[1024];
    memset(temp, 0, 1024);
    char targs[1024];
    memset(targs, 0, 1024);

    if (v.size() > 0) {
        strcpy(targs, " ");
        if (v.size() > 1) {
            for (unsigned int i = 1; i < v.size(); i++) {
                strcat(targs, v[i].c_str());
                strcat(targs, " ");
            }
        }

        if (dlcs_strcasecmp(v[0].c_str(), "/help")) {
            LogEntry("[List of available commands]====================================");

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

            /*LogEntry("/ban <user>......................Ban player (will ban user)");
              LogEntry("/banip <user>....................Ban player (will ban user's ip address)");
              LogEntry("/unban <user>....................Unban player (will unban user)");
              LogEntry("/unbanip <ipaddress>.............Unban player (will unban user's ip address)");
              LogEntry("/bandomain <domain>..............Ban entire domain");
              LogEntry("/unbandomain <domain>............Unban entire domain");
              LogEntry("/banlist.........................List all banned ip's");*/

            LogEntry("======================================================[End List]");
            return;
        }

        ///////////////////////////////////////////////////////////////////////////

        if ((dlcs_strcasecmp(v[0].c_str(), "/sql"))) {
            pSQLite->db_queryl(targs);
        }

        //////////////////////////////////////////

        if ((dlcs_strcasecmp(v[0].c_str(), "/users"))) {
            pSQLite->db_queryl("select username,access from users");
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

        if (dlcs_strcasecmp(v[0].c_str(), "/remove_user")) {
            if (v.size() > 1) remove_user((char *)v[1].c_str());
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/access")) {
            if (v.size() > 2) {
                int dx = atoi(v[2].c_str());  // dx is access
                if ((dx < 0) || (dx > 255)) dx = 0;
                user_access((char *)v[1].c_str(), dx);
            }
        }

        //////////////////////////////////////////

        if ((dlcs_strcasecmp(v[0].c_str(), "/time"))) {
            strcpy(temp, dlcs_get_time());
            LogEntry(temp);
            dlcs_timestamp(temp);
            LogEntry("timestamp: %s", temp);
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/ann")) {
            if (v.size() > 1) {
                chat(0, "Admin", targs, CHANNEL_SYSTEM);
                LogEntry(targs);
            }
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/msg")) {
            if (v.size() > 2) {
                strcpy(targs, " ");
                if (v.size() > 2) {
                    for (unsigned int i = 2; i < v.size(); i++) {
                        strcat(targs, v[i].c_str());
                        strcat(targs, " ");
                    }
                }
                chat(get_client((char *)v[1].c_str()), "Admin", targs, CHANNEL_WHISPER);
            }
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/kick_user")) {
            if (v.size() > 2) {
                kick_user((char *)v[1].c_str(), (char *)v[2].c_str());
            } else if (v.size() > 1) {
                kick_user((char *)v[1].c_str(), "Kicked by admin");
            }
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/kill")) bQuit = true;

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/restart")) bRestart = 1;

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/nuy")) {
            r_data.b_new_accounts = true;
            LogEntry("Now accepting new users");
        }

        //////////////////////////////////////////

        if (dlcs_strcasecmp(v[0].c_str(), "/nun")) {
            r_data.b_new_accounts = false;
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
/////////////////////////////////////////
char *CServer::GetConsoleHistory(int x) {
    if (ConsoleHistory) return (ConsoleHistory[x].text);
    return (char *)(va("con history error![%d]", x));
}
/////////////////////////////////////////
void CServer::SetupConsoleHistory(void) {
    ConsoleHistory = 0;
    ConsoleHistory = new ConHistory[MAX_CONSOLE_INPUT_BUFFER + 1];
    for (int i = 0; i < MAX_CONSOLE_INPUT_BUFFER; i++) memset(ConsoleHistory[i].text, 0, 1024);
    ConsoleHistoryPosition = 0;
}
/////////////////////////////////////////
void CServer::AddToConsoleHistory(char *s) {
    for (int i = MAX_CONSOLE_INPUT_BUFFER; i > 0; i--) strcpy(ConsoleHistory[i].text, ConsoleHistory[i - 1].text);
    strcpy(ConsoleHistory[0].text, s);
}
/////////////////////////////////////////
void CServer::RemoveConsoleHistory(void) {
    delete[] ConsoleHistory;
    ConsoleHistory = 0;
}
/////////////////////////////////////////
bool CServer::load_data(void) {
    FILE *         fp;
    char           In[256];
    vector<string> v;
    fp = fopen("server.ini", "rt");
    if (!fp) return false;
    while (1) {
        if (!fgets(In, 255, fp)) break;
        if (In[strlen(In) - 1] == '\n') In[strlen(In) - 1] = 0;

        v = dlcs_explode("=", In);

        if (v.size() == 2) {
            if (dlcs_strcasecmp(v[0].c_str(), "s_name")) strcpy(r_data.s_name, v[1].c_str());
            if (dlcs_strcasecmp(v[0].c_str(), "s_admin_email")) strcpy(r_data.s_admin_email, v[1].c_str());
            if (dlcs_strcasecmp(v[0].c_str(), "s_motd")) strcpy(r_data.s_motd, v[1].c_str());
            if (dlcs_strcasecmp(v[0].c_str(), "s_website_link")) strcpy(r_data.s_website_link, v[1].c_str());
            if (dlcs_strcasecmp(v[0].c_str(), "b_new_accounts")) r_data.b_new_accounts = atoi(v[1].c_str());
            if (dlcs_strcasecmp(v[0].c_str(), "i_port")) r_data.i_port = atoi(v[1].c_str());
            if (dlcs_strcasecmp(v[0].c_str(), "i_max_clients")) r_data.i_max_clients = atoi(v[1].c_str());
            if (dlcs_strcasecmp(v[0].c_str(), "b_require_website")) r_data.b_require_website = atoi(v[1].c_str());
        }
    }

    fclose(fp);

    if (r_data.i_max_clients == 0) r_data.i_max_clients = 2000;

    LogEntry("server.ini loaded");

    return true;
}
/////////////////////////////////////////
bool CServer::save_data(void) {
    FILE *fp;
    fp = fopen("server.ini", "wt");
    if (!fp) return false;
    fputs("** Mantra Server Configuration\n", fp);
    fputs(va("s_name=%s\n", r_data.s_name), fp);
    fputs(va("s_admin_email=%s\n", r_data.s_admin_email), fp);
    fputs(va("s_motd=%s\n", r_data.s_motd), fp);
    fputs(va("s_website_link=%s\n", r_data.s_website_link), fp);
    fputs(va("b_new_accounts=%d\n", r_data.b_new_accounts), fp);
    fputs(va("i_port=%d\n", r_data.i_port), fp);
    fputs(va("i_max_clients=%d\n", r_data.i_max_clients), fp);
    fputs(va("b_require_website=%d\n", r_data.b_require_website), fp);
    fclose(fp);

    LogEntry("server.ini saved");

    return true;
}
/////////////////////////////////////////
int CServer::num_clients() {
    int    x = 0;
    C_GSC *c = pFirstPlayer;
    while (c) {
        x++;
        c = c->pNext;
    }
    return x;
}

/////////////////////////////////////////
void CServer::create_guid(const char *cin, char *cout) {
    char t[1024];
    memset(t, 0, 1024);
    char t2[1024];
    memset(t2, 0, 1024);
    strcpy(t, va("%s%d", cin, rand() % 99999));
    strcpy(t2, dlcs_md5_digest(t).c_str());
    dlcs_timestamp(t);
    strcpy(cout, va("%s-%s-%05d", t2, t, rand() % 99999));
}
/////////////////////////////////////////
/*
void CServer::ReportToMaster(void){
    static bool starter=false; static long dwReportTimer=GetTickCount();
     //if(!dlcs_strcasecmp(gmvar("sys.master_report"),"yes")) return;
         if(!pFMMS_Connection) return;
    if((GetTickCount()-dwReportTimer)<15000) {
        if(!starter) starter=true; else return;
    }
    dwReportTimer=GetTickCount(); //LogEntry("Reported to master");
    pFMMS_Connection->Connect("127.0.0.1","40404");
}
*/
