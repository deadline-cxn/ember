// EMBER

#ifndef _EMBER_NETWORK_COMMON_H
#define _EMBER_NETWORK_COMMON_H

#define NET_DEFAULT_EGS_IP             "localhost"
#define NET_DEFAULT_EGS_PORT           40404
#define NET_DEFAULT_EMS_IP             "localhost"
#define NET_DEFAULT_EMS_PORT           40403

typedef enum
{
    LOGIN_WHAT,
    GOOD_LOGIN,
    BAD_LOGIN,
    NEW_ACCOUNT,
    ALREADY_LOGGED_IN,
    TOO_MANY_PLAYERS,
    SERVER_MAXXED_OUT,
    ACCOUNT_EXPIRED,
    MASTER_LOGIN,
} eLoginStuff;

typedef enum
{
    NET_NOTCONNECT=0,     // not connected to any host
    NET_LOGGINPROC,     // connected but not logged in
    NET_SYSBUSY,        // if some class internal things (filetransfer) are done
    NET_CONNECTED,      // ok normal connecting
} eConState;

typedef enum{
    FVM_NOP=0,
    FVM_PLAYSOUND,
    FVM_PLAYMUSIC,
    FVM_LOADBACKGROUND,
    FVM_MOUSEINFO,
    FVM_GUICOMPONENT,
    FVM_PROMPT,
    FVM_GUIBUTTONPRESSED,
    FVM_GETTARGET,
    FVM_MOVECAM,
    FVM_SET_MAP_DRAW,
    FVM_SET_GAME_MODE,
    FVM_SET_DAYLIGHT,
    FVM_SET_LIMBO_STATE,

} tFVMstuff;


#endif//_EMBER_NETWORK_COMMON_H
