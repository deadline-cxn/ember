 //strcpy(szTemp1,"choose_server");
        //if(strcmp(szTemp1,pClientData->szGeneric))
        //{
            //strcpy(pClientData->szGeneric,szTemp1);
            //pLog->_Add("CHOOSE_SERVER");
            // INPUTMODE=NORMAL;
        //}


        /*

        if(LOGINMODE==LM_FAVORITES)
        {
            if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false))
            {
                //gPrintPct(.1875,.03,"^0Server",0);
                //gPrintPct(.375,.03, "^0Address",0);
                ///gPrintPct(.65,.03,  "^0Players",0);
                // gPrintPct(.775,.03 ,"^0Ping",0);


                if((INPUTMODE!=CONSOLE1)&&((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false)))
                {
                    if(MouseIn(137,32,780,536))
                    {
                        i = pClientData->ServerListOffset+((GetMouseY()-32)/12);
                        j = pClientData->ServerListOffset+((GetMouseY()-32)/12);

                        pClientData->FavoriteServer=pClientData->FirstFavoriteServer;
                        while(pClientData->FavoriteServer)
                        {
                            if(pClientData->FavoriteServer->key==i)
                                break;
                            pClientData->FavoriteServer=pClientData->FavoriteServer->pNext;
                        }
                        if(pClientData->FavoriteServer)
                        {

                            if(strlen(pClientData->FavoriteServer->name))
                            {
                                DrawBar(153,(i-pClientData->ServerListOffset)*12+33,759,(i-pClientData->ServerListOffset)*12+33+13,LONGRGB(GetFade(1),0,200),LONGRGB(110,GetFade(1),100));
                                if(GetMouseRelease(SDL_BUTTON_LEFT))
                                {
                                    ProcessConsoleCommand(pClientData->PlayMouseLB,0);
                                    if(strlen(pClientData->FavoriteServer->name))
                                    {
                                        pClientData->SelectedServer=i;
                                        strcpy(pClientData->ServerName,pClientData->FavoriteServer->name);
                                        strcpy(pClientData->IPAddress,pClientData->FavoriteServer->ip_address);
                                        strcpy(pClientData->Port,pClientData->FavoriteServer->port);
                                    }
                                }
                            }
                        }
                    }
                }


                for(i=0+pClientData->ServerListOffset;i<pClientData->ServerListOffset+42;i++)
                {
                    if(i<MAX_SERVERS)
                    {

                        pClientData->FavoriteServer=pClientData->FirstFavoriteServer;
                        while(pClientData->FavoriteServer)
                        {
                            if(pClientData->FavoriteServer->key==i)
                                break;
                            pClientData->FavoriteServer=pClientData->FavoriteServer->pNext;
                        }

                        if(!pClientData->FavoriteServer)
                            continue;


                        if(strlen(pClientData->FavoriteServer->name))
                        {
                            if(i==pClientData->SelectedServer)
                            {

                                if(i==j)
                                   DrawBar(153,(i-pClientData->ServerListOffset)*12+33,759,(i-pClientData->ServerListOffset)*12+33+13,LONGRGB(GetFade(1),0,200),LONGRGB(110,GetFade(3),100));
                                else
                                    DrawBar(153,(i-pClientData->ServerListOffset)*12+33,759,(i-pClientData->ServerListOffset)*12+33+13,LONGRGB(GetFade(1),200,0),LONGRGB(0,GetFade(2),0));

                            }

                            //gPrint(140,(i-pClientData->ServerListOffset)*12+32,pClientData->FavoriteServer[i].user,0);
                            gPrint(150,(i-pClientData->ServerListOffset)*12+32,va("^&%s",pClientData->FavoriteServer->name),0);
                            gPrint(300,(i-pClientData->ServerListOffset)*12+32,va("^&%s:%s",pClientData->FavoriteServer->ip_address,pClientData->FavoriteServer->port),0);
                            // gPrint(440,(i-pClientData->ServerListOffset)*12+32,pClientData->FavoriteServer[i].resource_id,0);
                            // Draw OS
                            DrawOSIcon(726,(i-pClientData->ServerListOffset)*12+32,pClientData->FavoriteServer->operating_system);

                            if(pClientData->FavoriteServer->allow_new_users)
                                strcpy(szTemp,"Q ");
                            else
                                strcpy(szTemp,"R ");

                            if(pClientData->FavoriteServer->locked)
                                strcat(szTemp,"L ");
                            else
                                strcat(szTemp,"M ");
                            gPrint(685,(i-pClientData->ServerListOffset)*12+32,szTemp,2);

                            if(pClientData->SelectedServer==i)
                            {
                                if(!pFMMS) pFMMS=new C_FMMS;

                                pClientData->FavoriteServer->dwPing =
                                    pFMMS->PingServer( pClientData->FavoriteServer );

                                sprintf(szTemp,"^&^3%d",pClientData->FavoriteServer->dwPing);
                                if(pClientData->FavoriteServer->dwPing>200)
                                    sprintf(szTemp,"^&^5%d",pClientData->FavoriteServer->dwPing);
                                if(pClientData->FavoriteServer->dwPing>500)
                                    sprintf(szTemp,"^&^4%d",pClientData->FavoriteServer->dwPing);
                                gPrint(620,(i-pClientData->ServerListOffset)*12+32,szTemp,0);
                            }

                            sprintf(szTemp,"^&%d/%04d",atoi(pClientData->FavoriteServer->players),atoi(pClientData->FavoriteServer->maximum_players));
                            gPrint(520,(i-pClientData->ServerListOffset)*12+32,szTemp,0);
                        }
                    }
                }
                DrawButton(BC_NEXT,UP,720,550);

                if((INPUTMODE!=CONSOLE1)&&((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false)))
                {
                    if((MouseIn(720,550,764,570)) && (GetMouseRelease(SDL_BUTTON_LEFT)) )
                    {   DrawButton(BC_NEXT,DOWN,720,550);
        			    ProcessConsoleCommand(pClientData->PlayMouseLB,0);
        			    sprintf(szTemp1,"%s%cmedia%cstandard%cgfx%cstart.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP);
                        strcpy(pClientData->szGeneric,szTemp1);
        			    LoadGeneric(szTemp1);
        			    INPUTMODE=NORMAL;

                        pClientData->FavoriteServer=pClientData->FirstFavoriteServer;
                        while(pClientData->FavoriteServer)
                        {
                            if(pClientData->FavoriteServer->key==pClientData->SelectedServer)
                                break;
                            pClientData->FavoriteServer=pClientData->FavoriteServer->pNext;
                        }

                        if(pClientData->FavoriteServer)
                            strcpy(pClientData->FavoriteServer->name,pClientData->ServerName);
                        DEL(pFMMS);
        			    pClientData->GAME_MODE=CONNECT;
                        ClearGUIs();
                        break;
        		    }
                }

            }
        }

        if(LOGINMODE==LM_INTERNET)
        {

            if(pFMMS)
            {
                if(!pFMMS->bLoggedin)
                {
                    if( (!strlen(pClientData->MasterIPAddress)) || (!strlen(pClientData->MasterPort)) )
                    {
                        // pLog->_Add("Can't login to master server because configuration file is not working...");
                        strcpy(pClientData->MasterIPAddress,"127.0.0.1");
                    }
                    pFMMS->Connect(pClientData->MasterIPAddress,pClientData->MasterPort);
                }

                if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false))
                {
        			//gPrintPct(.1875,.03,"^0Server",0);
        		//	gPrintPct(.375,.03, "^0Address",0);
        		//	gPrintPct(.65,.03,  "^0Players",0);
        		//	gPrintPct(.775,.03 ,"^0Ping",0);

                    if((INPUTMODE!=CONSOLE1)&&((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false)))
                    {
                        if(MouseIn(137,32,780,536))
                        {
                            i = pClientData->ServerListOffset+((GetMouseY()-32)/12);
                            j = pClientData->ServerListOffset+((GetMouseY()-32)/12);
                            if(strlen(pFMMS->pServerList[i].name))
                            {
                                DrawBar(153,(i-pClientData->ServerListOffset)*12+33,759,(i-pClientData->ServerListOffset)*12+33+13,LONGRGB(GetFade(1),0,200),LONGRGB(110,GetFade(1),100));
                                if(GetMouseRelease(SDL_BUTTON_LEFT))
                                {
                                    ProcessConsoleCommand(pClientData->PlayMouseLB,0);
                                    if(strlen(pFMMS->pServerList[i].name))
                                    {
                                        pClientData->SelectedServer=i;
                                        strcpy(pClientData->ServerName,pFMMS->pServerList[i].name);
                                        strcpy(pClientData->IPAddress,pFMMS->pServerList[i].ip_address);
                                        strcpy(pClientData->Port,pFMMS->pServerList[i].port);
                                    }
                                }
                            }
                        }
                    }


                    for(i=0+pClientData->ServerListOffset;i<pClientData->ServerListOffset+42;i++)
                    {
                        if(i<MAX_SERVERS)
                        {

                            if(strlen(pFMMS->pServerList[i].name))
                            {
                                if(i==pClientData->SelectedServer)
                                {

                                    if(i==j)
                                        DrawBar(153,(i-pClientData->ServerListOffset)*12+33,759,(i-pClientData->ServerListOffset)*12+33+13,LONGRGB(GetFade(1),0,200),LONGRGB(110,GetFade(3),100));
                                    else
                                        DrawBar(153,(i-pClientData->ServerListOffset)*12+33,759,(i-pClientData->ServerListOffset)*12+33+13,LONGRGB(GetFade(1),200,0),LONGRGB(0,GetFade(2),0));

                                }

                                //gPrint(140,(i-pClientData->ServerListOffset)*12+32,pFMMS->pServerList[i].user,LONGRGB(255,0,0));
                                gPrint(150,(i-pClientData->ServerListOffset)*12+32,va("^&%s",pFMMS->pServerList[i].name),0);
                                gPrint(300,(i-pClientData->ServerListOffset)*12+32,va("^&%s:%s",pFMMS->pServerList[i].ip_address,pFMMS->pServerList[i].port),0);
                                // gPrint(440,(i-pClientData->ServerListOffset)*12+32,pFMMS->pServerList[i].resource_id,LONGRGB(255,0,0));
                                // Draw OS
                                DrawOSIcon(726,(i-pClientData->ServerListOffset)*12+32,pFMMS->pServerList[i].operating_system);

                                if(pFMMS->pServerList[i].allow_new_users)
                                    strcpy(szTemp,"Q ");
                                else
                                    strcpy(szTemp,"R ");

                                if(pFMMS->pServerList[i].locked)
                                    strcat(szTemp,"L ");
                                else
                                    strcat(szTemp,"M ");
                                gPrint(685,(i-pClientData->ServerListOffset)*12+32,szTemp,2);

                                pFMMS->pServerList[i].dwPing=pFMMS->PingServer(i);

                                sprintf(szTemp,"^&^3%d",pFMMS->pServerList[i].dwPing);
                                if(pFMMS->pServerList[i].dwPing>200)
                                    sprintf(szTemp,"^&^5%d",pFMMS->pServerList[i].dwPing);
                                if(pFMMS->pServerList[i].dwPing>500)
                                    sprintf(szTemp,"^&^4%d",pFMMS->pServerList[i].dwPing);

                                gPrint(620,(i-pClientData->ServerListOffset)*12+32,szTemp,0);

                                sprintf(szTemp,"^&%d/%04d",atoi(pFMMS->pServerList[i].players),atoi(pFMMS->pServerList[i].maximum_players));
                                gPrint(520,(i-pClientData->ServerListOffset)*12+32,szTemp,0);
                            }
                        }
                    }
                }

                DrawButton(BC_NEXT,UP,720,550);

                if((INPUTMODE!=CONSOLE1)&&((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false)))
                {
                    if((MouseIn(720,550,764,570)) && (GetMouseRelease(SDL_BUTTON_LEFT)) )
                    {   DrawButton(BC_NEXT,DOWN,720,550);
        			    ProcessConsoleCommand(pClientData->PlayMouseLB,0);
        			    sprintf(szTemp1,"%s%cmedia%cstandard%cgfx%cstart.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP);
                        strcpy(pClientData->szGeneric,szTemp1);
        			    LoadGeneric(szTemp1);
        			    INPUTMODE=NORMAL;
                        strcpy(pFMMS->pServerList[pClientData->SelectedServer].name,pClientData->ServerName);
                        DEL(pFMMS);
        			    pClientData->GAME_MODE=CONNECT;
                        break;
        		    }
                }
            }
        }

        // DRAW TABS ACROSS TOP TO DEFINE WHERE THE SERVER LIST IS BEING RETRIEVED

        switch(LOGINMODE)
        {
            case LM_NONE:
                gPrint(300,550,"^3Choose server search method",0);
                break;
            case LM_INTERNET:
                gPrint(300,550,"^4Internet Search",0);
                break;
            case LM_FAVORITES:
                gPrint(300,550,"^5Favorites",0);
                break;
            case LM_LOCAL:
                gPrint(300,550,"^6Local (LAN)",0);
                break;
            default:
                gPrint(300,550,"^1!ErRoR!",0);
                break;
        }



        //////////////////////////////////////////////////////////////
        // MASTER SERVER (INTERNET)
        //           Button ~ add selected internet server to favorites

        gPrint(22,22,"Internet",0);

        if(LOGINMODE==LM_INTERNET)
        {
            gPrint(22,91, "^2Add Favorite",0);
            if(MouseIn(20,90,120,110))
            {
                gPrint(22,91, "^>ffffffAdd Favorite",0);
                if(GetMouseRelease(SDL_BUTTON_LEFT))
                {
                    ClearGUIs();
                    if(pFMMS)
                    {
                        if(pClientData->SelectedServer<MAX_SERVERS)
                            pClientData->AddFavoriteServer(&pFMMS->pServerList[pClientData->SelectedServer]);
                    }
                }
            }

            gPrint(22,114,"^2More Info",0);
            if(MouseIn(20,112,120,132))
            {

                gPrint(22,114,"^>ffffffMore Info",0);
            }

        }

        //////////////////////////////////////////////////////////////
        // FAVORITES
        //           Button ~ enter ip address
        //           Button ~ delete selected favorite

        gPrint(22,45,"Favorites",0);

        if(LOGINMODE==LM_FAVORITES)
        {
            gPrint(22,91, "^5Manual entry",0);
            if(MouseIn(20,90,120,110))
            {
                gPrint(22,91, "^>ffffffManual entry",0);
                if(GetMouseRelease(SDL_BUTTON_LEFT))
                {
                    ClearGUIs();

                    pGUI->call("fav_man_add.gui");

                }
            }
            gPrint(22,114,"^5Discard",0);
            if(MouseIn(20,112,120,132))
            {
                gPrint(22,114,"^>ffffffDiscard",0);
                if(GetMouseRelease(SDL_BUTTON_LEFT))
                {
                    ClearGUIs();

                    if(pClientData->SelectedServer<MAX_SERVERS)
                    {
                        pGUI->call("fav_man_del.gui");

                        gui->get_stump("fav_man_del.gui")->get_control("name")->set_value(pClientData->ServerName);
                        gui->get_stump("fav_man_del.gui")->get_control("ip_address")->set_value(pClientData->IPAddress);
                        gui->get_stump("fav_man_del.gui")->get_control("port")->set_value(pClientData->Port);
                        gui->get_stump("fav_man_del.gui")->get_control("id")->set_value(va("%d",pClientData->SelectedServer));

                    }
                }
            }
            gPrint(22,137,"^5Edit",0);
            if(MouseIn(20,136,120,156))
            {
                gPrint(22,137,"^>ffffffEdit",0);
                if(GetMouseRelease(SDL_BUTTON_LEFT))
                {
                    ClearGUIs();

                    if(pClientData->SelectedServer<MAX_SERVERS)
                    {
                        gui->call(va("%s%cgumps%cfav_man_mod.gui",pClientData->GDDir,PATH_SEP,PATH_SEP));

                        gui->get_stump("fav_man_mod.gui")->get_control("name")->set_value(pClientData->ServerName);
                        gui->get_stump("fav_man_mod.gui")->get_control("ip_address")->set_value(pClientData->IPAddress);
                        gui->get_stump("fav_man_mod.gui")->get_control("port")->set_value(pClientData->Port);
                        gui->get_stump("fav_man_mod.gui")->get_control("id")->set_value(va("%d",pClientData->SelectedServer));

                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////
        // LOCAL (LAN)
        //           Button ~ autodiscover
        //           Button ~ enter ip address
        //           Button ~ add to favorites

        gPrint(22,68,"Local (LAN)",0);

        if(LOGINMODE==LM_LOCAL)
        {
            gPrint(22,91,"^6Auto-Detect",0);
            if(MouseIn(20,90,120,110))
            {
                gPrint(22,91,"^>ffffffAuto-Detect",0);
                if(GetMouseRelease(SDL_BUTTON_LEFT))
                {
                    ClearGUIs();
                }
            }
        }

        DrawGUIResource(BC_BLANK_BUTTON  ,760,  34, 770,  44);
        DrawGUIResourceC(BC_ARROW_UP     ,761,  35, 769,  43,0,0,0); // up arrow
        DrawGUIResource(BC_BLANK_BUTTON  ,760, 320, 770, 330);
        DrawGUIResourceC(BC_ARROW_DOWN   ,761, 321, 769, 329,0,0,0); // down arrow
        DrawGUIResource(BC_SCROLLBAR_UD  ,760,  45, 770, 319); // updown slide bar

        DrawButton(BC_BACK,UP,40,550);

        if((INPUTMODE!=CONSOLE1)&&((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false)))
        {
            // Back button
            if( MouseIn(40,550,84,570))
            {
                if( GetMouseRelease(SDL_BUTTON_LEFT) )
                {
                    DrawButton(BC_BACK,DOWN,40,550);
                    ProcessConsoleCommand("disconnect",0);
        		    ProcessConsoleCommand(pClientData->PlayMouseLB,0);
                    DEL(pFMMS);
                    ClearGUIs();

                    pClientData->GAME_MODE=MAIN_MENU;
                    break;
                }
        	}

            // Internet
            if( MouseIn(20,20,120,40) )
            {
                gPrint(22,22,"^>ffffffInternet",0);

                if( GetMouseRelease(SDL_BUTTON_LEFT) )
                {
                    LOGINMODE=LM_INTERNET;
                    pClientData->GAME_MODE=GATHER_SERVER_LIST;

                    ClearGUIs();
                    break;
                }
            }

            // Favorites
            if( MouseIn(20,43,120,63) )
            {
                gPrint(22,45,"^>ffffffFavorites",0);

                if( GetMouseRelease(SDL_BUTTON_LEFT) )
                {
                    LOGINMODE=LM_FAVORITES;
                    pClientData->GAME_MODE=GATHER_SERVER_LIST;

                    ClearGUIs();
                    break;
                }
            }

            // Local (LAN)
            if( MouseIn(20,65,120,85) )
            {
                gPrint(22,68,"^>ffffffLocal (LAN)",0);

                if( GetMouseRelease(SDL_BUTTON_LEFT) )
                {
                    LOGINMODE=LM_LOCAL;
                    pClientData->GAME_MODE=GATHER_SERVER_LIST;

                    ClearGUIs();
                    break;
                }
            }
        }
        */



            /*********************************************************************************
             ** NETMSG_GUI                                                                  **
             *********************************************************************************

            case NETMSG_GUI:
                dx=RecvData->cRead();
                switch(dx)
                {
                    case FM_GUI_PROGRAM_FINISH: // end of gui
                        GAME_MODE=NEXT_GAME_MODE;
                        break;

                    case FM_GUI_STUMP_CREATE: // add gui stump (name)
                        strcpy(temp1,RecvData->pRead());
                        DLog("NETMSG_GUI[FM_GUI_STUMP_CREATE] [%s]",temp1);
                        ax=RecvData->iRead();
                        ay=RecvData->iRead();
                        bx=RecvData->iRead();
                        by=RecvData->iRead();
                        cx=RecvData->iRead();
                        strcpy(temp2,RecvData->pRead());
                        DLog("add stump->[%s,%d,%d,%d,%d,%d,%s]",temp1,ax,ay,bx,by,cx,temp2);
                        gui->add_stump(temp1,ax,ay,bx,by,cx,temp2);
                        break;

                    case FM_GUI_STUMP_UPDATE:
                        strcpy(temp1,RecvData->pRead());
                        ax=RecvData->iRead();
                        ay=RecvData->iRead();
                        bx=RecvData->iRead();
                        by=RecvData->iRead();
                        cx=RecvData->iRead();
                        strcpy(temp2,RecvData->pRead());
                        gui->mod_stump(temp1,ax,ay,by,by,cx,temp2);
                        DLog("NETMSG_GUI[FM_GUI_STUMP_UPDATE] [%s]",temp1);
                        break;

                    case FM_GUI_STUMP_REMOVE: // remove gui stump
                        strcpy(temp1,RecvData->pRead());
                        DLog("NETMSG_GUI[FM_GUI_STUMP_REMOVE] [%s]",temp1);
                        if(focus_stump==gui->get_stump(temp1))focus_stump=0;
                        gui->del_stump(temp1);

                        break;

                    case FM_GUI_STUMP_CAPTION:
                        strcpy(temp1,RecvData->pRead());
                        strcpy(temp2,RecvData->pRead());
                        DLog("NETMSG_GUI[FM_GUI_STUMP_CAPTION] [%s][%s]",temp1,temp2);
                        if(gui)
                            if(gui->get_stump(temp1))
                                gui->get_stump(temp1)->set_caption(temp2);
                        break;

                    case FM_GUI_CONTROL_CREATE: // add gui control to a stump
                        strcpy(temp1,RecvData->pRead());
                        strcpy(temp2,RecvData->pRead());
                        DLog("NETMSG_GUI[FM_GUI_CONTROL_CREATE] [%s][%s]",temp1,temp2);
                        cx=RecvData->iRead();
                        ax=RecvData->iRead();
                        ay=RecvData->iRead();
                        bx=RecvData->iRead();
                        by=RecvData->iRead();
                        dx=RecvData->iRead();
                        strcpy(temp3,RecvData->pRead());
                        strcpy(temp4,RecvData->pRead());
                        gui->get_stump(temp1)->add_control(temp2,cx,ax,ay,bx,by,dx,temp3,temp4);
                        DLog("Control create!");
                        break;

                    case FM_GUI_CONTROL_REMOVE: // remove gui control from a stump
                        strcpy(temp1,RecvData->pRead());
                        strcpy(temp2,RecvData->pRead());
                        strcpy(temp3,RecvData->pRead());
                        DLog("NETMSG_GUI[FM_GUI_CONTROL_REMOVE] [%s][%s][%s]",temp1,temp2,temp3);
                        break;

                    case FM_GUI_CONTROL_VALUE:
                        strcpy(temp1,RecvData->pRead());
                        strcpy(temp2,RecvData->pRead());
                        strcpy(temp3,RecvData->pRead());
                        DLog("NETMSG_GUI[FM_GUI_CONTROL_VALUE]  [%s][%s][%s]",temp1,temp2,temp3);
                        if(gui)
                            if(gui->get_stump(temp1))
                                if(gui->get_stump(temp1)->get_control(temp2))
                                    strcpy(gui->get_stump(temp1)->get_control(temp2)->value,temp3);
                        break;

                    case FM_GUI_CLEAR_ALL:
                        DLog("NETMSG_GUI[FM_GUI_CLEAR_ALL]");
                        DEL(gui);
                        gui=new C_GUI();
                        break;

                    default:
                        break;
                }
                break;

                */
