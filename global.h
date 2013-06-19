/***************************************************************

	Ember

 ***************************************************************
 ** Global Stuff                                              **
 ***************************************************************/

#ifndef _EMBER_GLOBALS_H
#define _EMBER_GLOBALS_H

#include "dlstorm_lib.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VERSION             "v05.08.16a"    // Version format change to vYY.MM.DDa
#define NET_REVISION        "01"            // Network protocol version
#define COPYRIGHT           "(C) 2005 Seth Parson"
#define EGC_REVISION        "00"
#define EGS_REVISION        "00"
#define EMS_REVISION        "00"

#define NAME   "Ember"
#define TITLE  "Ember"
#define AUTHOR "Seth Parson"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define SCREEN_COLORS 16


//////////////////////////////////////////////////////////////////////////////////////////////////
// Maximum values

#define MAX_SERVERS					1024    // Servers that can be listed
#define MAX_SERVER_CHARS			8		// Character slots per server
#define MAX_PLAYERS					100		// Players per server
#define MAX_GUIS					255		// GUI objects
#define MAX_OPEN_GUIS				64		// GUI objects that can be open at once
#define MAX_CLANS					512		// Clans per server
#define MAX_GUILDS					32		// Guilds per server
#define MAX_ACTIONS					128		// Actions that a player can perform
#define MAX_CLIENT_ITEMS		    4096	// Items per server/mission (Client Side)
#define MAX_ITEM_STORE				512     // Item Storage -> unique item types (Server Side)
#define MAX_INVENTORY_ITEMS         512     // Player inventory items
#define MAX_CONTAINER_ITEMS         512     // Container items
#define MAX_TILE_ITEMS              32      // Items per map tile
#define MAX_SERVER_ITEMS			4096  	// Total number of active items on the server world (No more than 65535!)
#define MAX_RACES					1024	// Races per server/mission
#define MAX_RACE_ADJ				16		// Special attributes/skills adjustments per race
#define MAX_RACE_BIAS				16		// Race biases per race
#define MAX_CLASSES					128     // Classes per server/mission
#define MAX_SKILLS					128		// Skills per server/mission
#define MAX_MAGIC					16		// Magic schools per server
#define MAX_SPELL_LEVEL				16		// Spell levels per magic school
#define MAX_SPELLS					16		// Spells per level (16x16x16 = 4096 spells - should be more than plenty)
#define MAX_CONSOLE_BUFFER			1024	// Lines in console scrollback buffer
#define MAX_CONSOLE_MULTI_COMMANDS	32		// Number of console commands on a single line seperated by ;

#define MAX_ATTRIBUTES				16		// Number of attributes per server/mission
#define MAX_WEAPON_RANGE			32		// Max weapon range for missle weapons
#define MAX_TREASURE_TYPES			2048	// Different treasure types that can be aquired
#define MAX_CHAT_BUFFER				10		// Show some chat lines on the screen
#define MAX_ENTITIES				512		// Maximum entities on screen
#define MAX_BASE_GFX				256	    // GFX Index array
#define MAX_WAV_BUFFER				64		// How many sounds can be played at one time
#define MAX_AI_WAYPOINTS			32		// Waypoints
#define MAX_MONEY_TYPES				16		// Different monetary units
#define MAX_ACCESS_GROUPS			256 	// Access groups
#define MAX_ATTRIBUTE_POINTS		256		// Distributable attribute points
#define MAX_BUTTONS					128		// Button objects
#define MAX_MOUSE_CURSORS			16		// Mouse cursor objects
#define MAX_TELEPORTS               2048    // Teleports that can be defined
#define	MAX_STRING_CHARS	        1024	// Length of a string
#define	MAX_STRING_TOKENS	        256		// Tokens
#define	MAX_TOKEN_CHARS		        1024	// Length of token
#define MAX_FVM                     512     // FVM files that can be loaded into memory
#define MAX_DATAGRAM                1024    // Length of unreliable message
#define MAX_ICONS                   128     // System Icons
#define MAX_PROFILES                32      // max login profiles
#define MAX_FONTS                   32
#define MAX_MODELS                  128

#define FM_NORTH        5
#define FM_NORTHEAST    6
#define FM_NORTHWEST    4
#define FM_SOUTH        1
#define FM_SOUTHEAST    0
#define FM_SOUTHWEST    2
#define FM_EAST         7
#define FM_WEST         3

