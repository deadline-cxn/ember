/***************************************************************
 **
 **   DLSTORM   Deadline's Code Storm Library
 **
 **          /\
 **   ---- D/L \----
 **       \/
 **
 **   License:      BSD
 **   Copyright:    2013
 **   File:         manclient.h
 **   Description:  MANTRA GAME CLIENT / DLSTORM GAME ENGINE / by Seth Parson
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ******************************************************************************/
#ifndef _MANTRA_GAME_CLIENT
#define _MANTRA_GAME_CLIENT

#include "dlcs.h"

#include "c_data.h"
#include "c_timer.h"
#include "c_sdl.h"
#include "c_gui.h"
#include "c_gui_chat_enum.h"
#include "c_log.h"
#include "c_net.h"
#include "c_entity.h"
#include "net_messages.h"
#include "c_snd.h"

#include "mantra_version.h"
#include "mantra_common.h"
#include "manmaster.h"


///////////////////////////////////// Game operating modes
enum MODE {
    DUMMY_DUMMY_MODE=0,
    MAIN_MENU,
    MAIN_MENU_2,
    RETRO_INTRO_INIT,
    RETRO_INTRO_PLAY,
    CREDITS,
    CONNECT,
    CONSOLE_CONNECT,
    GLOBAL_OPTIONS,
    LOGIN_AUTO_LOGIN,
    LOGIN_FROM_GUI,
    LOGIN_SCREEN_ENTRY,
    GATHER_SERVER_LIST,
    CHOOSE_SERVER_INIT,
    CHOOSE_SERVER,
    PROFILE_SELECT,
    CONNECT_TO_SERVER,
    CONNECT_CHECK,

    GET_SERVER_INFORMATION_START,
    GET_SERVER_INFORMATION_SPIN,

    GET_CHARACTERS,
    GET_CHARACTERS_SPIN,
    GET_CHARACTERS_SEND,
    GET_CHARACTERS_RECV,

    CHOOSE_CHARACTER_STAGE,

    CHOOSE_CHARACTER,
    CHOOSE_CHARACTER_SPIN,

    CREATE_CHARACTER,
    CREATE_CHARACTER_SPIN,

    CREATE_CHARACTER_SEND,
    CREATE_CHARACTER_SEND_SPIN,

    LOGIN_SEND,
    LOGIN_RECV,
    LOGIN_OK,
    LOGIN_FAILED,
    GATHER_GAME_DATA,

    /*
    SI_RACES_SEND,
    SI_RACES_RECV,
    SI_ATTRIBUTES_SEND,
    SI_ATTRIBUTES_RECV,
    SI_CLASSES_SEND,
    SI_CLASSES_RECV,
    SI_SKILLS_SEND,
    SI_SKILLS_RECV,
    */

    //SI_GETCHARS_SEND,
    //SI_GETCHARS_RECV,

    SI_GENERIC_SEND,
    SI_GENERIC_RECV,

    LOGIN_PROGRAM_START,
    LOGIN_PROGRAM_END,

    GATHER_GAME_DATA_LOADER,
    GET_SELECTED_CHARACTER_SEND,
    GET_SELECTED_CHARACTER_RECV,
    DELETE_CHARACTER_SEND,
    DELETE_CHARACTER_RECV,
    INITIALIZE_GAME,
    ITEM_INITIALIZE,
    GAME_ON,
    GAME_LIMBO,

    GAME_EDITOR,

    EDIT_WORLD_INIT,
    EDIT_WORLD,

    EDIT_MODELS_INIT,
    EDIT_MODELS,

    QUIT,
    WAIT_LOOP,
    DEBUG_LOOP,
};
///////////////////////////////// Function declarations
#ifdef __cplusplus
extern "C"{
#endif
bool    DoGameMode();
void    MainGameLoop(void);
bool    doInit(void);
void    ShutDown(void);
void    DoSDLEvents(void);
void    SetGameMode(int);
#ifdef __cplusplus
}
#endif
///////////////////////////////// Network
class C_FMGS : public CCSocket {
public:
    C_FMGS();
    virtual ~C_FMGS();
    long        emgPing(char *pHost,char *pPort,long &dwPing,bool &bPinging,long &dwFuncTimer,long &dwWaitTimer);//
    long        Ping(void);
    int         emgConnect(char *pHost, char *pPort,char *pUser,char *pPasswd);
    void        emgSendNetMessage(char cMethod);
    void        emgSetState(eConState eNewState);       // state of the connection
    int         emgiGetMessage(void);                          // tries to receive a message
    void        emgDisconnect(void);
    eConState   emgeGetState(void);
    CCSocket    *pPingSocket;                               // lowlevel network interface
    long        dwPing;
    bool        bLoggedin;                                  // is the player logged in
    bool        bSystemBusy;                                // set if internal stuff to do
    char        cLastNetMessage;
    bool        bConnected;
    void        GetFile(char *filename);
    void        DoNetwork(void);
    void        Chat(char *msg,char *name, int channel);
    CTimer      *spin_timer;
};
#endif // _MANTRA_GAME_CLIENT
