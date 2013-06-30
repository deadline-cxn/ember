
/*


/////////////////////////////////////////////////////////////////////////
extern "C" void ex_ProcessConsoleCommand(char *szCommandIn, bool bEcho)
{
    // ARROW THROUGH RECENT COMMANDS 26 Jul 00

    if(pClientData)
    {
        if(bEcho)
        {
            pGUI->ConsoleScrollInput();
        }
    }

    szInput = strtok(szInputCopy,"? ");
    if(szInput==NULL) // NO COMMAND IN LINE
    {
        if(bEcho)
        {
            pCon->ConsoleBufferAdd(szCommand);
            pCon->ConsoleBufferAdd("What?");
        }
        return;
    }

    if(bEcho) pCon->ConsoleBufferAdd(szCommand);

    /////////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // /<REMOTE>
    // Passes command line to server for execution

    if(szInput[0]=='/')
    {
        if(pFMGS)
        {
            if(pFMGS->eGetState()!=NET_NOTCONNECT)
            {
               // Log("Sending %s to the server",szCommand);

                pCon->ConsoleBufferAdd("Sending %s to the server",szCommand);

                SendData.Reset();
                SendData.Write((char)NETMSG_GENERIC_MSG);
                SendData.Write((char *)szCommand);
                SendNetMessage(0);
                return;
            }
        }
        pCon->ConsoleBufferAdd("Usage : / funcs require server connection");
        return;
    }


	/////////////////////////////////////////////////////////////////////////
	// CC! (Console command)
	// GETFILE
	//

	if(dlcs_strcasecmp(szInput,"getfile"))
	{
		szInput=strtok(NULL," \n");

		if(szInput)
		{
			GetFile(szInput);

            //Log("Sent %d %d %d %s %s",NETMSG_FM_DOWNLOAD,FM_DOWNLOAD_REQUEST,FM_DOWNLOAD_RES_GFX,szInput,szInput);

			return;
		}
        ProcessConsoleCommand("help getfile",0);
		return;
	}

	/////////////////////////////////////////////////////////////////////////
	// CC! (Console command)
	// DELETECHARACTER
	//

	if(dlcs_strcasecmp(szInput,"deletecharacter"))
	{
		szInput=strtok(NULL,"\n");
		if(szInput)
		{
			SendData.Reset();
			SendData.Write((char)NETMSG_DELETE_CHARACTER);
			SendData.Write((char)atoi(szInput));
			SendNetMessage(1);
			return;
		}
		ProcessConsoleCommand("help deletecharacter",0);
		return;
	}




    /////////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // PROMPT
    //

    if(dlcs_strcasecmp(szInput,"prompt"))
    {

        szInput=strtok(NULL,",\n");
        if(szInput!=NULL)
        {

            strcpy(szTemp1,szInput);
			szInput=strtok(NULL,"\n");
			if(szInput!=NULL)
			{
				strcpy(szTemp2,szInput);
                pGUI->Prompt(szTemp1,szTemp2);
				return;
			}
        }
        // Default help text
		ProcessConsoleCommand("help prompt",0);
        return;
    }

    /////////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // HELP
    //

    if(dlcs_strcasecmp(szInput,"help"))
    {

        szInput=strtok(NULL," \n");
        if(szInput!=NULL)
        {
            sprintf(szTemp1,"type help%c%s.txt",PATH_SEP,szInput);
            ProcessConsoleCommand(szTemp1,0);
            return;
        }

        // Default help text
        sprintf(szTemp1,"type help%chelp.txt",PATH_SEP);
        ProcessConsoleCommand(szTemp1,0);
        return;
    }


    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // REFRESH
    // Refreshes certain game environment objects

    if(dlcs_strcasecmp(szInput,"refresh"))
    {
        szInput=strtok(NULL," \n\r");

        if(szInput==NULL)
        {
            if(bEcho)
                pCon->ConsoleBufferAdd("Usage : invalid refresh option : refresh <setting>");
            return;
        }

        /////////////////////////
        // CC! (Console command)
        // refresh basegfx

        if(dlcs_strcasecmp(szInput,"basegfx"))
        {
            LoadBaseGFX();
            if(bEcho)
                pCon->ConsoleBufferAdd("FCOM: Base GFX refreshed....");
            return;
        }

        /////////////////////////
        // CC! (Console command)
        // refresh models

        if(dlcs_strcasecmp(szInput,"models"))
        {
            LoadModels();
            if(bEcho)
                pCon->ConsoleBufferAdd("FCOM: Models refreshed....");
            return;
        }

        if(bEcho)
            pCon->ConsoleBufferAdd("Usage : invalid refresh option : refresh <setting>");
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // ALIAS
    // Alias a command

    if(dlcs_strcasecmp(szInput,"alias"))
    {
        pCon->ConsoleBufferAdd("Alias not working yet...");
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // INFO
    // Show various information

    if(dlcs_strcasecmp(szInput,"info"))
    {
       // Log("Ember Information");
       // Log("Client Version......%s",VERSION);
       // Log("Server Version......%s",pClientData->szServerVersion);
       // Log("Server ID...........%s",pClientData->ServerID);

        if(pFMGS)
        {
            i=pFMGS->eGetState();
            if(i==NET_NOTCONNECT)
               // Log("Network.............Not Connected");

            else
            {
               // Log("Network.............Connected to %s",pClientData->IPAddress);

                switch(i)
                {
                    case NET_NOTCONNECT:
                        strcpy(szTemp1,"NET_NOTCONNECT");
                        break;
                    case NET_CONNECTED:
                        strcpy(szTemp1,"NET_CONNECTED");
                        break;
                    case NET_SYSBUSY:
                        strcpy(szTemp1,"NET_SYSBUSY");
                        break;
                    case NET_LOGGINPROC:
                        strcpy(szTemp1,"NET_LOGGINPROC");
                        break;
                    default:
                        strcpy(szTemp1,"UNKNOWN");
                        break;
                }

               // Log("Network State.......%s",szTemp1);

                if(pPlayer)
                {
                   // Log("Name................%s",pPlayer->szGetClientName());
                   // Log("Character Name......%s",pPlayer->szGetCharacterName());
                   // Log("Access Level........%d(%s)",pPlayer->cGetAccess(),pClientData->szAccessName);
                }
            }
        }

        //// Log("Map Location=[%d][%d][%d] (type calc for more info)", pPlayer->GetX(),pPlayer->GetY(),pPlayer->GetZ());

        return;

    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // TOGGLE
    // Enable/disable certain switches

    if(dlcs_strcasecmp(szInput,"toggle"))
    {
        szInput=strtok(NULL," \n\r");
        if(szInput==NULL)
        {
            pCon->ConsoleBufferAdd("Usage : toggle <setting>");
            return;
        }

        /////////////////////////
        // CC! (Console command)
        // toggle console

        if(dlcs_strcasecmp(szInput,"console"))
        {
            pCon->ToggleConsole();
            return;

        }

        /////////////////////////
        // CC! (Console command)
        // toggle fps

        if(dlcs_strcasecmp(szInput,"fps"))
        {
            if(pGUI->bShowFPS==true)
                pGUI->bShowFPS=false;
            else
                pGUI->bShowFPS=true;
            return;
        }

        /////////////////////////
        // CC! (Console command)
        // toggle ping

        if(dlcs_strcasecmp(szInput,"ping"))
        {
            if(pClientData->bShowPING==true)
                pClientData->bShowPING=false;
            else
                pClientData->bShowPING=true;
            return;
        }

        /////////////////////////
        // CC! (Console command)
        // toggle sound

        if(dlcs_strcasecmp(szInput,"sound"))
        {
            if(pClientData->bAudioFailure) return;
            if(pClientData->bSound==true)
            {
                pClientData->bSound=false;
            }
            else
                pClientData->bSound=true;
            return;
        }

        /////////////////////////
        // CC! (Console command)
        // toggle music

        if(dlcs_strcasecmp(szInput,"music"))
        {
            if(pClientData->bAudioFailure) return;
            if(pClientData->bMusic==true)
            {
                pClientData->bMusic=false;
                StopAudio();
            }
            else
                pClientData->bMusic=true;
            return;
        }

        /////////////////////////
        // CC! (Console command)
        // toggle drawtiles

        if(dlcs_strcasecmp(szInput,"drawtiles"))
        {
            if(pClientData->bDrawMap==true)
                pClientData->bDrawMap=false;
            else
                pClientData->bDrawMap=true;
            return;
        }


        /////////////////////////
        // CC! (Console command)
        // toggle drawobs

        if(dlcs_strcasecmp(szInput,"drawobs"))
        {
            if(pClientData->bDrawMapObjects==true)
                pClientData->bDrawMapObjects=false;
            else
                pClientData->bDrawMapObjects=true;
            return;
        }

        /////////////////////////
        // CC! (Console command)
        // toggle blockglow

        if(dlcs_strcasecmp(szInput,"blockglow"))
        {
            if(pClientData->bBlockGlow==true)
                pClientData->bBlockGlow=false;
            else
                pClientData->bBlockGlow=true;
            return;
        }

        /////////////////////////
        // CC! (Console command)
        // toggle vsm

        if(dlcs_strcasecmp(szInput,"vsm"))
        {
            if(pClientData->bVertice==true)
                pClientData->bVertice=false;
            else
                pClientData->bVertice=true;
            return;
        }


        if(bEcho)
            pCon->ConsoleBufferAdd("unrecognized option");
        return;
    }


    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // CD
    // Change working directory to specified directory

    if(dlcs_strcasecmp(szInput,"cd"))
    {
        szInput=strtok(NULL,"\n\r");
        if(szInput==NULL)
        {
            if(bEcho)
            {
                pCon->ConsoleBufferAdd("Usage : You must supply a directory to change to!");
                pCon->ConsoleBufferAdd("Usage : cd <directory>");
            }
            return;
        }
        //if(
			dlcs_chdir(szInput);//')
//            strcpy(pClientData->CWDir,dlcs_getcwd());
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // DIR
    // Show directory information


    if( (dlcs_strcasecmp(szInput,"dir")) || (dlcs_strcasecmp(szInput,"ls")) )
    {

#ifdef _WIN32

        HANDLE          dirsearch;
        WIN32_FIND_DATA FileData;

        char WildCard[_MAX_PATH+100];

        szInput=strtok(NULL,"\n\r");
        if(szInput==NULL)
        {
            strcpy(WildCard ,"*.*");
        }
        else
        {
            // change to check to see if input was a wildcard, or a directory, or
            // a directory and wildcard
            // for now, just get the current directory - so fill the
            // wildcard with *.*

            strcpy(szTemp1,szInput);

            j=0;

            for(i=0;i<strlen(szTemp1);i++)
            {
                if( (szTemp1[i]=='*') ||
                    (szTemp1[i]=='.') ||
                    (szTemp1[i]=='?') )
                    j++;
            }

            if(j)
                strcpy(WildCard , szInput);
            else
                sprintf(WildCard,"%s%c*.*",szInput,PATH_SEP);
        }


        // strcpy(WildCard ,"*.*");

        pCon->ConsoleBufferAdd(" ");
        getcwd(szTemp2,_MAX_PATH);
        sprintf(szTemp1,"Directory of %s",szTemp2);
        pCon->ConsoleBufferAdd(szTemp1);
        pCon->ConsoleBufferAdd("===============================================================");
        dirsearch = FindFirstFile( WildCard, &FileData );
        while(GetLastError() != ERROR_NO_MORE_FILES)
        {
            strcpy(szTemp2,FileData.cFileName);
            while(strlen(szTemp2)<25)
                strcat(szTemp2," ");
            if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                sprintf(szTemp1,"%s <DIR> ",szTemp2);
            else
                sprintf(szTemp1,"%s %d ",szTemp2,FileData.nFileSizeLow);
            while(strlen(szTemp1)<35)
                strcat(szTemp1," ");
            if(FileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
                strcat(szTemp1,"A");
            else
                strcat(szTemp1," ");
            if(FileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
                strcat(szTemp1,"H");
            else
                strcat(szTemp1," ");
            if(FileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                strcat(szTemp1,"R");
            else
                strcat(szTemp1," ");
            if(FileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
                strcat(szTemp1,"S");
            else
                strcat(szTemp1," ");
            pCon->ConsoleBufferAdd(szTemp1);
            FindNextFile(dirsearch, &FileData);
        }
        SetLastError(ERROR_SUCCESS);
        FindClose(dirsearch);
        return;
#endif

#ifdef __linux__
        pCon->ConsoleBufferAdd("dir function not implemented in linux version yet.");
        return;
#endif

    }


    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // TYPE
    // Display contents of a text file

    if(dlcs_strcasecmp(szInput,"type"))
    {
        if(bEcho)
        {
            pCon->ConsoleBufferAdd("===============================================================");
            pCon->ConsoleBufferAdd(" ");
        }

        szInput=strtok(NULL,"\n\r");
        if(szInput==NULL)
        {
            pCon->ConsoleBufferAdd("Usage : type <filename>");
            return;
        }
        FILE *fp;
        fp=fopen(szInput,"r");

        if(fp)
        {
            while(1)
            {
                if(!fgets(szTemp1,96,fp))
                    return;

                pCon->ConsoleBufferAdd(szTemp1);
            }
            fclose(fp);
        }

        else
        {
            sprintf(szCommand,"Can't find file: %s",szInput);
        }

        return;

    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // SET
    // change different system variables

    if(dlcs_strcasecmp(szInput,"set"))
    {
        szInput=strtok(NULL," \n\r");
        if(szInput==NULL)
        {
            pCon->ConsoleBufferAdd("Usage: set <option> <value>");
            return;
        }


        /////////////////////////
        // CC! (Console command)
        // set debug <level>

        if(dlcs_strcasecmp(szInput,"debug"))
        {
            szInput=strtok(NULL,"\n\r");
            if(szInput!=NULL)
            {
                pClientData->cDebug=atoi(szInput); // 0 = no debug messages
                                                   // 1 = on screen debug messages
                                                   // 2 =// Log debug messages
                return;
            }
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // CC! (Console command)
        // set musicvolume

        if(dlcs_strcasecmp(szInput,"musicvolume"))
        {
            if(pClientData->bAudioFailure) return;
            szInput=strtok(NULL,"\n\r");
            if(szInput!=NULL)
            {
                f=atof(szInput);
                if( (f == 0) &&
                    (strcmp(szInput,"0")) )
                {
                    if(bEcho)
                        pCon->ConsoleBufferAdd("> Illegal < : Volume value is from 0.0 to 1.0");
                    return;
                }

                if( (f > 1.0f) ||
                    (f < 0.0f) )
                {
                    if(bEcho)
                        pCon->ConsoleBufferAdd("> Illegal < : Volume value is from 0.0 to 1.0");
                    return;
                }
                pClientData->fMusicVolume=f;
                //SetMusicVolume(f);
                pCon->ConsoleBufferAdd("Music volume set to %f",f);
            }
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // CC! (Console command)
        // set soundvolume

        if(dlcs_strcasecmp(szInput,"soundvolume"))
        {
            if(pClientData->bAudioFailure) return;
            szInput=strtok(NULL,"\n\r");
            if(szInput!=NULL)
            {
                f=atof(szInput);
                if( ( f == 0 ) &&
                    ( strcmp(szInput,"0") ) )
                {
                    if(bEcho)
                        pCon->ConsoleBufferAdd("> Illegal < : Volume value is from 0.0 to 1.0");
                    return;
                }
                if( (f > 1.0f) ||
                    (f < 0.0f) )
                {
                    if(bEcho)
                        pCon->ConsoleBufferAdd("> Illegal < : Volume value is from 0.0 to 1.0");
                    return;
                }
                pClientData->fSoundVolume=f;
                pCon->ConsoleBufferAdd("Sound volume set to %f",f);
            }
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // CC! (Console command)
        // set name

        if(dlcs_strcasecmp(szInput,"name"))
        {
            szInput=strtok(NULL,"\n\r");
            if(szInput==NULL)
            {
                if(bEcho)
                    pCon->ConsoleBufferAdd("Usage : Enter a name!");
                return;
            }
            strcpy(pClientData->Name,szInput);
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // CC! (Console command)
        // set password

        if(dlcs_strcasecmp(szInput,"password"))
        {
            szInput=strtok(NULL,"\n\r");
            if(szInput==NULL)
            {
                if(bEcho)
                    pCon->ConsoleBufferAdd("Usage : Enter the password!");
                return;
            }
            strcpy(pClientData->Password,szInput);
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // CC! (Console command)
        // set ipaddress

        if(dlcs_strcasecmp(szInput,"ipaddress"))
        {
            szInput=strtok(NULL,"\n\r");
            if(szInput==NULL)
            {
                if(bEcho)
                    pCon->ConsoleBufferAdd("Usage : Enter the IP Address!");
                return;
            }
            strcpy(pClientData->IPAddress,szInput);
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // CC! (Console command)
        // set port

        if(dlcs_strcasecmp(szInput,"port"))
        {
            szInput=strtok(NULL,"\n\r");
            if(szInput==NULL)
            {
                if(bEcho)
                    pCon->ConsoleBufferAdd("Usage : Enter the Port!");
                return;
            }
            strcpy(pClientData->Port,szInput);
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // CC! (Console command)
        // set savepassword

        if(dlcs_strcasecmp(szInput,"savepassword"))
        {
            szInput=strtok(NULL,"\n\r");
            if(szInput==NULL)
            {
                if(bEcho)
                    pCon->ConsoleBufferAdd("Usage : set savepassword (on or off)");
                return;
            }
            if(strcmp(szInput,"on") && strcmp(szInput,"off"))
            {
                if(bEcho)
                    pCon->ConsoleBufferAdd("Usage : set savepassword (on or off)");
                return;
            }
            if(dlcs_strcasecmp(szInput,"on"))
                pClientData->bSavePassword = true;
            else
                pClientData->bSavePassword = false;
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // CC! (Console command)
        // set// Log

        if(dlcs_strcasecmp(szInput,"log"))
        {
            szInput=strtok(NULL,"\n\r");
            if(szInput==NULL)
            {
                if(bEcho)
                    pCon->ConsoleBufferAdd("Usage : set// Log (on or off)");
                return;
            }
            if(strcmp(szInput,"on") && strcmp(szInput,"off"))
            {
                if(bEcho)
                    pCon->ConsoleBufferAdd("Usage : set// Log (on or off)");
                return;
            }
            if(dlcs_strcasecmp(szInput,"on"))
            {
                pLog->On();
               // Log("Log activated from console.");
                pClientData->bLog = true;
            }
            else
            {
                pLog->On();
               // Log("Log deactivated from console.");
                pLog->Off();
                pClientData->bLog = false;
            }
            bLog=pClientData->bLog;
            return;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // CC! (Console command)
        // set consolebmp

        if(dlcs_strcasecmp(szInput,"consolebmp"))
        {
            szInput=strtok(NULL,"\n\r");
            if(szInput==NULL)
            {
                if(bEcho)
                    pCon->ConsoleBufferAdd("Usage : set consolebmp (filename)");
                return;
            }
            if(!pCon->LoadConsoleTexture(szInput))
                pCon->LoadConsoleTexture(va("base%c%s",PATH_SEP,szInput));
            return;
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // SHOW
    // show various sytsem information

    if( (dlcs_strcasecmp(szInput,"show"))   ||
        (dlcs_strcasecmp(szInput,"q"))  ||
        (dlcs_strcasecmp(szInput,"?"))  )
    {
        if(bEcho)
            pCon->ConsoleBufferAdd(szCommand);
        szInput=strtok(NULL," \n\r");
        if(szInput==NULL)
        {
            pCon->ConsoleBufferAdd("Usage : show <option>");
            return;
        }

        if(dlcs_strcasecmp(szInput,"musicvolume"))
        {
            pCon->ConsoleBufferAdd("Music volume is set to:%f",pClientData->fMusicVolume);
            return;
        }

        if(dlcs_strcasecmp(szInput,"soundvolume"))
        {
            pCon->ConsoleBufferAdd("Sound volume is set to:%f",pClientData->fSoundVolume);
            return;
        }

        //////////////////////////
        // CC! (Console command)
        // show version

        if(dlcs_strcasecmp(szInput,"version"))
        {
			CxImage *img=new CxImage();
           // Log("==============================================================");
           // Log("Ember Client version %s (%s) %s",VERSION,CPUSTRING,COPYRIGHT);
           // Log("Revision(%s) Network Revision(%s)",EGC_REVISION,NET_REVISION);
			Log("SDL version %d.%d.%d",SDL_MAJOR_VERSION,SDL_MINOR_VERSION,SDL_PATCHLEVEL);
			Log("FMOD version %s",     FMODVersion());
			Log("ZLIB version %s",     ZLIB_VERSION);
			Log("%s",  img->GetVersion());
           // Log("==============================================================");
           // Log("Type HELP for help.");
			DEL(img);
            return;
        }

        //////////////////////////
        // CC! (Console command)
        // show name

        if(dlcs_strcasecmp(szInput,"name"))
        {
            sprintf(szInput,"name = '%s'",pClientData->Name);
            if(bEcho)
                pCon->ConsoleBufferAdd(szInput);
            return;
        }

        //////////////////////////
        // CC! (Console command)
        // show password

        if(dlcs_strcasecmp(szInput,"password"))
        {
            memset(szTemp3,0,100);
            for(i=0;i<strlen(pClientData->Password);i++)
            {
                szTemp3[i]='*';
                szTemp3[i+1]=0;
            }
            sprintf(szInput,"password = '%s'",szTemp3);
            if(bEcho)
                pCon->ConsoleBufferAdd(szInput);
            return;
        }

        //////////////////////////
        // CC! (Console command)
        // show ipaddress

        if(dlcs_strcasecmp(szInput,"ipaddress"))
        {
            sprintf(szInput,"ipaddress = '%s'",pClientData->IPAddress);
            if(bEcho)
                pCon->ConsoleBufferAdd(szInput);
            return;
        }

        //////////////////////////
        // CC! (Console command)
        // show port

        if(dlcs_strcasecmp(szInput,"port"))
        {
            sprintf(szInput,"port = '%s'",pClientData->Port);
            if(bEcho)
                pCon->ConsoleBufferAdd(szInput);
            return;
        }

        //////////////////////////
        // CC! (Console command)
        // show savepassword

        if(dlcs_strcasecmp(szInput,"savepassword"))
        {
            if(pClientData->bSavePassword)
                strcpy(szInput,"savepassword = on");
            else
                strcpy(szInput,"savepassword = off");
            if(bEcho)
                pCon->ConsoleBufferAdd(szInput);
            return;
        }

        //////////////////////////
        // CC! (Console command)
        // show// Log

        if(dlcs_strcasecmp(szInput,"log"))
        {
            if(pClientData->bLog)
                strcpy(szInput,"log = on");
            else
                strcpy(szInput,"log = off");
            if(bEcho)
                pCon->ConsoleBufferAdd(szInput);
            return;
        }

        //////////////////////////
        // CC! (Console command)
        // show mode

        if(dlcs_strcasecmp(szInput,"mode"))
        {

            switch(pClientData->Mode)
            {
                case PLAY:
                    strcpy(szInput,"mode = play");
                    break;
                case BUILD:
                    strcpy(szInput,"mode = build");
                    break;
                case BUILD_GHOST:
                    strcpy(szInput,"mode = build ghost");
                    break;
                case SPECTATOR:
                    strcpy(szInput,"mode = spectator");
                    break;
                default:
                    pClientData->Mode=PLAY;
                    strcpy(szInput,"mode = play");
                    break;
            }

            if(bEcho)
                pCon->ConsoleBufferAdd(szInput);
            return;
        }


        if(bEcho)
            pCon->ConsoleBufferAdd("unrecognized option");

        return;
    }

    //****************************************************************
//     *                      Network COMMANDS                        *
  //   ****************************************************************

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // CONNECT
    // The Network should connect through the console buffer...
    // SYNTAX: connect <ipaddress or host name>[:<port>] <username> <password>

    if(dlcs_strcasecmp(szInput,"connect"))
    {
        if(bEcho)
            pCon->ConsoleBufferAdd(szCommand);

        szInput=strtok(NULL," \n\r");
        if(szInput!=NULL)
        {
            strcpy(szTemp1,szInput);
            szInput=strtok(NULL," \n\r");
            if(szInput!=NULL)
            {
                strcpy(szTemp2,szInput);
                szInput=strtok(NULL," \n\r");
                if(szInput!=NULL)
                {
                    strcpy(pClientData->Name,szTemp2);
                    strcpy(pClientData->Password,szInput);

                    strcpy(szCommand,szTemp1); // Seperate IP and Port

                    strcpy(pClientData->Port,va("%d",NET_DEFAULT_EGS_PORT)); // Default PORT

                    szInput=strtok(szCommand,":\n\r");
                    if(szInput!=NULL)
                    {
                        strcpy(pClientData->IPAddress,szInput);
                        szInput=strtok(NULL,"\n\r");
                        if(szInput!=NULL)
                        {
                            strcpy(pClientData->Port,szInput);
                        }
                    }

                   // Log("Connecting to %s:%s",pClientData->IPAddress,pClientData->Port);

                    GAME_MODE=CONSOLE_CONNECT;

                    return;
                }
            }
        }
        pCon->ConsoleBufferAdd("Usage : connect [<ipaddress or hostname>[:<port>] <username> <password>]");
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // DISCONNECT
    // disconnect from server - puts you back to the main menu, and drops console

    if(dlcs_strcasecmp(szInput,"disconnect"))
    {
        // DLog("NET: Attempting disconnect from server...");

        if(bEcho)
            pCon->ConsoleBufferAdd(szCommand);

        if(pFMGS)
        {
            if(pFMGS->eGetState()!=NET_NOTCONNECT)
            {
                SendData.Reset();
                SendData.Write((char)NETMSG_CLIENTSHUTDOWN);
                SendData.Write((char)1);
                SendNetMessage(1);
                pFMGS->Disconnect();
                // DLog("NET: Disconnected from server...");
            }

            else
                // DLog("NET: Not connected...");
        }

        NetworkSetup();

        GAME_MODE=LOGIN_SCREEN_ENTRY;
        //INPUTMODE=CONSOLE1;
        return;
    }



    /////////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // SAY
    // Local Chat - players on the screen can see this chat

    if(dlcs_strcasecmp(szInput,"say"))
    {

        if(bEcho)
            pCon->ConsoleBufferAdd(szCommand);

        if(pFMGS)
        {
            if(pFMGS->eGetState()==NET_NOTCONNECT)
            {
                pCon->ConsoleBufferAdd("Usage : Connect to a server first!");
                return;
            }

            szInput=strtok(NULL,"\n\r");
            if(szInput)
            {
                // construct the packet
                SendData.Reset();
                SendData.Write((char)NETMSG_LOCALCHAT);
                SendData.Write((char *)szInput);
                SendData.Write((char)pPlayer->CL_Data->cChatR); // Red element
                SendData.Write((char)pPlayer->CL_Data->cChatG); // Blue element
                SendData.Write((char)pPlayer->CL_Data->cChatB); // Green element
                SendData.Write((char *)pPlayer->GetChatFont());   // Which font to use?
                SendNetMessage(0);
                return;
            }
            return;
        }

        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // SAYIN
    // Directs the program to enter LOCALCHATINPUT Input mode
    // (at bottom of screen)

    if(dlcs_strcasecmp(szInput,"sayin"))
    {
        // INPUTMODE=LOCALCHATINPUT;
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // GLOBSAY
    // Global Chat - All Players on the server can see this chat

    if(dlcs_strcasecmp(szInput,"globsay"))
    {

        if(bEcho)
            pCon->ConsoleBufferAdd(szCommand);

        if(pFMGS)
        {
            if(pFMGS->eGetState()==NET_NOTCONNECT)
            {
                pCon->ConsoleBufferAdd("Usage : Connect to a server first!");
                return;
            }

            szInput=strtok(NULL,"\n\r");
            if(szInput)
            {
                SendData.Reset();
                SendData.Write((char)NETMSG_SYSTEMMSG);
                SendData.Write((char *)szInput);
                SendData.Write((char)0);
                SendNetMessage(0);
            }
            return;
        }
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // GLOBSAYIN
    // Directs the program to enter GLOBALCHATINPUT Input mode
    // (at bottom of screen)

    if(dlcs_strcasecmp(szInput,"globsayin"))
    {
        // INPUTMODE=GLOBALCHATINPUT;
        return;
    }


    /****************************************************************
     *              End Of Network COMMANDS                         *
     ****************************************************************




    ///////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // BIND
    // Bind a key to a console command

            if(ikey==SDLK_BACKQUOTE)
            {
               // Log("Usage : can't bind console toggle keys");
                return;
            }

            strcpy(pGUI->Key[ikey].Command,szInput);
            for(i=0;i<strlen(pGUI->Key[ikey].Command);i++)
                if(pGUI->Key[ikey].Command[i]=='Æ')
                    pGUI->Key[ikey].Command[i]=';';
            strcpy(szInputCopy,szCommand);
            for(i=0;i<strlen(szInputCopy);i++)
                if(szInputCopy[i]=='Æ')
                    szInputCopy[i]=';';
            if(bEcho)   //bEcho indicates a manual change to the binds. Save the binds if bEcho!
            {
                pCon->ConsoleBufferAdd(szInputCopy);
                pGUI->bSaveBinds("config.cfg");
            }
            return;
        }
        else
        {
           // Log("bind : Invalid key - No bind was issued. %s",szInput);
           // Log("     : bind <key> <command>");
            return;
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // UNBIND
    // Removes a console command that has been bound to a key
    if(dlcs_strcasecmp(szInput,"unbind"))
    {
        int ikey;
        if(bEcho)
            pCon->ConsoleBufferAdd(szCommand);
        szInput=strtok(NULL," \n\r");
        if(szInput==NULL)
        {
            pCon->ConsoleBufferAdd("Usage : unbind <key> | unbind all");
            return;
        }
        if(dlcs_strcasecmp(szInput,"all"))
        {
            for(i=0;i<SDLK_LAST;i++)
                memset(pGUI->Key[i].Command,0,256);
            pCon->ConsoleBufferAdd("All keys unbound.");
            strcpy(pGUI->Key['`'].Command,"toggle console");
            strcpy(pGUI->Key['~'].Command,"toggle console");
            return;
        }
        ikey = 0;
        if(dlcs_strcasecmp(szInput,"esc"))
            ikey = SDLK_ESCAPE;
        if(dlcs_strcasecmp(szInput,"1"))
            ikey = SDLK_1;
        if(dlcs_strcasecmp(szInput,"2"))
            ikey = SDLK_2;
        if(dlcs_strcasecmp(szInput,"3"))
            ikey = SDLK_3;
        if(dlcs_strcasecmp(szInput,"4"))
            ikey = SDLK_4;
        if(dlcs_strcasecmp(szInput,"5"))
            ikey = SDLK_5;
        if(dlcs_strcasecmp(szInput,"6"))
            ikey = SDLK_6;
        if(dlcs_strcasecmp(szInput,"7"))
            ikey = SDLK_7;
        if(dlcs_strcasecmp(szInput,"8"))
            ikey = SDLK_8;
        if(dlcs_strcasecmp(szInput,"9"))
            ikey = SDLK_9;
        if(dlcs_strcasecmp(szInput,"0"))
            ikey = SDLK_0;
        if(dlcs_strcasecmp(szInput,"-"))
            ikey = SDLK_MINUS;
        if(dlcs_strcasecmp(szInput,"="))
            ikey = SDLK_EQUALS;
        if(dlcs_strcasecmp(szInput,"backspace"))
            ikey = SDLK_BACKSPACE;
        if(dlcs_strcasecmp(szInput,"tab"))
            ikey = SDLK_TAB;
        if(dlcs_strcasecmp(szInput,"q"))
            ikey = SDLK_q;
        if(dlcs_strcasecmp(szInput,"w"))
            ikey = SDLK_w;
        if(dlcs_strcasecmp(szInput,"e"))
            ikey = SDLK_e;
        if(dlcs_strcasecmp(szInput,"r"))
            ikey = SDLK_r;
        if(dlcs_strcasecmp(szInput,"t"))
            ikey = SDLK_t;
        if(dlcs_strcasecmp(szInput,"y"))
            ikey = SDLK_y;
        if(dlcs_strcasecmp(szInput,"u"))
            ikey = SDLK_u;
        if(dlcs_strcasecmp(szInput,"i"))
            ikey = SDLK_i;
        if(dlcs_strcasecmp(szInput,"o"))
            ikey = SDLK_o;
        if(dlcs_strcasecmp(szInput,"p"))
            ikey = SDLK_p;
        if(dlcs_strcasecmp(szInput,"["))
            ikey = SDLK_LEFTBRACKET;
        if(dlcs_strcasecmp(szInput,"]"))
            ikey = SDLK_RIGHTBRACKET;
        if(dlcs_strcasecmp(szInput,"enter"))
            ikey = SDLK_RETURN;
        if(dlcs_strcasecmp(szInput,"l_control"))
            ikey = SDLK_LCTRL;
        if(dlcs_strcasecmp(szInput,"a"))
            ikey = SDLK_a;
        if(dlcs_strcasecmp(szInput,"s"))
            ikey = SDLK_s;
        if(dlcs_strcasecmp(szInput,"d"))
            ikey = SDLK_d;
        if(dlcs_strcasecmp(szInput,"f"))
            ikey = SDLK_f;
        if(dlcs_strcasecmp(szInput,"g"))
            ikey = SDLK_g;
        if(dlcs_strcasecmp(szInput,"h"))
            ikey = SDLK_h;
        if(dlcs_strcasecmp(szInput,"j"))
            ikey = SDLK_j;
        if(dlcs_strcasecmp(szInput,"k"))
            ikey = SDLK_k;
        if(dlcs_strcasecmp(szInput,"l"))
            ikey = SDLK_l;
        if(dlcs_strcasecmp(szInput,";"))
            ikey = SDLK_SEMICOLON;
        if(dlcs_strcasecmp(szInput,"'"))
            ikey = SDLK_QUOTE;
        if(dlcs_strcasecmp(szInput,"`"))
            ikey = SDLK_BACKQUOTE;
        if(dlcs_strcasecmp(szInput,"l_shift"))
            ikey = SDLK_LSHIFT;
        if(dlcs_strcasecmp(szInput,"\\"))
            ikey = SDLK_BACKSLASH;
        if(dlcs_strcasecmp(szInput,"z"))
            ikey = SDLK_z;
        if(dlcs_strcasecmp(szInput,"x"))
            ikey = SDLK_x;
        if(dlcs_strcasecmp(szInput,"c"))
            ikey = SDLK_c;
        if(dlcs_strcasecmp(szInput,"v"))
            ikey = SDLK_v;
        if(dlcs_strcasecmp(szInput,"b"))
            ikey = SDLK_b;
        if(dlcs_strcasecmp(szInput,"n"))
            ikey = SDLK_n;
        if(dlcs_strcasecmp(szInput,"m"))
            ikey = SDLK_m;
        if(dlcs_strcasecmp(szInput,","))
            ikey = SDLK_COMMA;
        if(dlcs_strcasecmp(szInput,"."))
            ikey = SDLK_PERIOD;
        if(dlcs_strcasecmp(szInput,"/"))
            ikey = SDLK_SLASH;
        if(dlcs_strcasecmp(szInput,"r_shift"))
            ikey = SDLK_RSHIFT;
        if(dlcs_strcasecmp(szInput,"*"))
            ikey = SDLK_KP_MULTIPLY;
        if(dlcs_strcasecmp(szInput,"l_alt"))
            ikey = SDLK_LALT;
        if(dlcs_strcasecmp(szInput,"space"))
            ikey = SDLK_SPACE;
        if(dlcs_strcasecmp(szInput,"capslock"))
            ikey = SDLK_CAPSLOCK;
        if(dlcs_strcasecmp(szInput,"f1"))
            ikey = SDLK_F1;
        if(dlcs_strcasecmp(szInput,"f2"))
            ikey = SDLK_F2;
        if(dlcs_strcasecmp(szInput,"f3"))
            ikey = SDLK_F3;
        if(dlcs_strcasecmp(szInput,"f4"))
            ikey = SDLK_F4;
        if(dlcs_strcasecmp(szInput,"f5"))
            ikey = SDLK_F5;
        if(dlcs_strcasecmp(szInput,"f6"))
            ikey = SDLK_F6;
        if(dlcs_strcasecmp(szInput,"f7"))
            ikey = SDLK_F7;
        if(dlcs_strcasecmp(szInput,"f8"))
            ikey = SDLK_F8;
        if(dlcs_strcasecmp(szInput,"f9"))
            ikey = SDLK_F9;
        if(dlcs_strcasecmp(szInput,"f10"))
            ikey = SDLK_F10;
        if(dlcs_strcasecmp(szInput,"numlock"))
            ikey = SDLK_NUMLOCK;
        if(dlcs_strcasecmp(szInput,"scroll"))
            ikey = SDLK_SCROLLOCK;
        if(dlcs_strcasecmp(szInput,"numpad_7"))
            ikey = SDLK_KP7;
        if(dlcs_strcasecmp(szInput,"numpad_8"))
            ikey = SDLK_KP8;
        if(dlcs_strcasecmp(szInput,"numpad_9"))
            ikey = SDLK_KP9;
        if(dlcs_strcasecmp(szInput,"-"))
            ikey = SDLK_KP_MINUS;
        if(dlcs_strcasecmp(szInput,"numpad_4"))
            ikey = SDLK_KP4;
        if(dlcs_strcasecmp(szInput,"numpad_5"))
            ikey = SDLK_KP5;
        if(dlcs_strcasecmp(szInput,"numpad_6"))
            ikey = SDLK_KP6;
        if(dlcs_strcasecmp(szInput,"+"))
            ikey = SDLK_KP_PLUS;
        if(dlcs_strcasecmp(szInput,"numpad_1"))
            ikey = SDLK_KP1;
        if(dlcs_strcasecmp(szInput,"numpad_2"))
            ikey = SDLK_KP2;
        if(dlcs_strcasecmp(szInput,"numpad_3"))
            ikey = SDLK_KP3;
        if(dlcs_strcasecmp(szInput,"numpad_0"))
            ikey = SDLK_KP0;
        if(dlcs_strcasecmp(szInput,"numpad_decimal"))
            ikey = SDLK_KP_PERIOD;
        //if(dlcs_strcasecmp(szInput,"oem_102"))
            //ikey = SDLK_OEM_102;
        if(dlcs_strcasecmp(szInput,"f11"))
            ikey = SDLK_F11;
        if(dlcs_strcasecmp(szInput,"f12"))
            ikey = SDLK_F12;
        if(dlcs_strcasecmp(szInput,"f13"))
            ikey = SDLK_F13;
        if(dlcs_strcasecmp(szInput,"f14"))
            ikey = SDLK_F14;
        if(dlcs_strcasecmp(szInput,"f15"))
            ikey = SDLK_F15;
//        if(dlcs_strcasecmp(szInput,"kana"))
//            ikey = SDLK_KANA;
//        if(dlcs_strcasecmp(szInput,"abnt_c1"))
//            ikey = SDLK_ABNT_C1; // ? on Portugese (Brazilian) keyboards
        ///if(dlcs_strcasecmp(szInput,"convert"))
            //ikey = SDLK_CONVERT;
        //if(dlcs_strcasecmp(szInput,"noconvert"))
            //ikey = SDLK_NOCONVERT;
        //if(dlcs_strcasecmp(szInput,"yen"))
            //ikey = SDLK_YEN;
        //if(dlcs_strcasecmp(szInput,"abnt_c2"))
            //ikey = SDLK_ABNT_C2; // Numpad . on Portugese (Brazilian) keyboards
        if(dlcs_strcasecmp(szInput,"numpad_equals"))
            ikey = SDLK_KP_EQUALS;
//        if(dlcs_strcasecmp(szInput,"circumflex"))
//            ikey = SDLK_CIRCUMFLEX;
        if(dlcs_strcasecmp(szInput,"at"))
            ikey = SDLK_AT;
        if(dlcs_strcasecmp(szInput,"colon"))
            ikey = SDLK_COLON;
        if(dlcs_strcasecmp(szInput,"underline"))
            ikey = SDLK_UNDERSCORE;
        //if(dlcs_strcasecmp(szInput,"kanji"))
            //ikey = SDLK_KANJI;
        //if(dlcs_strcasecmp(szInput,"stop"))
            //ikey = SDLK_STOP;
        //if(dlcs_strcasecmp(szInput,"ax"))
            //ikey = SDLK_AX;
        //if(dlcs_strcasecmp(szInput,"unlabeled"))
            //ikey = SDLK_UNLABELED;
        //if(dlcs_strcasecmp(szInput,"nexttrack"))
            //ikey = SDLK_NEXTTRACK; // Next Track Key?
        if(dlcs_strcasecmp(szInput,"numpad_enter"))
            ikey = SDLK_KP_ENTER;
        if(dlcs_strcasecmp(szInput,"r_control"))
            ikey = SDLK_RCTRL;
        //if(dlcs_strcasecmp(szInput,"mute"))
            //ikey = SDLK_MUTE; // Mute Key?
        //if(dlcs_strcasecmp(szInput,"calculator"))
            //ikey = SDLK_CALCULATOR; // Calculator Key?
        //if(dlcs_strcasecmp(szInput,"playpause"))
            //ikey = SDLK_PLAYPAUSE; // Play / Pause Key?
        //if(dlcs_strcasecmp(szInput,"mediastop"))
            //ikey = SDLK_MEDIASTOP; // Media Stop Key?
        //if(dlcs_strcasecmp(szInput,"volumedown"))
            //ikey = SDLK_VOLUMEDOWN; // Volume - Key?
        //if(dlcs_strcasecmp(szInput,"volumeup"))
            //ikey = SDLK_VOLUMEUP; // Volume + Key?
        //if(dlcs_strcasecmp(szInput,"webhome"))
            //ikey = SDLK_WEBHOME; // Web home Key?
        //if(dlcs_strcasecmp(szInput,"numpad_comma"))
          //  ikey = SDLK_NUMPADCOMMA;
        if(dlcs_strcasecmp(szInput,"/"))
            ikey = SDLK_KP_DIVIDE;
        //if(dlcs_strcasecmp(szInput,"sysrq"))
            //ikey = SDLK_SYSRQ;
        if(dlcs_strcasecmp(szInput,"r_alt"))
            ikey = SDLK_RALT;
        if(dlcs_strcasecmp(szInput,"pause"))
            ikey = SDLK_PAUSE;
        if(dlcs_strcasecmp(szInput,"home"))
            ikey = SDLK_HOME;
        if(dlcs_strcasecmp(szInput,"up"))
            ikey = SDLK_UP;
        if(dlcs_strcasecmp(szInput,"pageup"))
            ikey = SDLK_PAGEUP;
        if(dlcs_strcasecmp(szInput,"left"))
            ikey = SDLK_LEFT;
        if(dlcs_strcasecmp(szInput,"right"))
            ikey = SDLK_RIGHT;
        if(dlcs_strcasecmp(szInput,"end"))
            ikey = SDLK_END;
        if(dlcs_strcasecmp(szInput,"down"))
            ikey = SDLK_DOWN;
        if(dlcs_strcasecmp(szInput,"pagedown"))
            ikey = SDLK_PAGEDOWN;
        if(dlcs_strcasecmp(szInput,"insert"))
            ikey = SDLK_INSERT;
        if(dlcs_strcasecmp(szInput,"delete"))
            ikey = SDLK_DELETE;
        if(dlcs_strcasecmp(szInput,"l_windows"))
            ikey = SDLK_LSUPER;
        if(dlcs_strcasecmp(szInput,"r_windows"))
            ikey = SDLK_RSUPER;
        //if(dlcs_strcasecmp(szInput,"start"))
            //ikey = SDLK_APPS;
        if(dlcs_strcasecmp(szInput,"power"))
            ikey = SDLK_POWER;
        //if(dlcs_strcasecmp(szInput,"sleep"))
          //  ikey = SDLK_SLEEP;
        //if(dlcs_strcasecmp(szInput,"wake"))
            //ikey = SDLK_WAKE;
        //if(dlcs_strcasecmp(szInput,"websearch"))
            //ikey = SDLK_WEBSEARCH;
        //if(dlcs_strcasecmp(szInput,"webfavorites"))
            //ikey = SDLK_WEBFAVORITES;
        //if(dlcs_strcasecmp(szInput,"webrefresh"))
            //ikey = SDLK_WEBREFRESH;
        //if(dlcs_strcasecmp(szInput,"webstop"))
            //ikey = SDLK_WEBSTOP;
        //if(dlcs_strcasecmp(szInput,"webforward"))
            //ikey = SDLK_WEBFORWARD;
        //if(dlcs_strcasecmp(szInput,"webback"))
          //  ikey = SDLK_WEBBACK;
        //if(dlcs_strcasecmp(szInput,"mycomputer"))
            //ikey = SDLK_MYCOMPUTER;
        //if(dlcs_strcasecmp(szInput,"mail"))
          //  ikey = SDLK_MAIL;
        //if(dlcs_strcasecmp(szInput,"mediaselect"))
            //ikey = SDLK_MEDIASELECT;
        if((ikey>0)&&(ikey<SDLK_LAST))
        {
            memset(pGUI->Key[ikey].Command,0,256);
            if(bEcho) //bEcho indicates a manual change to the binds. Save the binds if bEcho!
            {
                //sprintf(szTemp1,"%s%c%s",pClientData->FMDir,PATH_SEP,"config.cfg");
                pGUI->bSaveBinds("config.cfg");
            }
            return;
        }
        else
        {
            pCon->ConsoleBufferAdd("unbind : Invalid key - No unbind was issued.");
            pCon->ConsoleBufferAdd("       : unbind <key> | unbind all");
            return;
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // SAVEBINDS
    // Saves the binded keys to specified file

    if(dlcs_strcasecmp(szInput,"savebinds"))
    {
        if(bEcho) pCon->ConsoleBufferAdd(szCommand);

        szInput=strtok(NULL," \n\r");
        if(szInput)
        {
            pGUI->bSaveBinds(szInput);
        }
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // STOPAUDIO
    // stop audio

    if(dlcs_strcasecmp(szInput,"stopaudio"))
    {
        if(bEcho)// Log("stopaudio");
        StopAudio();
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // PLAY
    // Play a specified sound file

    if(dlcs_strcasecmp(szInput,"play"))
    {
        szInput=strtok(NULL,"\n\r");
        if(szInput)
        {
            strcpy(szTemp1,szInput);

            if(szInput)
            {
                abx=szInput[strlen(szInput)-3];
                aby=szInput[strlen(szInput)-2];
                abz=szInput[strlen(szInput)-1];

                if( ( (abx=='w') &&
                      (aby=='a') &&
                      (abz=='v') ) ||

                    ( (abx=='m') &&
                      (aby=='p') &&
                      (abz=='3') ) )
                {
                    if(!pClientData->bSound)
                    {
                        if(bEcho)
                           // Log("Sound is turned off!");
                        return;
                    }
                    if(!PlaySample(szInput))
                        //if(!PlaySample(va("media%c%s%csnd%c%s",PATH_SEP,pClientData->ServerID,PATH_SEP,PATH_SEP,szInput)))
                            PlaySample(va("snd%c%s",PATH_SEP,szInput));
                    return;
                }

                if( ( (abx=='m') &&
                      (aby=='o') &&
                      (abz=='d') ) ||

                    ( (abx=='.') &&
                      (aby=='x') &&
                      (abz=='m') ) ||

                    ( (abx=='s') &&
                      (aby=='3') &&
                      (abz=='m') ) ||

                    ( (abx=='.') &&
                      (aby=='i') &&
                      (abz=='t') ) ||

                    ( (abx=='m') &&
                      (aby=='i') &&
                      (abz=='d') ) )
                {
                    if(!pClientData->bMusic)
                    {
                        if(bEcho)
                           // Log("Music is turned off!");
                        return;
                    }
                    if(!PlayMusic(szInput))
                        //if(!PlayMusic(va("%s%cmedia%c%s%csnd%c%s",pClientData->FMDir,PATH_SEP,PATH_SEP,pClientData->ServerID,PATH_SEP,PATH_SEP,szInput)))
                            PlayMusic(va("snd%c%s",PATH_SEP,szInput));
                    return;
                }
            }

        }

        pCon->ConsoleBufferAdd("Usage : play <filename.(wav|mp3|mod|xm|it|s3m|mid)>");
        pCon->ConsoleBufferAdd("        To stop a playing file, use stopaudio command.");
        return;
    }

    if(dlcs_strcasecmp(szInput,"gafrebuild"))
    {

        pCab->Close();

       // Log("Rebuilding ember.gaf...\n");
        remove("ember.gaf");

        pCab->Open("ember.gaf");

       // Log("Adding base...\n");
        pCab->AddDir("base");

       // Log("Adding buttons...\n");
        pCab->AddDir("buttons");

       // Log("Adding fonts...\n");
        pCab->AddDir("fonts");

       // Log("Adding gumps...\n");
        pCab->AddDir("gumps");

       // Log("Adding mouse...\n");
        pCab->AddDir("mouse");

       // Log("Adding snd...\n");
        pCab->AddDir("snd");

        pCab->Close();

        pCab->Open("ember.gaf");
        return;
    }

    /////////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // guibstore
    //

    if(dlcs_strcasecmp(szInput,"guibstore"))
    {
        pGUI->bstore(pGUI->first_gui_stump->name);
        return;
    }


    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
	// CALLGUI

	if(dlcs_strcasecmp(szInput,"guicall"))
	{
		szInput=strtok(NULL,"\n\r");
		if(szInput)
		{
			// DLog("GUI CALL -> [%s]",szInput);
			pGUI->call(szInput);
		}
		return;
	}

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
	// GUICLEAR

	if(dlcs_strcasecmp(szInput,"guiclear"))
	{
        // DLog("GUI CLEARED");
        pGUI->clear();
		return;
	}

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
	// GUIGO

	if(dlcs_strcasecmp(szInput,"guigo"))
	{
		szInput=strtok(NULL,"\n\r");
		if(szInput)
		{
			// DLog(" ProcessConsoleCommand() - GUIGO -> [%s]",szInput);

			pGUI->parse(szInput);
		}
		return;
	}


    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
	// Set game mode

    if( (dlcs_strcasecmp(szInput,"setgamemode")) ||
        (dlcs_strcasecmp(szInput,"sgm")) )
    {

		szInput=strtok(NULL," \n\r");
		if(szInput)
		{
			if(dlcs_strcasecmp(szInput,"back"))
			{
				SetGameMode(LAST_GAME_MODE);
				return;
			}

			if(dlcs_strcasecmp(szInput,"edit"))
			{
			    SetGameMode(EDIT_WORLD_INIT);
			    return;
			}

			if(dlcs_strcasecmp(szInput,"main"))
			{
				ProcessConsoleCommand("disconnect",0);

				SetGameMode(MAIN_MENU);
				return;
			}

			if(dlcs_strcasecmp(szInput,"login"))
			{
			    strcpy(pClientData->Name,pGUI->getdata("username"));
			    strcpy(pClientData->Password,pGUI->getdata("password"));

				Log("setgamemode// Login %s %s",pClientData->Name,pClientData->Password);

				SetGameMode(CONNECT);
				return;
			}

			if(dlcs_strcasecmp(szInput,"create_character"))
			{
				SetGameMode(CREATE_CHARACTER);
				return;
			}

			if(dlcs_strcasecmp(szInput,"choose_character"))
			{
				SetGameMode(GET_CHARACTERS);
				return;
			}

			if(dlcs_strcasecmp(szInput,"gather_game_data"))
			{
				SetGameMode(GATHER_GAME_DATA);
				return;

			}

			if(dlcs_strcasecmp(szInput,"options"))
			{
				if(pClientData->drawoptions)
					pClientData->drawoptions=false;
				else
					pClientData->drawoptions=true;
				return;
			}

			if(dlcs_strcasecmp(szInput,"quit"))
			{
				ProcessConsoleCommand("quit",0);
				return;
			}

			if(dlcs_strcasecmp(szInput,"connect"))
			{

			}
		}
        return;
    }

    /////////////////////////////////
    // CC! (Console command)
    // addfavorite

    if(dlcs_strcasecmp(szInput,"addfavorite"))
    {
        ServerData *pD=new ServerData;
        szInput=strtok(NULL,",\n\r");
        if(szInput)
        {
            strcpy(pD->name,szInput);
            szInput=strtok(NULL,",\n\r");
            if(szInput)
            {
                strcpy(pD->ip_address,szInput);
                szInput=strtok(NULL,",\n\r");
                if(szInput)
                {
                    strcpy(pD->port,szInput);

                    pClientData->AddFavoriteServer(pD);
                    pClientData->LoadFavoriteServers();
                }
            }
        }

        DEL(pD);
        return;
    }

    /////////////////////////////////
    // CC! (Console command)
    // modfavorite

    if(dlcs_strcasecmp(szInput,"modfavorite"))
    {
        ServerData *pD=new ServerData;
        szInput=strtok(NULL,",\n\r");
        if(szInput)
        {
            ax=atoi(szInput);
            szInput=strtok(NULL,",\n\r");
            {
                strcpy(pD->name,szInput);
                szInput=strtok(NULL,",\n\r");
                if(szInput)
                {
                    strcpy(pD->ip_address,szInput);
                    szInput=strtok(NULL,",\n\r");
                    if(szInput)
                    {
                        strcpy(pD->port,szInput);

                        pClientData->DiscardFavoriteServer(ax);
                        pClientData->AddFavoriteServer(pD);
                        pClientData->LoadFavoriteServers();
                    }
                }
            }
        }

        DEL(pD);
        return;
    }

    /////////////////////////////////
    // CC! (Console command)
    // delfavorite

    if(dlcs_strcasecmp(szInput,"delfavorite"))
    {
        ServerData *pD=new ServerData;
        szInput=strtok(NULL,",\n\r");
        if(szInput)
        {
            ax=atoi(szInput);
            szInput=strtok(NULL,",\n\r");
            {
                strcpy(pD->name,szInput);
                szInput=strtok(NULL,",\n\r");
                if(szInput)
                {
                    strcpy(pD->ip_address,szInput);
                    szInput=strtok(NULL,",\n\r");
                    if(szInput)
                    {
                        strcpy(pD->port,szInput);

                        pClientData->DiscardFavoriteServer(ax);
                        pClientData->LoadFavoriteServers();
                    }
                }
            }
        }

        DEL(pD);
        return;
    }

    // pClientData->AddFavoriteServer(ServerData *pData)


    /////////////////////////////////
    // CC! (Console command)
    // !0

    if(dlcs_strcasecmp(szInput,"!0"))
    {
        pCon->ConsoleBufferAdd(" ");
        pCon->ConsoleBufferAdd("!0 by Gorgon on 28 Aug 99 @ 12:04 EST");
        pCon->ConsoleBufferAdd(" ");
        return;
    }

    /////////////////////////////////
    // CC! (Console command)
    // !1

    if(dlcs_strcasecmp(szInput,"!1"))
    {
        pCon->ConsoleBufferAdd(" ");
        pCon->ConsoleBufferAdd("!1 by Sir Bungalot on 11 Oct 99 @ 03:45 EST");
        pCon->ConsoleBufferAdd(" ");
        return;
    }

    /////////////////////////////////
    // CC! (Console command)
    // god

    if(dlcs_strcasecmp(szInput,"god"))
    {
        pCon->ConsoleBufferAdd(" ");
        pCon->ConsoleBufferAdd("Um... Nope.");
        pCon->ConsoleBufferAdd(" ");
        return;
    }

    /////////////////////////////////
    // CC! (Console command)
    // dog

    if(dlcs_strcasecmp(szInput,"dog"))
    {
        pCon->ConsoleBufferAdd(" ");
        pCon->ConsoleBufferAdd("Ok, your a dog.");
        pCon->ConsoleBufferAdd(" ");
        return;
    }

    /////////////////////////////////
    // CC! (Console command)
    // cheat

    if(dlcs_strcasecmp(szInput,"cheat"))
    {
        pCon->ConsoleBufferAdd(" ");
        pCon->ConsoleBufferAdd("No cheating allowed");
        pCon->ConsoleBufferAdd(" ");
        return;
    }

    if(bEcho)
    {
        pCon->ConsoleBufferAdd(szCommand);
        pCon->ConsoleBufferAdd(" Usage : type in a legal command");
    }

    return;

} // ProcessConsoleCommand


*/





