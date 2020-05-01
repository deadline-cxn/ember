/////////////////////////////////////////
#ifndef _MANTRA_SERVER_H
#define _MANTRA_SERVER_H
/////////////////////////////////////////

#include "c_console.h"
#include "c_gui_chat_enum.h"
#include "c_log.h"
#include "c_net.h"
#include "dlcs.h"
#include "manserver.client.h"
#include "mantra_common.h"
#include "mantra_version.h"

/////////////////////////////////////////

/*
struct RetainedData {  // will be stored in a file in cwd, and saved into databases server_config table
    char s_name[MANTRA_TEXT_LEN];
    char s_admin_email[MANTRA_TEXT_LEN];
    int  i_port;
    int  i_max_clients;
    bool b_new_accounts;
    bool b_require_website;
    char s_website_link[MANTRA_TEXT_LEN];
    char s_motd[MANTRA_TEXT_LEN];
    char s_db_type[MANTRA_TEXT_LEN];  // file, mysql, sqlite
    //
    char s_db_sqlite_file[MANTRA_TEXT_LEN];
    //
    char s_db_mysql_host[MANTRA_TEXT_LEN];
    char s_db_mysql_port[MANTRA_TEXT_LEN];
    char s_db_mysql_db[MANTRA_TEXT_LEN];
    char s_db_mysql_user[MANTRA_TEXT_LEN];
    char s_db_mysql_pass[MANTRA_TEXT_LEN];
    //
    char s_db_lmdb_1[MANTRA_TEXT_LEN];
    char s_db_lmdb_2[MANTRA_TEXT_LEN];
    char s_db_lmdb_3[MANTRA_TEXT_LEN];
    char s_db_lmdb_4[MANTRA_TEXT_LEN];
    char s_db_lmdb_5[MANTRA_TEXT_LEN];
};
*/

class C_GSC;

class CServer : public CCSocket {
   public:
    CServer(bool bIsQuiet);
    virtual ~CServer();

    // C_SQLite *   pSQLite; // add dlcs_db

    CLog *  pLog;
    C_GSC * pFirstPlayer;
    bool    bQuit;
    bool    bRestart;
    bool    bQuiet;
    long    dwStartTime;
    C_CONS *pConsole;

    void        StartUp(void);
    bool        bNetStartup(void);
    void        shut_down(void);
    void        cycle(void);
    bool        check_restart(void);
    void        console_command(char *command);
    void        SetupConsoleHistory(void);
    void        RemoveConsoleHistory(void);
    void        AddToConsoleHistory(char *s);
    char *      GetConsoleHistory(int x);
    void        do_keyboard(void);
    bool        save_data(void);
    C_GSC *     get_client(const char *user_name);  // network / client stuff
    void        accept_connection(void);
    void        reject_connection(int iReSocket, sockaddr *pReAddr, const char *reason);
    void        send_all(CPacket *pPacket, float fBlockTime);
    void        disconnect(C_GSC *pClient, const char *reason);
    int         num_clients(void);
    void        purge_clients(void);
    void        do_world(void);
    void        save_world(void);
    void        load_world(void);
    void        kick_user(const char *name, const char *reason);
    void        kick_all(const char *reason);
    void        chat(C_GSC *pClient, const char *from, const char *msg, int iChannel);
    void        create_guid(const char *cin, char *cout);
    void        add_user(const char *in, u_char ia);
    void        delete_user(const char *szName);
    void        set_user_access(const char *szInUserName, int iAccess);
    const char *get_user_access(const char *szInUserName);

    // bool        LoadServerIni(const char *szFilename);
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
