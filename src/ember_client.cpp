/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         manclient.cpp
 **   Description:  MANTRA GAME CLIENT / DLSTORM GAME ENGINE / by Seth Parson
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ******************************************************************************/

#include "manclient.h"

////////////////////////////////////////////////////// Global Variables
bool     bShutDown;    // Shut down the program by setting this to true
CC_Data *pClientData;  // Client Data (ini file)
C_GFX *  pGFX;         // SDL / OpenGL
C_GUI *  pGUI;         // OpenGL GUI
CGAF *   pGAF;         // Game Archive File (GAF)
CLog *   pLog;         // Log file
C_FMGS * pFMGS;        // Game Server connection
#ifdef DLCS_WINDOWS
C_Sound *pSND;  // FMOD Sounds
#endif

////////////////////////////////////////////////////// Main
int main(int argc, char *argv[]) {
    bShutDown = false;
    if (doInit()) {
        while (!bShutDown) {
            MainGameLoop();
        }
    }
    ShutDown();
    return false;
}
////////////////////////////////////////////////////// Console STUFF
void con_terraform(const string &s) { pLog->_Add("TERRAFORM!"); }
void con_getint_str(char *pString, int n) {
    string                     temp;
    map<string, int>::iterator ii;
    for (ii = pGUI->pCons->intmap.begin(); ii != pGUI->pCons->intmap.end(); ++ii) {
        if (((*ii).second) == (n)) {
            temp = va("%s ", (const char *)(*ii).first.c_str());
        }
    }
    strcpy(pString, temp.c_str());
    return;
}
int con_getint(const string &s) {
    if (pGUI->pCons->intmap.find(s.c_str()) != pGUI->pCons->intmap.end()) {
        return (pGUI->pCons->intmap.find(s.c_str())->second);
    }
    return false;
}
void con_functest(const string &s) { pLog->_Add("functest(%s)", (char *)s.c_str()); }
void con_setgamemode(const string &s) {
    pLog->_Add("setgamemode %s", (char *)s.c_str());
    SetGameMode((MODE)con_getint(s.c_str()));
    pLog->_Add("con_setgamemode()");
}
void con_guirender(const string &s) {
    char name[1024];
    memset(name, 0, 1024);
    char media[1024];
    memset(media, 0, 1024);
    char temp[1024];
    memset(temp, 0, 1024);
    int x, y, w, h, props;
    pGUI->getdata(name, "name");
    pGUI->getdata(media, "media");
    pGUI->getdata(temp, "x");
    x = atoi(temp);
    pGUI->getdata(temp, "y");
    y = atoi(temp);
    pGUI->getdata(temp, "w");
    w = atoi(temp);
    pGUI->getdata(temp, "h");
    h     = atoi(temp);
    props = 0;
    map<string, int>::iterator ii;
    for (ii = pGUI->GC_PROP.begin(); ii != pGUI->GC_PROP.end(); ++ii)
        if (dlcs_strcasecmp(pGUI->getdata((char *)(*ii).first.c_str()), "on")) props |= (*ii).second;

    pGUI->add_stump(name, x, y, w, h, props, media);

    pGUI->getdata(temp, "caption");
    if (pGUI->get_stump(name)) {
        strcpy(pGUI->get_stump(name)->caption, temp);
        pGUI->get_stump(name)->bEditing = true;

        for (ii = pGUI->GC_RELATIVE.begin(); ii != pGUI->GC_RELATIVE.end(); ++ii)
            if (dlcs_strcasecmp(pGUI->getdata("relativeto"), (char *)(*ii).first.c_str())) pGUI->get_stump(name)->iRelativeTo = (*ii).second;
    }
}
void con_guictrlrender(const string &s) {
    vector<string> vs;
    vector<string> ncmd = dlcs_explode(" ", s.c_str());

    char name[1024];
    memset(name, 0, 1024);
    strcpy(name, pGUI->getdata("name"));
    char media[1024];
    memset(media, 0, 1024);
    strcpy(media, pGUI->getdata("media"));
    char value[1024];
    memset(value, 0, 1024);
    strcpy(value, pGUI->getdata("value"));
    char pstump[1024];
    memset(pstump, 0, 1024);
    strcpy(pstump, pGUI->getdata("parentstumpname"));

    int      props = 0;
    C_GCTRL *tctrl;
    tctrl = 0;

    if (pGUI->get_stump(pstump)) {
        pLog->_Add("ADD CONTROL TO STUMP: %s", pGUI->get_stump(pstump)->name);

        props = 0;
        map<string, int>::iterator ii;
        for (ii = pGUI->GC_PROP.begin(); ii != pGUI->GC_PROP.end(); ++ii)
            if (dlcs_strcasecmp(pGUI->getdata((char *)(*ii).first.c_str()), "on")) props |= (*ii).second;

        pLog->_Add(" %d %s", pGUI->GC_TYPE[pGUI->getdata("type")], pGUI->getdata("type"));

        pGUI->get_stump(pstump)->add_control(name, pGUI->GC_TYPE[pGUI->getdata("type")], atoi(pGUI->getdata("x")), atoi(pGUI->getdata("y")), atoi(pGUI->getdata("w")), atoi(pGUI->getdata("h")), props, media, value);

        tctrl = pGUI->get_stump(pstump)->get_control(name);
        if (tctrl) {
            pLog->_Add("FOUND CTRL %s IN STUMP %s", tctrl->name, pGUI->get_stump(pstump)->name);
            tctrl->bEditing = true;

            strcpy(tctrl->media_hover, pGUI->getdata("media_hover"));
            strcpy(tctrl->media_click, pGUI->getdata("media_click"));
            strcpy(tctrl->console, pGUI->getdata("console"));
            strcpy(tctrl->hover_text, pGUI->getdata("hover_text"));
            strcpy(tctrl->group, pGUI->getdata("group"));
            strcpy(tctrl->action, pGUI->getdata("action"));

            // strcpy(tctrl->relativeto,pGUI->getdata("relativeto"));
            for (ii = pGUI->GC_RELATIVE.begin(); ii != pGUI->GC_RELATIVE.end(); ++ii)
                if (dlcs_strcasecmp(pGUI->getdata("relativeto"), (char *)(*ii).first.c_str())) tctrl->iRelativeTo = (*ii).second;

            vs = dlcs_explode(",", pGUI->getdata("border_color"));
            if (vs.size() > 1) tctrl->border_color = dlcsm_RGB(atoi(vs[0].c_str()), atoi(vs[1].c_str()), atoi(vs[2].c_str()));

            vs = dlcs_explode(",", pGUI->getdata("background_color"));
            if (vs.size() > 1) tctrl->background_color = dlcsm_RGB(atoi(vs[0].c_str()), atoi(vs[1].c_str()), atoi(vs[2].c_str()));

            tctrl->font     = atoi(pGUI->getdata("font"));
            tctrl->fontbank = atoi(pGUI->getdata("fontbank"));
        }
    }
}
void con_guiedit(const string &s) { pGUI->edit_stump((char *)s.c_str()); }
void con_guiremovecontrol(const string &s) {
    // vector <string> vs;
    vector<string> ncmd = dlcs_explode(" ", s.c_str());

    if (ncmd.size() > 1) {
        pLog->_Add("guiremovecontrol [%s] [%s]", (char *)ncmd[0].c_str(), (char *)ncmd[1].c_str());
        pGUI->remove_control((char *)ncmd[0].c_str(), (char *)ncmd[1].c_str());
    }
}
void con_guiclose(const string &s) {
    if (pGUI->get_stump((char *)s.c_str())) pGUI->get_stump((char *)s.c_str())->bDeleteMe = true;
}
void con_guistore(const string &s) { pGUI->store((char *)s.c_str()); }
void con_guicall(const string &s) { pGUI->call((char *)s.c_str()); }
void con_guicallfile(const string &s) { pGUI->call_file((char *)s.c_str()); }
void con_guicloseprompt(const string &s) {
    pGUI->bClosePrompt = true;
    pLog->_Add("Close Prompt");
}
void con_guicallback(const string &s) {
    if (!pFMGS) return;
    CPacket SendData(NET_DATAGRAMSIZE);
    if (pFMGS->emgeGetState() == NET_NOTCONNECT) return;
    SendData.Reset();
    SendData.Write((char)NETMSG_GUI_CALLBACK);

    pFMGS->SendUnreliableMessage(&SendData);
}
void con_echo(const string &s) { pLog->_Add((char *)s.c_str()); }
void con_exec(const string &s) {
    pLog->_Add("exec [%s]:", s.c_str());

    char temp[1024];
    memset(temp, 0, 1024);
    FILE *fp;
    fp = fopen(s.c_str(), "r");
    if (!fp) {
        pLog->_Add("Usage : Can't find file: %s", s.c_str());
        return;
    }
    while (1) {
        if (!fgets(temp, _MAX_PATH, fp)) break;
        pGUI->pCons->_Execute(temp);
    }
    fclose(fp);
}
void con_bind(const string &s) {
    int            key;
    vector<string> vs;
    vector<string> v2;
    char           ss[1024];
    memset(ss, 0, 1024);
    char ls[1024];
    memset(ls, 0, 1024);
    strcpy(ls, s.c_str());
    bool bQuotes;
    bQuotes = 0;
    for (key = 0; key < strlen(ls); key++) {
        ls[key] = tolower(ls[key]);
        if (ls[key] == '\"') bQuotes = 1;
    }

    if (bQuotes) {
        vs = dlcs_explode("\"", ls);
        strcpy(ls, vs[1].c_str());
        v2 = dlcs_explode(" ", vs[0].c_str());
        strcpy(ss, v2[0].c_str());
    } else {
        vs = dlcs_explode(" ", ls);
        strcpy(ls, "");
        for (key = 1; key < vs.size(); key++) {
            strcat(ls, va("%s ", vs[key].c_str()));
        }
        strcpy(ss, vs[0].c_str());
    }
    while (ss[strlen(ss) - 1] == ' ') ss[strlen(ss) - 1] = 0;
    while (ls[strlen(ls) - 1] == ' ') ls[strlen(ls) - 1] = 0;
    key                            = con_getint((char *)ss);
    pGUI->KeyMap[(SDL_Keycode)key] = (char *)ls;

    pLog->_Add("bind [%s] [%s]:", (char *)ss, (char *)ls);
}
void con_cvar(const string &s) {
    vector<string> vs;
    int            i, ivartype;
    char           temp[1024];
    memset(temp, 0, 1024);
    char name[1024];
    memset(name, 0, 1024);

    if (!strlen(s.c_str())) {
        pLog->_Add("CVAR LIST:");
        pLog->_Add("-----------------------------------------------");

        for (pGUI->pCons->svm_i = pGUI->pCons->varmap.begin(); pGUI->pCons->svm_i != pGUI->pCons->varmap.end(); ++pGUI->pCons->svm_i) {
            pGUI->pCons->get_cvar((char *)(*pGUI->pCons->svm_i).first.c_str(), temp);
            pLog->_Add("[%s]=[%s]", (*pGUI->pCons->svm_i).first.c_str(), temp);
        }
        return;
    }

    ivartype = pGUI->pCons->get_cvartype(s.c_str());
    if (ivartype == CVAR_NULL) {
        pLog->_Add("That isn't a CVAR");
        return;
    }
    vs = dlcs_explode("=", s.c_str());
    if (vs.size() > 1) {
        strcpy(name, vs[0].c_str());
        pGUI->pCons->set_cvar(name, (char *)vs[1].c_str());
    } else
        strcpy(name, s.c_str());

    if (pGUI->pCons->varmap.find(name) != pGUI->pCons->varmap.end()) {
        pGUI->pCons->get_cvar(name, temp);
        pLog->_Add("CVAR: [%s]=[%s]", name, temp);
    } else {
        pLog->_Add("That isn't a CVAR");
    }
}
void con_quit(const string &s) { bShutDown = true; }
void con_chat(const string &s) {
    vector<string> ncmd = dlcs_explode("|", s.c_str());

    pLog->_Add("%d", ncmd.size());

    if (ncmd.size() < 2) return;

    if (pFMGS) pFMGS->Chat((char *)ncmd[0].c_str(), (char *)ncmd[1].c_str(), 0);
    // char *msg,char *name,int channel){
};
////////////////////////////////////////////////////// Game Mode STUFF
void MainGameLoop(void) {  // **  Main Game Loop

    if (bShutDown) return;
    if (!pGUI) return;
    if (!pClientData) return;

    if (pFMGS) pFMGS->DoNetwork();

    pGUI->DoInput();

    // if( (pGUI->iKeyUp   == SDLK_RETURN) && (pGUI->modstate & KMOD_ALT) )pGFX->ToggleFullScreen();

    pGFX->BeginScene();

    DoGameMode();

    pGUI->draw();
    pGUI->drawFPS(0, 0);
    pGUI->gPrint(15, (int)(pClientData->get_cvar("i_ScreenHeight")) - 64, va("MOUSE POS X[%d] Y[%d]", pGUI->pMouse->ix, pGUI->pMouse->iy), 3, 1);
    // pGUI->gPrint(15, (int)(pClientData->get_cvar("i_ScreenHeight")) - 48, va("CAM   POS X[%f] Y[%f] Z[%f]", pGFX->pCamera->loc.x, pGFX->pCamera->loc.x, pGFX->pCamera->loc.z), 3, 1);
    // pGUI->gPrint(15, (int)(pClientData->get_cvar("i_ScreenHeight")) - 32, va("CAM   ROT X[%f] Y[%f] Z[%f] ANGLE[%f]", pGFX->pCamera->rot.x, pGFX->pCamera->rot.y, pGFX->pCamera->rot.z, pGFX->pCamera->angle), 3, 1);

    pGFX->EndScene();
}
void SetGameMode(int x) {
    if (pFMGS) pFMGS->spin_timer->Reset();
    if (pClientData) pClientData->set_cvar("i_GAME_MODE", x);
}

