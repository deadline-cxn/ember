/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_CLIENT_DATA_CLASS
#define _EMBER_CLIENT_DATA_CLASS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "c_net.h"
#include "c_gui.h"
#include "c_gaf.h"
#include "dlstorm.h"

// Game play modes

#define PLAY            0
#define BUILD_GHOST     1
#define BUILD           2
#define SPECTATOR       3
#define SPECTATOR_LOGIN 4
#define CHATROOM        5
#define LOGIN           6

// Game operating modes


enum MODE
{
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


	SI_RACES_SEND,
	SI_RACES_RECV,
	SI_ATTRIBUTES_SEND,
	SI_ATTRIBUTES_RECV,
    SI_CLASSES_SEND,
    SI_CLASSES_RECV,
    SI_SKILLS_SEND,
    SI_SKILLS_RECV,


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

    QUIT,
    WAIT_LOOP,
    DEBUG_LOOP,
};



/*
enum MODE
{
    MAIN_MENU=0,
    MAIN_MENU_2,
    RETRO_INTRO_INIT,
    RETRO_INTRO_PLAY,
    CREDITS,
    CONNECT,
    CONSOLE_CONNECT,
    GLOBAL_OPTIONS,
    LOGIN_AUTO_LOGIN,
    LOGIN_SCREEN_ENTRY,
    GATHER_SERVER_LIST,
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


	SI_RACES_SEND,
	SI_RACES_RECV,
	SI_ATTRIBUTES_SEND,
	SI_ATTRIBUTES_RECV,
    SI_CLASSES_SEND,
    SI_CLASSES_RECV,
    SI_SKILLS_SEND,
    SI_SKILLS_RECV,


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

    QUIT,
    WAIT_LOOP,
    DEBUG_LOOP,
};
*/
class CFM_Profile
{
public:
    CFM_Profile();
    ~CFM_Profile();
    char name[32];
    bool savepw;
    char passwd[32];
    CFM_Profile *pNext;
};

class CC_Data
{
public:
	CC_Data(void);
	~CC_Data(void);

	bool bLoad(void);
	bool bSave(void);

	void CleanUp(void);
	void SetToDefaults(void);

    void ClearFavoriteServers(void);

    void ClearProfiles(void);

    bool LoadFavoriteServers(void);
    bool SaveFavoriteServers(void);
    bool DiscardFavoriteServer(int i);
    //bool AddFavoriteServer(ServerData *pData);

    bool LoadProfiles(void);
    bool SaveProfiles(void);
    bool DiscardProfile(void);
    bool AddProfile(void);

    ///////////////////////////////////////////////////////////////

	char			Name[255];
	char			Password[255];
	bool			bSavePassword;
	char			ServerMessage[1024];
	char			ServerName[1024];
	char			ServerID[15];           // Server / Mission Identifier
    char            szServerVersion[15];
	char			IPAddress[255];
	char			Port[255];
    char            MasterIPAddress[255];
    char            MasterPort[255];
	bool			bLog;
	bool			bDownload;

    bool            bFullScreen;
    bool            bAudioFailure;
    bool            bSound;					    	// Is sound system present?
    bool			bMusic;					    	// Play Music? (MIDI only for now)
	float           fSoundVolume;
	float           fMusicVolume;
	char			MouseLeftButtonSound[1024];
    int             ScreenWidth;
    int             ScreenHeight;
    int             ScreenColors;


    ///////////////////////////////////////////////////////////////
	// Discardable variables:

    bool            drawoptions;

    char            screen_res_640x480x16;
    char            screen_res_800x600x16;
    char            screen_res_1024x768x16;
    char            screen_res_1280x1024x16;

    char            screen_res_640x480x32;
    char            screen_res_800x600x32;
    char            screen_res_1024x768x32;
    char            screen_res_1280x1024x32;

    char            cDebug;                         // Debug level 1 = onscreen messages only 2 = log
    bool            bShowPING;			    		// On or Off show PING
    int				SelectedServer;			    	// For server selection
    char			CharacterSlots;			    	// Character slots on selected server
    unsigned char			SelectedCharacterSlot;	    	// For choosing your character when logging on or creating a character

    char			currentsample;				    // Which sample is positioned

    char			Mode;

	char			ServerAuthor[1024];

	int				ServerListOffset;
	int				ServerCharacterListOffset;
	int				ID;
	unsigned char			Access;
	char			szAccessName[255];

    unsigned long   dwKeyPressTimer;

    bool            bDrawMap;
    bool            bDrawMapObjects;
    bool            bBlockGlow;
    bool            bVertice;
    bool            bServerInfoUpdated[64];

    CFM_Profile     *Profile;
    CFM_Profile     *FirstProfile;

//    ServerData      *FavoriteServer;
//    ServerData      *FirstFavoriteServer;

//	C_ServerInformation *ServerInfo;

	float		    x;
	float		    y;
	float		    z;

    float           fCredscroll;
    long            dwScrollTime;

    bool            bShowOptions;
    bool            bTargeting;
    long            dwRetroTimer;
    char            cRetroMode;


////////////////////////////////////////////////////////////
// Var declarations


    CPacket         SendData; //(8192); // Send Packet

    CPacket         *RecvData;  // Recv Packet

    // LMODE           LOGINMODE;

    MODE            GAME_MODE;
    MODE            LAST_GAME_MODE;
    MODE            NEXT_GAME_MODE;


};

#endif // _EMBER_CLIENT_DATA_CLASS