/*
    static float mcx;
    static float mcy;
    static bool  mclmbd;
    static bool  mcrmbd;

    bool what;

    if(camera)
    {
        if(GetMouseWheelUp())
        {
            camera->tz++;

        }

        if(GetMouseWheelDown())
        {
            camera->tz--;

        }

        // camera->rx=-90;
        // camera->z=9;

        //mcx=0;
        //mcy=0;

        if(mcrmbd)
        {
            if(mcx==0) mcx=GetMouseX();
            if(mcy==0) mcy=GetMouseY();

            what=0;


            if((mcy<GetMouseY()) && (what==0))
            {
                what=1;
                camera->rx+=2;

            }

            if((mcy>GetMouseY()) && (what==0))
            {
                what=1;
                camera->rx-=2;
            }

            if((mcx>GetMouseX()) && (what==0))
            {
                what=1;
                camera->ry-=2;
            }

            if((mcx<GetMouseX()) && (what==0))
            {
                what=1;
                camera->ry+=2;
            }
            //if(what) SDL_WarpMouse(mcx,mcy);
        }
        else
        {
            mcx=0;
            mcy=0;
        }


        mcrmbd=GetMouseDown(SDL_BUTTON_RIGHT);
        mclmbd=GetMouseDown(SDL_BUTTON_LEFT);

        if(camera->rx>0)
            camera->rx=0;

        if(camera->rx<-60)
            camera->rx=-60;

        if((camera->tz) > (-1.0f))
            camera->tz=-1.0f;
        if((camera->tz) < (-40.0f))
            camera->tz=-40.0f;
    }
    */

    ////////////////////////////////////////////////////////
    /*    // Set local scope vars

    bool    bLoader;
    bool    bMoveInSync;
    char    szTemp[1024];
    char    szTemp1[1024];
    int     i;
    int     z;

    CPacket *pFMMSRec;

    bLoader=false;
    bMoveInSync=true;
    memset(szTemp,0,1024);
    pFMMSRec=NULL;
    */


    ////////////////////////////////////////////////////////////////////////////////
    // Set mouse cursor

    //    MOUSEMODE=MP_MENU; // Turn mouse into normal pointer for menus and such
    // if(bTargeting) MOUSEMODE=MP_TARGET;	// Turn mouse cursor into targetting cursor (Crosshairs)

    ////////////////////////////////////////////////////////////////////////////////





