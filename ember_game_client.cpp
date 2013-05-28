/*
 ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
 º                                                               º
 º        .      °°°°°° °°   °° °°°°°  °°°°°° °°°°°              º
 º               °°     °°° °°± °°  °° °°     °° .°°        .    º
 º               ±±±±   ±°°°°±± ±±±±±  ±±±±   °°  ±° .           º
 º    .          ²±    .±± ± ±² ±±  ±± ²±    .±±²²±              º
 º              .²²     ²±   ²² ²² .²² ²²     ²²  ²²  .          º
 º               Û²ÛÛ²Û Û²  .²Û ²ÛÛ²²  Û²ÛÛ²Û Û²  Û²             º
 º                                                               º
 º                                                             . º
 º                    º             .        º             )     º
 º       .            ººº                  ººº                   º
 º                     ºººº              ºººº         .          º
 º                      ºººººº        ºººººº                 .   º
 º                      ººººººººº  ººººººººº                     º
 º              .       ºººººººººººººººººººº                     º
 º                      ººººº ºººººººº ºººººº         .          º
 º  .    .             ºººº    ºººººº    ºººº                    º
 º                     ººº   .  ºººº  .   ººº                    º
 º                     ººº   *;  ºº   *;  ººº   .             .  º
 º             *        ººº    ºººººº    ººº                     º
 º                      ººººººººº  ººººººººº                     º
 º                        ºººººº ºº ºººººº                       º
 º   .                    ºººººººººººººººº                       º
 º                        º ºººººººººººº º              .        º
 º        o          .     ºº ºººººººº ºº             .          º
 º                         ºº ºº ºº ºº ºº               .        º
 º             .            ºººº ºº ºººº                         º
 º   .                       ºººººººººº        .                 º
 º                             ºººººº                       .    º
 º                                                               º
 º           ABANDON ALL HOPE YE WHO LOOKS BELOW HERE!           º
 º                                                               º
 ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
                         Ember Client Program                  */

#define _EMBER_C

#include "ember_game_client.h"
#include "c_data.h"
#include "c_snd.h"
#include "c_gfx.h"
#include "c_gui.h"
#include "c_log.h"
#include "lua.h"

#define VERSION     "11.09.30"
#define COPYRIGHT   "(C) 2011"

bool        bShutDown;
CC_Data     *pClientData;
C_GFX       *pGFX;      // SDL / OpenGL
C_FM_GUI    *pGUI;      // OpenGL GUI
CGAF        *pGAF;      // Game Archive File (GAF)
CLog        *pLog;      // Log file
C_Sound     *pSND;      // FMOD Sounds

// C_FMGS      *pFMGS;     // Game Server connection
lua_State   *L;

/*****************************************************************
 ** User members of classes                                     **
 *****************************************************************/

void setgamemode(MODE m)
{
    pClientData->GAME_MODE=m;
}

void C_EM_CONSOLE::_Execute(char *cmd)
{

    int i;

    C_FM_GUI_CTRL_DATA *pGD;

    pLog->_Add("CONSOLE[%s]",cmd);

    vector <string> ncmd=explode(";",cmd);


    if(ncmd.size()>1)
    {
        for(i=0;i<ncmd.size();i++)
        {
            pGUI->_ConsoleExecute((char *)ncmd[i].c_str());
        }
        return;
    }

    vector <string> narg=explode(" ",cmd);
    if(narg.size()>0)
    {

        /****************************************************/

        if(dscc((char *)narg[0].c_str(),"quit"))
        {
            bShutDown=true;
            return;
        }

        /****************************************************/

        if(dscc((char *)narg[0].c_str(),"setgamemode"))
        {
            if(narg.size()>1)
            {
                if(dscc((char *)narg[1].c_str(),"edit"))
                {
                    pClientData->GAME_MODE=EDIT_WORLD_INIT;
                }

                if(dscc((char *)narg[1].c_str(),"main"))
                {
                    pClientData->GAME_MODE=MAIN_MENU;

                }
            }
        }

        /****************************************************/

        if(dscc((char *)narg[0].c_str(),"guigo"))
        {
            pGUI->parse(cmd);

            pGD=pGUI->pFirstGUIData;
            if(pGD)
            {
                if(dscc(pGD->name,"action"))
                {
                    if(dscc(pGD->value,"setgamemode"))
                    {
                        pGD=pGD->next;
                        if(pGD)
                        {
                            _Execute(va("setgamemode %s",pGD->name));
                            return;
                        }
                    }
                }
            }
        }

        /****************************************************/

        if(dscc((char *)narg[0].c_str(),"guicall"))
        {
            pGUI->call((char *)narg[1].c_str());
        }

        /****************************************************/
    }
}

/*****************************************************************/

void CLog::_Add(char *fmt, ...)
{
    char ach[1024];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
	if(ach[strlen(ach)-1]!='\n')
    strcat(ach,"\n");
    AddEntry(ach);
    if(pGUI)
        pGUI->ConsoleEntry(ach);
}

/*****************************************************************
 ** SDL main                                                    **
 *****************************************************************/

int main(int argc, char *argv[])
{
    bShutDown=0;
    if(doInit())
        while(!bShutDown)
            MainGameLoop();
    ShutDown();
    return 0;
}

/*****************************************************************
 **  Main Game Loop                                             **
 *****************************************************************/