#ifndef NULL
#define NULL 0
#endif




// Login results

#define GOOD_LOGIN            1
#define BAD_LOGIN             2
#define NEW_ACCOUNT           3
#define ALREADY_LOGGED_IN     4
#define TOO_MANY_PLAYERS      5
#define SERVER_MAXXED_OUT     5
#define ACCOUNT_EXPIRED       6
#define MASTER_LOGIN          7

typedef enum
{
    CON_NOTCONNECT,     // not connected to any host
    CON_LOGGINPROC,     // connected but not logged in
    CON_SYSBUSY,        // if some class internal things (filetransfer) are done
    CON_CONNECTED,      // ok normal connecting
} eConnectionState;

typedef enum {



} eCtlMessages;

typedef enum
{
    NET_HOSTPORT,
    NET_ALIVETIMEOUT,
    NET_CONNECTRETRY,
    NET_CONNECTTIMEOUT,
    NET_ACKTIMEOUT,
    NET_GUESSREBOOT,
    NET_MASTERRETRY,
    NET_MASTERTIMEOUT,
    NET_MAXBACKUPSIZE,
    NET_MAXBACKUPQUEUE,
    NET_LASTOPTION,
}eNetOptions;


// NET MESSAGES
// Any time you modify this section, you must update the NET_VERSION at the top then recompile
// all applications... Sorry, this is the law

typedef enum{
	CTL_CONNECT = 1,
	CTL_SERVERINFO,
	CTL_SERVERINFO_GET,
	CTL_SERVERINFO_PUT,
	CTL_SERVERINFO_RESET,
	CTL_SERVERINFO_END,
	CTL_PLACEHOLDER,
	CTL_REJECT,
	CTL_ACCEPT,
	CTL_PING,
	CTL_KILLFMMS,
    NETMSG_NOMESSAGE,
    NETMSG_NOP,
    NETMSG_LOGIN_REQUEST,
    NETMSG_LOGIN_REQUEST_REPLY,
    NETMSG_LOGOUT,
    NETMSG_SYSTEMMSG,
    NETMSG_LOCALCHAT,
    NETMSG_VIS_UPDATE,
    NETMSG_VIS_REMOVE,
    NETMSG_MOVEPLAYER,
    NETMSG_CREATE_CHARACTER,
    NETMSG_MODIFY_CHARACTER,
    NETMSG_RETRIEVECHARS,
    NETMSG_RETRIEVECHARINFO,
    NETMSG_GETINVENTORY,
    NETMSG_CHANGETILE,
    NETMSG_CHANGEOBJECT,
    NETMSG_CLIENTSHUTDOWN,
    NETMSG_DELETE_CHARACTER,
    NETMSG_GENERIC_MSG,
    NETMSG_SET_TILE,
    NETMSG_SET_OBJECT,
    NETMSG_CHANGE_MODE,
    NETMSG_MODIFY_MAP,
    NETMSG_SET_VERTEX,
    NETMSG_HEARTBEAT,
    NETMSG_HEARTBEAT_REQUEST,
    NETMSG_CHAR_STATUS,
    NETMSG_EMBER__DOWNLOAD,
    NETMSG_EMBER__UPLOAD,
    NETMSG_PING,
    NETMSG_PROMPT,
    NETMSG_FVM,
    NETMSG_SERVER_INFO,
    NETMSG_PUT_SERVER_INFO,
    NETMSG_GET_SERVER_LIST,
    NETMSG_MASTER_TO_GAME,
    NETMSG_GUI,
	NETMSG_GUI_COMMAND,
    NETMSG_GET_LOGIN_PROGRAM,
	NETMSG_FILE_XFER,


} tNetmsg;

//  NETMSG_MODE_CHANGE,

/*  NETMSG_ITEM_REMOVE,
    NETMSG_ITEM_MOVE,
    NETMSG_ITEM_CREATE,
    NETMSG_ITEM_LIST,
    NETMSG_ITEM_INFO,
    NETMSG_ITEM_USE, */


