//////////////////////////////////////////////////////////
//
// Ember

#include "ember_game_server_client_socket.h"
#include "ember_network_common.h"
#include "ember_game_server_enum.h"
#include "ember_version.h"
#include "c_serverinfo.h"
#include "c_gui.h"


//////////////////////////////////////////////////////////
//C_GSC::C_GSC(void){    fm_client_init();    pLog=new CLog("C_GSC.log");    bMadeLog=true; }
C_GSC::C_GSC(CLog *pInLog)
{
    pLog=pInLog;
    bMadeLog=false;
    fm_client_init();
}
//////////////////////////////////////////////////////////
void C_GSC::fm_client_init(void)
{
    pSocket = NULL;
    bDelete = 0;
	memset(username,0,32);
	memset(password,0,32);

	inactivity_timer=new CTimer(300000);
	heartbeat_timer=new CTimer(10000);
	bHeartBeatCheck=0;

    pNext=NULL;
    pPrevious=NULL;
}
//////////////////////////////////////////////////////////
C_GSC::~C_GSC()
{
    DEL(inactivity_timer);
    DEL(pSocket);
    if(bMadeLog) DEL(pLog);
}
//////////////////////////////////////////////////////////
/*
int C_GSC::iGetMessage()
{
    if(!pSocket)
    {
        pLog->_Add("s_client.cpp -> C_GSC::iGetMessage() tried to operate on a null socket...");
        return 0;
    }
    int i;
    if (eGetState() == NET_NOTCONNECT) return 0;
    i = pSocket->iGetMessage();
    if(i==-1) i=0;
    return i;
}
//////////////////////////////////////////////////////////
eConState C_GSC::eGetState()
{
    if(!this)       return NET_NOTCONNECT;
    if(!pSocket)    return NET_NOTCONNECT;
    if(!bLoggedin)  return NET_LOGGINPROC;
    if(bSystemBusy) return NET_SYSBUSY;
                    return NET_CONNECTED;
}
//////////////////////////////////////////////////////////
void C_GSC::ecsSetState(eConState eNewState)
{
    if((pSocket)&&(eNewState==NET_CONNECTED)) bLoggedin = true;
    if(eNewState == NET_NOTCONNECT)
    {
        bLoggedin = false;
        if(pSocket) DEL(pSocket);
    }
    if(eNewState == NET_SYSBUSY) bSystemBusy = true;
}
//////////////////////////////////////////////////////////
void C_GSC::ecsDisconnect()
{
    if(pSocket) { pSocket->Disconnect(); DEL(pSocket); }
}

//////////////////////////////////////////////////////////
CCSocket *C_GSC::pGetSocket()
{
return pSocket;
}

//////////////////////////////////////////////////////////
void C_GSC::ecsSetSocket(CCSocket *pNewSocket) { DEL(pSocket); pSocket = pNewSocket; }

*/
//////////////////////////////////////////////////////////
void C_GSC::VortexInsert(void)
{
	//sid=GetSerialNumber(0);
    //ExecString(va("sys.addclient(%d);",sid));
    //ExecString(va("player[%d].ip_address=\"%s\";",sid,pSocket->pGetAddress()));
}
//////////////////////////////////////////////////////////
void C_GSC::do_script(char *file)
{
    if(!file) return;
    FILE *fp;
    char name[_MAX_PATH];
    sprintf(name,"scripts%c%s",PATH_SEP,file);
    fp=fopen(name,"r");
    if(!fp)
    {
        strcat(name,".gm");
        fp=fopen(name,"r");
        if(!fp)
        {
            sprintf(name,"%s",file);
            fp=fopen(name,"r");
            if(!fp)
            {
                strcat(name,".gm");
                fp=fopen(name,"r");
                if(!fp)
                {
                    //Log("File [%s] not found!",file);
                    return;
                }
            }
        }
    }
    fclose(fp);

    //lua_dostring(luaGVM, va("client=nil;client=get_client_x_name(\"%s\");",l_client_prop(windex,"name")));
    //lua_dofile(luaGVM, name);
}
//////////////////////////////////////////////////////////
void C_GSC::do_func(char *func) { }
//////////////////////////////////////////////////////////
/*
char *C_GSC::get_remote_ip()
{
    //CInetAddress *rip;
    //rip=pSocket->pGetAddress();
    return 0;//strdup(l_client_prop(windex,"remote_ip"));
}
*/
//////////////////////////////////////////////////////////
bool C_GSC::ClearGUI()
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_GUI);
    SendPacket->Write((char)FM_GUI_CLEAR_ALL);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::AddGUIStump(char *name,int x,int y,int x2,int y2,int props,char *media)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_GUI);
    SendPacket->Write((char)FM_GUI_STUMP_CREATE);
    SendPacket->Write((char *) name);
    SendPacket->Write((int)    x);
    SendPacket->Write((int)    y);
    SendPacket->Write((int)    x2);
    SendPacket->Write((int)    y2);
    SendPacket->Write((int)    props);
    SendPacket->Write((char *) media);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1 ;
}
//////////////////////////////////////////////////////////
//bool C_GSC::ModGUIStump(char *name,int x,int y,int x2,int y2,int props,char *media)
//{
/*SendPacket->Reset();
    SendPacket->Write((char)NETMSG_GUI);
    SendPacket->Write((char)FM_GUI_STUMP_UPDATE);
    SendPacket->Write((char *) name);
    SendPacket->Write((int)    x);
    SendPacket->Write((int)    y);
    SendPacket->Write((int)    x2);
    SendPacket->Write((int)    y2);
    SendPacket->Write((int)    props);
    SendPacket->Write((char *) media);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    //return 1;
//}
//////////////////////////////////////////////////////////
//bool C_GSC::DelGUIStump(char *name)
//{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_GUI);
    SendPacket->Write((char)FM_GUI_STUMP_REMOVE);
    SendPacket->Write((char *) name);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
  //  return 1;
//}
//////////////////////////////////////////////////////////
bool C_GSC::GUIStumpCaption(char *name,char *caption)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_GUI);
    SendPacket->Write((char)FM_GUI_STUMP_CAPTION);
    SendPacket->Write((char *) name);
    SendPacket->Write((char *) caption);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::AddGUIControl(char *stump,char *name,int type,int x,int y,int x2,int y2,int props,char *media,char *value)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_GUI);
    SendPacket->Write((char)FM_GUI_CONTROL_CREATE);
    SendPacket->Write((char *) stump);
    SendPacket->Write((char *) name);
    SendPacket->Write((int)    type);
    SendPacket->Write((int)    x);
    SendPacket->Write((int)    y);
    SendPacket->Write((int)    x2);
    SendPacket->Write((int)    y2);
    SendPacket->Write((int)    props);
    SendPacket->Write((char *) media);
    SendPacket->Write((char *) value);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::DelGUIControl(char *stump,char *name)
{
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::GUIControlValue(char *stump,char *name,char *value)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_GUI);
    SendPacket->Write((char)FM_GUI_CONTROL_VALUE);
    SendPacket->Write((char *) stump);
    SendPacket->Write((char *) name);
    SendPacket->Write((char *) value);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetLimboState(int lm)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_FVM);
    //SendPacket->Write((char)FVM_SET_LIMBO_STATE);
    SendPacket->Write((int)lm);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetDrawMap(int dm)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_FVM);
    //SendPacket->Write((char)FVM_SET_MAP_DRAW);
    SendPacket->Write((char)dm);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetGameMode(int gm)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_CHANGE_MODE);
    SendPacket->Write((char)gm);
    SendPacket->Write((char)1);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetDayLight(float r,float g,float b)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_FVM);
    //SendPacket->Write((char)FVM_SET_DAYLIGHT);
    SendPacket->Write((float)r);
    SendPacket->Write((float)g);
    SendPacket->Write((float)b);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetCam(int x,int y,int z,int ax,int ay,int az,int ox,int oy,int oz)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_FVM);
    //SendPacket->Write((char)FVM_MOVECAM);
    SendPacket->Write((int)x);
    SendPacket->Write((int)y);
    SendPacket->Write((int)z);
    SendPacket->Write((int)ax);
    SendPacket->Write((int)ay);
    SendPacket->Write((int)az);
    SendPacket->Write((int)ox);
    SendPacket->Write((int)oy);
    SendPacket->Write((int)oz);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::MoveCamTo   (int x,int y,int z)
{
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::ZoomCamTo   (int zoom)
{
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::ScrollCamTo (int x,int y,int z)
{
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::VisRemove(int ntt)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_VIS_REMOVE);
    SendPacket->Write((int)ntt);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////

bool C_GSC::VisMove(int ntt,int x,int y,int z,char* media,int media_type,int head,int dir)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_VIS_UPDATE);
    SendPacket->Write((int)ntt);
    SendPacket->Write((int)x);
    SendPacket->Write((int)y);
    SendPacket->Write((int)z);
    SendPacket->Write((char *)media);
    SendPacket->Write((int)media_type);
    SendPacket->Write((int)head);
    SendPacket->Write((int)dir);
    this->pSocket->SendUnreliableMessage(SendPacket);
    */
    return 1;
}
//////////////////////////////////////////////////////////
void C_GSC::LoginReply(int iLoginType,char * szAnswerMessage)
{
/*
    pLog->_Add("LoginReply(%d,%s)",iLoginType,szAnswerMessage);
	SendPacket->Reset();
	SendPacket->Write((char) NETMSG_LOGIN_REQUEST_REPLY);
	SendPacket->Write((char)   iLoginType);
	SendPacket->Write((char *) szAnswerMessage);
	SendPacket->Write((char *) VERSION);
	SendPacket->Write((int)    sid);
	SendPacket->Write((char *) "sys.media");
	SendPacket->Write((char *) "sys.name");
	SendPacket->Write((char *) "sys.owner");
	SendPacket->Write((int)    255);
	SendPacket->Write((char *) "access what");
	SendPacket->Write((char)   10); // character slots
	pSocket->SendUnreliableMessage(SendPacket);
	*/
}
//////////////////////////////////////////////////////////
bool C_GSC::SetMapTile   (int x,int y,int z,int bank,int tile)
{
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetMapObject (int x,int y,int z,int l,int bank,int obj)
{
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetMapProp   (int x,int y,int z,int prop)
{
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetMapVWidth (int x,int y,int z,int v,int w)
{
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetMapVHeight(int x,int y,int z,int v,int h)
{
    return 1;
}
//////////////////////////////////////////////////////////
bool C_GSC::SetMapVColor (int x,int y,int z,int v,char r,char g,char b)
{
    return 1;
}

//////////////////////////////////////////////////////////