void MainGameLoop(void)
{

    int key=pGUI->ProcessKeyboard();


    if(key == SDLK_F12) bShutDown=true;
    if(bShutDown) return;

    if(key == SDLK_BACKQUOTE)
    {
        if(pGUI)
        {
            if(pGUI->pConsole)
            {
                if(pGUI->get_stump("console.gui"))
                    pGUI->del_stump("console.gui");
                else
                    pGUI->pConsole->_Execute("guicall console.gui");
            }
        }
    }



    if(!pClientData) return;

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

    pGFX->BeginScene();

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

    // DoNetwork();    // Get any network messages

    pGFX->GetFade(0);     // Update faders

    ////////////////////////////////////////////////////////////////////////////////
    // Do stuff for current game mode

    switch(pClientData->GAME_MODE)
    {
        /////////////////////////////
        //
        // °°°°°  °°°°°° °°°°°°°° °°°°°   °°°°     °° °°  °° °°°°°°°° °°°°°   °°°°
        // °° .°° °°        °°  . °° .°° °°  °°    °° °°° °°    °°  . °° .°° °°  °°
        // °°  ±° °±±±      °°    °°  ±° °±  ±±    °° °°°±±±    °°    °°  ±° °±  ±±
        // ±±²²±  ±±        ±±    ±±²²±  ±±  ²±    ±± ±± ²²±    ±±    ±±²²±  ±±  ²±
        // ²²  ²² ²²        ²²    ²²  ²² ²²  ²²    ²² ±² .²²    ²²    ²²  ²² ²²  ²²
        // Û²  Û² ²ÛÛ²²Û    Û²    Û²  Û²  ÛÛÛÛ     Û² ²Û  Û²    Û²    Û²  Û²  ÛÛÛÛ
        //
        /////////////////////////////

    case RETRO_INTRO_INIT:

        pClientData->GAME_MODE=RETRO_INTRO_PLAY;
        break;

    case RETRO_INTRO_PLAY:

        pClientData->GAME_MODE=MAIN_MENU;
        break;

        ///////////////////////////////////////////////////////////////////////////////////////
        //
        // °°   °°  °°°°. °° °°  °°     °°   °° °°°°°° °°  °° °°  °°
        // °°° °°± °°  °° °° °°° °°     °°° °°± °°     °°° °° °°  °°
        // ±°°°°±± °°°±±± °° °°°±±±     ±°°°°±± °±°°   °°°±±± °°  ±±
        // ±± ± ±² ±±  ²± ±± ±± ²²±     ±± ± ±² ±±   . ±± ²²± ±±  ²²
        // ²±   ²² ±²  ²² ±² ±² .²²     ²±   ²² ²²     ±² .²² ±² .²Û
        // Û²  .²Û ²Û .Û² ²Û ²Û  Û²     Û²  .²Û Û²²²Û² ²Û  Û²  ÛÛÛÛ
        //
        ///////////////////////////////////////////////////////////////////////////////////////

    case MAIN_MENU:

        // if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false)) INPUTMODE=NORMAL;

        pClientData->LAST_GAME_MODE=MAIN_MENU;

        pGUI->clear();
        pGUI->call("main.gui");

        pGUI->setdata("username",pClientData->Name);
        if(pClientData->bSavePassword)
            pGUI->setdata("password",pClientData->Password);
        pGUI->setdata("savepassword",va("%d",pClientData->bSavePassword));

        pClientData->GAME_MODE=MAIN_MENU_2;

    case MAIN_MENU_2:

        pGUI->WriteText(15,pClientData->ScreenHeight-16,va("^+^1(^5%s^1)^5 %s",CPUSTRING,VERSION),0);
        // DrawOSIcon(5,pClientData->ScreenHeight-16,CPUSTRING);

        break; // End MAIN_MENU

        /////////////////////////////
        //
        //  °°°°  °°°°°  °°°°°° °°°°°  °° °°°°°°°°  °°°°°
        // °°  °° °° .°° °°     °°  °° °°    °°  . °° .
        // °±     °°  ±° °±±±   °±  ±± °°    °°     °°°°
        // ±±     ±±²²±  ±±     ±±  ²± ±±    ±±        ²±
        // ²²  Û² ²²  ²² ²²     ²²  ²² ²²    ²²        ²²
        //  ÛÛÛÛ  Û²  Û² ²ÛÛ²²Û ÛÛÛÛÛ  Û²    Û²    ÛÛÛÛ²
        //
        /////////////////////////////

    case CREDITS:

        pClientData->GAME_MODE=MAIN_MENU;
        break;
        /*

        if(!dlcs_strcasecmp(va("%s%cmedia%cstandard%cgfx%ccredits.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP),pClientData->szGeneric))
        {
            LoadGeneric(va("%s%cmedia%cstandard%cgfx%ccredits.bmp",pClientData->FMDir,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP));
            ProcessConsoleCommand("stopaudio",0);
            ProcessConsoleCommand(va("play %s%cmedia%cstandard%csnd%ccredits.xm",pClientData->FMDir,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP),0);
            // initialize the scroll here
            fCredscroll=400;
            INPUTMODE=NORMAL;
            pClientData->LAST_GAME_MODE=MAIN_MENU;
        }

        if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false))
        {
            DrawGenericSurface();
            DrawBit4ge(700,10,764,74,1);

            WriteText(240,272+(int)fCredscroll,"^+^1Seth Parson.................Creator/Programming",5);
            WriteText(240,292+(int)fCredscroll,"^+^1Stife.......................Programming",5);
            WriteText(240,312+(int)fCredscroll,"^+^1Erik........................Website",5);
            WriteText(240,332+(int)fCredscroll,"^+^1Melfis......................Graphics",5);
            WriteText(240,352+(int)fCredscroll,"^+^1Dennis Paulsen..............Programming",5);
            WriteText(240,372+(int)fCredscroll,"^+^1Jubie.......................Graphics",5);
            WriteText(240,392+(int)fCredscroll,"^+^1Odin Jensen.................FMUE/DX Help",5);
            WriteText(240,412+(int)fCredscroll,"^+^1Lasse S.....................DX Help",5);
            WriteText(240,432+(int)fCredscroll,"^+^1Chris Bromley...............Test Server",5);
            WriteText(240,452+(int)fCredscroll,"^+^1Sphinx......................Graphics",5);
            WriteText(240,472+(int)fCredscroll,"^+^1Herbert Wolverson...........D3D 2D Iso tutorial",5);
            WriteText(240,492+(int)fCredscroll,"^+^1Nick Taylor (Lympex)........Graphics",5);
            WriteText(240,512+(int)fCredscroll,"^+^1Stephen D. Baynham(Slinky)..FragMENTAL Design",5);
            WriteText(240,532+(int)fCredscroll,"^+^1icmtb.......................FragMENTAL Design",5);
            WriteText(240,552+(int)fCredscroll,"^+^1Jonatan Borling (warhawk)...Music",5);
            WriteText(240,572+(int)fCredscroll,"^+       ^2(www.modarchive.com/artists/warhawk)",5);
            WriteText(240,592+(int)fCredscroll,"^+^1Stewart Leonard.............Programming",5);
            WriteText(200,632+(int)fCredscroll,"^+^3Special thanks to everyone who keeps up with the project",5);
            WriteText(240,652+(int)fCredscroll,"^+^eF^dr^ca^1gmere was created using the following",5);
            WriteText(240,672+(int)fCredscroll,"^+                   ^1software development tools:",5);
            WriteText(240,692+(int)fCredscroll,"^+^2OpenGL   ^1: ^2http^1://^2www^1.^2opengl^1.^2org^1/",5);
            WriteText(240,712+(int)fCredscroll,"^+^4 S^7D^8L     ^1: ^2http^1://^2www^1.^2libsdl^1.^2org^1/",5);
            WriteText(240,732+(int)fCredscroll,"^+^5  FMOD   ^1: ^2http^1://^2www^1.^2fmod^1.^2org^1/",5);
            WriteText(240,752+(int)fCredscroll,"^+^6   LUA   ^1: ^2http^1://^2www^1.^2lua^1.^2org^1/",5);


            if(dlcs_get_tickcount()-dwScrollTime>30)
            {
                dwScrollTime=dlcs_get_tickcount();
                fCredscroll-=1;
            }

            if(fCredscroll<-800)
                fCredscroll=400;


            WriteText(20,580,va("^+^1(^5%s^1)^5%s",CPUSTRING,VERSION),0);
            DrawOSIcon(10,580,CPUSTRING);
        }

        if(GetMouseRelease(SDL_BUTTON_LEFT))
        {
            sprintf(szTemp1,"play %s%cmedia%cstandard%csnd%cmain.xm",pClientData->FMDir,PATH_SEP,PATH_SEP,PATH_SEP,PATH_SEP);
            ProcessConsoleCommand(szTemp1,0);
            pClientData->GAME_MODE=MAIN_MENU;
        }

        break;
        */

        /////////////////////////////
        //
        // °°      °°°°   °°°°  °°°° °°°  °°
        // °°     °°  °° °°  °°  °°  °°°° °°
        // °±     ±±  °° ±°      °±  °± °±±±
        // ±±     ±±  ±± ²² ±±±  ±±  ±±. ±±±
        // ²²     Û²  ²² ²²  ²²  ²²  Û²   ±²
        // ÛÛÛÛÛ²  ²ÛÛÛ   ÛÛÛÛ  ÛÛ²Û Û²   ÛÛ
        //
        /////////////////////////////

    case LOGIN_AUTO_LOGIN:

        pLog->_Add("LOGIN_AUTO_LOGIN");
        pGUI->clear();
        pLog->_Add("execing [autologin.cfg]");

        // ProcessConsoleCommand("exec autologin.cfg",0);

        pClientData->GAME_MODE=LOGIN_SCREEN_ENTRY;
        break;

    case LOGIN_SCREEN_ENTRY:

        pLog->_Add("LOGIN_SCREEN_ENTRY");

        //SDL_WM_SetCaption(va("EGC %s(%s) Net Revision(%s) %s",VERSION,CPUSTRING,NET_REVISION,COPYRIGHT),"icon");

        if(pClientData)
        {
            pClientData->ServerListOffset=0;
//            pClientData->SelectedServer=MAX_SERVERS+1;
            pClientData->Mode=PLAY;
        }
//         DEL(pFMMS);
        //LOGINMODE=LM_NONE;
        pClientData->GAME_MODE=GATHER_SERVER_LIST;

    case GATHER_SERVER_LIST:

        pLog->_Add("GATHER_SERVER_LIST");

        //switch(LOGINMODE)
       // {
       // case LM_NONE:
       //     break;
       // case LM_INTERNET:
       //     DEL(pFMMS);
       ////     pFMMS=new C_FMMS;
       //     break;
       // case LM_FAVORITES:
       //     pClientData->LoadFavoriteServers();
       //     break;
       // case LM_LOCAL:
       //     break;
       // default:
       //     break;
       // }

        pClientData->LoadProfiles();
        pClientData->GAME_MODE=CHOOSE_SERVER;

    case CHOOSE_SERVER:

        //strcpy(szTemp1,"choose_server");
        //if(strcmp(szTemp1,pClientData->szGeneric))
        //{
            //strcpy(pClientData->szGeneric,szTemp1);
            //pLog->_Add("CHOOSE_SERVER");
            // INPUTMODE=NORMAL;
        //}

        pGUI->clear();
        pGUI->call("servers.gui");



        /*

        if(LOGINMODE==LM_FAVORITES)
        {
            if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false))
            {
                //WriteTextPct(.1875,.03,"^0Server",0);
                //WriteTextPct(.375,.03, "^0Address",0);
                ///WriteTextPct(.65,.03,  "^0Players",0);
                // WriteTextPct(.775,.03 ,"^0Ping",0);


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

                            //WriteText(140,(i-pClientData->ServerListOffset)*12+32,pClientData->FavoriteServer[i].user,0);
                            WriteText(150,(i-pClientData->ServerListOffset)*12+32,va("^&%s",pClientData->FavoriteServer->name),0);
                            WriteText(300,(i-pClientData->ServerListOffset)*12+32,va("^&%s:%s",pClientData->FavoriteServer->ip_address,pClientData->FavoriteServer->port),0);
                            // WriteText(440,(i-pClientData->ServerListOffset)*12+32,pClientData->FavoriteServer[i].resource_id,0);
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
                            WriteText(685,(i-pClientData->ServerListOffset)*12+32,szTemp,2);

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
                                WriteText(620,(i-pClientData->ServerListOffset)*12+32,szTemp,0);
                            }

                            sprintf(szTemp,"^&%d/%04d",atoi(pClientData->FavoriteServer->players),atoi(pClientData->FavoriteServer->maximum_players));
                            WriteText(520,(i-pClientData->ServerListOffset)*12+32,szTemp,0);
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
        			//WriteTextPct(.1875,.03,"^0Server",0);
        		//	WriteTextPct(.375,.03, "^0Address",0);
        		//	WriteTextPct(.65,.03,  "^0Players",0);
        		//	WriteTextPct(.775,.03 ,"^0Ping",0);

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

                                //WriteText(140,(i-pClientData->ServerListOffset)*12+32,pFMMS->pServerList[i].user,LONGRGB(255,0,0));
                                WriteText(150,(i-pClientData->ServerListOffset)*12+32,va("^&%s",pFMMS->pServerList[i].name),0);
                                WriteText(300,(i-pClientData->ServerListOffset)*12+32,va("^&%s:%s",pFMMS->pServerList[i].ip_address,pFMMS->pServerList[i].port),0);
                                // WriteText(440,(i-pClientData->ServerListOffset)*12+32,pFMMS->pServerList[i].resource_id,LONGRGB(255,0,0));
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
                                WriteText(685,(i-pClientData->ServerListOffset)*12+32,szTemp,2);

                                pFMMS->pServerList[i].dwPing=pFMMS->PingServer(i);

                                sprintf(szTemp,"^&^3%d",pFMMS->pServerList[i].dwPing);
                                if(pFMMS->pServerList[i].dwPing>200)
                                    sprintf(szTemp,"^&^5%d",pFMMS->pServerList[i].dwPing);
                                if(pFMMS->pServerList[i].dwPing>500)
                                    sprintf(szTemp,"^&^4%d",pFMMS->pServerList[i].dwPing);

                                WriteText(620,(i-pClientData->ServerListOffset)*12+32,szTemp,0);

                                sprintf(szTemp,"^&%d/%04d",atoi(pFMMS->pServerList[i].players),atoi(pFMMS->pServerList[i].maximum_players));
                                WriteText(520,(i-pClientData->ServerListOffset)*12+32,szTemp,0);
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
                WriteText(300,550,"^3Choose server search method",0);
                break;
            case LM_INTERNET:
                WriteText(300,550,"^4Internet Search",0);
                break;
            case LM_FAVORITES:
                WriteText(300,550,"^5Favorites",0);
                break;
            case LM_LOCAL:
                WriteText(300,550,"^6Local (LAN)",0);
                break;
            default:
                WriteText(300,550,"^1!ErRoR!",0);
                break;
        }



        //////////////////////////////////////////////////////////////
        // MASTER SERVER (INTERNET)
        //           Button ~ add selected internet server to favorites

        WriteText(22,22,"Internet",0);

        if(LOGINMODE==LM_INTERNET)
        {
            WriteText(22,91, "^2Add Favorite",0);
            if(MouseIn(20,90,120,110))
            {
                WriteText(22,91, "^>ffffffAdd Favorite",0);
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

            WriteText(22,114,"^2More Info",0);
            if(MouseIn(20,112,120,132))
            {

                WriteText(22,114,"^>ffffffMore Info",0);
            }

        }

        //////////////////////////////////////////////////////////////
        // FAVORITES
        //           Button ~ enter ip address
        //           Button ~ delete selected favorite

        WriteText(22,45,"Favorites",0);

        if(LOGINMODE==LM_FAVORITES)
        {
            WriteText(22,91, "^5Manual entry",0);
            if(MouseIn(20,90,120,110))
            {
                WriteText(22,91, "^>ffffffManual entry",0);
                if(GetMouseRelease(SDL_BUTTON_LEFT))
                {
                    ClearGUIs();

                    pGUI->call("fav_man_add.gui");

                }
            }
            WriteText(22,114,"^5Discard",0);
            if(MouseIn(20,112,120,132))
            {
                WriteText(22,114,"^>ffffffDiscard",0);
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
            WriteText(22,137,"^5Edit",0);
            if(MouseIn(20,136,120,156))
            {
                WriteText(22,137,"^>ffffffEdit",0);
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

        WriteText(22,68,"Local (LAN)",0);

        if(LOGINMODE==LM_LOCAL)
        {
            WriteText(22,91,"^6Auto-Detect",0);
            if(MouseIn(20,90,120,110))
            {
                WriteText(22,91,"^>ffffffAuto-Detect",0);
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
                WriteText(22,22,"^>ffffffInternet",0);

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
                WriteText(22,45,"^>ffffffFavorites",0);

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
                WriteText(22,68,"^>ffffffLocal (LAN)",0);

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

        break;

        /////////////////////////////

    case CONNECT:

        pLog->_Add("CONNECT");

        pGUI->clear();// ClearGUIs();


        strcpy(pClientData->ServerID,"s001-frag-mere");
        if( (!strlen(pClientData->Name)) ||
                (!strlen(pClientData->Password)) )
        {
            pGUI->Prompt("Must enter user info           to proceed.","nop");
            pClientData->GAME_MODE=GATHER_SERVER_LIST;
            break;
        }

        strcpy(pClientData->IPAddress,"127.0.0.1");
        strcpy(pClientData->Port,"40403");

        // Run the connect command through the console...


        // ProcessConsoleCommand(va("connect %s:%s %s %s",                                 pClientData->IPAddress,pClientData->Port,                                 pClientData->Name,pClientData->Password),1);

        break; // End CONNECT

    case CONSOLE_CONNECT:

        pLog->_Add("CONSOLE_CONNECT");

        pGUI->clear();
        pGUI->call("connect.gui");

        //NetworkSetup();
        //NetworkConnect();
        //pFMGS->dMsgTime=dlcs_get_tickcount();

        // ProcessConsoleCommand("stopaudio",0);

        pClientData->GAME_MODE=LOGIN_RECV;
        break;

        /////////////////////////////

    case LOGIN_RECV: // Spin loop for logging in
        pLog->_Add("LOGIN_RECV");

        //MOUSEMODE=MP_SYSTEMBUSY;

        pGUI->WriteText(270,270,"^&^7Connecting to the server...",0);

        /*

        if((dlcs_get_tickcount()-pFMGS->dMsgTime) > NET_WAITFORANSWER)
        {
            pGUI->Prompt("^&^6No response from server^1!","nop");
            pClientData->GAME_MODE=LOGIN_SCREEN_ENTRY;
        }
        */

        break; // End LOGIN_RECV

        //////////////////////////////

    case GET_SERVER_INFORMATION_START:
        pLog->_Add("GET_SERVER_INFORMATION_START");

/*        SendData.Reset();
        SendData.Write((char)NETMSG_SERVERINFORMATION);
        SendData.Write((char)1);
        SendNetMessage(0);
        */
        pClientData->GAME_MODE=GET_SERVER_INFORMATION_SPIN;


    case GET_SERVER_INFORMATION_SPIN:
        pLog->_Add("GET_SERVER_INFORMATION_SPIN");

        //MOUSEMODE=MP_SYSTEMBUSY;
        pGUI->WriteText(270,270,"^&^7Updating Server Information...",0);


/*
        if((dlcs_get_tickcount()-pFMGS->dMsgTime) > NET_WAITFORANSWER)
        {
            pGUI->Prompt("^&^6No response from server^1!","nop");
            pClientData->GAME_MODE=LOGIN_SCREEN_ENTRY;
        }
        */

        break;

        /////////////////////////////

    case GET_CHARACTERS:
        pLog->_Add("GET_CHARACTERS");



//        for(i=0; i<MAX_TOONS; i++)
  //      {
    //        memset(pClientData->ServerCharacter[i].t_name,0,32);
      //  }

//        SendData.Reset();
  //      SendData.Write((char)NETMSG_RETRIEVECHARS);
    //    SendData.Write((char)1);
      //  SendNetMessage(0);
        pClientData->GAME_MODE=GET_CHARACTERS_SPIN;
        break;

    case GET_CHARACTERS_SPIN:

//        MOUSEMODE=MP_SYSTEMBUSY;
        pGUI->WriteText(270,270,"^&^7Downloading character information...",0);

        /*
        if((dlcs_get_tickcount()-pFMGS->dMsgTime) > NET_WAITFORANSWER)
        {
            pGUI->Prompt("^&^6No response from server^1!","nop");
            pClientData->GAME_MODE=LOGIN_SCREEN_ENTRY;
        }
        */
        break;


    case CHOOSE_CHARACTER:

        //strcpy(szTemp1,"character");
        //if(strcmp(szTemp1,pClientData->szGeneric))
         //   strcpy(pClientData->szGeneric,szTemp1);

       // pGUI->clear();
       //pGUI->call("choose_chars.gui");

      //  for(i=0; i<MAX_TOONS; i++)
      //  {
//            pClientData->ServerCharacter[i].t_name[strlen(pClientData->ServerCharacter[i].t_name)-1]=0;

         //   if(strlen(pClientData->ServerCharacter[i].t_name))
         //   {
               // if(!dlcs_strcasecmp(pClientData->ServerCharacter[i].t_name,"null"))
               // {

                  //  pLog->_Add(pClientData->ServerCharacter[i].t_name);

//                    gc=0;

                 //   pLog->_Add(va("toon_%d",i+1));

//                    gc=pGUI->first_gui_stump->get_control(va("toon_%d",i+1));

                 //   pLog->_Add("%d",gc);

                //    if(gc)
                 //   {
                 //       pLog->_Add("hi !!! %s",pClientData->ServerCharacter[i].t_name);
                  //      gc->set_value(va("%s",pClientData->ServerCharacter[i].t_name));
                  //  }
          //      }
       //     }
     //   }


        pClientData->GAME_MODE=CHOOSE_CHARACTER_SPIN;

    case CHOOSE_CHARACTER_SPIN:

        // pClientData->GAME_MODE=GATHER_GAME_DATA;


        break;


    case CREATE_CHARACTER:

        //strcpy(szTemp1,"create_character");
        //if(strcmp(szTemp1,pClientData->szGeneric))
         //   strcpy(pClientData->szGeneric,szTemp1);

        pGUI->clear();

        pGUI->call("create_character.gui");


        pClientData->GAME_MODE=CREATE_CHARACTER_SPIN;

        break;


    case CREATE_CHARACTER_SPIN:


        break;


    case CREATE_CHARACTER_SEND:

        break;

    case CREATE_CHARACTER_SEND_SPIN:

        break;



        /////////////////////////////

    case GATHER_GAME_DATA: // Logged in, now get all data needed

        pGUI->clear();//ClearGUIs();

        pLog->_Add("===============================================================");
        if(!strlen(pClientData->ServerName)) strcpy(pClientData->ServerName,"Ember Server");
        pLog->_Add("Server Name.....%s",pClientData->ServerName);
        if(!strlen(pClientData->szServerVersion)) strcpy(pClientData->szServerVersion,"v??.??.???");
        pLog->_Add("Server Version..%s",pClientData->szServerVersion); // Extracted at login time
        if(!strlen(pClientData->ServerID)) strcpy(pClientData->ServerID,"S001-EMBE-R001");
        pLog->_Add("Server ID.......%s",pClientData->ServerID);
        if(!strlen(pClientData->ServerAuthor)) strcpy(pClientData->ServerAuthor,"Admin");
        pLog->_Add("Administrator...%s",pClientData->ServerAuthor);
        pLog->_Add("===============================================================");

        SDL_WM_SetCaption(va("EGC %s(%s) [%s]",VERSION,CPUSTRING,pClientData->ServerName),"icon"); // nr(%s) ,NET_REVISION

        pClientData->GAME_MODE=GATHER_GAME_DATA_LOADER;
        break;

        /////////////////////////////

    case GATHER_GAME_DATA_LOADER:

//        MOUSEMODE=MP_SYSTEMBUSY;
        //if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false)) DrawGenericSurface();
        pClientData->LAST_GAME_MODE=LOGIN_SCREEN_ENTRY;
        pClientData->NEXT_GAME_MODE=LOGIN_PROGRAM_START;

        //pFMGS->dMsgTime=dlcs_get_tickcount();

        //GAME_MODE=WAIT_LOOP;
        //SendData.Reset();
        //SendData.Write((char)NETMSG_GET_LOGIN_PROGRAM);
        //SendData.Write((char)1);
        //SendNetMessage(0);
        //break;

    case LOGIN_PROGRAM_START:

        //if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false)) DrawGenericSurface();
        pClientData->LAST_GAME_MODE=LOGIN_SCREEN_ENTRY;
        pClientData->NEXT_GAME_MODE=LOGIN_PROGRAM_END;

        // wait for login program from the server, it will be fetched in NET_UTIL

        pClientData->GAME_MODE=LOGIN_PROGRAM_END; // when user completed the program...

        break;

    case LOGIN_PROGRAM_END:

        pClientData->GAME_MODE=INITIALIZE_GAME;
        pClientData->LAST_GAME_MODE=LOGIN_SCREEN_ENTRY;
        pClientData->NEXT_GAME_MODE=INITIALIZE_GAME;

        break;

    case INITIALIZE_GAME:

        pGUI->clear();
        pGUI->call("gameon.gui");

/*        camera->px=0;
        camera->py=0;
        camera->pz=0;

        camera->vx=0;
        camera->vy=0;
        camera->vz=0;

        camera->ux=0;
        camera->uy=0;
        camera->uz=0;
        */

        // ProcessConsoleCommand("set consolebmp console.bmp",0);

        pLog->_Add("Done initializing player setup...");

        pClientData->LAST_GAME_MODE=LOGIN_SCREEN_ENTRY;
        pClientData->NEXT_GAME_MODE=ITEM_INITIALIZE;
        pClientData->GAME_MODE=ITEM_INITIALIZE;

        break;
        //
        //
        //
        //
        //
        /////////////////////////////

    case ITEM_INITIALIZE:



        pClientData->LAST_GAME_MODE=LOGIN_SCREEN_ENTRY;
        pClientData->NEXT_GAME_MODE=GAME_ON;
        pClientData->GAME_MODE=GAME_LIMBO;
        break;
        //
        //
        //
        //
        //
        /////////////////////////////

    case GAME_ON:

        pGFX->RenderScene();
        break; // End GAME_ON

    case GAME_LIMBO:
        pClientData->NEXT_GAME_MODE=GAME_ON;
        pClientData->GAME_MODE=GAME_ON;
        break;

        /////////////////////////////

    case QUIT:

    case DEBUG_LOOP:

        //if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false))
       // {

       // }

        break; // End DEBUG_LOOP

    case WAIT_LOOP:

        //if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false))
        //{

        //}
        /*
        if(dlcs_get_tickcount()-pFMGS->dMsgTime>10000)
        {
            pGUI->Prompt("Timed out","nop");
            pClientData->GAME_MODE=pClientData->LAST_GAME_MODE;
        }
        */
        break; // End DEBUG_LOOP

    case EDIT_WORLD_INIT:

        pLog->_Add("Edit World Init Start");

        pGUI->clear();
        pGUI->call("editmenu.gui");

        pClientData->bDrawMap=false;
        pClientData->Mode=BUILD;

        pLog->_Add("Edit World Init End");

        pClientData->GAME_MODE=EDIT_WORLD;

    case EDIT_WORLD:

        // pLog->_Add("Game mode: EDIT_WORLD start");

        pGFX->RenderScene();

        // pLog->_Add("Game mode: EDIT_WORLD end");

    break;

    default:
        break; // End switch(GAME_MODE)
    }

    /// END OF GAME MODE SPECIFICS

    /////////////////////////////////////////////////////////////////////////////////

    pGUI->draw();

    /////////////////////////////////////////////
    // Flip the back buffer to the primary buffer

    pGFX->FlipSurfaces();

} // End MAINGAMELOOP

