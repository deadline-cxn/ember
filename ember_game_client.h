/***************************************************************

 ***************************************************************/

#ifndef _EMBER_GAME_CLIENT
#define _EMBER_GAME_CLIENT

#include "c_gfx.h"
#include "c_data.h"
#include "c_snd.h"

#include "c_gui.h"
#include "c_log.h"
#include "c_var.h"
#include "c_net.h"
#include "ember_version.h"
#include "ember_network_common.h"
#include "ember_game_server_enum.h"
//#include "lua.h"

// Game operating modes


////////////////////////////////////////////////////////////
// Function declarations

#ifdef __cplusplus
extern "C"
{
#endif

    bool    DoGameMode();
	void    MainGameLoop(void);
	bool    doInit(void);
	void    ShutDown(void);
	void    DoSDLEvents(void);
    void    DoNetwork(void);

#ifdef __cplusplus
}
#endif


class C_FMGS : public CCSocket
{
public:

    C_FMGS();
    ~C_FMGS();
    long         emgPing(char *pHost,char *pPort,long &dwPing,bool &bPinging,long &dwFuncTimer,long &dwWaitTimer);//
    int          emgConnect(char *pHost, char *pPort,char *pUser,char *pPasswd);
    void         emgSendNetMessage(char cMethod);
    void         emgSetState(eConState eNewState);       // state of the connection
    int          emgiGetMessage(void);                          // tries to receive a message
    void        emgDisconnect(void);
    eConState   emgeGetState(void);

    CCSocket    *pPingSocket;                               // lowlevel network interface

    long        emgdwPing;
    bool        bLoggedin;                                  // is the player logged in
    bool        bSystemBusy;                                // set if internal stuff to do
    long        dMsgTime;
    char        cLastNetMessage;
    bool        bConnected;
    //CPacket     SendData;
    //CPacket     *RecvData;
};



#endif // _EMBER_GAME_CLIENT
