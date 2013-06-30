// EMBER

#ifndef _EMBER_S_CLIENT_H
#define _EMBER_S_CLIENT_H

#include "ember_network_common.h"
#include "c_log.h"
#include "c_net.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////

class C_GSC
{
public:
    C_GSC();
    C_GSC(CLog *pLog);
    ~C_GSC();
	// char		name[1024];

    CLog    *pLog;
	bool    bMadeLog;

	void		VortexInsert(void);

	int			sid;
	char		access;
    int         windex;

    struct   sockaddr_in    FromAddr;

    char username[32];
    char password[32];

    bool    bDelete;

    //void        ecsDisconnect(void);
    //void        ecsSetState(eConState eNewState);       // state of the connection
    //void        ecsSetSocket(CCSocket *pNewSocket);             // sets the lowlevel netinterface
    CCSocket     *pSocket;                                   // lowlevel network interface
    //CCSocket     *pGetSocket(void);                          // gets the lowlevel netinterface
    //bool        bLoggedin;                                  // is the player logged in
    //bool        bSystemBusy;                                // set if internal stuff to do
    // int         iGetMessage(void);                          // tries to receive a message
    //char        *get_remote_ip(void);

    C_GSC       *pNext;
    C_GSC       *pPrevious;
    void        do_script(char *file);
    void        do_func(char *func);
    void        do_net(void);

    // CPacket     *SendPacket;

    // commands to push net messages the actual client follow :

    bool        AddGUIStump     (char *name,int x,int y,int x2,int y2,int props,char *media);
    bool        ModGUIStump     (char *name,int x,int y,int x2,int y2,int props,char *media);
    bool        DelGUIStump     (char *name);
    bool        GUIStumpCaption (char *name,char *caption);
    bool        AddGUIControl   (char *stump,char *name,int type,int props,int x,int y,int x2,int y2,char *media,char *value);
    bool        DelGUIControl   (char *stump,char *name);
    int         ModGUIControl   (char *stump,char *name);
    bool        GUIControlValue (char *stump,char *name,char *value);
    bool        ClearGUI        (void);

    bool        SetDrawMap      (int dm);
    bool        SetLimboState   (int lm);
    bool        SetGameMode     (int gm);
    bool        SetDayLight     (float r,float g,float b);

    bool        SetCam          (int x,int y,int z, int ax,int ay,int az,int ox,int oy,int oz);
    bool        MoveCamTo       (int x,int y,int z);
    bool        ZoomCamTo       (int zoom);
    bool        ScrollCamTo     (int x,int y,int z);

    bool        VisRemove       (int ntt);
    bool        VisMove         (int ntt,int x,int y,int z,char* media,int media_type,int head,int dir);

    bool        SetMapTile      (int x,int y,int z,int bank,int tile);
    bool        SetMapObject    (int x,int y,int z,int l,int bank,int obj);
    bool        SetMapProp      (int x,int y,int z,int prop);
    bool        SetMapVWidth    (int x,int y,int z,int v,int w);
    bool        SetMapVHeight   (int x,int y,int z,int v,int h);
    bool        SetMapVColor    (int x,int y,int z,int v,char r,char g,char b);

	void		LoginReply(int iLoginType,char * szAnswerMessage);

    // eConState   eGetState(void);

    void fm_client_init(void);
};

#endif // _EMBER_S_CLIENT_H
