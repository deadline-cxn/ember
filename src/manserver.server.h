/////////////////////////////////////////
#ifndef _MANTRA_SERVER_H
#define _MANTRA_SERVER_H
/////////////////////////////////////////

#include "c_console.h"
#include "c_gui_chat_enum.h"
#include "c_log.h"
#include "c_net.h"
#include "c_sqlitedb.h"
#include "dlcs.h"
#include "manserver.client.h"
#include "mantra_common.h"
#include "mantra_version.h"

//#define MAX_CONSOLE_INPUT_BUFFER    10		// Number of console commands to scroll back to execute again

/////////////////////////////////////////

struct RetainedData {
    char s_name[MANTRA_TEXT_LEN];
    char s_admin_email[MANTRA_TEXT_LEN];
    int  i_port;
    int  i_max_clients;
    bool b_new_accounts;
    bool b_require_website;
    char s_website_link[MANTRA_TEXT_LEN];
    char s_motd[MANTRA_TEXT_LEN];
};

class C_GSC;
class CServer : public CCSocket {
   public:
    CServer(bool bIsQuiet);
    virtual ~CServer();

    /////////////////////////////////////////
    // flow stuff

    bool bQuit;
    bool bRestart;

    void start_up(void);
    void shut_down(void);
    void cycle(void);
    bool check_restart(void);

    /////////////////////////////////////////
    // console stuff

    bool  bQuiet;
    CLog *pLog;
    void  Log(string s);
    void  Log(const char *fmt, ...);

    void console_command(char *command);
    char szConsoleInput[1024];
    int  ConsoleHistoryPosition;
    struct ConHistory {
        char text[1024];
    };
    ConHistory *ConsoleHistory;

    void  SetupConsoleHistory(void);
    void  RemoveConsoleHistory(void);
    void  AddToConsoleHistory(char *s);
    char *GetConsoleHistory(int x);

    void do_keyboard(void);

    /////////////////////////////////////////
    // unretained data

    long start_time;

    /////////////////////////////////////////
    // retained data

    bool load_data(void);
    bool save_data(void);

    RetainedData r_data;

    /////////////////////////////////////////
    // network / client stuff

    C_GSC *pFirstPlayer;
    C_GSC *get_client(const char *user_name);

    void accept_connection(void);
    void reject_connection(int iReSocket, sockaddr *pReAddr, const char *reason);
    void send_all(CPacket *pPacket, float fBlockTime);
    void disconnect(C_GSC *pClient, const char *reason);
    int  num_clients(void);
    void purge_clients(void);

    void do_world(void);
    void save_world(void);
    void load_world(void);

    void kick_user(const char *name, const char *reason);
    void kick_all(const char *reason);
    void chat(C_GSC *pClient, const char *from, const char *msg, int iChannel);

    void create_guid(const char *cin, char *cout);

    void add_user(const char *in, u_char ia);
    void remove_user(const char *szName);
    void user_access(const char *in_user_name, int in_access);

    // void    ReportToMaster(void);
    // CPacket *pServerInfo;
    // void    SetServerInfo(CPacket *pPacket){ pServerInfo = pPacket; }
    // CPacket *pGetServerInfo(void){ return pServerInfo; } // serverinfo getter and setter
    // void    SetUpMessage(CPacket *pPacket); // configure the use of the master server
    // void    SetMaster(CMasterAddress *pMaster);
    // CMasterAddress *pGetMaster();
    // CMasterAddress *pMa;
    // char   szLastAddress[1024];
    // time_t  pServerTime;
    // struct  tm *tTime;
};
/////////////////////////////////////////
#endif  //_MANTRA_SERVER_H
/////////////////////////////////////////