/***********************************************************************************/

GAF_SCANCALLBACK what(GAFFile_ElmHeader *ElmInfo,LPSTR FullPath)
{
    switch(ElmInfo->Type)
    {
    case GAFELMTYPE_FILE:

        pLog->_Add("FILE: %25s %d",ElmInfo->Name,ElmInfo->FileSize);

        break;

    case GAFELMTYPE_DIR:

        pLog->_Add("<DIR> %25s ",ElmInfo->Name);

        break;

    default:
        break;
    }
    return 0;
}



bool doInit(void)
{
    /////////////////////////////////////////////////////////////////////////////////
    // Zeroize pointers

    pClientData     = NULL;
    pGFX            = NULL;
    pLog            = NULL;
    pGAF			= NULL;
    pGUI            = NULL;

    //pFMGS           = NULL;

    /////////////////////////////////////////////////////////////////////////////////
    // Fill random seed with time for better randomizing

    srand((int)dlcs_get_tickcount());

    /////////////////////////////////////////////////////////////////////////////////
    // Create the Log

    pLog = new CLog("client.log");
    if(!pLog) return FALSE;
    pLog->On();
    pLog->LineFeedsOff();
    pLog->Restart();
    pLog->AddLineSep();

    pLog->_Add("Log created");

    /////////////////////////////////////////////////////////////////////////////////
    // Load in client.ini

    pLog->_Add("Setting up Client Data");

    pClientData = new CC_Data;
    if(!pClientData)
        return FALSE;
    pClientData->bLoad();

    if(!pClientData->bLog) pLog->Off();

    /////////////////////////////////////////////////////////////////////////////////
    // Create GAF File

    pLog->_Add("Setting up GAF");

    pGAF = new CGAF();
    if(!pGAF) return 0;
    pGAF->Open("ember.gaf");

    /////////////////////////////////////////////////////////////////////////////////
    // SDL

    pLog->_Add("Starting SDL");

    SDL_Init(SDL_INIT_EVENTTHREAD);

    /////////////////////////////////////////////////////////////////////////////////
    // sound

    pLog->_Add("Setting up SND");


    pSND=new C_Sound();

    if(!pSND)
    {
        pLog->_Add("Sound can not be initialized, turning off sound and music");
        pClientData->bMusic=false;
        pClientData->bSound=false;
    }
    else
    {
        pSND->SetSoundVolume((pClientData->fSoundVolume*255));
        pSND->SetMusicVolume((pClientData->fMusicVolume*255));
    }

    /////////////////////////////////////////////////////////////////////////////////
    // Initialize GFX

    pLog->_Add("Setting up GFX");

    pGFX = new C_GFX(   pClientData->ScreenWidth,
                        pClientData->ScreenHeight,
                        pClientData->ScreenColors,
                        pClientData->bFullScreen,
                        va("EGC %s(%s)  %s",VERSION,CPUSTRING,COPYRIGHT), // Net Revision(%s) NET_REVISION,
                        pLog, pGAF );

    if(!pGFX)
    {
        pLog->_Add("pGFX initialization failure, quitting");
        return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////
    // GUI

    pLog->_Add("Setting up GUI");

    pGUI = new C_FM_GUI(pGFX,pGAF,pLog);

    if(!pGUI)
    {
        pLog->_Add("pGUI initialization failure, quitting");
        return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////

    NET_Init();

    pClientData->LAST_GAME_MODE=MAIN_MENU; // Start off with the main menu
    pClientData->GAME_MODE=RETRO_INTRO_INIT; // what the

    pLog->_Add("execing [autoexec.cfg]");

    // ProcessConsoleCommand("exec autoexec.cfg",0);

    pLog->_Add("execing [config.cfg]");

    // ProcessConsoleCommand("exec config.cfg",0);


    // SDL_PeepEvents(SDL_PEEKEVENT);

    return TRUE;
}

/***************************************************************
 **  Shut Down the Program                                    **
 ***************************************************************/

void ShutDown(void)
{
    // ProcessConsoleCommand("disconnect",0);
    // ProcessConsoleCommand("stopaudio",0);


    DEL(pSND);
    DEL(pGUI);

//    DEL(pFMGS);
//    DEL(pFMMS);

    NET_Shutdown();

    if(pClientData)
    {
        pClientData->bSave();
        pLog->_Add("Client data saved...");
        pClientData->CleanUp();
        DEL(pClientData);
        pLog->_Add("Client data destroyed...");
    }

    DEL(pGFX);

    pGAF->Close();
    DEL(pGAF);


    ///////////////////////////////////////////////////
    // Finish shutting down

    pLog->_Add("Ember engine shut down!");

    if(pLog)
    {
        pLog->AddLineSep();
        DEL(pLog);
    }
    bShutDown=true;
}



/*


/////////////////////////////////////////////////////////////////////////
extern "C" void ex_ProcessConsoleCommand(char *szCommandIn, bool bEcho)
{
    char szTemp1[1024]; memset(szTemp1,0,1024);
    char szTemp2[1024]; memset(szTemp2,0,1024);
    char szTemp3[1024]; memset(szTemp3,0,1024);
    char *szInput;
    char szInputCopy[_MAX_PATH];
    char szCommand[_MAX_PATH];
    FILE* fp;
    int i,j,k,l;
    float f;
    int ax,ay,az,bx,by,bz,cx,cy,cz,dx,dy,dz,ex,ey,ez;
    float fx,fy,fz,fa;
    fx=fy=fz=fa=0;
    u_char abx,aby,abz;
    char cQuoteCount;
    bool bQuote2;
    strcpy(szCommand,szCommandIn); if(szCommand == NULL) return;
    i=j=k=l=0; f=0;
    ax=ay=az=bx=by=bz=cx=cy=cz=dx=dy=dz=ex=ey=ez=0;
    abx=aby=abz=0;
    fp=NULL;

    // ARROW THROUGH RECENT COMMANDS 26 Jul 00

    if(pClientData)
    {
        if(bEcho)
        {
            pGUI->ConsoleScrollInput();
        }
    }

    // Check " - Any ; in between quotes will temp change to 0xFF

    cQuoteCount=0;
    //bQuote2;
    strcpy(szInputCopy,szCommand);
    for(i=0;i<strlen(szInputCopy);i++)
    {
        switch(szInputCopy[i])
        {
            case '"':
                cQuoteCount++;
                break;
            case ';':
                if(cQuoteCount&1)
                {
                    szInputCopy[i]='Æ';
                    break;
                }
                else
                    bQuote2=true;
                break;
            default:
                break;
        }
    }
    if(cQuoteCount&1)
    {
       // Log("Mismatched quote error.");
        return;
    }

    // Check ;
    strcpy(szCommand,szInputCopy);
    szInput=strtok(szInputCopy,";");
    if(szInput == NULL) return; j=0;
    if(strlen(szInput)!=strlen(szCommand))
    {
        if(bEcho) pCon->ConsoleBufferAdd(szCommand);

        while(1)
        {
            //strcpy(pClientData->ConsoleCommandBuffer[j].Text,szInput);
            j++;
            if(j>MAX_CONSOLE_MULTI_COMMANDS) j=MAX_CONSOLE_MULTI_COMMANDS;
            szInput = strtok(NULL,";");
            if(szInput == NULL)
            {
                for(i=0;i<j;i++) ProcessConsoleCommand(szInput,bEcho);//pClientData->ConsoleCommandBuffer[i].Text,bEcho);
                return;
            }
        }
    }
    if(szInputCopy[0]=='"')
    {
        for(i=0;i<strlen(szInputCopy);i++)
        {
            if(szInputCopy[i]=='"')
            {
                for(j=i;j<strlen(szInputCopy);j++) szInputCopy[j]=szInputCopy[j+1];
            }
            if(szInputCopy[i]=='Æ') szInputCopy[i]=';';
        }
        ProcessConsoleCommand(szInputCopy,0);
        return;
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
            if(pFMGS->eGetState()!=CON_NOTCONNECT)
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
            if(i==CON_NOTCONNECT)
               // Log("Network.............Not Connected");

            else
            {
               // Log("Network.............Connected to %s",pClientData->IPAddress);

                switch(i)
                {
                    case CON_NOTCONNECT:
                        strcpy(szTemp1,"CON_NOTCONNECT");
                        break;
                    case CON_CONNECTED:
                        strcpy(szTemp1,"CON_CONNECTED");
                        break;
                    case CON_SYSBUSY:
                        strcpy(szTemp1,"CON_SYSBUSY");
                        break;
                    case CON_LOGGINPROC:
                        strcpy(szTemp1,"CON_LOGGINPROC");
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
    // QUIT
    // Quit the program unconditionally

    if( (dlcs_strcasecmp(szInput,"quit")) ||
        (dlcs_strcasecmp(szInput,"exit")) )
    {
        pGUI->clear();
		// ClearGUIs();
        bShutDown=1;
        ProcessConsoleCommand("disconnect",0);
		SetGameMode(QUIT);
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // CLS
    // Clear the console buffer

    if(dlcs_strcasecmp(szInput,"cls"))
    {
        pCon->ClearConsole();

        return;
    }


    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // CAB
    // CAB File commands

	if(dlcs_strcasecmp(szInput,"cab"))
	{

		szInput=strtok(NULL," \n\r");
        if(szInput==NULL)
        {
            if(bEcho)
            {
                pCon->ConsoleBufferAdd("CAB: No command");
            }
            return;
        }

		pCon->ConsoleBufferAdd("CAB: command [%s]",szInput);

        if(dlcs_strcasecmp(szInput,"rd"))
		{


			pCon->ConsoleBufferAdd("CAB: refreshdir");


            szInput=strtok(NULL," \n\r");
			if(szInput)
			{

				pCon->ConsoleBufferAdd("CAB: refreshing %s",szInput);
				pCon->ConsoleBufferAdd("CAB: refreshdir operation may take a while");
				pCab->RemoveDir(szInput);
				pCab->CreateDir(szInput);
				pCab->AddDir(szInput,szInput,1);
			}
			return;
		}

		if(bEcho)
		{
			pCon->ConsoleBufferAdd("CAB: No command");
		}

		return;
	}

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // CD..
    // Change working directory up one directory

    if(dlcs_strcasecmp(szInput,"cd.."))
    {
        dlcs_chdir("..");
//        strcpy(pClientData->CWDir,dlcs_getcwd());
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
    // ECHO
    // Echos back to the console.

    if(dlcs_strcasecmp(szInput,"echo"))
    {
        szInput=strtok(NULL,"\n\r");
        if(szInput==NULL)
        {
            pCon->ConsoleBufferAdd("Usage : echo <string>");
            return;
        }
        pCon->ConsoleBufferAdd(szInput);
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    // CC! (Console command)
    // EXEC
    // Execute a script file that has other console commands in it.

    if(dlcs_strcasecmp(szInput,"exec"))
    {
        szInput=strtok(NULL,"\n\r");
        if(szInput==NULL)
        {
            pCon->ConsoleBufferAdd("Usage : exec <filename>");
            return;
        }

        strcpy(szTemp1,szInput);

        FILE *fp;
        fp=fopen(szTemp1,"r");

        if(!fp)
        {
            sprintf(szCommand,"Usage : Can't find file: %s",szInput);
            return;
        }

        // DLog("FM: Execing [%s]",szTemp1);

        if(bEcho)
        {
            pCon->ConsoleBufferAdd("Execing [%s]",szTemp1);
        }

        while(1)
        {
            if(!fgets(szTemp1,_MAX_PATH,fp))
                break;
            ProcessConsoleCommand(szTemp1,0);
        }
        fclose(fp);
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
            if(pFMGS->eGetState()!=CON_NOTCONNECT)
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
            if(pFMGS->eGetState()==CON_NOTCONNECT)
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
            if(pFMGS->eGetState()==CON_NOTCONNECT)
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

    if(dlcs_strcasecmp(szInput,"bind"))
    {
        int ikey;
        szInput=strtok(NULL," \n\r");
        if(szInput==NULL)
        {
            pCon->ConsoleBufferAdd("Usage : bind <key> <command>");
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
        //if(dlcs_strcasecmp(szInput,"`"))
            //ikey = SDLK_BACKQUOTE;
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
        /*
        if(dlcs_strcasecmp(szInput,"kana"))
            ikey = SDLK_KANA;
        if(dlcs_strcasecmp(szInput,"abnt_c1"))
            ikey = SDLK_ABNT_C1; // ? on Portugese (Brazilian) keyboards
        if(dlcs_strcasecmp(szInput,"convert"))
            ikey = SDLK_CONVERT;
        if(dlcs_strcasecmp(szInput,"noconvert"))
            ikey = SDLK_NOCONVERT;
        if(dlcs_strcasecmp(szInput,"yen"))
            ikey = SDLK_YEN;
        if(dlcs_strcasecmp(szInput,"abnt_c2"))
            ikey = SDLK_ABNT_C2; // Numpad . on Portugese (Brazilian) keyboards
            *
        if(dlcs_strcasecmp(szInput,"numpad_equals"))
            ikey = SDLK_KP_EQUALS;
        /*
        if(dlcs_strcasecmp(szInput,"circumflex"))
            ikey = SDLK_CIRCUMFLEX;
            *
        if(dlcs_strcasecmp(szInput,"at"))
            ikey = SDLK_AT;
        if(dlcs_strcasecmp(szInput,"colon"))
            ikey = SDLK_COLON;
        if(dlcs_strcasecmp(szInput,"_"))
            ikey = SDLK_UNDERSCORE;
        /*
        if(dlcs_strcasecmp(szInput,"kanji"))
            ikey = SDLK_KANJI;
        if(dlcs_strcasecmp(szInput,"stop"))
            ikey = SDLK_STOP;
        if(dlcs_strcasecmp(szInput,"ax"))
            ikey = SDLK_AX;
        if(dlcs_strcasecmp(szInput,"unlabeled"))
            ikey = SDLK_UNLABELED;
        if(dlcs_strcasecmp(szInput,"nexttrack"))
            ikey = SDLK_NEXTTRACK; // Next Track Key?
            *
        if(dlcs_strcasecmp(szInput,"numpad_enter"))
            ikey = SDLK_KP_ENTER;
        if(dlcs_strcasecmp(szInput,"r_control"))
            ikey = SDLK_RCTRL;
        /*
        if(dlcs_strcasecmp(szInput,"mute"))
            ikey = SDLK_MUTE; // Mute Key?
        if(dlcs_strcasecmp(szInput,"calculator"))
            ikey = SDLK_CALCULATOR; // Calculator Key?
        if(dlcs_strcasecmp(szInput,"playpause"))
            ikey = SDLK_PLAYPAUSE; // Play / Pause Key?
        if(dlcs_strcasecmp(szInput,"mediastop"))
            ikey = SDLK_MEDIASTOP; // Media Stop Key?
        if(dlcs_strcasecmp(szInput,"volumedown"))
            ikey = SDLK_VOLUMEDOWN; // Volume - Key?
        if(dlcs_strcasecmp(szInput,"volumeup"))
            ikey = SDLK_VOLUMEUP; // Volume + Key?
        if(dlcs_strcasecmp(szInput,"webhome"))
            ikey = SDLK_WEBHOME; // Web home Key?
        if(dlcs_strcasecmp(szInput,"numpad_comma"))
            ikey = SDLK_NUMPADCOMMA;
            *
        if(dlcs_strcasecmp(szInput,"/"))
            ikey = SDLK_KP_DIVIDE;
        /*
        if(dlcs_strcasecmp(szInput,"sysrq"))
            ikey = SDLK_SYSRQ;
            *
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
        /*
        if(dlcs_strcasecmp(szInput,"start"))
            ikey = SDLK_START;
            *

        if(dlcs_strcasecmp(szInput,"power"))
            ikey = SDLK_POWER;
        /*
        if(dlcs_strcasecmp(szInput,"sleep"))
            ikey = SDLK_SLEEP;
        if(dlcs_strcasecmp(szInput,"wake"))
            ikey = SDLK_WAKE;
        if(dlcs_strcasecmp(szInput,"websearch"))
            ikey = SDLK_WEBSEARCH;
        if(dlcs_strcasecmp(szInput,"webfavorites"))
            ikey = SDLK_WEBFAVORITES;
        if(dlcs_strcasecmp(szInput,"webrefresh"))
            ikey = SDLK_WEBREFRESH;
        if(dlcs_strcasecmp(szInput,"webstop"))
            ikey = SDLK_WEBSTOP;
        if(dlcs_strcasecmp(szInput,"webforward"))
            ikey = SDLK_WEBFORWARD;
        if(dlcs_strcasecmp(szInput,"webback"))
            ikey = SDLK_WEBBACK;
        if(dlcs_strcasecmp(szInput,"mycomputer"))
            ikey = SDLK_MYCOMPUTER;
        if(dlcs_strcasecmp(szInput,"mail"))
            ikey = SDLK_MAIL;
        if(dlcs_strcasecmp(szInput,"mediaselect"))
            ikey = SDLK_MEDIASELECT;
            *

        if((ikey>0)&&(ikey<SDLK_LAST))
        {
            szInput=strtok(NULL,"\n\r");
            if(szInput==NULL)
            {
               // Log("Usage : bind <key> <command>");
                return;
            }
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