#define FM_GP_NOPROPS           -1
#define FM_GP_ALWAYS_ON_TOP      1
#define FM_GP_SIZEABLE           2
#define FM_GP_CLOSEX             4
#define FM_GP_MOVEABLE           8
#define FM_GP_MENU              16
#define FM_GP_USEBACKGROUND     32
#define FM_GP_BORDER            64
#define FM_GP_MINIMIZE         128
#define FM_GP_CLIENT_ONLY      256
#define FM_GP_ALWAYS_ON_BOTTOM 512
#define FM_GP_NO_RETURN       1024

#define FM_GSM_ALPHA_ASCEND  1
#define FM_GSM_ALPHA_DESCEND 2

typedef enum
{
    FM_GC_BUTTON=1,
    FM_GC_SUBMIT,
    FM_GC_CANCEL,
    FM_GC_INPUT,
    FM_GC_TEXTBOX,
    FM_GC_STATIC_TEXT,
    FM_GC_LISTBOX,
    FM_GC_3D_BOX,
    FM_GC_MESSAGE,
    FM_GC_CHATBOX,
    FM_GC_SELECT,
    FM_GC_TICKBOX,
    FM_GC_HIDDEN,
    FM_GC_MENU,
    FM_GC_DROPBOX,
    FM_GC_STATIC_IMAGE,
    FM_GC_REPORTER,
    FM_GC_GROUPTICK

} tGUIComponentTypes;

typedef enum{
    FM_GUI_PROGRAM_FINISH=0,
    FM_GUI_STUMP_CREATE,
    FM_GUI_STUMP_UPDATE,
    FM_GUI_STUMP_REMOVE,
    FM_GUI_STUMP_CAPTION,
    FM_GUI_CONTROL_CREATE,
    FM_GUI_CONTROL_UPDATE,
    FM_GUI_CONTROL_REMOVE,
    FM_GUI_CONTROL_VALUE,
    FM_GUI_CLEAR_ALL,

} tGUINetworkMessages;



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

typedef enum{
    SI_GENERIC=0,
    SI_GET_SERVER_LIST,
    SI_PUT_SERVER,
} tServerInfoUpdates;

typedef enum{
    NET_FILE_NOP=0,
    NET_FILE_START,
	NET_FILE_START_OK,
    NET_FILE_DATA,
	NET_FILE_DATA_OK,
	NET_FILE_DATA_RESEND,
    NET_FILE_ACK,
    NET_FILE_ABORT,
    NET_FILE_EOF,
	NET_FILE_EOF_RESEND,
    NET_FILE_RESUME,
	NET_FILE_ERROR,
} tXferMode;

typedef enum{
    NET_FILE_RES_NONE=0,
    NET_FILE_RES_MEDIA,
    NET_FILE_RES_SCRIPT,
} tXferType;


// Game play modes

#define PLAY            0
#define BUILD_GHOST     1
#define BUILD           2
#define SPECTATOR       3
#define SPECTATOR_LOGIN 4
#define CHATROOM        5
#define LOGIN           6

#define DOWN			0
#define UP				1

// Fragmere GUI system default buttons

#define BC_ARROW_LEFT	3
#define BC_ARROW_RIGHT	4
#define BC_ARROW_UP		2
#define BC_ARROW_DOWN	1
#define BC_SCROLLBAR_UD 5
#define BC_SCROLL_SLIDE 6
#define BC_TICKBOX      7
#define BC_FREEBSD      8
#define BC_LINUX        9
#define BC_WIN32        10
#define BC_SCROLLBAR_LR 11
#define BC_NEXT			12
#define BC_OK			13
#define BC_CANCEL		14
#define BC_BACK			15

#define BC_SAVE			17
#define BC_BLANK_BUTTON 18
//#define BC_DISCARD		18
#define BC_MINUS		19
#define BC_PLUS			20
#define BC_VERTEX       21
#define BC_CLOSE        22
#define BC_MINIMIZE     23

typedef enum {
    NORMAL=0,
    CONSOLE1,
    CONSOLE2,
    SERVERNAME,
    SERVERIP,
    SERVERPORT,
    USERNAME,
    PASSWORD,
    GLOBALCHATINPUT,
    LOCALCHATINPUT,
    CHARACTERNAME,
} INMODE;

typedef enum
{
    LM_NONE=0,
    LM_INTERNET,
    LM_FAVORITES,
    LM_LOCAL
} LMODE;


#endif // _EMBER_GLOBALS_H
