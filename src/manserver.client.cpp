///////////////////////////////////////////
// MANTRA
///////////////////////////////////////////
#include "manserver.client.h"
///////////////////////////////////////////
// C_GSC::C_GSC(void){   init();    pLog=new CLog("C_GSC.log");    bMadeLog=true; }
///////////////////////////////////////////
C_GSC::C_GSC(CLog *pInLog, CServer *pInServer) {
    initc();
    pLog     = pInLog;
    bMadeLog = false;
    pServer  = pInServer;
}
///////////////////////////////////////////
void C_GSC::initc(void) {
    character = 0;
    memset(session_id, 0, 64);
    pServer          = 0;
    bDelete          = 0;
    inactivity_timer = new CTimer(300000);
    heartbeat_timer  = new CTimer(5000);
    bHeartBeatCheck  = 0;

    character = new CMPCharacter();
    // character->pOwner=this;

    pNext     = NULL;
    pPrevious = NULL;
}
///////////////////////////////////////////
C_GSC::~C_GSC() {
    save();
    DEL(inactivity_timer);
    DEL(heartbeat_timer);
    DEL(character);
    if (bMadeLog) DEL(pLog);
}
///////////////////////////////////////////
void C_GSC::load(void) {
    LogEntry("Loading %s", username);
    vector<string> tv;
    pServer->pSQLite->db_query(va("select * from users where username='%s'", username), 0);

    access           = atoi(pServer->pSQLite->db_getvalue("username", username, "access").c_str());
    timeout_override = atoi(pServer->pSQLite->db_getvalue("username", username, "timeout_override").c_str());

    tv = dlcs_explode(",", pServer->pSQLite->db_getvalue("username", username, "chat_channels").c_str());
    for (unsigned int i = 0; i < tv.size(); i++) {
        join_channel(tv[i].c_str());
        LogEntry("Joined chat channel: %s", tv[i].c_str());
    }
}
void C_GSC::save(void) {
    LogEntry("Saving %s", username);

    string s_temp;

    db_update("access", va("%d", access));
    db_update("timeout_override", va("%d", timeout_override));

    s_temp.clear();
    vector<string>::iterator vi;
    for (vi = chat_channel.begin(); vi != chat_channel.end(); ++vi) {
        if (GUI_CHAT[(*vi).c_str()]) {
            s_temp = s_temp + (*vi).c_str();
            s_temp = s_temp + ",";
            LogEntry("%s->%d", (*vi).c_str(), GUI_CHAT[(*vi).c_str()]);
        }
    }
    if (!s_temp.empty()) {
        s_temp.erase(s_temp.size() - 1);
        LogEntry("Saving chat setup %s", s_temp.c_str());
        db_update("chat_channels", s_temp.c_str());
    }
}
///////////////////////////////////////////
void C_GSC::do_script(char *file) {}
///////////////////////////////////////////
void C_GSC::do_func(char *func) {}
///////////////////////////////////////////
bool C_GSC::gui_call(char *gui) {
    CPacket Send(8192);
    Send.Reset();
    Send.Write((char)NM_GUI);
    Send.Write((char)FM_GUI_CALL);
    Send.Write((char *)gui);
    SendUnreliableMessage(&Send);
    return true;
}  ///////////////////////////////////////////
bool C_GSC::gui_clear() {
    CPacket Send(8192);
    Send.Reset();
    Send.Write((char)NM_GUI);
    Send.Write((char)FM_GUI_CLEAR_ALL);
    SendUnreliableMessage(&Send);
    return true;
}
///////////////////////////////////////////
void C_GSC::do_net(void) {
    CPacket             Send(8192);
    CPacket *           Recv = NULL;
    map<string, string> pairdata;
    pairdata.clear();
    vector<string> tv;
    char           szTemp[1024];
    memset(szTemp, 0, 1024);
    char szTemp2[1024];
    memset(szTemp2, 0, 1024);
    char szTemp3[1024];
    memset(szTemp3, 0, 1024);
    char szTemp4[2048];
    memset(szTemp4, 0, 2048);
    int iMsg, j, k, x, y, z, ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz, ex, ey, ez, fx, fy, fz;
    iMsg = j = k = x = y = z = ax = ay = az = bx = by = bz = cx = cy = cz = dx = dy = dz = ex = ey = ez = fx = fy = fz = 0;
    double gx, gy, gz;
    gx = gy = gz = 0;
    // unsigned char vx, vy, vz;
    // vx = vy = vz = 0;
    char   cMsgType;
    C_GSC *other_client;

    iMsg = iGetMessage();
    if (iMsg == -1) return;

    if (iMsg > 0) {
        Recv = pGetMessage();
        if (!Recv) return;
        cMsgType = Recv->cRead();
        Recv     = pGetMessage();
        if (ToAddr.sin_addr.s_addr != FromAddr.sin_addr.s_addr) {
            LogEntry("forged net message");
            return;
        }

        cMsgType = Recv->cRead();

        switch (cMsgType) {
            case NM_FUNCTION:
                ax = Recv->iRead();
                switch (ax) {
                    case NF_CHAT_JOIN:
                        strcpy(szTemp, Recv->pRead());
                        join_channel(szTemp);
                        break;

                    case NF_CHAT_LEAVE:
                        strcpy(szTemp, Recv->pRead());
                        leave_channel(szTemp);
                        break;

                    case NF_CHAT_WHO: break;

                    default: break;
                }

                break;

            case NM_LOGIN_REQUEST:

                // set ax to default login result, copy username, password
                ax = BAD_LOGIN;
                strcpy(szTemp, "null");
                strcpy(username, Recv->pRead());
                strcpy(szTemp4, Recv->pRead());

                // check for NULL result
                if ((username == NULL) || (!strlen(username))) {
                    strcpy(szTemp, "Bad network message");
                    bDelete = true;
                    break;
                }

                load();

                if (pServer->pSQLite->dbr_nrow == 0) ax = NEW_ACCOUNT;

                // check for maximum players logged in
                if (pServer->num_clients() > pServer->r_data.i_max_clients) ax = TOO_MANY_PLAYERS;

                // check to see if player is already logged in
                // SYSTEM is reserved
                // add any other reserved usernames here
                if (dlcs_strcasecmp(username, "SYSTEM")) ax = ALREADY_LOGGED_IN;

                other_client = pServer->pFirstPlayer;
                while (other_client) {
                    if (other_client != this)
                        if (dlcs_strcasecmp(other_client->username, username)) ax = ALREADY_LOGGED_IN;
                    other_client = other_client->pNext;
                }

                // compare passwords
                if (pServer->pSQLite->dbr_nrow == 0) {
                    ax = BAD_LOGIN;
                } else {
                    if (ax == BAD_LOGIN) {
                        if ((dlcs_strcasecmp(szTemp4, pServer->pSQLite->db_getvalue("username", username, "password").c_str()))) ax = GOOD_LOGIN;
                    }
                }

                switch (ax) {
                    case BAD_LOGIN: strcpy(szTemp, "Incorrect password..."); break;
                    case ALREADY_LOGGED_IN: strcpy(szTemp, "This user is already logged in from another machine..."); break;
                    case ACCOUNT_EXPIRED: strcpy(szTemp, "Your account has expired. Contact system administrator."); break;
                    case TOO_MANY_PLAYERS: strcpy(szTemp, "Server is full. Too many players already logged in..."); break;
                    case GOOD_LOGIN:
                        if (!strlen(username)) {
                            ax = BAD_LOGIN;
                            strcpy(szTemp, "Bad network try again.");
                            break;
                        }
                        break;
                    case NEW_ACCOUNT:

                        if (pServer->r_data.b_new_accounts) {
                            if (!pServer->r_data.b_require_website) {
                                ax = GOOD_LOGIN;
                                strcpy(szTemp, "Welcome to the server! New account created.");
                                strcpy(password, szTemp4);
                                access = 0;
                                pServer->add_user(username, 0);
                                db_update("password", password);
                                // pServer->pServer->pSQLite->db_query((char *)va("INSERT INTO users VALUES ('%s','%s', 0)",username,szTemp4));
                            } else {
                                ax = BAD_LOGIN;
                                strcpy(szTemp, va("Please register @ %s", pServer->r_data.s_website_link));
                            }

                            break;
                        } else {
                            ax = BAD_LOGIN;
                            strcpy(szTemp, "Sorry, we're not accepting new accounts at this time. Try again later.");
                            break;
                        }
                        break;
                    default: break;
                }

                if (ax == GOOD_LOGIN) {
                    strcpy(szTemp, "Welcome to the server.");
                }

                pServer->create_guid(username, session_id);

                db_update("access", "32");
                pServer->pSQLite->db_queryl("select * from users where username=%s", username);

                LogEntry("%s.access [%d]", username, atoi((char *)pServer->pSQLite->db_getvalue("username", username, "access").c_str()));

                Send.Reset();
                Send.Write((char)NM_LOGIN_REQUEST_REPLY);
                Send.Write((char)ax);
                Send.Write((char *)szTemp);
                Send.Write((char *)MANTRA_VERSION);
                Send.Write((char *)session_id);
                Send.Write((char *)"sys.media");
                Send.Write((char *)pServer->r_data.s_name);
                Send.Write((char *)pServer->r_data.s_admin_email);
                Send.Write((int)atoi((char *)pServer->pSQLite->db_getvalue("username", username, "access").c_str()));
                Send.Write((char *)"access what");
                Send.Write((char)10);  // character slots
                SendUnreliableMessage(&Send);

                if (ax != GOOD_LOGIN) {
                    bDelete = true;
                    return;
                }

                LogEntry("%s logged in from %s:%d", username, inet_ntoa(ToAddr.sin_addr), ntohs(ToAddr.sin_port));

                tv.clear();
                tv = dlcs_explode(",", pServer->pSQLite->db_getvalue("username", username, "chat_channels").c_str());
                for (unsigned int i = 0; i < tv.size(); i++) {
                    join_channel(tv[i].c_str());
                }

                break;

            case NM_LOGOUT:
                bDelete = true;
                LogEntry("%s logged out", username);
                break;

            case NM_GUI_CALLBACK:
                strcpy(szTemp3, Recv->pRead());
                ax = Recv->cRead();  // number of data to get
                for (j = 0; j < ax; j++) {
                    strcpy(szTemp, Recv->pRead());
                    strcpy(szTemp2, Recv->pRead());
                    pairdata[szTemp] = szTemp2;
                    // for now, only pull out console function
                    if (dlcs_strcasecmp(szTemp, "console")) {
                        LogEntry("%s GUI_CALLBACK (%s)", username, szTemp2);
                    }
                }
                break;

            case NM_GET_SERVER_INFO:
                switch (Recv->cRead()) {
                    case NETMSG_SERVERINFORMATION: break;
                    default: break;
                }
                break;

            case NM_MESSAGE:
                // Global Chat / System Message
                inactivity_timer->Reset();

                strcpy(szTemp, Recv->pRead());
                ax = Recv->iRead();

                switch (ax) {  // channel

                    case CHANNEL_LOCAL:
                    case CHANNEL_TRADE:
                    case CHANNEL_GENERAL:
                    case CHANNEL_WHISPER:
                    case CHANNEL_PARTY:
                    case CHANNEL_RAID:
                    case CHANNEL_SAY:
                    case CHANNEL_YELL:
                    case CHANNEL_CLAN:
                    case CHANNEL_SYSTEM: pServer->chat(this, this->username, (char *)va("%s:%s", username, szTemp), ax); break;

                    case CHANNEL_CUSTOM: break;

                    case CHANNEL_CONSOLE:

                        inactivity_timer->Reset();
                        strcpy(szTemp, Recv->pRead());
                        memset(szTemp2, 0, 1024);
                        j = 0;
                        for (unsigned int i = 0; i < strlen(szTemp); i++) {
                            if (szTemp[i] == '\'') {
                                szTemp2[j] = '\\';
                                j++;
                                szTemp2[j] = '\'';
                                j++;
                            } else {
                                szTemp2[j] = szTemp[i];
                                j++;
                            }
                        }
                        for (unsigned int i = 0; i < strlen(szTemp2); i++) szTemp2[i] = szTemp2[i + 1];  // should have a / in front, remove it
                        // l_interpret(va("world.client[%d]:con_parse(\"%s\")",windex,szTemp2));
                        break;

                    default: break;
                }
                break;

            case NM_MOVEPLAYER: break;

            case NM_CREATE_CHARACTER:

                /*
                inactivity_timer->Reset();
                client->avatar->hail(Recv->cRead());
                strcpy(client->avatar->name,Recv->pRead());
                // call hook to do stuff with client here...
                client->avatar->x=15015;
                client->avatar->y=15015;
                client->avatar->z=15015;
                client->avatar->save(client->avatar->slot); // Save the character slot
                Send.Reset();
                Send.Write((char)NM_CREATE_CHARACTER);
                Send.Write((char)client->avatar->slot);
                client->SendUnreliableMessage((CPacket *)&Send);
                */
                break;

            case NM_DELETE_CHARACTER:
                inactivity_timer->Reset();
                cx = Recv->cRead();
                // !DoesHaveAccess(client->cGetAccess(),"fast_char_delete"))
                // avatar->load(cx);
                // tTime=localtime(&client->CL_Data->tCreationTime);
                // ax=tTime->tm_year;
                // ay=tTime->tm_yday;
                // time(&fmgsTime);
                // tTime=localtime(&fmgsTime);
                // bx=tTime->tm_year;
                // by=tTime->tm_yday;
                // if(bx==ax)
                //-
                //    if( (by-ay) < l_sys("minimum_char_age)
                //    -
                //        sprintf(szTemp,"Character must age at least %s real days!",l_sys("minimum_char_age"));
                //        Announce(client,szTemp,50,175,255);
                //        break;
                //    -
                // -
                // client->avatar->wipe();
                // client->avatar->save(cx);
                Send.Reset();
                Send.Write((char)NM_DELETE_CHARACTER);
                Send.Write((char)cx);
                SendUnreliableMessage((CPacket *)&Send);
                break;

            case NM_RETRIEVECHARS:
                /*
                inactivity_timer->Reset();
                   client->avatar->wipe();
                   for(i=0;i<atoi(l_sys("character_slots"));i++)
                   {
                       client->avatar->load(i);
                       if(strlen(client->avatar->name))
                       {
                           Send.Reset();
                           Send.Write((char)NM_RETRIEVECHARS);
                           Send.Write((char)i);
                           Send.Write((char *)client->avatar->name);
                           client->SendUnreliableMessage((CPacket *)&Send);
                           client->avatar->wipe();
                       }
                   }
                */
                break;

            case NM_RETRIEVECHARINFO:
                /*
                inactivity_timer->Reset();
                cx=Recv->cRead();
                if(client->in_limbo==false)
                {
                    client->avatar->load(cx);
                    client->in_limbo=true;
                    Send.Reset();
                    Send.Write((char)NM_RETRIEVECHARINFO);
                    Send.Write((char *)client->avatar->name);
                    Send.Write((int)client->sid);
                    Send.Write((char)client->avatar->direction);
                    Send.Write((int)15015);
                    Send.Write((int)15015);
                    Send.Write((int)15015);
                    client->SendUnreliableMessage((CPacket *)&Send);
                    MovePlayer(client,client->avatar->x,client->avatar->y,client->avatar->z,1);
                }
                */
                break;

            case NM_MODIFY_CHARACTER:
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
                  Send.Write((char)NM_MODIFY_CHARACTER);
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
                          other_client->SendUnreliableMessage((CPacket *)&Send);
                      }
                      other_client=other_client->pNext;
                  }
                  */
                break;

            case NM_CLIENT_SHUTDOWN:

                other_client = pServer->pFirstPlayer;
                while (other_client) {
                    if (other_client != this) {
                        Send.Reset();
                        Send.Write((char)NM_OTHER_CLIENT_SHUTDOWN);
                        Send.Write((char *)session_id);
                        other_client->SendUnreliableMessage((CPacket *)&Send);
                    }
                    other_client = other_client->pNext;
                }
                bDelete = true;
                return;

                break;

            case NM_HEARTBEAT:
                // LogEntry("Heartbeat check from %s",username);
                bHeartBeatCheck = false;  // OK, clear the heart beat check
                break;

            case NM_PING:
                inactivity_timer->Reset();
                gx = Recv->dwRead();
                Send.Reset();
                Send.Write((char)NM_PING);
                Send.Write((long)gx);
                SendUnreliableMessage((CPacket *)&Send);

                break;

            case NM_FILE_XFER:
                inactivity_timer->Reset();
                // char *pFileBuf;
                // FILE *pFile;
                // bool bFileFail;
                switch (Recv->cRead()) {
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
                                    SendData.Write((char)NM_FILE_XFER);
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
                                SendData.Write((char)NM_FILE_XFER);
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
                                    SendData.Write((char)NM_FILE_XFER);
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
                                SendData.Write((char)NM_FILE_XFER);
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
                                SendData.Write((char)NM_FILE_XFER);
                                SendData.Write((char)NET_FILE_EOF_RESEND);
                                SendData.Write((char *)szTemp1);
                            }
                            break;

                        case NET_FILE_RESUME:
                            break;
                            */

                        //						case NET_FILE_START_OK:
                        //							break;

                    default: break;
                }

                break;

            case NM_NOP:
                if (strcmp(Recv->pRead(), "1234567890") == 0) {
                    Send.Reset();
                    Send.Write((char)NM_NOP);
                    Send.Write("0987654321");
                    SendUnreliableMessage(&Send);
                }
                break;

            default: break;
        }
    }

    if (inactivity_timer->Up()) {  // Check for inactivity

        // LogEntry("No activity from %s",username);

        Send.Reset();
        Send.Write((char)NM_HEARTBEAT);
        Send.Write((int)1);
        SendUnreliableMessage((CPacket *)&Send);
        bHeartBeatCheck = true;
        heartbeat_timer->Reset();
        inactivity_timer->Reset();

        if (timeout_override == false) {
            // Log("%s(%s) being kicked due to inactivity.",client->avatar->name,client->avatar->name);
            inactivity_timer->Reset();  // to prevent server from doing this twice (or more)

            other_client = pServer->pFirstPlayer;
            while (other_client) {
                // change to only send to onscreen players later...
                if (other_client != this) {
                    Send.Reset();
                    Send.Write((char)NM_OTHER_CLIENT_SHUTDOWN);
                    Send.Write((char *)session_id);
                    other_client->SendUnreliableMessage((CPacket *)&Send);
                }
                other_client = other_client->pNext;
            }
            pServer->disconnect(this, "Inactivity");
        }
    }

    if (bHeartBeatCheck) {  // Check for heartbeats and remove if not found
        if (heartbeat_timer->Up()) pServer->disconnect(this, "Network disconnected");
    }
}

void C_GSC::db_update(string s_col, string s_val) { pServer->pSQLite->db_query(va("UPDATE users SET %s = '%s' where username = '%s'", (char *)s_col.c_str(), (char *)s_val.c_str(), username)); }
