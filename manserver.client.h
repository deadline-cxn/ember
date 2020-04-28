// MANTRA

#ifndef _MANTRA_S_CLIENT_H
#define _MANTRA_S_CLIENT_H

#include "c_gui_chat_enum.h"
#include "c_log.h"
#include "c_net.h"
#include "c_serverinfo.h"
#include "c_timer.h"
#include "dlcs.h"
#include "manserver.server.h"
#include "mantra_common.h"
#include "mantra_player_character.h"
#include "mantra_userdata.h"
#include "mantra_version.h"
#include "net_messages.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
class CServer;
class CMPCharacter;

class C_GSC : public CCSocket, public C_UserData {
   public:
    C_GSC(CLog *pInLog, CServer *pInServer);
    virtual ~C_GSC();

    void load(void);
    void save(void);

    CServer *pServer;  // parent server object
    void     db_update(string s_col, string s_val);

    CLog *pLog;      // log object
    bool  bMadeLog;  // internal destruction flag

    bool bDelete;          // flag for object destruction
    bool bHeartBeatCheck;  // heartbeat flag

    CTimer *inactivity_timer;
    CTimer *heartbeat_timer;

    C_GSC *pNext;
    C_GSC *pPrevious;

    CMPCharacter *character;

    void initc(void);

    void do_script(char *file);
    void do_func(char *func);
    void do_net(void);

    // commands to push net messages to the actual client follow :
    bool gui_clear(void);
    bool gui_call(char *name);
};

#endif  // _MANTRA_S_CLIENT_H
