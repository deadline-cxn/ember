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

// includes

#include "ember_game_client.h"

int         testint;
char        testingstring[1024];
bool        bShutDown;
CC_Data     *pClientData;
C_GFX       *pGFX;      // SDL / OpenGL
C_FM_GUI    *pGUI;      // OpenGL GUI
CGAF        *pGAF;      // Game Archive File (GAF)
CLog        *pLog;      // Log file
// C_Sound     *pSND;      // FMOD Sounds
c_var       *pVar;
C_FMGS      *pFMGS;     // Game Server connection
//lua_State   *L;

// Console Functions

char *con_getint_str(int n)
{
    char temp[1024]; memset(temp,0,1024);
    map <string, int>::iterator ii;
    for( ii=pGUI->pConsole->intmap.begin(); ii!=pGUI->pConsole->intmap.end(); ++ii)
    {
        if( ( (*ii).second ) == (n) )
        {
            strcat(temp,va("%s ",(const char *)(*ii).first.c_str()));
        }
    }
    while(temp[strlen(temp)-1]==' ') temp[strlen(temp)-1]=0;
    return strdup(temp);
}

int con_getint(const string &s)
{
    if( pGUI->pConsole->intmap.find(s.c_str()) != pGUI->pConsole->intmap.end() )
    {
        return (pGUI->pConsole->intmap.find(s.c_str())->second);
    }
    return 0;
}

void con_functest(const string &s)
{
    pLog->_Add("functest(%s)",(char *)s.c_str());
}

void con_setgamemode(const string &s)
{
    pClientData->GAME_MODE = (MODE) con_getint(s.c_str());
}

void con_guigo(const string &s)
{
    C_FM_GUI_CTRL_DATA *pGD;
    pGUI->parse((char *)s.c_str());
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
                    pGUI->pConsole->_Execute(va("setgamemode %s",pGD->name));
                    return;
                }
            }
        }
    }
}

void con_guicall(const string &s)
{
    pLog->_Add("con_guicall(%s)",s.c_str());
    pGUI->call((char *)s.c_str());
}

void con_echo(const string &s)
{
    pLog->_Add((char *)s.c_str());
}

void con_exec(const string &s)
{
    // pLog->_Add("Exec [%s]:",s.c_str());
    char temp[1024];memset(temp,0,1024);
    FILE *fp;
    fp=fopen(s.c_str(),"r");
    if(!fp)
    {
        pLog->_Add("Usage : Can't find file: %s",s.c_str());
        return;
    }
    while(1)
    {
        if(!fgets(temp,_MAX_PATH,fp)) break;
        pGUI->pConsole->_Execute(temp);
    }
    fclose(fp);
}

void con_bind(const string &s)
{
    int key;
    vector <string> vs;
    vector <string> v2;
    char ss[1024]; memset(ss,0,1024);
    char ls[1024]; memset(ls,0,1024);
    strcpy(ls,s.c_str());
    bool bQuotes;
    bQuotes=0;
    for(key=0;key<strlen(ls);key++)
    {
        ls[key]=tolower(ls[key]);
        if(ls[key]=='\"') bQuotes=1;
    }
    pLog->_Add("bind [%s]",ls);

    if(bQuotes)
    {
        //pLog->_Add("[[[%s]]]",ls);
        vs=explode("\"",ls);
        strcpy(ls,vs[1].c_str());

        v2=explode(" ",vs[0].c_str());
        strcpy(ss,v2[0].c_str());
        //pLog->_Add("[[[%s]]] [[[%s]]]",ss,ls);
    }
    else
    {
        //pLog->_Add("[[[%s]]]",ls);
        vs=explode(" ",ls);
        strcpy(ls,"");
        for(key=1;key<vs.size();key++)
        {
            strcat(ls,va("%s ",vs[key].c_str()));
        }
        strcpy(ss,vs[0].c_str());
        //pLog->_Add("What the fudge [%s] [%s]",ss,ls);
    }

    while(ss[strlen(ss)-1]==' ') ss[strlen(ss)-1]=0;
    while(ls[strlen(ls)-1]==' ') ls[strlen(ls)-1]=0;

    key=con_getint((char *)ss);
    //pLog->_Add(" BINDING KEY [%d (%s)]->[%s]",key,con_getint_str(key),ls);
    pGUI->KeyMap[(SDLKey)key]=(char *)ls;
}