bool DoGameMode(void) {
    string temp1;
    string temp2;  //    char temp1[1024];    memset(temp1,0,1024);    char temp2[1024];    memset(temp2,0,1024);

    switch ((int)pClientData->get_cvar("i_GAME_MODE")) {
        case RETRO_INTRO_INIT: SetGameMode(RETRO_INTRO_PLAY); break;

        case RETRO_INTRO_PLAY: SetGameMode(MAIN_MENU); break;

        case MAIN_MENU:
            dlcsm_delete(pFMGS);
            pGUI->clear();
            pGUI->call("main.gui");
            pGUI->setdata("main.gui", "username", (const char *)pClientData->get_cvar("s_Name"));
            if ((bool)pClientData->get_cvar("b_SavePassword")) {
                pGUI->setdata("main.gui", "password", (const char *)pClientData->get_cvar("s_Password"));
                pGUI->setdata("main.gui", "savepassword", (const char *)"on");
            }
            SetGameMode(MAIN_MENU_2);

        case MAIN_MENU_2: break;

        case CREDITS: SetGameMode(MAIN_MENU); break;

        case LOGIN_AUTO_LOGIN:
            pLog->_Add("LOGIN_AUTO_LOGIN");
            pGUI->clear();
            pGUI->pCons->_Execute("exec autologin.cfg");
            SetGameMode(LOGIN_SCREEN_ENTRY);
            break;

        case LOGIN_SCREEN_ENTRY:
            pLog->_Add("LOGIN_SCREEN_ENTRY");
            if (pClientData) {
                pClientData->set_cvar("i_ServerListOffset", "0");
                // pClientData->get_cvar("SelectedServer=MAX_SERVERS+1;
            }
            // DEL(pFMMS);
            // LOGINMODE=LM_NONE;
            SetGameMode(GATHER_SERVER_LIST);

        case GATHER_SERVER_LIST:
            dlcsm_delete(pFMGS);
            pLog->_Add("GATHER_SERVER_LIST");
            // switch(LOGINMODE)
            //
            // case LM_NONE:
            //     break;
            // case LM_INTERNET:
            //     DEL(pFMMS);
            ////     pFMMS=new C_FMMS;
            //     break;
            // case LM_FAVORITES:
            //     pClientData->get_cvar("LoadFavoriteServers();
            //     break;
            // case LM_LOCAL:
            //     break;
            // default:
            //     break;
            //

            // pClientData->get_cvar("LoadProfiles();

            SetGameMode(CHOOSE_SERVER);

        case CHOOSE_SERVER_INIT:
            dlcsm_delete(pFMGS);
            pGUI->clear();
            pGUI->call("servers.gui");
            SetGameMode(CHOOSE_SERVER);

        case CHOOSE_SERVER:
            break;

            /////////////////////////////

        case LOGIN_FROM_GUI:
            char buf[512];
            memset(buf, 0, 512);
            pGUI->getdata(buf, "username");
            temp1.assign(buf);
            memset(buf, 0, 512);
            pGUI->getdata(buf, "password");
            temp2.assign(buf);
            temp2 = dlcs_md5_digest(temp2);
            pLog->_Add("%s", temp1.c_str());
            pClientData->set_cvar("s_Name", temp1.c_str());
            pClientData->set_cvar("s_Password", temp2.c_str());
            SetGameMode(CONNECT);
            break;

        case CONNECT:
            pGUI->clear();

            // strcpy(
            pClientData->set_cvar("s_ServerID", "s001-frag-mere");
            if ((!strlen((char *)pClientData->get_cvar("s_Name"))) || (!strlen((char *)pClientData->get_cvar("s_Password")))) {
                pGUI->prompt("Must enter user info to proceed.", "nop");
                SetGameMode(GATHER_SERVER_LIST);
                break;
            }
            // strcpy(pClientData->get_cvar("IPAddress,"127.0.0.1");
            // strcpy(pClientData->get_cvar("Port, "40227");
            // Run the connect command through the console...
            // ProcessConsoleCommand(va("connect %s:%s %s %s", pClientData->get_cvar("IPAddress,pClientData->get_cvar("Port, pClientData->get_cvar("Name,pClientData->get_cvar("Password),1);
            dlcsm_delete(pFMGS);
            pFMGS = new C_FMGS();
            if (!pFMGS) {
                SetGameMode(MAIN_MENU);
                pGUI->prompt("Can't establish network object", "nop");
                break;
            }
            temp1 = dlcs_md5_digest((const char *)pClientData->get_cvar("s_Password"));
            pFMGS->emgConnect((char *)pClientData->get_cvar("s_IPAddress"), (char *)pClientData->get_cvar("s_Port"), (char *)pClientData->get_cvar("s_Name"), (char *)temp1.c_str());
            SetGameMode(LOGIN_RECV);
            break;  // End CONNECT

        case CONSOLE_CONNECT:
            pLog->_Add("CONSOLE_CONNECT");
            pGUI->clear();
            pGUI->call("connect.gui");
            dlcsm_delete(pFMGS);
            pFMGS = new C_FMGS();
            temp1 = dlcs_md5_digest((const char *)pClientData->get_cvar("s_Password"));
            pFMGS->emgConnect((char *)pClientData->get_cvar("s_IPAddress"), (char *)pClientData->get_cvar("s_Port"), (char *)pClientData->get_cvar("s_Name"), (char *)temp1.c_str());

            // pFMGS->emgConnect((const char *)pClientData->get_cvar("IPAddress,pClientData->get_cvar("Port,pClientData->get_cvar("Name.c_str(),temp1.c_str());

            SetGameMode(LOGIN_RECV);
            break;
            /////////////////////////////

        case LOGIN_RECV:  // Spin loop for logging in
            pLog->_Add("LOGIN_RECV");
            // pGUI->MOUSEMODE=MP_SYSTEMBUSY;
            pGUI->gPrint(270, 270, "^&^7Connecting to the server...", 1);
            if (pFMGS->spin_timer->Up()) {
                pGUI->prompt("^&^6No response from server^1!", "nop");
                SetGameMode(MAIN_MENU);
            }
            break;  // End LOGIN_RECV
            //////////////////////////////

        case GET_SERVER_INFORMATION_START:
            pLog->_Add("GET_SERVER_INFORMATION_START");
            // SendData.Reset();
            // SendData.Write((char)NETMSG_SERVERINFORMATION);
            // SendData.Write((char)1);
            // SendNetMessage(0);
            SetGameMode(GET_SERVER_INFORMATION_SPIN);
        case GET_SERVER_INFORMATION_SPIN:
            pLog->_Add("GET_SERVER_INFORMATION_SPIN");
            // MOUSEMODE=MP_SYSTEMBUSY;
            pGUI->gPrint(270, 270, "^&^7Updating Server Information...", 1);
            if (pFMGS->spin_timer->Up()) {
                // pGUI->Prompt("^&^6No response from server^1!","nop");
                // SetGameMode(LOGIN_SCREEN_ENTRY);
            }
            break;
            /////////////////////////////

        case GET_CHARACTERS:
            pLog->_Add("GET_CHARACTERS");
            // SendData.Reset();
            // SendData.Write((char)NETMSG_RETRIEVECHARS);
            // SendData.Write((char)1);
            // SendNetMessage(0);
            SetGameMode(GET_CHARACTERS_SPIN);
            break;

        case GET_CHARACTERS_SPIN:
            pGUI->gPrint(270, 270, "^&^7Downloading character information...", 1);
            if (pFMGS->spin_timer->Up()) {
                //    pGUI->Prompt("^&^6No response from server^1!","nop");
                //    SetGameMode(LOGIN_SCREEN_ENTRY);
            }
            break;

        case CHOOSE_CHARACTER: SetGameMode(CHOOSE_CHARACTER_SPIN);
        case CHOOSE_CHARACTER_SPIN: break;

        case CREATE_CHARACTER:
            // strcpy(szTemp1,"create_character");
            // if(strcmp(szTemp1,pClientData->get_cvar("szGeneric))
            //   strcpy(pClientData->get_cvar("szGeneric,szTemp1);
            pGUI->clear();
            pGUI->call("create_character.gui");
            SetGameMode(CREATE_CHARACTER_SPIN);
            break;

        case CREATE_CHARACTER_SPIN: break;

        case CREATE_CHARACTER_SEND: break;

        case CREATE_CHARACTER_SEND_SPIN:
            break;
            /////////////////////////////

        case GATHER_GAME_DATA:  // Logged in, now get all data needed
            pGUI->clear();
            pLog->_Add("===============================================================");
            if (!strlen((const char *)pClientData->get_cvar("s_ServerName"))) pClientData->set_cvar("s_ServerName", "Mantra Server");
            pLog->_Add("Server Name.....%s", (const char *)pClientData->get_cvar("s_ServerName"));
            if (!strlen((const char *)pClientData->get_cvar("s_ServerVersion"))) pClientData->set_cvar("s_ServerVersion", "v??.??.???");
            pLog->_Add("Server Version..%s", (const char *)pClientData->get_cvar("s_ServerVersion"));  // Extracted at login time
            if (!strlen((const char *)pClientData->get_cvar("s_ServerID"))) pClientData->set_cvar("s_ServerID", "S001-MANT-RA01");
            pLog->_Add("Server ID.......%s", (const char *)pClientData->get_cvar("s_ServerID"));
            if (!strlen((const char *)pClientData->get_cvar("s_ServerAuthor"))) pClientData->set_cvar("s_ServerAdmin", "Admin");
            pLog->_Add("Administrator...%s", (const char *)pClientData->get_cvar("s_ServerAdmin"));
            pLog->_Add("===============================================================");
            pGFX->SetWindowTitle("%s %s(%s) [%s]", MANTRA_NAME, MANTRA_VERSION, DLCS_OS_STRING, pClientData->get_cvar("s_ServerName"));
            SetGameMode(GATHER_GAME_DATA_LOADER);
            break;
            /////////////////////////////

        case GATHER_GAME_DATA_LOADER:
            // MOUSEMODE=MP_SYSTEMBUSY;
            // if((INPUTMODE!=CONSOLE2)&&(pClientData->get_cvar("drawoptions==false)) DrawGenericSurface();
            // pFMGS->dMsgTime=dlcs_get_tickcount();
            // GAME_MODE=WAIT_LOOP;
            // SendData.Reset();
            // SendData.Write((char)NETMSG_GET_LOGIN_PROGRAM);
            // SendData.Write((char)1);
            // SendNetMessage(0);
            break;

        case LOGIN_PROGRAM_START:
            // if((INPUTMODE!=CONSOLE2)&&(pClientData->get_cvar("drawoptions==false)) DrawGenericSurface();
            // wait for login program from the server, it will be fetched in NET_UTIL
            SetGameMode(LOGIN_PROGRAM_END);  // when user completed the program...
            break;

        case LOGIN_PROGRAM_END: SetGameMode(INITIALIZE_GAME); break;

        case INITIALIZE_GAME:
            pGUI->clear();
            pGUI->call("gameon.gui");
            pLog->_Add("Done initializing player...");
            SetGameMode(ITEM_INITIALIZE);
            pGFX->pCamera->rot.y = 144.0f;
            break;

        case ITEM_INITIALIZE:
            pLog->_Add("Done initializing items...");
            SetGameMode(GAME_ON);
            break;

        case GAME_ON:

            pGFX->_glRendermode = GL_RENDER;
            pGFX->RenderScene(pGUI->pMouse->X(), pGUI->pMouse->Y());

            // Keys

            if (pGUI->iKeyUp == SDLK_F12) bShutDown = 1;
            if (pGUI->iKeyDown == SDLK_w) pGFX->pCamera->Move_Forward_Start();
            if (pGUI->iKeyUp == SDLK_w) pGFX->pCamera->Move_Forward_Stop();
            if (pGUI->iKeyDown == SDLK_s) pGFX->pCamera->Move_Backward_Start();
            if (pGUI->iKeyUp == SDLK_s) pGFX->pCamera->Move_Backward_Stop();
            if (pGUI->iKeyDown == SDLK_a) pGFX->pCamera->Move_Left_Start();
            if (pGUI->iKeyUp == SDLK_a) pGFX->pCamera->Move_Left_Stop();
            if (pGUI->iKeyDown == SDLK_d) pGFX->pCamera->Move_Right_Start();
            if (pGUI->iKeyUp == SDLK_d) pGFX->pCamera->Move_Right_Stop();
            if (pGUI->iKeyDown == SDLK_e) pGFX->pCamera->Rotate_Right_Start();
            if (pGUI->iKeyUp == SDLK_e) pGFX->pCamera->Rotate_Right_Stop();
            if (pGUI->iKeyDown == SDLK_q) pGFX->pCamera->Rotate_Left_Start();
            if (pGUI->iKeyUp == SDLK_q) pGFX->pCamera->Rotate_Left_Stop();

            if (pGUI->iKeyDown == SDLK_SPACE) pGFX->pCamera->bMovingUp = true;
            if (pGUI->iKeyUp == SDLK_SPACE) pGFX->pCamera->bMovingUp = false;

            if (pGUI->iKeyDown == SDLK_x) pGFX->pCamera->bMovingDown = true;
            if (pGUI->iKeyUp == SDLK_x) pGFX->pCamera->bMovingDown = false;

            if (pGUI->iKeyUp == SDLK_F11) {
                if (pGFX->bEditEntities)
                    pGFX->bEditEntities = false;
                else
                    pGFX->bEditEntities = true;
            }

            if (pGFX->bEditEntities) {
                pGUI->gPrint(15, (int)pClientData->get_cvar("i_ScreenHeight") - 80, va("^3[^2EDIT ENTITIES^3]"), 3, 1);

                if ((!(pGUI->modstate & KMOD_SHIFT)) && (!(pGUI->modstate & KMOD_CTRL)) && (!(pGUI->modstate & KMOD_ALT))) {
                    if (pGUI->iKeyUp == SDLK_F2) {
                        dlcs_V3 sec;
                        pGFX->SaveEntities(sec);
                    }

                    if (pGUI->iKeyUp == SDLK_INSERT) {
                        // pGFX->ClearSelectEntity();
                        // pGFX->pSelectedEntity=pGFX->MakeEntity("NEW ENTITY",pGFX->pCamera->loc.x,pGFX->pCamera->loc.y,pGFX->pCamera->loc.z);
                        // pGFX->pSelectedEntity->bSelected=true;
                    }

                    if (pGUI->iKeyUp == SDLK_DELETE) {
                        pGFX->DeleteEntity(pGFX->pSelectedEntity);
                        pGFX->pSelectedEntity = 0;
                    }

                    if (pGUI->iKeyUp == SDLK_TAB) pGFX->SelectClosestEntity();
                }

                if (pGFX->pSelectedEntity) {
                    // if(pGUI->iKeyUp==SDLK_F6) pGFX->pSelectedEntity->type=ENTITY_STATIC;
                    // if(pGUI->iKeyUp==SDLK_F8) pGFX->pSelectedEntity->pModel=pGFX->GetRandomModel();
                    // if(pGUI->iKeyUp==SDLK_F7) pGFX->pSelectedEntity->pTexture=pGFX->GetRandomTexture();
                    // if((pGUI->iKeyUp==SDLK_F7)&& (pGUI->modstate & KMOD_SHIFT)) pGFX->pSelectedEntity->pTexture=0;

                    if (pGUI->modstate & KMOD_SHIFT) {
                        if (pGUI->iKeyDown == SDLK_PAGEUP) pGFX->OpRot.bXp = true;
                        if (pGUI->iKeyDown == SDLK_PAGEDOWN) pGFX->OpRot.bXn = true;
                        if (pGUI->iKeyDown == SDLK_HOME) pGFX->OpRot.bYp = true;
                        if (pGUI->iKeyDown == SDLK_END) pGFX->OpRot.bYn = true;
                        if (pGUI->iKeyDown == SDLK_INSERT) pGFX->OpRot.bZp = true;
                        if (pGUI->iKeyDown == SDLK_DELETE) pGFX->OpRot.bZn = true;

                        if (pGUI->iKeyUp == SDLK_PAGEUP) pGFX->OpRot.bXp = false;
                        if (pGUI->iKeyUp == SDLK_PAGEDOWN) pGFX->OpRot.bXn = false;
                        if (pGUI->iKeyUp == SDLK_HOME) pGFX->OpRot.bYp = false;
                        if (pGUI->iKeyUp == SDLK_END) pGFX->OpRot.bYn = false;
                        if (pGUI->iKeyUp == SDLK_INSERT) pGFX->OpRot.bZp = false;
                        if (pGUI->iKeyUp == SDLK_DELETE) pGFX->OpRot.bZn = false;

                        if (pGUI->iKeyUp == SDLK_F9) {
                            pGFX->pSelectedEntity->rot.x     = 0.0f;
                            pGFX->pSelectedEntity->rot.y     = 0.0f;
                            pGFX->pSelectedEntity->rot.z     = 0.0f;
                            pGFX->pSelectedEntity->autorot.x = 0.0f;
                            pGFX->pSelectedEntity->autorot.y = 0.0f;
                            pGFX->pSelectedEntity->autorot.z = 0.0f;
                        }

                        if (pGFX->OpRot.bXp) pGFX->pSelectedEntity->rot.x += 1.3f;
                        if (pGFX->OpRot.bXn) pGFX->pSelectedEntity->rot.x -= 1.3f;
                        if (pGFX->OpRot.bYp) pGFX->pSelectedEntity->rot.y += 1.3f;
                        if (pGFX->OpRot.bYn) pGFX->pSelectedEntity->rot.y -= 1.3f;
                        if (pGFX->OpRot.bZp) pGFX->pSelectedEntity->rot.z += 1.3f;
                        if (pGFX->OpRot.bZn) pGFX->pSelectedEntity->rot.z -= 1.3f;
                    }

                    if (pGUI->modstate & KMOD_ALT) {
                        if (pGUI->iKeyDown == SDLK_PAGEUP) pGFX->OpLoc.bXp = true;
                        if (pGUI->iKeyDown == SDLK_PAGEDOWN) pGFX->OpLoc.bXn = true;
                        if (pGUI->iKeyDown == SDLK_HOME) pGFX->OpLoc.bYp = true;
                        if (pGUI->iKeyDown == SDLK_END) pGFX->OpLoc.bYn = true;
                        if (pGUI->iKeyDown == SDLK_INSERT) pGFX->OpLoc.bZp = true;
                        if (pGUI->iKeyDown == SDLK_DELETE) pGFX->OpLoc.bZn = true;

                        if (pGUI->iKeyUp == SDLK_PAGEUP) pGFX->OpLoc.bXp = false;
                        if (pGUI->iKeyUp == SDLK_PAGEDOWN) pGFX->OpLoc.bXn = false;
                        if (pGUI->iKeyUp == SDLK_HOME) pGFX->OpLoc.bYp = false;
                        if (pGUI->iKeyUp == SDLK_END) pGFX->OpLoc.bYn = false;
                        if (pGUI->iKeyUp == SDLK_INSERT) pGFX->OpLoc.bZp = false;
                        if (pGUI->iKeyUp == SDLK_DELETE) pGFX->OpLoc.bZn = false;

                        if (pGUI->iKeyUp == SDLK_F9) {
                            pGFX->pSelectedEntity->loc.x = pGFX->pCamera->loc.x;
                            pGFX->pSelectedEntity->loc.y = pGFX->pCamera->loc.y;
                            pGFX->pSelectedEntity->loc.z = pGFX->pCamera->loc.z;
                        }

                        if (pGFX->OpLoc.bXp) pGFX->pSelectedEntity->loc.x += 1.3f;
                        if (pGFX->OpLoc.bXn) pGFX->pSelectedEntity->loc.x -= 1.3f;
                        if (pGFX->OpLoc.bYp) pGFX->pSelectedEntity->loc.y += 1.3f;
                        if (pGFX->OpLoc.bYn) pGFX->pSelectedEntity->loc.y -= 1.3f;
                        if (pGFX->OpLoc.bZp) pGFX->pSelectedEntity->loc.z += 1.3f;
                        if (pGFX->OpLoc.bZn) pGFX->pSelectedEntity->loc.z -= 1.3f;
                    }

                    if (pGUI->modstate & KMOD_CTRL) {
                        if (pGUI->iKeyDown == SDLK_PAGEUP) pGFX->OpScale.bXp = true;
                        if (pGUI->iKeyDown == SDLK_PAGEDOWN) pGFX->OpScale.bXn = true;
                        if (pGUI->iKeyUp == SDLK_PAGEUP) pGFX->OpScale.bXp = false;
                        if (pGUI->iKeyUp == SDLK_PAGEDOWN) pGFX->OpScale.bXn = false;

                        if (pGUI->iKeyUp == SDLK_F9) {
                            pGFX->pSelectedEntity->scale.x = 1.0f;
                            pGFX->pSelectedEntity->scale.y = 1.0f;
                            pGFX->pSelectedEntity->scale.z = 1.0f;
                        }
                        if (pGFX->OpScale.bXp) {
                            pGFX->pSelectedEntity->scale.x += 0.002f;
                            pGFX->pSelectedEntity->scale.y += 0.002f;
                            pGFX->pSelectedEntity->scale.z += 0.002f;
                        }
                        if (pGFX->OpScale.bXn) {
                            pGFX->pSelectedEntity->scale.x -= 0.002f;
                            pGFX->pSelectedEntity->scale.y -= 0.002f;
                            pGFX->pSelectedEntity->scale.z -= 0.002f;
                        }
                    }
                }
            }
            break;

        case GAME_LIMBO: SetGameMode(GAME_ON); break;

        case QUIT: break;

        case DEBUG_LOOP: break;

        case WAIT_LOOP: break;

        case EDIT_WORLD_INIT:
            pLog->_Add("Edit World Init Start");
            pGUI->clear();
            pGUI->call("editmenu.gui");
            pGUI->call("guistumpedit.gui");
            pGUI->call("guictrledit.gui");
            pClientData->set_cvar("b_DrawMap", 0);
            pLog->_Add("Edit World Init End");
            SetGameMode(EDIT_WORLD);

        case EDIT_WORLD:
            // pGFX->RenderScene();
            break;

        case EDIT_MODELS_INIT:
            pGUI->clear();
            // do stuff
            SetGameMode(EDIT_MODELS);
            break;

        case EDIT_MODELS: pGUI->gPrint(15, 32, "GAME MODE: EDIT_MODELS", 3, 1); break;

        default: break;
    }

    /////////////////////////////////////////////////////////////////////////////////

    return false;
}
bool doInit(void) {
    /////////////////////////////////////////////////////////////////////////////////
    // Temporary variables

    dlcsm_make_str(temp1);

    /////////////////////////////////////////////////////////////////////////////////
    // Other housekeeping

    bShutDown = false;

    /////////////////////////////////////////////////////////////////////////////////
    // Zeroize pointers

    pClientData = NULL;
    pGFX        = NULL;
    pLog        = NULL;
    pGAF        = NULL;
    pGUI        = NULL;
    pFMGS       = NULL;

    /////////////////////////////////////////////////////////////////////////////////
    // Fill random seed with time for better randomizing

    srand((int)dlcs_get_tickcount());

    /////////////////////////////////////////////////////////////////////////////////
    // Create the Log

    pLog = new CLog("client.log");
    if (!pLog) return FALSE;

    pLog->On();
    pLog->LineFeedsOff();
    pLog->Restart();
    pLog->AddLineSep();

    pLog->_Add(va("Mantra %s %s %s Net Revision(%s)", MANTRA_VERSION, MANTRA_COPYRIGHT, DLCS_OS_STRING, MANTRA_NET_REVISION));
    pLog->_Add("CWD[%s]", dlcs_getcwd());

    /////////////////////////////////////////////////////////////////////////////////
    // Set up network

    pLog->_Add("Setting up NETWORK");
    NET_Init();

    /////////////////////////////////////////////////////////////////////////////////
    // Load in client.ini
    dlcsm_make_str(szTmp);
    dlcs_get_ipaddress(szTmp);
    pLog->_Add("IP Address[%s]", szTmp);
    memset(szTmp, 0, _TEXTNAME_SIZE);
    dlcs_get_hostname(szTmp);
    pLog->_Add("HOSTNAME[%s]", szTmp);
    pLog->_Add("Setting up Client Data");

    pClientData = new CC_Data(va("client.%s.ini", szTmp), pLog);

    if (!pClientData) return FALSE;
    if (!(bool)pClientData->get_cvar("b_Log")) pLog->Off();

    /////////////////////////////////////////////////////////////////////////////////
    // Create GAF File

    pLog->_Add("Setting up GAF");
    pGAF = new CGAF();
    if (!pGAF) return false;
    pGAF->Open("ember.gaf");

    /////////////////////////////////////////////////////////////////////////////////
    // sound
#ifdef _WIN32
    pLog->_Add("Setting up SND");
    pSND = new C_Sound(pLog);
    if (!pSND) {
        pLog->_Add("Sound can not be initialized, turning off sound and music");
        pClientData->set_cvar("b_Music", "0");
        pClientData->set_cvar("b_Sound", "0");
    } else {
        // pSND->InitializeSound();
        // pSND->SetSoundVolume((pClientData->get_cvar("fSoundVolume*255));
        // pSND->SetMusicVolume((pClientData->get_cvar("fMusicVolume*255));
    }

    pSND->SetSoundVolume(255);
    pSND->SetMusicVolume(255);
    // pSND->PlaySample("mc.mp3");
#endif
    /////////////////////////////////////////////////////////////////////////////////
    // Initialize GFX

    pLog->_Add("Setting up GFX");

    pGFX = 0;

    LogEntry(va("i_ScreenWidth=%d,i_Screenheight=%d,i_ScreenColors=%d\n",  //
                pClientData->get_cvar("i_ScreenWidth"),                    //
                pClientData->get_cvar("i_ScreenHeight"),                   //
                pClientData->get_cvar("i_ScreenColors")));                 //

    pGFX = new C_GFX(                                                                                                   //
        (int)pClientData->get_cvar("i_ScreenWidth"),                                                                    //
        (int)pClientData->get_cvar("i_ScreenHeight"),                                                                   //
        (int)pClientData->get_cvar("i_ScreenColors"),                                                                   //
        true,                                                                                                           //
        va("Mantra %s %s %s Net Revision(%s)", MANTRA_VERSION, MANTRA_COPYRIGHT, DLCS_OS_STRING, MANTRA_NET_REVISION),  //
        "base/b0006.png",                                                                                               //
        pLog,                                                                                                           //
        pGAF);

    if (!pGFX) {
        pLog->_Add("GFX initialization failure, quitting");
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////
    // GUI

    pLog->_Add("Setting up GUI");
    pGUI = new C_GUI(pGFX, pGAF, pLog);
    if (!pGUI) {
        pLog->_Add("GUI initialization failure, quitting");
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////

    pGUI->pCons->RegVar("b_shutdown", (void *)&bShutDown);
    pGUI->pCons->set_cvar("s_teststring", "THIS IS A TEST OF THE GLOBAL VARS SYSTEM");
    pGUI->pCons->set_cvar("i_testint", "354");

    pGUI->pCons->RegFunc("functest", (void *)con_functest);
    pGUI->pCons->RegFunc("setgamemode", (void *)con_setgamemode);
    pGUI->pCons->RegFunc("guicall", (void *)con_guicall);
    pGUI->pCons->RegFunc("functest", (void *)con_functest);
    pGUI->pCons->RegFunc("setgamemode", (void *)con_setgamemode);
    pGUI->pCons->RegFunc("guicall", (void *)con_guicall);
    pGUI->pCons->RegFunc("guicallfile", (void *)con_guicallfile);
    pGUI->pCons->RegFunc("guistore", (void *)con_guistore);
    pGUI->pCons->RegFunc("guiclose", (void *)con_guiclose);
    pGUI->pCons->RegFunc("guirender", (void *)con_guirender);
    pGUI->pCons->RegFunc("guictrlrender", (void *)con_guictrlrender);
    pGUI->pCons->RegFunc("guiedit", (void *)con_guiedit);
    pGUI->pCons->RegFunc("guiremovecontrol", (void *)con_guiremovecontrol);
    pGUI->pCons->RegFunc("guicloseprompt", (void *)con_guicloseprompt);
    pGUI->pCons->RegFunc("echo", (void *)con_echo);
    pGUI->pCons->RegFunc("exec", (void *)con_exec);
    pGUI->pCons->RegFunc("bind", (void *)con_bind);
    pGUI->pCons->RegFunc("cvar", (void *)con_cvar);
    pGUI->pCons->RegFunc("quit", (void *)con_quit);
    pGUI->pCons->RegFunc("terraform", (void *)con_terraform);
    pGUI->pCons->RegFunc("tf", (void *)con_terraform);
    pGUI->pCons->RegFunc("chat", (void *)con_chat);
    pGUI->pCons->intmap["MAIN_MENU"]                    = MAIN_MENU;
    pGUI->pCons->intmap["MAIN_MENU_2"]                  = MAIN_MENU_2;
    pGUI->pCons->intmap["RETRO_INTRO_INIT"]             = RETRO_INTRO_INIT;
    pGUI->pCons->intmap["RETRO_INTRO_PLAY"]             = RETRO_INTRO_PLAY;
    pGUI->pCons->intmap["CREDITS"]                      = CREDITS;
    pGUI->pCons->intmap["CONNECT"]                      = CONNECT;
    pGUI->pCons->intmap["CONSOLE_CONNECT"]              = CONSOLE_CONNECT;
    pGUI->pCons->intmap["GLOBAL_OPTIONS"]               = GLOBAL_OPTIONS;
    pGUI->pCons->intmap["LOGIN_AUTO_LOGIN"]             = LOGIN_AUTO_LOGIN;
    pGUI->pCons->intmap["LOGIN_FROM_GUI"]               = LOGIN_FROM_GUI;
    pGUI->pCons->intmap["LOGIN_SCREEN_ENTRY"]           = LOGIN_SCREEN_ENTRY;
    pGUI->pCons->intmap["GATHER_SERVER_LIST"]           = GATHER_SERVER_LIST;
    pGUI->pCons->intmap["CHOOSE_SERVER_INIT"]           = CHOOSE_SERVER_INIT;
    pGUI->pCons->intmap["CHOOSE_SERVER"]                = CHOOSE_SERVER;
    pGUI->pCons->intmap["PROFILE_SELECT"]               = PROFILE_SELECT;
    pGUI->pCons->intmap["CONNECT_TO_SERVER"]            = CONNECT_TO_SERVER;
    pGUI->pCons->intmap["CONNECT_CHECK"]                = CONNECT_CHECK;
    pGUI->pCons->intmap["GET_SERVER_INFORMATION_START"] = GET_SERVER_INFORMATION_START;
    pGUI->pCons->intmap["GET_SERVER_INFORMATION_SPIN"]  = GET_SERVER_INFORMATION_SPIN;
    pGUI->pCons->intmap["GET_CHARACTERS"]               = GET_CHARACTERS;
    pGUI->pCons->intmap["GET_CHARACTERS_SPIN"]          = GET_CHARACTERS_SPIN;
    pGUI->pCons->intmap["GET_CHARACTERS_SEND"]          = GET_CHARACTERS_SEND;
    pGUI->pCons->intmap["GET_CHARACTERS_RECV"]          = GET_CHARACTERS_RECV;
    pGUI->pCons->intmap["CHOOSE_CHARACTER_STAGE"]       = CHOOSE_CHARACTER_STAGE;
    pGUI->pCons->intmap["CHOOSE_CHARACTER"]             = CHOOSE_CHARACTER;
    pGUI->pCons->intmap["CHOOSE_CHARACTER_SPIN"]        = CHOOSE_CHARACTER_SPIN;
    pGUI->pCons->intmap["CREATE_CHARACTER"]             = CREATE_CHARACTER;
    pGUI->pCons->intmap["CREATE_CHARACTER_SPIN"]        = CREATE_CHARACTER_SPIN;
    pGUI->pCons->intmap["CREATE_CHARACTER_SEND"]        = CREATE_CHARACTER_SEND;
    pGUI->pCons->intmap["CREATE_CHARACTER_SEND_SPIN"]   = CREATE_CHARACTER_SEND_SPIN;
    pGUI->pCons->intmap["LOGIN_SEND"]                   = LOGIN_SEND;
    pGUI->pCons->intmap["LOGIN_RECV"]                   = LOGIN_RECV;
    pGUI->pCons->intmap["LOGIN_OK"]                     = LOGIN_OK;
    pGUI->pCons->intmap["LOGIN_FAILED"]                 = LOGIN_FAILED;
    pGUI->pCons->intmap["GATHER_GAME_DATA"]             = GATHER_GAME_DATA;
    pGUI->pCons->intmap["SI_GENERIC_SEND"]              = SI_GENERIC_SEND;
    pGUI->pCons->intmap["SI_GENERIC_RECV"]              = SI_GENERIC_RECV;
    pGUI->pCons->intmap["LOGIN_PROGRAM_START"]          = LOGIN_PROGRAM_START;
    pGUI->pCons->intmap["LOGIN_PROGRAM_END"]            = LOGIN_PROGRAM_END;
    pGUI->pCons->intmap["GATHER_GAME_DATA_LOADER"]      = GATHER_GAME_DATA_LOADER;
    pGUI->pCons->intmap["GET_SELECTED_CHARACTER_SEND"]  = GET_SELECTED_CHARACTER_SEND;
    pGUI->pCons->intmap["GET_SELECTED_CHARACTER_RECV"]  = GET_SELECTED_CHARACTER_RECV;
    pGUI->pCons->intmap["DELETE_CHARACTER_SEND"]        = DELETE_CHARACTER_SEND;
    pGUI->pCons->intmap["DELETE_CHARACTER_RECV"]        = DELETE_CHARACTER_RECV;
    pGUI->pCons->intmap["INITIALIZE_GAME"]              = INITIALIZE_GAME;
    pGUI->pCons->intmap["ITEM_INITIALIZE"]              = ITEM_INITIALIZE;
    pGUI->pCons->intmap["GAME_ON"]                      = GAME_ON;
    pGUI->pCons->intmap["GAME_LIMBO"]                   = GAME_LIMBO;
    pGUI->pCons->intmap["GAME_EDITOR"]                  = GAME_EDITOR;
    pGUI->pCons->intmap["EDIT_WORLD_INIT"]              = EDIT_WORLD_INIT;
    pGUI->pCons->intmap["EDIT_WORLD"]                   = EDIT_WORLD;
    pGUI->pCons->intmap["EDIT_MODELS_INIT"]             = EDIT_MODELS_INIT;
    pGUI->pCons->intmap["EDIT_MODELS"]                  = EDIT_MODELS;
    pGUI->pCons->intmap["QUIT"]                         = QUIT;
    pGUI->pCons->intmap["WAIT_LOOP"]                    = WAIT_LOOP;
    pGUI->pCons->intmap["DEBUG_LOOP"]                   = DEBUG_LOOP;

    pGUI->pCons->_Execute("exec autoexec.cfg");
    pGUI->pCons->_Execute("exec config.cfg");

    SetGameMode(INITIALIZE_GAME);  // RETRO_INTRO_INIT);
    pLog->_Add("******************** FINISHED STARTUP ***************************");

    return true;
}

void ShutDown(void) {
    pLog->_Add("******************** START SHUTDOWN ***************************");

#ifdef DLCS_WINDOWS
    dlcsm_delete(pSND);
    pLog->_Add("Sound shut down");
#endif

    dlcsm_delete(pGUI);
    pLog->_Add("GUI shut down");
    dlcsm_delete(pFMGS);
    NET_Shutdown();
    pLog->_Add("FMGS shut down");

    pClientData->set_cvar("b_FullScreen", pGFX->bFullScreen);
    dlcsm_delete(pGFX);
    pLog->_Add("GFX shut down");
    dlcsm_delete(pGAF);
    pLog->_Add("GAF shut down");

    if (pClientData) {
        pClientData->bSave();
        pLog->_Add("Client data saved...");
        pClientData->CleanUp();
        dlcsm_delete(pClientData);
    }
    pLog->_Add("Client data shut down");
    pLog->_Add("Mantra engine shut down!");
    pLog->AddLineSep();
    dlcsm_delete(pLog);
}
////////////////////////////////////////////////////// NETWORK STUFF
long C_FMGS::Ping(void) {  // ** Ping the server
    CPacket     SendData(NET_DATAGRAMSIZE);
    static long dwPingTimer = dlcs_get_tickcount();

    if (emgeGetState() == NET_NOTCONNECT) return 9998;
    if (dlcs_get_tickcount() - dwPingTimer > 5000) {
        dwPingTimer = dlcs_get_tickcount();
        SendData.Reset();
        SendData.Write((char)NETMSG_PING);
        SendData.Write((long)dlcs_get_tickcount());
        SendUnreliableMessage(&SendData);
    }
    return dwPing;
}
void C_FMGS::DoNetwork(void) {  // ** Network Loop

    char temp1[1024];
    memset(temp1, 0, 1024);
    char temp2[1024];
    memset(temp2, 0, 1024);
    char temp3[1024];
    memset(temp3, 0, 1024);
    char temp4[1024];
    memset(temp4, 0, 1024);
    char szTemp1[1024];
    memset(szTemp1, 0, 1024);
    char szTemp2[1024];
    memset(szTemp2, 0, 1024);
    char szTemp3[1024];
    memset(szTemp3, 0, 1024);
    char szTemp4[1024];
    memset(szTemp4, 0, 1024);

    CPacket  SendData(NET_DATAGRAMSIZE);
    CPacket *RecvData;

    int           iState, i, ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz, ex, ey, ez, fx, fy, fz;
    long          bytetimer, gx, gy, gz;
    unsigned char vx, vy, vz;
    char          NetMessage;

    ax = ay = az = bx = by = bz = cx = cy = cz = dx = dy = dz = ex = ey = ez = fx = fy = fz = gx = gy = gz = vx = vy = vz = 0;

    bytetimer = dlcs_get_tickcount();

    iState = emgeGetState();

    if (iState == NET_NOTCONNECT) return;  // If you aren't connected, then return. No sense in trying to read packets if they aren't there.

    if ((iState != NET_LOGGINPROC) && (iState != NET_SYSBUSY) && (iState != NET_CONNECTED)) return;

    // if( (iState == NET_LOGGINPROC) || (iState == NET_SYSBUSY) ) return;

    NetMessage = iGetMessage();  // Update the Network once per frame here

    if (NetMessage == -1) {
        pGUI->prompt("Lost connection to server.", "disconnect");
        emgSetState(NET_NOTCONNECT);
        SetGameMode(MAIN_MENU);
        return;
    }

    if (NetMessage) {
        RecvData = pGetMessage();

        cLastNetMessage = RecvData->cRead();

        pLog->_Add("GOT NETMESSAGE [%d] [%d]", NetMessage, cLastNetMessage);

        switch (cLastNetMessage) {
            case NETMSG_PING: dwPing = dlcs_get_tickcount() - RecvData->dwRead(); break;

            case NETMSG_SERVERINFORMATION_GET:

                ax = RecvData->cRead();
                // Log("%d) Races",ax);
                for (i = 0; i < ax; i++) {
                    ay = RecvData->cRead();
                    // strcpy(pClientData->get_cvar("ServerInfo->Race[ay].Text,RecvData->pRead());
                    // Log("  %d) %s",ay,pClientData->get_cvar("ServerInfo->Race[ay].Text);
                }
                ax = RecvData->cRead();
                // Log("%d) Classes",ax);
                for (i = 0; i < ax; i++) {
                    ay = RecvData->cRead();
                    // strcpy(pClientData->get_cvar("ServerInfo->Class[ay].Text,RecvData->pRead());
                    // Log("  %d) %s",ay,pClientData->get_cvar("ServerInfo->Class[ay].Text);
                }
                // SetGameMode(GET_CHARACTERS);
                break;

            case NETMSG_RETRIEVECHARS:

                // Log("NETMSG_RETRIEVECHARS!");

                // pClientData->get_cvar("ClearCharacters();
                /*
                                                ax=RecvData->cRead();
                                                for(i=0;i<ax;i++)
                                                {
                                                        strcpy(temp1,RecvData->pRead());// name
                                                        strcpy(temp2,RecvData->pRead());// level
                                                        strcpy(temp3,RecvData->pRead());// race
                                                        strcpy(temp4,RecvData->pRead());// class
                                                        strcpy(szTemp1,RecvData->pRead());// gender

                                                        strcpy(pClientData->get_cvar("ServerCharacter[i].t_name,temp1);
                                                        pClientData->get_cvar("ServerCharacter[i].t_level = atoi(temp2);
                                                        pClientData->get_cvar("ServerCharacter[i].t_race  = atoi(temp3);
                                                        pClientData->get_cvar("ServerCharacter[i].t_class = atoi(temp4);
                                                        pClientData->get_cvar("ServerCharacter[i].t_gender= atoi(szTemp1);

                                                }
                */
                SetGameMode(CHOOSE_CHARACTER);

                break;

            case NETMSG_LOGIN_REQUEST_REPLY:

                pLog->_Add("Got a login request reply");

                if ((int)pClientData->get_cvar("i_GAME_MODE") != LOGIN_RECV) break;

                ax = RecvData->cRead();

                if ((ax == GOOD_LOGIN) || (ax == NEW_ACCOUNT)) {
                    pLog->_Add(" >>> It's a good login!");

                    pClientData->set_cvar("s_ServerMessage", RecvData->pRead());
                    pClientData->set_cvar("s_ServerVersion", RecvData->pRead());  // Extract Server's Version
                    pClientData->set_cvar("s_session_id", RecvData->pRead());
                    pClientData->set_cvar("s_ServerID", RecvData->pRead());         // Extract Server Resource ID
                    pClientData->set_cvar("s_ServerName", RecvData->pRead());       // Extract Server Name
                    pClientData->set_cvar("s_ServerAdmin", RecvData->pRead());      // Extract Server Admin
                    pClientData->set_cvar("i_Access", RecvData->iRead());           // Extract Player's Access level
                    pClientData->set_cvar("s_AccessName", RecvData->pRead());       // Extract Player's Access level name
                    pClientData->set_cvar("uc_CharacterSlots", RecvData->cRead());  // Extract Server num of character slots

                    pLog->_Add("%s", (const char *)pClientData->get_cvar("s_ServerMessage"));
                    pLog->_Add("%s running %s ID[%s] Admin[%s] Character Slots[%d]", (const char *)pClientData->get_cvar("s_ServerName"), (const char *)pClientData->get_cvar("s_ServerVersion"), (const char *)pClientData->get_cvar("s_ServerID"), (const char *)pClientData->get_cvar("s_ServerAdmin"),
                               (const char *)va("%d", (int)pClientData->get_cvar("uc_CharacterSlots")));

                    pLog->_Add("session_id[%s] and you have access[%d(%s)]", (const char *)pClientData->get_cvar("s_session_id"), (int)pClientData->get_cvar("i_Access"), (const char *)pClientData->get_cvar("s_AccessName"));

                    // if(pClientData->get_cvar("CharacterSlots>MAX_TOONS)
                    // pClientData->get_cvar("CharacterSlots   =   MAX_TOONS;

                    SetGameMode(GATHER_GAME_DATA);  // GET_SERVER_INFORMATION_START;
                    emgSetState(NET_CONNECTED);
                } else {
                    pLog->_Add(" >>> It's a bad login!");
                    SetGameMode(CHOOSE_SERVER_INIT);
                    if (ax != MASTER_LOGIN) pGUI->prompt(RecvData->pRead(), (char *)"nop");
                }
                break;

            case NETMSG_HEARTBEAT:
                // just return the heartbeat... no other thing needs to be done...
                SendData.Reset();
                SendData.Write((char)NETMSG_HEARTBEAT);
                SendData.Write((int)1);
                SendUnreliableMessage(&SendData);
                break;

            case NETMSG_CLIENT_SHUTDOWN:

                strcpy(temp2, RecvData->pRead());  // client session_id
                strcpy(temp1, RecvData->pRead());
                pGUI->prompt((char *)va("Server has shut down this client. Reason: %s", temp1), (char *)"nop");

                // ChatBufferAdd(255,0,0,"SYSTEM","Server has shut down this client. Reason[%s]",temp1);

                pLog->_Add("Server has shut down this client. Reason: %s", temp1);
                SetGameMode(LOGIN_SCREEN_ENTRY);
                break;

            case NETMSG_CHAT:
                // Global Chat / System Message

                strcpy(temp1, RecvData->pRead());  // message
                strcpy(temp2, RecvData->pRead());  // username
                ax = RecvData->iRead();            // channel
                strcpy(temp3, RecvData->pRead());  // session_id

                switch (ax) {  // channel
                    case CHANNEL_CUSTOM:
                    case CHANNEL_CONSOLE:
                    case CHANNEL_LOCAL:
                    case CHANNEL_TRADE:
                    case CHANNEL_GENERAL:
                    case CHANNEL_WHISPER:
                    case CHANNEL_PARTY:
                    case CHANNEL_RAID:
                    case CHANNEL_SAY:
                    case CHANNEL_YELL:
                    case CHANNEL_CLAN:
                    case CHANNEL_SYSTEM: pLog->_Add("NETMSG_MESSAGE channel[%d] [%s][%s] ", ax, temp2, temp1); pGUI->addChat(ax, temp2, temp1);

                    default: break;
                }
                break;

            case NETMSG_MODIFY_CHARACTER: break;

            case NETMSG_MOVEPLAYER: break;

            case NETMSG_FILE_XFER:

                char *pFileBuf;
                // char pFileBuf[NET_FILE_XFER_BLOCK_SIZE+1];
                // memset(pFileBuf,0,1024);
                FILE *pFile;
                bool  bFileFail;

                switch (RecvData->cRead()) {
                    case NET_FILE_START:

                        // filename
                        // filesize
                        // start file

                        strcpy(szTemp1, RecvData->pRead());
                        cx = RecvData->iRead();

                        bFileFail = false;
                        pFile     = fopen(szTemp3, "wb");
                        if (pFile) {
                            fclose(pFile);
                            pFile = fopen(szTemp2, "wt");
                            if (pFile) {
                                bx = 1;
                                fputs(va("%d\n%d\n", bx, cx), pFile);
                                fclose(pFile);
                                SendData.Reset();
                                SendData.Write((char)NETMSG_FILE_XFER);
                                SendData.Write((char *)szTemp1);
                                SendData.Write((char)NET_FILE_START_OK);
                                SendUnreliableMessage(&SendData);
                            } else
                                bFileFail = true;
                        } else
                            bFileFail = true;
                        if (bFileFail) {
                            SendData.Reset();
                            SendData.Write((char)NETMSG_FILE_XFER);
                            SendData.Write((char *)szTemp1);
                            SendData.Write((char)NET_FILE_ERROR);
                            SendUnreliableMessage(&SendData);
                        }

                        break;

                    case NET_FILE_DATA:

                        // sequence number
                        // filename
                        // data block (1024)

                        ax = RecvData->iRead();              // sequence number
                        strcpy(szTemp1, RecvData->pRead());  // filename
                        pFileBuf = (char *)RecvData->pRead(NET_FILE_XFER_BLOCK_SIZE);

                        bFileFail = false;
                        pFile     = fopen(szTemp2, "rb");
                        if (pFile) {
                            fgets(szTemp4, 256, pFile);
                            bx = atoi(szTemp4);
                            fgets(szTemp4, 256, pFile);
                            cx = atoi(szTemp4);
                            fclose(pFile);
                        } else
                            bFileFail = true;
                        if (ax == bx) {
                            pFile = fopen(szTemp3, "a");
                            if (pFile) {
                                fwrite(pFileBuf, NET_FILE_XFER_BLOCK_SIZE, 1, pFile);
                                fclose(pFile);
                                pFile = fopen(szTemp2, "wt");
                                if (pFile) {
                                    bx++;
                                    fputs(va("%d\n%d\n", bx, cx), pFile);
                                    fclose(pFile);
                                }

                                SendData.Reset();
                                SendData.Write((char)NETMSG_FILE_XFER);
                                SendData.Write((char)NET_FILE_DATA_OK);
                                SendData.Write((char *)szTemp1);
                                SendData.Write((int)bx);
                            } else {
                                bFileFail = true;
                            }
                        }
                        if (bFileFail) {
                            SendData.Reset();
                            SendData.Write((char)NETMSG_FILE_XFER);
                            SendData.Write((char)NET_FILE_DATA_RESEND);
                            SendData.Write((char *)szTemp1);
                            SendData.Write((int)bx);
                        }
                        break;

                    case NET_FILE_EOF:

                        // sequence number
                        // filename
                        // size of data block
                        // data block

                        ax = RecvData->iRead();              // sequence number
                        strcpy(szTemp1, RecvData->pRead());  // filename
                        dx       = RecvData->iRead();
                        pFileBuf = (char *)RecvData->pRead(dx);

                        bFileFail = false;
                        pFile     = fopen(szTemp3, "a");
                        if (pFile) {
                            fwrite(pFileBuf, dx, 1, pFile);
                            fclose(pFile);
                            remove(szTemp2);
                        } else {
                            SendData.Reset();
                            SendData.Write((char)NETMSG_FILE_XFER);
                            SendData.Write((char)NET_FILE_EOF_RESEND);
                            SendData.Write((char *)szTemp1);
                        }
                        break;

                    case NET_FILE_RESUME: break;

                    case NET_FILE_ACK: break;

                    case NET_FILE_ABORT:
                        break;

                        // NET_FILE_RES_MEDIA
                        // NET_FILE_RES_SCRIPT

                    case NET_FILE_NOP: break;

                    default: break;
                }
                break;

            default:

                // Do nothing... (should never execute this code, but who knows...)
                // DLog("Recieved a NETMSG (%d) but could not recognize it.",cLastNetMessage);
                // ChatBufferAdd(255,0,0,"SYSTEM","Recieved a NETMSG (%d) but could not recognize it.",cLastNetMessage);

                break;
        }
    }
}
C_FMGS::C_FMGS(void) {
    pPingSocket = NULL;
    bLoggedin   = false;
    bSystemBusy = false;
    dwPing      = 999;
    spin_timer  = new CTimer(5000);
    initSocket();
}
C_FMGS::~C_FMGS() {
    dlcsm_delete(spin_timer);
    emgDisconnect();
    dlcsm_delete(pPingSocket);
}
int C_FMGS::emgConnect(char *pHost, char *pPort, char *pUser, char *pPasswd) {
    // int errnum;

    pLog->_Add(" ********************* CONNECT START ********************* ");
    pLog->_Add("C_FMGS::emgConnect(%s,%s,%s,%s);", pHost, pPort, pUser, pPasswd);

    ToAddr.sin_addr.s_addr = inet_addr(pHost);
    ToAddr.sin_port        = htons(atoi(pPort));

    bConnected = false;
    long               fTryTime;
    int                iTry, ret = 0, iLength, iFlags, iT;
    struct sockaddr_in RetAddr;
    CPacket            Send(NET_DATAGRAMSIZE);
    CPacket            Recv(NET_DATAGRAMSIZE);
    bLoggedin = false;
    if (emgeGetState() == NET_NOTCONNECT) return false;
    bCanSend = true;
    CloseSocket(iSocket);
    iSocket = zOpenSocket(0);

    if (iSocket == SOCKET_ERROR) {
        pLog->_Add(" ********************* NO SOCKET ********************* ");
    }

    pLog->_Add("C_FMGS::Connect() zOpenSocket passed");

    if (iSocket == -1) {
        pLog->_Add("C_FMGS::Connect() Can't open network socket!");
        return false;
    }

    bConnected = false;

    ToAddr.sin_addr.s_addr = inet_addr(pHost);
    ToAddr.sin_port        = htons(atoi(pPort));

    pLog->_Add("%s:%d", pGetRemoteIPAddress(), iGetRemotePort());

    bCanSend = false;

    Send.Reset();
    Send.Write(0);
    Send.Write(0);  // save header space which is filled in later
    Send.Write((char)CTL_CONNECT);
    Send.Write("MANTRA");
    Send.Write(atoi(MANTRA_NET_REVISION));
    FinishCtlPacket(&Send);

    for (iTry = 0; iTry < (int)dConnectRetry; iTry++) {
        if (nSend(iSocket, Send.pGetPacketBuffer(), Send.iGetCurSize(), (struct sockaddr *)&ToAddr) == -1) {
            pLog->_Add("C_FMGS::Connect() nSend Failed [%s]", NET_pGetLastError());
            return false;
        }

        fTryTime = dlcs_get_tickcount();

        do {
            ret = nRecv(Recv.pGetPacketBuffer(), Recv.iGetMaxSize(), (struct sockaddr *)&RetAddr);
            if (ret == -1) {
                pLog->_Add("C_FMGS::Connect() -> connection read failed");
                return false;
            }
            if (ret > 0) {
                Recv.SetCurSize(ret);
                iLength = ntohl(Recv.iRead());
                iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
                iLength &= NET_FLAG_LENGTH_MASK;
                if (!(iFlags & NET_FLAG_CTL)) {
                    ret = 0;
                    continue;
                }
                if (iLength != ret) {
                    ret = 0;
                    continue;
                }
                if (iLength < (int)(sizeof(int) * 2 + sizeof(char) * 2)) {
                    ret = 0;
                    continue;
                }
                break;
            }
        } while ((ret == 0) && ((dlcs_get_tickcount() - fTryTime) < dConnectTimeOut));
        if (ret) break;
    }

    if (!ret) return false;

    Recv.iRead();
    char ctltype = Recv.cRead();
    if (ctltype == CTL_REJECT) return false;
    if (ctltype != CTL_ACCEPT) return false;
    pLog->_Add("Got a CTL_ACCEPT here");
    bConnected = true;
    memcpy(&ToAddr, &RetAddr, sizeof(sockaddr));
    iT              = Recv.iRead();
    ToAddr.sin_port = htons(iT);
    // CloseSocket(iSocket);
    // iSocket=zOpenSocket(iT);
    pLog->_Add("SOCKET PORT SET TO (i=%d)%d", iT, ntohs(ToAddr.sin_port));
    bConnected = true;
    bCanSend   = true;
    emgSetState(NET_CONNECTED);
    Send.Reset();
    Send.Write((char)NETMSG_LOGIN_REQUEST);
    Send.Write((char *)pUser);
    Send.Write((char *)pPasswd);
    SendUnreliableMessage(&Send);  // Log in

    return 1;
}
void C_FMGS::emgDisconnect() {
    pLog->_Add("Disconnecting (%d) %s:%d", iSocket, pGetRemoteIPAddress(), iGetRemotePort());

    CPacket SendData(NET_DATAGRAMSIZE);
    SendData.Reset();
    SendData.Write((char)NETMSG_LOGOUT);
    // int retval;
    // retval=
    SendUnreliableMessage(&SendData);
    // pLog->_Add("C_FMGS::Disconnect() (%d) [%s]", retval  , NET_pGetLastError());
}
void C_FMGS::emgSendNetMessage(char cMethod) {
    /*  if(emgeGetState()==NET_NOTCONNECT) return;
        switch(cMethod)
        {
            case 1: // Unreliable
                //pSocket->
                SendUnreliableMessage((CPacket *)&SendData);
                dMsgTime=dlcs_get_tickcount();
                break;

            case 0: // Reliable
                while(! //pSocket->
                    bCanSendMessage()) DoNetwork();

                // pSocket->
                SendReliableMessage((CPacket *)&SendData);
                dMsgTime=dlcs_get_tickcount();
                break;

    //            case 2: // Buffered
    //                while(!pSocket->bCanSendMessage()) DoNetwork();
    //                pSocket->SendBufferedMessage((CPacket *)&SendData);
    //                dMsgTime=dlcs_get_tickcount();
    //                break;

            default:
                //pSocket->
                SendUnreliableMessage((CPacket *)&SendData);
                dMsgTime=dlcs_get_tickcount();
                break;
        }
        */
}
int C_FMGS::emgiGetMessage() {
    int i;
    if (emgeGetState() == NET_NOTCONNECT) return false;
    i = iGetMessage();
    if (i == -1) i = 0;
    return i;
}
eConState C_FMGS::emgeGetState() {
    if (!this) return NET_NOTCONNECT;
    if (!bLoggedin) return NET_LOGGINPROC;
    if (bSystemBusy) return NET_SYSBUSY;
    return NET_CONNECTED;
}
void C_FMGS::emgSetState(eConState eNewState) {
    if (eNewState == NET_CONNECTED) bLoggedin = true;

    if (eNewState == NET_NOTCONNECT) {
        bLoggedin = false;
    }
    if (eNewState == NET_SYSBUSY) bSystemBusy = true;
}
long C_FMGS::emgPing(char *pHost, char *pPort, long &dwPing, bool &bPinging, long &dwFuncTimer, long &dwWaitTimer) {
    // static bool bPinging=false;
    // static long dwFuncTimer=dlcs_get_tickcount();
    // static long dwWaitTimer=dlcs_get_tickcount();

    int     ret, iLength, iFlags;
    CPacket Send(NET_DATAGRAMSIZE);
    CPacket Recv(NET_DATAGRAMSIZE);

    // CInetAddress
    struct sockaddr RetAddr;

    if (!pPingSocket) {
        pPingSocket = new CCSocket();
        bPinging    = false;
    }
    if (!pPingSocket) {
        bPinging = false;
        return dwPing;
    }
    if (!bPinging) {
        if (dlcs_get_tickcount() - dwFuncTimer < 5000) {
            bPinging = false;
            return dwPing;
        }
        dwFuncTimer           = dlcs_get_tickcount();
        pPingSocket->bCanSend = true;

        pPingSocket->OpenSocket(pHost, atoi(pPort));

        if (pPingSocket->iSocket >= 0) {
            // pPingSocket->NET_SetSocketPort(&pPingSocket->stAddr,atoi(pPort));
            pPingSocket->bConnected = false;
            pPingSocket->bCanSend   = false;
            Send.Reset();
            Send.Write(0);  // save header space which is filled in later
            Send.Write(0);
            Send.Write((char)CTL_PING);
            Send.Write((long)dlcs_get_tickcount());
            Send.Write(atoi(MANTRA_NET_REVISION));
            pPingSocket->FinishCtlPacket(&Send);
            pPingSocket->nSend(Send.pGetPacketBuffer(), Send.iGetCurSize(), (struct sockaddr *)&pPingSocket->ToAddr);
            bPinging    = true;
            dwWaitTimer = dlcs_get_tickcount();
        }
    } else {
        ret = pPingSocket->nRecv(Recv.pGetPacketBuffer(), Recv.iGetMaxSize(), &RetAddr);
        if (ret == -1) ret = 0;
        if (ret > 0) {
            Recv.SetCurSize(ret);
            iLength = ntohl(Recv.iRead());
            iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
            iLength &= NET_FLAG_LENGTH_MASK;
            if (!(iFlags & NET_FLAG_CTL)) ret = 0;
            if (iLength != ret) ret = 0;
            if (iLength < (int)(sizeof(int) * 2 + sizeof(char) * 2)) ret = 0;
        }
        if (ret == 0) {
            if (dlcs_get_tickcount() - dwWaitTimer > 5000) {
                dwPing   = 9994;
                bPinging = false;
            }
        } else {
            Recv.iRead();
            char ctltype = Recv.cRead();
            if (ctltype == CTL_PING) {
                dwPing   = (dlcs_get_tickcount() - Recv.dwRead());
                bPinging = false;
            }
        }
    }
    return dwPing;
}
void C_FMGS::GetFile(char *filename) {  // ** Get a file from server
    CPacket SendData(NET_DATAGRAMSIZE);
    SendData.Reset();
    SendData.Write((char)NETMSG_FILE_XFER);
    SendData.Write((char)NET_FILE_START);
    SendData.Write((char *)filename);
    SendUnreliableMessage(&SendData);
}
void C_FMGS::Chat(char *msg, char *name, int channel) {
    CPacket SendData(NET_DATAGRAMSIZE);

    SendData.Reset();
    SendData.Write((char)NETMSG_CHAT);
    SendData.Write((char *)msg);
    SendData.Write((char *)name);
    SendData.Write((int)channel);
    // SendData.Write((char *)session_id);
    SendUnreliableMessage(&SendData);
}
////////////////////////////////////////////////////////////////////////////////////////
void CLog::_Add(const char *fmt, ...) {
    char    ach[1024];
    va_list va;
    va_start(va, fmt);
    vsprintf(ach, fmt, va);
    va_end(va);
    if (ach[strlen(ach) - 1] != '\n') strcat(ach, "\n");
    AddEntry(ach);
    if (pGUI) pGUI->consEntry(ach);
}

void CLog::_DebugAdd(const char *fmt, ...) {
    if (!bDebug) return;
    char    ach[1024];
    va_list va;
    va_start(va, fmt);
    vsprintf(ach, fmt, va);
    va_end(va);
    if (ach[strlen(ach) - 1] != '\n') strcat(ach, "\n");
    AddEntry(ach);
    if (pGUI) pGUI->consEntry(ach);
}

////////////////////////////////////////////////////////////////////////////////////////
GAF_SCANCALLBACK GAF_Scanner(GAFFile_ElmHeader *ElmInfo, LPSTR FullPath) {
    switch (ElmInfo->Type) {
        case GAFELMTYPE_FILE: pLog->_Add("FILE: %25s %d", ElmInfo->Name, ElmInfo->FileSize); break;
        case GAFELMTYPE_DIR: pLog->_Add("<DIR> %25s ", ElmInfo->Name); break;
        default: break;
    }
    // return false;
    return 0;
}