void con_cvar(const string &s)
{
    vector <string> vs;
    int i,ivartype;
    char temp[1024]; memset(temp,0,1024);
    ivartype=pGUI->pConsole->get_cvartype(s.c_str());
    if(ivartype==CVAR_NULL)
    {
        pLog->_Add("CVAR: Can not determine cvar [%s] type",s.c_str() );
        return;
    }

    pLog->_Add("CVAR: Type[%s]",pGUI->pConsole->get_cvartype_string(ivartype));

    vs=explode("=",s.c_str());

    if(vs.size()>1)
    {
        for(i=0;i<vs.size();i++)
        {
            pLog->_Add("CVAR: exploded = vs[%d]=[%s]",i,vs[i].c_str());
        }

        for( pGUI->pConsole->svm_i=pGUI->pConsole->varmap.begin(); pGUI->pConsole->svm_i!=pGUI->pConsole->varmap.end(); ++pGUI->pConsole->svm_i)
        {
            if( dscc( ((*pGUI->pConsole->svm_i).first.c_str()), vs[0].c_str()) )
            break;
        }

        switch(ivartype)
        {
            case CVAR_BOOL:
                (*(bool *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case CVAR_INT:
                    (*(int *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case CVAR_UINT:
                    (*(unsigned int *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case CVAR_CHAR:
                    (*(char *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case CVAR_UCHAR:
                    (*(unsigned char *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case CVAR_FLOAT:
                    (*(float *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case CVAR_LONG:
                    (*(long *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case CVAR_ULONG:
                    (*(unsigned long *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;


            case CVAR_STRING:
                    strcpy((char *)(*pGUI->pConsole->svm_i).second, vs[1].c_str());
                break;
        }

    }

    if(pGUI->pConsole->varmap.find(s.c_str())!=pGUI->pConsole->varmap.end())
    {
        switch(ivartype)
        {
            case CVAR_BOOL:
                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
                    (*(bool *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                break;

            case CVAR_INT:
                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
                    (*(int *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case CVAR_UINT:
                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
                    (*(unsigned int *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case CVAR_CHAR:
                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
                    (*(char *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case CVAR_UCHAR:
                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
                    (*(unsigned char *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case CVAR_FLOAT:
                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
                    (*(float *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case CVAR_LONG:
                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
                    (*(long *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case CVAR_ULONG:
                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
                    (*(unsigned long *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case CVAR_STRING:
                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
                     ( pGUI->pConsole->varmap.find(s.c_str())->second)));
                break;

            default:
                strcpy(temp,"UNKNOWN");
            break;
        }
        pLog->_Add("CVAR: [%s]=[%s]",s.c_str(),temp);

    }
}

/***********************************************************************************/

//int C_FM_GUI::* fuk;

void C_EM_CONSOLE::_GlobalVars()
{

    pVar->add("yourmom",&bShutDown);

    varmap.clear();

    bShutDown=0;
    varmap["b_shutdown"]    = &bShutDown;

    strcpy(testingstring,"THIS IS A TEST OF THE GLOBAL VARS SYSTEM");
    varmap["s_teststring"]  = &testingstring;

    testint=354;
    varmap["i_testint"]     = &testint;

 //   int C_FM_GUI::*fuk = (&C_FM_GUI::stump_count);
//    varmap["i_stumps"]      = pGUI->*fuk;
}

void C_EM_CONSOLE::_GlobalStrings()
{
}

void C_EM_CONSOLE::_GlobalIntegers()
{
    intmap.clear();
    intmap["MAIN_MENU"]         = MAIN_MENU;
    intmap["MAIN_MENU_2"]       = MAIN_MENU_2;
    intmap["RETRO_INTRO_INIT"]  = RETRO_INTRO_INIT;
    intmap["RETRO_INTRO_PLAY"]  = RETRO_INTRO_PLAY;
    intmap["CREDITS"]           = CREDITS;
    intmap["CONNECT"]           = CONNECT;
    intmap["CONSOLE_CONNECT"]   = CONSOLE_CONNECT;
    intmap["GLOBAL_OPTIONS"]    = GLOBAL_OPTIONS;
    intmap["LOGIN_AUTO_LOGIN"]  = LOGIN_AUTO_LOGIN;
    intmap["LOGIN_FROM_GUI"]    = LOGIN_FROM_GUI;
    intmap["LOGIN_SCREEN_ENTRY"]= LOGIN_SCREEN_ENTRY;
    intmap["GATHER_SERVER_LIST"]= GATHER_SERVER_LIST;
    intmap["CHOOSE_SERVER_INIT"]= CHOOSE_SERVER_INIT;
    intmap["CHOOSE_SERVER"]     = CHOOSE_SERVER;
    intmap["PROFILE_SELECT"]    = PROFILE_SELECT;
    intmap["CONNECT_TO_SERVER"] = CONNECT_TO_SERVER;
    intmap["CONNECT_CHECK"]     = CONNECT_CHECK;
	intmap["GET_SERVER_INFORMATION_START"] = GET_SERVER_INFORMATION_START;
	intmap["GET_SERVER_INFORMATION_SPIN"]  = GET_SERVER_INFORMATION_SPIN;
    intmap["GET_CHARACTERS"]     = GET_CHARACTERS;
	intmap["GET_CHARACTERS_SPIN"]= GET_CHARACTERS_SPIN;
    intmap["GET_CHARACTERS_SEND"]= GET_CHARACTERS_SEND;
    intmap["GET_CHARACTERS_RECV"]= GET_CHARACTERS_RECV;
	intmap["CHOOSE_CHARACTER_STAGE"]=CHOOSE_CHARACTER_STAGE;
	intmap["CHOOSE_CHARACTER"]  = CHOOSE_CHARACTER;
    intmap["CHOOSE_CHARACTER_SPIN"]= CHOOSE_CHARACTER_SPIN;
	intmap["CREATE_CHARACTER"]  = CREATE_CHARACTER;
	intmap["CREATE_CHARACTER_SPIN"]= CREATE_CHARACTER_SPIN;
    intmap["CREATE_CHARACTER_SEND"]= CREATE_CHARACTER_SEND;
    intmap["CREATE_CHARACTER_SEND_SPIN"]= CREATE_CHARACTER_SEND_SPIN;
    intmap["LOGIN_SEND"]        = LOGIN_SEND;
    intmap["LOGIN_RECV"]        = LOGIN_RECV;
    intmap["LOGIN_OK"]          = LOGIN_OK;
    intmap["LOGIN_FAILED"]      = LOGIN_FAILED;
    intmap["GATHER_GAME_DATA"]  = GATHER_GAME_DATA;
    intmap["SI_GENERIC_SEND"]   = SI_GENERIC_SEND;
	intmap["SI_GENERIC_RECV"]   = SI_GENERIC_RECV;
    intmap["LOGIN_PROGRAM_START"]= LOGIN_PROGRAM_START;
    intmap["LOGIN_PROGRAM_END"] = LOGIN_PROGRAM_END;
    intmap["GATHER_GAME_DATA_LOADER"]= GATHER_GAME_DATA_LOADER;
    intmap["GET_SELECTED_CHARACTER_SEND"]= GET_SELECTED_CHARACTER_SEND;
    intmap["GET_SELECTED_CHARACTER_RECV"]= GET_SELECTED_CHARACTER_RECV;
    intmap["DELETE_CHARACTER_SEND"]= DELETE_CHARACTER_SEND;
    intmap["DELETE_CHARACTER_RECV"]= DELETE_CHARACTER_RECV;
    intmap["INITIALIZE_GAME"]   = INITIALIZE_GAME;
    intmap["ITEM_INITIALIZE"]   = ITEM_INITIALIZE;
    intmap["GAME_ON"]           = GAME_ON;
    intmap["GAME_LIMBO"]        = GAME_LIMBO;
	intmap["GAME_EDITOR"]       = GAME_EDITOR;
	intmap["EDIT_WORLD_INIT"]   = EDIT_WORLD_INIT;
	intmap["EDIT_WORLD"]        = EDIT_WORLD;
    intmap["QUIT"]              = QUIT;
    intmap["WAIT_LOOP"]         = WAIT_LOOP;
    intmap["DEBUG_LOOP"]        = DEBUG_LOOP;



    intmap["f1"]    = SDLK_F1;
    intmap["f2"]    = SDLK_F2;
    intmap["f3"]    = SDLK_F3;
    intmap["f4"]    = SDLK_F4;
    intmap["f5"]    = SDLK_F5;
    intmap["f6"]    = SDLK_F6;
    intmap["f7"]    = SDLK_F7;
    intmap["f8"]    = SDLK_F8;
    intmap["f9"]    = SDLK_F9;
    intmap["f10"]   = SDLK_F10;
    intmap["f11"]   = SDLK_F11;
    intmap["f12"]   = SDLK_F12;
    intmap["f13"]   = SDLK_F13;
    intmap["f14"]   = SDLK_F14;
    intmap["f15"]   = SDLK_F15;

    intmap["numlock"]   = SDLK_NUMLOCK;
    intmap["scroll"]    = SDLK_SCROLLOCK;
    intmap["numpad_7"]  = SDLK_KP7;
    intmap["numpad_8"]  = SDLK_KP8;
    intmap["numpad_9"]  = SDLK_KP9;
    intmap["-"]         = SDLK_KP_MINUS;
    intmap["numpad_4"]  = SDLK_KP4;
    intmap["numpad_5"]  = SDLK_KP5;
    intmap["numpad_6"]  = SDLK_KP6;
    intmap["+"]         = SDLK_KP_PLUS;
    intmap["numpad_1"]  = SDLK_KP1;
    intmap["numpad_2"]  = SDLK_KP2;
    intmap["numpad_3"]  = SDLK_KP3;
    intmap["numpad_0"]  = SDLK_KP0;

    intmap["numpad_decimal"] = SDLK_KP_PERIOD;
//    intmap["oem_102"]   = SDLK_OEM_102;

    intmap["esc"] = SDLK_ESCAPE;
    intmap["1"] = SDLK_1;
    intmap["2"] = SDLK_2;
    intmap["3"] = SDLK_3;
    intmap["4"] = SDLK_4;
    intmap["5"] = SDLK_5;
    intmap["6"] = SDLK_6;
    intmap["7"] = SDLK_7;
    intmap["8"] = SDLK_8;
    intmap["9"] = SDLK_9;
    intmap["0"] = SDLK_0;
    intmap["-"] = SDLK_MINUS;
    intmap["="] = SDLK_EQUALS;
    intmap["backspace"] = SDLK_BACKSPACE;
    intmap["tab"] = SDLK_TAB;
    intmap["q"] = SDLK_q;
    intmap["w"] = SDLK_w;
    intmap["e"] = SDLK_e;
    intmap["r"] = SDLK_r;
    intmap["t"] = SDLK_t;
    intmap["y"] = SDLK_y;
    intmap["u"] = SDLK_u;
    intmap["i"] = SDLK_i;
    intmap["o"] = SDLK_o;
    intmap["p"] = SDLK_p;
    intmap["["] = SDLK_LEFTBRACKET;
    intmap["]"] = SDLK_RIGHTBRACKET;
    intmap["enter"] = SDLK_RETURN;
    intmap["l_control"] = SDLK_LCTRL;
    intmap["a"] = SDLK_a;
    intmap["s"] = SDLK_s;
    intmap["d"] = SDLK_d;
    intmap["f"] = SDLK_f;
    intmap["g"] = SDLK_g;
    intmap["h"] = SDLK_h;
    intmap["j"] = SDLK_j;
    intmap["k"] = SDLK_k;
    intmap["l"] = SDLK_l;
    intmap[";"] = SDLK_SEMICOLON;
    intmap["'"] = SDLK_QUOTE;
    intmap["`"] = SDLK_BACKQUOTE;
    intmap["l_shift"] = SDLK_LSHIFT;
    intmap["\\"] = SDLK_BACKSLASH;
    intmap["z"] = SDLK_z;
    intmap["x"] = SDLK_x;
    intmap["c"] = SDLK_c;
    intmap["v"] = SDLK_v;
    intmap["b"] = SDLK_b;
    intmap["n"] = SDLK_n;
    intmap["m"] = SDLK_m;
    intmap[","] = SDLK_COMMA;
    intmap["."] = SDLK_PERIOD;
    intmap["/"] = SDLK_SLASH;
    intmap["r_shift"] = SDLK_RSHIFT;
    intmap["*"] = SDLK_KP_MULTIPLY;
    intmap["l_alt"] = SDLK_LALT;
    intmap["space"] = SDLK_SPACE;
    intmap["capslock"] = SDLK_CAPSLOCK;
//    intmap["kana"] = SDLK_KANA;
//    intmap["abnt_c1"] = SDLK_ABNT_C1;
//    intmap["convert"] = SDLK_CONVERT;
//    intmap["noconvert"] = SDLK_NOCONVERT;
//    intmap["yen"] = SDLK_YEN;
//    intmap["abnt_c2"] = SDLK_ABNT_C2; // Numpad . on Portugese (Brazilian) keyboards
    intmap["numpad_equals"] = SDLK_KP_EQUALS;
//    intmap["circumflex"] = SDLK_CIRCUMFLEX;
    intmap["at"] = SDLK_AT;
    intmap["colon"] = SDLK_COLON;
    intmap["_"] = SDLK_UNDERSCORE;
//    intmap["kanji"] = SDLK_KANJI;
//    intmap["stop"] = SDLK_STOP;
//    intmap["ax"] = SDLK_AX;
//    intmap["unlabeled"] = SDLK_UNLABELED;
//    intmap["nexttrack"] = SDLK_NEXTTRACK; // Next Track Key?
    intmap["numpad_enter"] = SDLK_KP_ENTER;
    intmap["r_control"] = SDLK_RCTRL;
//    intmap["mute"] = SDLK_MUTE; // Mute Key?
//    intmap["calculator"] = SDLK_CALCULATOR; // Calculator Key?
//    intmap["playpause"] = SDLK_PLAYPAUSE; // Play / Pause Key?
//    intmap["mediastop"] = SDLK_MEDIASTOP; // Media Stop Key?
//    intmap["volumedown"] = SDLK_VOLUMEDOWN; // Volume - Key?
//    intmap["volumeup"] = SDLK_VOLUMEUP; // Volume + Key?
//    intmap["webhome"] = SDLK_WEBHOME; // Web home Key?
//    intmap["numpad_comma"] = SDLK_NUMPADCOMMA;
    intmap["/"] = SDLK_KP_DIVIDE;
//    intmap["sysrq"] = SDLK_SYSRQ;
    intmap["r_alt"] = SDLK_RALT;
    intmap["pause"] = SDLK_PAUSE;
    intmap["home"] = SDLK_HOME;
    intmap["up"] = SDLK_UP;
    intmap["pageup"] = SDLK_PAGEUP;
    intmap["left"] = SDLK_LEFT;
    intmap["right"] = SDLK_RIGHT;
    intmap["end"] = SDLK_END;
    intmap["down"] = SDLK_DOWN;
    intmap["pagedown"] = SDLK_PAGEDOWN;
    intmap["insert"] = SDLK_INSERT;
    intmap["delete"] = SDLK_DELETE;
    intmap["l_windows"] = SDLK_LSUPER;
    intmap["r_windows"] = SDLK_RSUPER;
//    intmap["start"] = SDLK_START;
    intmap["power"] = SDLK_POWER;
//    intmap["sleep"] = SDLK_SLEEP;
//    intmap["wake"] = SDLK_WAKE;
//    intmap["websearch"] = SDLK_WEBSEARCH;
//    intmap["webfavorites"] = SDLK_WEBFAVORITES;
//    intmap["webrefresh"] = SDLK_WEBREFRESH;
//    intmap["webstop"] = SDLK_WEBSTOP;
//    intmap["webforward"] = SDLK_WEBFORWARD;
//    intmap["webback"] = SDLK_WEBBACK;
//    intmap["mycomputer"] = SDLK_MYCOMPUTER;
//    intmap["mail"] = SDLK_MAIL;
//    intmap["mediaselect"] = SDLK_MEDIASELECT;


}
void C_EM_CONSOLE::_GlobalFunctions()
{
    funcmap.clear();
    funcmap["functest"]     = con_functest;
    funcmap["setgamemode"]  = con_setgamemode;
    funcmap["guigo"]        = con_guigo;
    funcmap["guicall"]      = con_guicall;
    funcmap["echo"]         = con_echo;
    funcmap["exec"]         = con_exec;
    funcmap["bind"]         = con_bind;
    funcmap["cvar"]         = con_cvar;
    funcmap.find("functest")->second("CONSOLE: functions defined");
}

void C_EM_CONSOLE::_Execute(const char *cmd)
{
    int i,j; char temp[1024]; memset(temp,0,1024);

    // Check " - Any ; in between quotes will temp change to 0xFF

    int cQuoteCount=0;
    bool bQuote2=0;
    char cmd2[1024]; memset(cmd2,0,1024);
    strcpy(cmd2,cmd);
    for(i=0;i<strlen(cmd2);i++)
    {
        switch(cmd2[i])
        {
            case '\n':
            case '\r':
            case '\t':
                cmd2[i]=' ';
                break;
            case '"':
                cQuoteCount++;
                break;
            case ';':
                if(cQuoteCount&1)
                {
                    cmd2[i]= '';
                    break;
                }
                else
                    bQuote2=true;
                break;
            default:
                break;
        }
    }
    if(cQuoteCount&1)  return; // mismatched quote

    vector <string> ncmd=explode(";",cmd2);
    if(ncmd.size()>1)
    {
        for(i=0;i<ncmd.size();i++)
        {
            pGUI->_ConsoleExecute((char *)ncmd[i].c_str());
        }
        return;
    }

    for(i=0;i<strlen(cmd2);i++)
    {
        switch(cmd2[i])
        {
            case '':
                cmd2[i]=';';
                break;
        }
    }

    //pLog->_Add("CONSOLE[%s]",cmd2);

    vector <string> narg=explode(" ",cmd2);
    if(narg.size()>0)
    {
        if(funcmap.find((char *)narg[0].c_str())!=funcmap.end())
        {
            //Log->_Add("Found a function! %s",narg[0].c_str());
            if(narg.size()>0)
            {
                sprintf(temp,(char *)narg[1].c_str());
                if(narg.size()>1)
                for(i=2;i<narg.size();i++)
                {
                    strcat(temp,va(" %s",narg[i].c_str()));
                }
                funcmap.find(narg[0].c_str())->second(temp);
            }
            return;
        }
        if(dscc((char *)narg[0].c_str(),"quit"))
        {
            bShutDown=true;
            return;
        }
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

/***********************************************************************************/

FMG_CALLBACK C_FM_GUI::_gcdatafill(string whatgui, string whatdata)
{
    pLog->_Add("_gcdatafill whatgui[%s] whatdata[%s]",whatgui.c_str(),whatdata.c_str());

}

/*****************************************************************
 ** SDL main                                                    **
 *****************************************************************/

int main(int argc, char *argv[])
{
    pVar=new c_var();
    bShutDown=0;
    if(doInit())
        while(!bShutDown)
            MainGameLoop();
    ShutDown();
    DEL(pVar);
    return 0;
}

/*****************************************************************
 **  Main Game Loop                                             **
 *****************************************************************/

void MainGameLoop(void)
{
    if(bShutDown) return;
    if(!pGUI) return;
    if(!pClientData) return;

    if ( pGUI->DoInput() == SDLK_F12 )
    {
        bShutDown=true;
        return;
    }

    DoNetwork();    // Get any network messages

    pGFX->BeginScene();

    DoGameMode();

    pGUI->draw();

    pGUI->WriteText(15,pClientData->ScreenHeight-16,va("^+^1(^5%s^1)^5 %s",CPUSTRING,VERSION),0);
    pGUI->DrawOSIcon(5,pClientData->ScreenHeight-16,CPUSTRING);

    pGFX->EndScene();

} // End MAINGAMELOOP

bool DoGameMode(void)
{
    switch(pClientData->GAME_MODE)
    {
    case RETRO_INTRO_INIT:
        pClientData->GAME_MODE=RETRO_INTRO_PLAY;
        break;

    case RETRO_INTRO_PLAY:
        pClientData->GAME_MODE=MAIN_MENU;
        break;

    case MAIN_MENU:

        // if((INPUTMODE!=CONSOLE2)&&(pClientData->drawoptions==false)) INPUTMODE=NORMAL;

        if(pFMGS) { pFMGS->emgDisconnect(); DEL(pFMGS); }

        pClientData->LAST_GAME_MODE=MAIN_MENU;

        pGUI->clear();
        pGUI->call("main.gui");

        pGUI->setdata("username",pClientData->Name);
        if(pClientData->bSavePassword)
            pGUI->setdata("password",pClientData->Password);
        pGUI->setdata("savepassword",va("%d",pClientData->bSavePassword));

        pClientData->GAME_MODE=MAIN_MENU_2;

    case MAIN_MENU_2:

        break; // End MAIN_MENU

    case CREDITS:

        pClientData->GAME_MODE=MAIN_MENU;
        break;

    case LOGIN_AUTO_LOGIN:

        pLog->_Add("LOGIN_AUTO_LOGIN");
        pGUI->clear();
        pGUI->pConsole->_Execute("exec autologin.cfg");
        pClientData->GAME_MODE=LOGIN_SCREEN_ENTRY;
        break;


    case LOGIN_SCREEN_ENTRY:

        pLog->_Add("LOGIN_SCREEN_ENTRY");

        //SDL_WM_SetCaption(va("EGC %s(%s) Net Revision(%s) %s",VERSION,CPUSTRING,NET_REVISION,COPYRIGHT),"icon");

        if(pClientData)
        {
            pClientData->ServerListOffset=0;
            //pClientData->SelectedServer=MAX_SERVERS+1;
            pClientData->Mode=PLAY;
        }

        //DEL(pFMMS);
        //LOGINMODE=LM_NONE;

        pClientData->GAME_MODE=GATHER_SERVER_LIST;

    case GATHER_SERVER_LIST:

        if(pFMGS) { pFMGS->emgDisconnect(); DEL(pFMGS); }

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

    case CHOOSE_SERVER_INIT:

        if(pFMGS) { pFMGS->emgDisconnect(); DEL(pFMGS); }

        pGUI->clear();
        pGUI->call("servers.gui");
        pClientData->GAME_MODE=CHOOSE_SERVER;


    case CHOOSE_SERVER:

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

    case LOGIN_FROM_GUI:

        pLog->_Add("%s %s",pGUI->getdata("username"),md5_digest(pGUI->getdata("password")));

        strcpy(pClientData->Name,       pGUI->getdata("username"));
        strcpy(pClientData->Password,   pGUI->getdata("password"));

        pClientData->GAME_MODE=CONNECT;
        break;

    case CONNECT:

        // pLog->_Add("CONNECT");
        pGUI->clear();

        strcpy(pClientData->ServerID,"s001-frag-mere");
        if(     (!strlen(pClientData->Name)) ||
                (!strlen(pClientData->Password)) )
        {
            pGUI->Prompt("Must enter user info           to proceed.","nop");
            pClientData->GAME_MODE=GATHER_SERVER_LIST;
            break;
        }

        strcpy(pClientData->IPAddress,"127.0.0.1");
        strcpy(pClientData->Port, "40404");

        // Run the connect command through the console...
        // ProcessConsoleCommand(va("connect %s:%s %s %s", pClientData->IPAddress,pClientData->Port, pClientData->Name,pClientData->Password),1);

        if(pFMGS) { pFMGS->emgDisconnect(); DEL(pFMGS); } pFMGS=new C_FMGS();

        pFMGS->emgConnect(pClientData->IPAddress,pClientData->Port,pClientData->Name,md5_digest(pClientData->Password));
        pFMGS->dMsgTime=dlcs_get_tickcount();
        pClientData->GAME_MODE=LOGIN_RECV;

        break; // End CONNECT

    case CONSOLE_CONNECT:

        pLog->_Add("CONSOLE_CONNECT");

        pGUI->clear();
        pGUI->call("connect.gui");

        if(pFMGS) { pFMGS->emgDisconnect(); DEL(pFMGS); } pFMGS=new C_FMGS();

        pFMGS->emgConnect(pClientData->IPAddress,pClientData->Port,pClientData->Name,md5_digest(pClientData->Password));
        pFMGS->dMsgTime=dlcs_get_tickcount();
        pClientData->GAME_MODE=LOGIN_RECV;

        break;

        /////////////////////////////

    case LOGIN_RECV: // Spin loop for logging in

        pLog->_Add("LOGIN_RECV");
        //pGUI->MOUSEMODE=MP_SYSTEMBUSY;
        pGUI->WriteText(270,270,"^&^7Connecting to the server...",0);
        if((dlcs_get_tickcount()-pFMGS->dMsgTime) > NET_WAITFORANSWER)
        {
            pGUI->Prompt("^&^6No response from server^1!","nop");
            pClientData->GAME_MODE=MAIN_MENU;
        }
        //if(pFMGS->bConnected)
          //  pClientData->GAME_MODE=GATHER_GAME_DATA;
        break; // End LOGIN_RECV

        //////////////////////////////

    case GET_SERVER_INFORMATION_START:

        pLog->_Add("GET_SERVER_INFORMATION_START");

        //SendData.Reset();
        //SendData.Write((char)NETMSG_SERVERINFORMATION);
        //SendData.Write((char)1);
        //SendNetMessage(0);

        pClientData->GAME_MODE=GET_SERVER_INFORMATION_SPIN;

    case GET_SERVER_INFORMATION_SPIN:

        pLog->_Add("GET_SERVER_INFORMATION_SPIN");

        //MOUSEMODE=MP_SYSTEMBUSY;

        pGUI->WriteText(270,270,"^&^7Updating Server Information...",0);

        //if((dlcs_get_tickcount()-pFMGS->dMsgTime) > NET_WAITFORANSWER)
        //{
        //    pGUI->Prompt("^&^6No response from server^1!","nop");
        //    pClientData->GAME_MODE=LOGIN_SCREEN_ENTRY;
        //}


        break;

        /////////////////////////////

    case GET_CHARACTERS:

        pLog->_Add("GET_CHARACTERS");

        //for(i=0; i<MAX_TOONS; i++)
        //{
        //  memset(pClientData->ServerCharacter[i].t_name,0,32);
        //}

        //SendData.Reset();
        //SendData.Write((char)NETMSG_RETRIEVECHARS);
        //SendData.Write((char)1);
        //SendNetMessage(0);

        pClientData->GAME_MODE=GET_CHARACTERS_SPIN;

        break;

    case GET_CHARACTERS_SPIN:

        //MOUSEMODE=MP_SYSTEMBUSY;

        pGUI->WriteText(270,270,"^&^7Downloading character information...",0);

        //if((dlcs_get_tickcount()-pFMGS->dMsgTime) > NET_WAITFORANSWER)
        //{
        //    pGUI->Prompt("^&^6No response from server^1!","nop");
        //    pClientData->GAME_MODE=LOGIN_SCREEN_ENTRY;
        //}

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

        //MOUSEMODE=MP_SYSTEMBUSY;
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

/*      camera->px=0;
        camera->py=0;
        camera->pz=0;

        camera->vx=0;
        camera->vy=0;
        camera->vz=0;

        camera->ux=0;
        camera->uy=0;
        camera->uz=0;       */

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

    return false;
}

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
    pFMGS           = NULL;

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

    pLog->_Add((char *)va("EMBER %s %s %s",VERSION,CPUSTRING,COPYRIGHT));

    pLog->_Add("Log created");
    pLog->_Add(getos());

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


/*
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

    */

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

    pGUI->pConsole->_Execute("exec autoexec.cfg");
    pGUI->pConsole->_Execute("exec config.cfg");

    pLog->_Add("******************** FINISHED STARTUP ***************************");

    return TRUE;
}

/***************************************************************
 **  Shut Down the Program                                    **
 ***************************************************************/

void ShutDown(void)
{
    pLog->_Add("******************** START SHUTDOWN ***************************");
    // ProcessConsoleCommand("disconnect",0);
    // ProcessConsoleCommand("stopaudio",0);

//    DEL(pSND);
    DEL(pGUI);

    DEL(pFMGS);
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

/**************************************************
 ** Ping the server                              **
 **************************************************/

long Ping(void)
{
    CPacket SendData(NET_DATAGRAMSIZE);
    static long dwPingTimer=dlcs_get_tickcount();
    if(!pFMGS)                              return 9999;
    if(pFMGS->emgeGetState()==NET_NOTCONNECT)  return 999;
    if(dlcs_get_tickcount()-dwPingTimer>5000)
    {
        dwPingTimer=dlcs_get_tickcount();
        SendData.Reset();
        SendData.Write((char)NETMSG_PING);
        SendData.Write((long)dlcs_get_tickcount());
        pFMGS->SendUnreliableMessage(&SendData);
    }
    return pFMGS->emgdwPing;
}

/**************************************************
 ** Get a file from server                       **
 **************************************************/

void GetFile(char *filename)
{
    CPacket SendData(NET_DATAGRAMSIZE);
	if(!pFMGS) return;
	SendData.Reset();
	SendData.Write((char)NETMSG_FILE_XFER);
//	SendData.Write((char)NET_FILE_START);
	SendData.Write((char *)filename);
	pFMGS->SendUnreliableMessage(&SendData);
}

/**************************************************
 ** Network Loop                                 **
 **************************************************/

void DoNetwork(void)
{


    if(!pFMGS) return;

    char temp1[1024];   memset(temp1,0,1024);
    char temp2[1024];   memset(temp2,0,1024);
    char temp3[1024];   memset(temp3,0,1024);
    char temp4[1024];   memset(temp4,0,1024);
    char szTemp1[1024]; memset(szTemp1,0,1024);
    char szTemp2[1024]; memset(szTemp2,0,1024);
    char szTemp3[1024]; memset(szTemp3,0,1024);
    char szTemp4[1024]; memset(szTemp4,0,1024);

    CPacket SendData(NET_DATAGRAMSIZE);
    CPacket *RecvData;

    int             iState,i,ax,ay,az,bx,by,bz,cx,cy,cz,dx,dy,dz,ex,ey,ez,fx,fy,fz;
    long            bytetimer,gx,gy,gz;
    unsigned char  vx,vy,vz;
    char            NetMessage;

    ax=ay=az=bx=by=bz=cx=cy=cz=dx=dy=dz=ex=ey=ez=fx=fy=fz=gx=gy=gz=vx=vy=vz=0;

    bytetimer=dlcs_get_tickcount();

    iState = pFMGS->emgeGetState();

    /*******************************************************/

    if( iState  == NET_NOTCONNECT ) return; // If you aren't connected, then return. No sense in trying to read packets if they aren't there.

    if( (iState != NET_LOGGINPROC) &&
        (iState != NET_SYSBUSY) &&
        (iState != NET_CONNECTED) ) return;

    //if( (iState == NET_LOGGINPROC) || (iState == NET_SYSBUSY) ) return;

    NetMessage = pFMGS->iGetMessage(); // Update the Network once per frame here

    if(NetMessage == -1)
    {
        pGUI->Prompt("Lost connection to server.","disconnect");
        pFMGS->emgSetState(NET_NOTCONNECT);
        pClientData->GAME_MODE=MAIN_MENU;
        return;
    }

    if(NetMessage) // Check for network message
    {
        RecvData        = pFMGS->pGetMessage();
        pFMGS->cLastNetMessage = RecvData->cRead();

        switch(pFMGS->cLastNetMessage)
        {

            /*********************************************************************************
             ** NETMSG_SERVERINFORMATION                                                    **
             *********************************************************************************/

			case NETMSG_SERVERINFORMATION:

                //Log("NETMSG_SERVERINFORMATION");

				ax=RecvData->cRead();
                //Log("%d) Races",ax);
				for(i=0;i<ax;i++)
				{
					ay=RecvData->cRead();
					//strcpy(pClientData->ServerInfo->Race[ay].Text,RecvData->pRead());
					//Log("  %d) %s",ay,pClientData->ServerInfo->Race[ay].Text);

				}
				ax=RecvData->cRead();
                    //Log("%d) Classes",ax);
				for(i=0;i<ax;i++)
				{
					ay=RecvData->cRead();
                    //strcpy(pClientData->ServerInfo->Class[ay].Text,RecvData->pRead());
                    //Log("  %d) %s",ay,pClientData->ServerInfo->Class[ay].Text);

				}
				// SetGameMode(GET_CHARACTERS);

				break;

            /*********************************************************************************
             ** NETMSG_RETRIEVECHARS                                                        **
             *********************************************************************************

			case NETMSG_RETRIEVECHARS:

				//Log("NETMSG_RETRIEVECHARS!");

				pClientData->ClearCharacters();

				ax=RecvData->cRead();
				for(i=0;i<ax;i++)
				{
					strcpy(temp1,RecvData->pRead());// name
					strcpy(temp2,RecvData->pRead());// level
					strcpy(temp3,RecvData->pRead());// race
					strcpy(temp4,RecvData->pRead());// class
					strcpy(szTemp1,RecvData->pRead());// gender

					strcpy(pClientData->ServerCharacter[i].t_name,temp1);
					pClientData->ServerCharacter[i].t_level = atoi(temp2);
					pClientData->ServerCharacter[i].t_race  = atoi(temp3);
					pClientData->ServerCharacter[i].t_class = atoi(temp4);
					pClientData->ServerCharacter[i].t_gender= atoi(szTemp1);

				}

				SetGameMode(CHOOSE_CHARACTER);


				break;
				*/

            /*********************************************************************************
             ** NETMSG_LOGIN_REQUEST_REPLY                                                  **
             *********************************************************************************/

            case NETMSG_LOGIN_REQUEST_REPLY:

				pLog->_Add("Got a login request reply");

                if(pClientData->GAME_MODE != LOGIN_RECV) break;

                ax = RecvData->cRead();

                if((ax == GOOD_LOGIN) || (ax == NEW_ACCOUNT))
                {
					pLog->_Add(" >>> It's a good login!");

  					strcpy(pClientData->ServerMessage,   RecvData->pRead());
                    strcpy(pClientData->szServerVersion, RecvData->pRead()); // Extract Server's Version
                    pClientData->ID=                     RecvData->iRead();  // Extract Server Assigned SID
                    strcpy(pClientData->ServerID,        RecvData->pRead()); // Extract Server Resource ID
                    strcpy(pClientData->ServerName,      RecvData->pRead()); // Extract Server Name
                    strcpy(pClientData->ServerAuthor,    RecvData->pRead()); // Extract Server Admin
                    pClientData->Access=                 RecvData->iRead();  // Extract Player's Access level
                    strcpy(pClientData->szAccessName,    RecvData->pRead()); // Extract Player's Access level name
                    pClientData->CharacterSlots=         RecvData->cRead();  // Extract Server num of character slots

                    pLog->_Add("%s",pClientData->ServerMessage);

                    pLog->_Add("%s running %s ID[%s] Admin[%s] Character Slots[%d]",
                    pClientData->ServerName,
                    pClientData->szServerVersion,
                    pClientData->ServerID,
                    pClientData->ServerAuthor,
                    pClientData->CharacterSlots);

                    pLog->_Add("You have been assigned ID[%d] and you have access[%d(%s)]",
                    pClientData->ID,
                    pClientData->Access,
                    pClientData->szAccessName);

                    //if(pClientData->CharacterSlots>MAX_TOONS)
                    //pClientData->CharacterSlots=MAX_TOONS;

                    pClientData->GAME_MODE=GATHER_GAME_DATA; // GET_SERVER_INFORMATION_START;

                    pFMGS->emgSetState(NET_CONNECTED);

                    //DLog("NET_CONNECTED!");
                }
                else
                {
					pLog->_Add(" >>> It's a bad login!");
                    pClientData->GAME_MODE=CHOOSE_SERVER_INIT;
                    if(ax!=MASTER_LOGIN)
                        pGUI->Prompt(RecvData->pRead(),"nop");
                }
                break;

            /*********************************************************************************
             ** NETMSG_HEARTBEAT                                                            **
             *********************************************************************************/

            case NETMSG_HEARTBEAT:  // just return the heartbeat... no other thing needs to be done...
                SendData.Reset();
                SendData.Write((char)NETMSG_HEARTBEAT);
                SendData.Write((int)1);
                pFMGS->SendUnreliableMessage(&SendData);
                // DLog("NET: Rx'd a Heartbeat request from server. Returned the same.");
                break;


            /*********************************************************************************
             ** NETMSG_CLIENTSHUTDOWN                                                       **
             *********************************************************************************

            case NETMSG_CLIENTSHUTDOWN:
                i=RecvData->iRead();
                strcpy(temp1,RecvData->pRead());
               DLog("FM: Server has shut down this client. Reason[%s]",temp1);
                sprintf(temp2,"Server has shut down this client. Reason:    %s",temp1);
                Prompt(temp2,"nop");
                // ChatBufferAdd(255,0,0,"SYSTEM","Server has shut down this client. Reason[%s]",temp1);
                NetworkSetup();
                SetGameMode(LOGIN_SCREEN_ENTRY);
                break;
*/
            /*********************************************************************************
             ** NETMSG_CHANGE_MODE                                                          **
             *********************************************************************************

            case NETMSG_CHANGE_MODE:

                ax = RecvData->cRead();
                bx = RecvData->cRead();
                if(bx == false) return;
                switch(ax)
                {
                    case PLAY:
                        ConsoleBufferAdd("Mode is now [PLAY]");
                        pClientData->Mode=ax;
                        break;
                    case BUILD:
                        ConsoleBufferAdd("Mode is now [BUILD]");
                        pClientData->Mode=ax;
                        break;
                    case BUILD_GHOST:
                        ConsoleBufferAdd("Mode is now [BUILD GHOST]");
                        pClientData->Mode=ax;
                        break;
                    case SPECTATOR:
                        ConsoleBufferAdd("Mode is now [SPECTATOR]");
                        pClientData->Mode=ax;
                        break;
                    default:
                        Log("NET: Set gameplay mode error! The value is out of range. Mode not changed.");
                        ChatBufferAdd(255,0,0,"SYSTEM","Mode not changed.");
                        break;
                }
                break;

*/
            /*********************************************************************************
             ** NETMSG_MODIFY_MAP                                                           **
             *********************************************************************************

            case NETMSG_MODIFY_MAP:
                if(!pOffScreenMap) { DLog("FCOM: Map not setup yet, not changing tile..."); break; }
                cx=RecvData->cRead(); // Type of modification
                dx=RecvData->cRead(); // boolean
                ax=RecvData->iRead(); // map x
                ay=RecvData->iRead(); // map y
                az=RecvData->iRead(); // map z
                DLog("NETMSG_MODIFY_MAP: [%d][%d][%d][%d][%d]",cx,dx,ax,ay,az);
                if(camera->in(ax-GMP_MAPSIZE,ay-GMP_MAPSIZE,az,ax+GMP_MAPSIZE,ay+GMP_MAPSIZE,az))
                {
                    DLog("NETMSG_MODIFY_MAP: (onscreen change)");
                    pMapBuffer->SetProperty(CamCoord(ax),CamCoord(ay),cx,TOBOOL(dx));
                    ProcessConsoleCommand("savemap",0); // save the map
                }
                else
                {
                    DLog("NETMSG_MODIFY_MAP: (offscreen change)");
                    if(!pOffScreenMap) { Log("NETMSG_MODIFY_MAP: Can't allocate offscreen map!"); break; }
                    pOffScreenMap->Load3D(va("%s%cmap%c%d-%d-%d.fmp",pClientData->GRDir,_PS,_PS,MapCoord(ax),MapCoord(ay),MapCoord(az)));
                    pOffScreenMap->SetProperty(CamCoord(ax),CamCoord(ay),cx,TOBOOL(dx));
                    pOffScreenMap->Save3D(va("%s%cmap%c%d-%d-%d.fmp",pClientData->GRDir,_PS,_PS,MapCoord(ax),MapCoord(ay),MapCoord(az)));
                }
                break;

*/
            /*********************************************************************************
             ** NETMSG_SET_TILE                                                             **
             *********************************************************************************

            case NETMSG_SET_TILE:

                if(!pMapBuffer) { Log("NETMSG_SET_TILE: Map not setup yet, not changing tile..."); break; }

                ax = RecvData->iRead(); // x
                ay = RecvData->iRead(); // y
                az = RecvData->iRead(); // z
                cx = RecvData->cRead(); // bank
                cy = RecvData->cRead(); // tile
                DLog("NETMSG_SET_TILE: [%d][%d][%d][%d][%d]",ax,ay,az,cx,cy);
                if(camera->in(ax-GMP_MAPSIZE,ay-GMP_MAPSIZE,az,ax+GMP_MAPSIZE,ay+GMP_MAPSIZE,az))
                {
                    DLog("NETMSG_SET_TILE: (onscreen change)");
                    pMapBuffer->SetTile(CamCoord(ax)+GMP_MAPSIZE,CamCoord(ay)+GMP_MAPSIZE,cx,cy); // change tile in map buffer
                    ProcessConsoleCommand("savemap",0); // save the map
                }
                else
                {
                    DLog("NETMSG_SET_TILE: (offscreen change)");
                    if(!pOffScreenMap) { Log("NETMSG_SET_TILE: Can't allocate offscreen map!"); break; }
                    sprintf(temp1,"%s%cmap%c%d-%d-%d.fmp",pClientData->GRDir,_PS,_PS,MapCoord(ax),MapCoord(ay),MapCoord(ay));// load map into offscreen buffer
                    if(!pOffScreenMap->Load3D(temp1)) { pOffScreenMap->Clear(); } // change the tile
                    pOffScreenMap->SetTile(CamCoord(ax),CamCoord(ay),cx,cy); // save the map
                    pOffScreenMap->Save3D(temp1);
                }
                break;

*/
            /*********************************************************************************
             ** NETMSG_SET_OBJECT                                                           **
             *********************************************************************************

            case NETMSG_SET_OBJECT:
                // obsolete
                break;
*/
            /*********************************************************************************
             ** NETMSG_SET_VERTEX                                                           **
             *********************************************************************************

            case NETMSG_SET_VERTEX:


                if(pMapBuffer==NULL)
                {
                    DLog("FCOM: Map not setup yet, not changing vertex...");
                    break;
                }

                ax = RecvData->iRead(); // x
                ay = RecvData->iRead(); // y
                az = RecvData->iRead(); // z
                cx = RecvData->cRead(); // height width or lighting?
                cy = RecvData->cRead(); // which vertex
                vx = RecvData->cRead(); // height / width / red element
                vy = RecvData->cRead(); // green element
                vz = RecvData->cRead(); // blue element

                dx=0;
                // if on the same screen
                if( ( ( pPlayer->GetX() ) >= ( ax - GMP_MAPSIZE ) ) &&
                    ( ( pPlayer->GetX() ) <= ( ax + GMP_MAPSIZE ) ) &&
                    ( ( pPlayer->GetY() ) >= ( ay - GMP_MAPSIZE ) ) &&
                    ( ( pPlayer->GetY() ) <= ( ay + GMP_MAPSIZE ) ) &&
                    (   pPlayer->GetZ()   ==   az ) )
                {
                    dx=1;
                }
                // load map into offscreen buffer
                sprintf(temp1,"%s%cmap%c%d-%d-%d.fmp",pClientData->GRDir,_PS,_PS,MapCoord(ax),MapCoord(ay),MapCoord(ay));
                if(!pOffScreenMap->Load3D(temp1))
                {
                    pOffScreenMap->Clear();
                }
                switch(cx)
                {
                    case 0: // height
                        if(dx)
                            pMapBuffer->SetVertexHeight(CamCoord(ax)+GMP_MAPSIZE,CamCoord(ay)+GMP_MAPSIZE,cy,vx);
                        else
                            pOffScreenMap->SetVertexHeight(CamCoord(ax),CamCoord(ay),cy,vx);
                        DLog("Vertex %d height set to %d",cy,vx);//pMapBuffer->GetVertexHeight(ax+GMP_MAPSIZE*ex,ay+GMP_MAPSIZE*ey,cy));
                        break;

                    case 1: // lighting
                        if(dx)
                            pMapBuffer->SetVertexColor(CamCoord(ax)+GMP_MAPSIZE,CamCoord(ay)+GMP_MAPSIZE,cy,vx,vy,vz);
                        else
                            pOffScreenMap->SetVertexColor(CamCoord(ax),CamCoord(ay),cy,vx,vy,vz);
                        DLog("Vertex %d lighting set to R%d G%d B%d",cy,vx,vy,vz);
                        break;

                    case 2: // width
                        if(dx)
                            pMapBuffer->SetVertexWidth(CamCoord(ax)+GMP_MAPSIZE,CamCoord(ay)+GMP_MAPSIZE,cy,vx);
                        else
                            pOffScreenMap->SetVertexWidth(CamCoord(ax),CamCoord(ay),cy,vx);
                        DLog("Vertex %d width set to %d",cy,vx);
                        break;

                    case 3: // relight map
                        if(dx)
                            pMapBuffer->ClearVertexColors(vx,vy,vz);
                        else
                            pOffScreenMap->ClearVertexColors(vx,vy,vz);
                        DLog("Map relit to %d %d %d",vx,vy,vz);
                        break;

                    default:
                        break;
                }
                // save the map
                if(dx)
                    ProcessConsoleCommand("savemap",0);
                else
                    pOffScreenMap->Save3D(temp1);


                //break;

                */

            /*********************************************************************************
             ** NETMSG_LOCALCHAT                                                            **
             *********************************************************************************

            case NETMSG_LOCALCHAT:  // Local Chat
                    // obsolete
                break;

                */

            /*********************************************************************************
             ** NETMSG_SYSTEMMSG                                                            **
             *********************************************************************************

            case NETMSG_SYSTEMMSG:  // Global Chat / System Message
                strcpy(temp1,RecvData->pRead());
                ChatBufferAdd(255,255,255,"font",temp1);
                DLog("NETMSG_SYSTEMMSG [%s]",temp1);
                break;

                */

            /*********************************************************************************
             ** NETMSG_MODIFY_CHARACTER                                                     **
             *********************************************************************************

            case NETMSG_MODIFY_CHARACTER:
                    // obsolete
                break;

                */

            /*********************************************************************************
             ** NETMSG_MOVEPLAYER                                                           **
             *********************************************************************************

            case NETMSG_MOVEPLAYER:
                    // obsolete
                break;

                */

            /*********************************************************************************
             ** NETMSG_VIS_MOVE                                                             **
             *********************************************************************************

            case NETMSG_VIS_REMOVE:
                i =RecvData->iRead(); // id
                ax=RecvData->iRead(); // number of path waypoints... or 0









                DLog("NETMSG_VIS_REMOVE sid[%d]",i);
                break;

                */


            /*********************************************************************************
             ** NETMSG_VIS_REMOVE                                                           **
             *********************************************************************************

            case NETMSG_VIS_REMOVE:
                i =RecvData->iRead(); // id
                Pop_Vis(i);
                DLog("NETMSG_VIS_REMOVE sid[%d]",i);
                break;

                */

            /*********************************************************************************
             ** NETMSG_VIS_UPDATE                                                           **
             *********************************************************************************

            case NETMSG_VIS_UPDATE:
                i =RecvData->iRead(); // id
                ax=RecvData->iRead(); // x
                ay=RecvData->iRead(); // y
                az=RecvData->iRead(); // z
                strcpy(temp1,RecvData->pRead()); // media
                ex=RecvData->iRead(); // media_type
                dx=RecvData->iRead(); // heading
                dy=RecvData->iRead(); // direction
                Pop_Vis(i);
                PushVis(i,ax,ay,az,temp1,ex,dx,dy);
                DLog("NETMSG_VIS_UPDATE sid[%d] x[%d] y[%d] z[%d] media[%s] m_type[%d] head[%d] dir[%d]",i,ax,ay,az,temp1,ex,dx,dy );
                break;

                */

            /*********************************************************************************
             ** NETMSG_SERVER_INFO                                                          **
             *********************************************************************************

            case NETMSG_SERVER_INFO:
                switch(RecvData->cRead()) // what is updating?
                {
                    case SI_GENERIC: // general server info -> fill in some kind of server info data struct
                        ay = RecvData->cRead();
                        switch(ay)
                        {
                            case 1: // this tells the client that it is done with updates
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;

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
                        gui=new C_FM_GUI();
                        break;

                    default:
                        break;
                }
                break;

                */

            /*********************************************************************************
             ** NETMSG_FVM                                                                  **
             *********************************************************************************

            case NETMSG_FVM:
                switch(RecvData->cRead())
                {
                    case FVM_GETTARGET:
                        DLog("NETMSG_FVM(FVM_GETTARGET):%s",RecvData->pRead());
                        bTargeting=1;
                        break;

                    case FVM_SET_DAYLIGHT:
                        camera->r=RecvData->fRead();
                        camera->g=RecvData->fRead();
                        camera->b=RecvData->fRead();
                        DLog("NETMSG_FVM(FVM_DAYLIGHT): Daylight changed to... r[%f] g[%f] b[%f]",camera->r,camera->g,camera->b);
                        break;

                    case FVM_MOVECAM:

                        camera->x =RecvData->iRead();
                        camera->y =RecvData->iRead();
                        camera->z =RecvData->iRead();
                        camera->ax=RecvData->iRead();
                        camera->ay=RecvData->iRead();
                        camera->az=RecvData->iRead();
                        camera->ox=RecvData->iRead();
                        camera->oy=RecvData->iRead();
                        camera->oz=RecvData->iRead();
                        LoadMap(camera->x,camera->y,camera->z);
                        DLog("NETMSG_FVM(FVM_MOVECAM): Camera position  x[%d]  y[%d]  z[%d]",camera->x,camera->y,camera->z);
                        DLog("                       : Camera angle    ax[%d] ay[%d] az[%d]",camera->ax,camera->ay,camera->az);
                        DLog("                       : Camera offset   ox[%d] oy[%d] oz[%d]",camera->ox,camera->oy,camera->oz);
                        DLog("                       : Camera zoom     zoom[%d]",camera->zoom);
                        break;

                    case FVM_SET_MAP_DRAW:
                        bDrawMap=TOBOOL(RecvData->cRead());
                        DLog("NETMSG_FVM(FVM_SET_MAP_DRAW): [%d]",bDrawMap);
                        break;

                    case FVM_SET_GAME_MODE: // change this to FVM_SET_PLAY_MODE
                        pClientData->Mode=RecvData->cRead();
                        DLog("NETMSG_FVM(FVM_SET_GAME_MODE): [%d]",pClientData->Mode);
                        break;

                    case FVM_SET_LIMBO_STATE: // put a way to change GAME_MODE
                        switch(RecvData->iRead())
                        {
                            case 0:
                                GAME_MODE=GAME_ON;
                                break;
                            case 1:
                                GAME_MODE=GAME_LIMBO;
                                break;
                            default:
                                GAME_MODE=NEXT_GAME_MODE;
                                break;
                        }
                        DLog("NETMSG_FVM(FVM_SET_LIMBO_STATE): [%d]",TOBOOL(GAME_MODE==GAME_LIMBO));
                        break;

                    default:
                        break;
                }
                break;

                */


            /*********************************************************************************
             ** NETMSG_FILE_XFER                                                            **
             *********************************************************************************

            case NETMSG_FILE_XFER:

//				char *pFileBuf; //char pFileBuf[NET_FILE_XFER_BLOCK_SIZE+1]; memset(pFileBuf,0,1024);
//				FILE *pFile;
//				bool bFileFail;

				switch(RecvData->cRead())
				{

							// NET_FILE_RES_MEDIA
							// NET_FILE_RES_SCRIPT

					case NET_FILE_NOP:
						break;

					case NET_FILE_START:
						// filename
						// filesize
						// start file, put a temporary sequencer in temp folder for the file
						// ie; hey.bmp will create temp/hey.bmp.sequence
						strcpy(szTemp1,RecvData->pRead());
						cx=RecvData->iRead();
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

					case NET_FILE_DATA:

						// sequence number
						// filename
						// data block (1024)

						ax=RecvData->iRead(); // sequence number
						strcpy(szTemp1,RecvData->pRead()); // filename
						pFileBuf=RecvData->pRead(NET_FILE_XFER_BLOCK_SIZE);
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

						ax=RecvData->iRead(); // sequence number
						strcpy(szTemp1,RecvData->pRead()); // filename
						dx=RecvData->iRead();
						pFileBuf=RecvData->pRead(dx);
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


					Log("File Xfer: %s",RecvData->pRead());

				}

                break;

                */

            /*********************************************************************************
             ** NETMSG_PING                                                                 **
             *********************************************************************************

            case NETMSG_PING:
                dwPing=dlcs_get_tickcount()-RecvData->dwRead();
                break;


                */



            default:

                //Do nothing... (should never execute this code, but who knows...)
                //DLog("Recieved a NETMSG (%d) but could not recognize it.",cLastNetMessage);
                //ChatBufferAdd(255,0,0,"SYSTEM","Recieved a NETMSG (%d) but could not recognize it.",cLastNetMessage);

                break;
        }
    }
}

//////////////////////////////////////////////////////

C_FMGS::C_FMGS(void)
{

    pPingSocket=NULL;
    bLoggedin=false;
    bSystemBusy=false;
    emgdwPing=999;
    initSocket();
}

//////////////////////////////////////////////////////

C_FMGS::~C_FMGS()
{
    DEL(pPingSocket);
}

//////////////////////////////////////////////////////

int C_FMGS::emgConnect(char *pHost,char *pPort,char *pUser,char *pPasswd)
{
    int errnum;

    pLog->_Add(" ********************* CONNECT START ********************* ");

    pLog->_Add("C_FMGS::emgConnect(%s,%s,%s,%s);",pHost,pPort,pUser,pPasswd);

    ToAddr.sin_addr.s_addr  = inet_addr(pHost);
    ToAddr.sin_port         = htons(atoi(pPort));

    bConnected=false;  long fTryTime; int iTry,ret=0,iLength,iFlags,iT;

    struct sockaddr_in RetAddr;

    CPacket Send(NET_DATAGRAMSIZE);
    CPacket Recv(NET_DATAGRAMSIZE);

    bLoggedin = false;

    if(emgeGetState()==NET_NOTCONNECT) return 0;

    bCanSend = true;

    CloseSocket(iSocket);

    iSocket=zOpenSocket(0);

    if(iSocket == SOCKET_ERROR)
    {
        pLog->_Add(" ********************* NO SOCKET ********************* ");
    }

    pLog->_Add("C_FMGS::Connect() zOpenSocket passed");

    if(iSocket == -1)    {        pLog->_Add("C_FMGS::Connect() Can't open network socket!");        return 0;        }

    bConnected = false;

    ToAddr.sin_addr.s_addr  = inet_addr(pHost);
    ToAddr.sin_port         = htons(atoi(pPort));

    pLog->_Add("%s:%d", pGetRemoteIPAddress(), iGetRemotePort());

    bCanSend = false;

    Send.Reset();
    Send.Write(0);
    Send.Write(0);  // save header space which is filled in later
    Send.Write((char)CTL_CONNECT);
    Send.Write("EMBER");
    Send.Write(atoi(NET_REVISION));
    FinishCtlPacket(&Send);

    for(iTry = 0; iTry < (int)dConnectRetry ;iTry++)
    {
        if( nSend(  iSocket, Send.pGetPacketBuffer(),
                     Send.iGetCurSize(),
                    (struct sockaddr*) &ToAddr)==-1 )
        {
            pLog->_Add("C_FMGS::Connect() nSend Failed [%s]", NET_pGetLastError());
            return 0;
        }

        fTryTime = dlcs_get_tickcount();

        do
        {
            ret = nRecv(Recv.pGetPacketBuffer(),Recv.iGetMaxSize(),(struct sockaddr*)&RetAddr);
            if (ret == -1)            {                pLog->_Add("C_FMGS::Connect() -> connection read failed");                return 0;            }
            if (ret > 0)
            {
                Recv.SetCurSize(ret);
                iLength = ntohl(Recv.iRead());
                iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
                iLength &= NET_FLAG_LENGTH_MASK;
                if (!(iFlags & NET_FLAG_CTL))               { ret=0; continue; }
                if (iLength != ret)                         { ret=0; continue; }
                if (iLength < (int)(sizeof(int)*2+sizeof(char)*2)) { ret=0; continue; }
                break;
            }
        } while((ret == 0) && ((dlcs_get_tickcount() - fTryTime) < dConnectTimeOut));
        if(ret) break;
    }

    if(!ret) return 0;

    Recv.iRead();

    char ctltype = Recv.cRead();

    if(ctltype == CTL_REJECT) return 0;
    if(ctltype != CTL_ACCEPT) return 0;

	pLog->_Add("Got a CTL_ACCEPT here");

	bConnected=true;

	memcpy(&ToAddr,&RetAddr,sizeof(sockaddr));

    iT = Recv.iRead();


    ToAddr.sin_port = htons(iT);

    //CloseSocket(iSocket);
    //iSocket=zOpenSocket(iT);

    pLog->_Add("SOCKET PORT SET TO (i=%d)%d",iT,ntohs(ToAddr.sin_port));

    bConnected = true;
    bCanSend = true;

    emgSetState(NET_CONNECTED);


    Send.Reset();
    Send.Write((char)NETMSG_LOGIN_REQUEST);
    Send.Write((char *)pUser);
    Send.Write((char *)pPasswd);
    SendUnreliableMessage(&Send); // Log in

    return 1;
}

void C_FMGS::emgDisconnect()
{
    pLog->_Add("Disconnecting (%d) %s:%d",iSocket, pGetRemoteIPAddress(), iGetRemotePort());

    CPacket SendData(NET_DATAGRAMSIZE);
    SendData.Reset();
    SendData.Write((char)NETMSG_LOGOUT);
    int retval;
    retval=SendUnreliableMessage(&SendData);
    // pLog->_Add("C_FMGS::Disconnect() (%d) [%s]", retval  , NET_pGetLastError());
}

//////////////////////////////////////////////////////////////////////////

//void C_FMGS::emgSendNetMessage(char cMethod)
//{
/*
    if(emgeGetState()==NET_NOTCONNECT) return;
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
//}

//////////////////////////////////////////////////////////////////////////

int C_FMGS::emgiGetMessage()
{
    int i;
    if (emgeGetState() == NET_NOTCONNECT)
        return 0;
    i = iGetMessage();
    if(i==-1) i=0;
    return i;
}

//////////////////////////////////////////////////////////////////////////

eConState C_FMGS::emgeGetState()
{
    if(!this)       return NET_NOTCONNECT;
    if(!bLoggedin)  return NET_LOGGINPROC;
    if(bSystemBusy) return NET_SYSBUSY;
                    return NET_CONNECTED;
}

//////////////////////////////////////////////////////////////////////////

void C_FMGS::emgSetState(eConState eNewState)
{
    if(eNewState == NET_CONNECTED)
        bLoggedin=true;

    if(eNewState == NET_NOTCONNECT)
    {
        bLoggedin=false;
    }
    if(eNewState == NET_SYSBUSY)
        bSystemBusy=true;
}

//////////////////////////////////////////////////////////

long C_FMGS::emgPing(char *pHost,char *pPort,long &dwPing,bool &bPinging,long &dwFuncTimer,long &dwWaitTimer)
{
    //static bool bPinging=false;
    //static long dwFuncTimer=dlcs_get_tickcount();
    //static long dwWaitTimer=dlcs_get_tickcount();

    int ret,iLength,iFlags;
    CPacket Send(NET_DATAGRAMSIZE);
    CPacket Recv(NET_DATAGRAMSIZE);

    //CInetAddress
    struct sockaddr RetAddr;

    if(!pPingSocket) { pPingSocket=new CCSocket(); bPinging=false; }
    if(!pPingSocket) { bPinging=false; return dwPing; }
    if(!bPinging)
    {
        if(dlcs_get_tickcount()-dwFuncTimer<5000) { bPinging=false; return dwPing; }
        dwFuncTimer=dlcs_get_tickcount();
        pPingSocket->bCanSend=true;

        pPingSocket->OpenSocket(pHost,atoi(pPort));

        if(pPingSocket->iSocket >= 0)
        {
            //pPingSocket->NET_SetSocketPort(&pPingSocket->stAddr,atoi(pPort));
            pPingSocket->bConnected=false;
            pPingSocket->bCanSend=false;
            Send.Reset();
            Send.Write(0);  // save header space which is filled in later
            Send.Write(0);
            Send.Write((char)CTL_PING);
            Send.Write((long)dlcs_get_tickcount());
            Send.Write(atoi(NET_REVISION));
            pPingSocket->FinishCtlPacket(&Send);
            pPingSocket->nSend(Send.pGetPacketBuffer(),Send.iGetCurSize(),(struct sockaddr*)&pPingSocket->ToAddr);
            bPinging=true;
            dwWaitTimer=dlcs_get_tickcount();
        }
    }
    else
    {
        ret = pPingSocket->nRecv(Recv.pGetPacketBuffer(),Recv.iGetMaxSize(),&RetAddr);
        if(ret==-1) ret=0;
        if(ret>0)
        {
            Recv.SetCurSize(ret);
            iLength = ntohl(Recv.iRead());
            iFlags  = iLength & (~NET_FLAG_LENGTH_MASK);
            iLength &= NET_FLAG_LENGTH_MASK;
            if(!(iFlags & NET_FLAG_CTL)) ret=0;
            if(iLength != ret)           ret=0;
            if(iLength < (int) (sizeof(int)*2+sizeof(char)*2)) ret=0;
        }
        if(ret==0)
        {
            if(dlcs_get_tickcount()-dwWaitTimer>5000)
            {
                dwPing=9994;
                bPinging=false;
            }
        }
        else
        {
            Recv.iRead();
            char ctltype = Recv.cRead();
            if(ctltype == CTL_PING)
            {
                dwPing=(dlcs_get_tickcount()-Recv.dwRead());
                bPinging=false;
            }
        }
    }
    return dwPing;
}

