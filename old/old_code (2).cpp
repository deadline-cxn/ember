///////////////////////////////////////////
/*
char *C_GSC::get_remote_ip()
{
    //CInetAddress *rip;
    //rip=pSocket->pGetAddress();
    return 0;//strdup(l_client_prop(windex,"remote_ip"));
}
*/
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
///////////////////////////////////////////
eConState C_GSC::eGetState()
{
    if(!this)       return NET_NOTCONNECT;
    if(!pSocket)    return NET_NOTCONNECT;
    if(!bLoggedin)  return NET_LOGGINPROC;
    if(bSystemBusy) return NET_SYSBUSY;
                    return NET_CONNECTED;
}
///////////////////////////////////////////
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
///////////////////////////////////////////
void C_GSC::ecsDisconnect()
{
    if(pSocket) { pSocket->Disconnect(); DEL(pSocket); }
}

///////////////////////////////////////////
CCSocket *C_GSC::pGetSocket()
{
return pSocket;
}

///////////////////////////////////////////
void C_GSC::ecsSetSocket(CCSocket *pNewSocket) { DEL(pSocket); pSocket = pNewSocket; }

*/
///////////////////////////////////////////
bool C_GSC::AddGUIStump(char *name,int x,int y,int x2,int y2,int props,char *media){
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
///////////////////////////////////////////
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
///////////////////////////////////////////
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
///////////////////////////////////////////
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
///////////////////////////////////////////
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
///////////////////////////////////////////
bool C_GSC::DelGUIControl(char *stump,char *name)
{
    return 1;
}
///////////////////////////////////////////
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

///////////////////////////////////////////
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


///////////////////////////////////////////
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
///////////////////////////////////////////
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

///////////////////////////////////////////
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
///////////////////////////////////////////

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
///////////////////////////////////////////
bool C_GSC::SetMapTile   (int x,int y,int z,int bank,int tile)
{
    return 1;
}
///////////////////////////////////////////
bool C_GSC::SetMapObject (int x,int y,int z,int l,int bank,int obj)
{
    return 1;
}
///////////////////////////////////////////
bool C_GSC::SetMapProp   (int x,int y,int z,int prop)
{
    return 1;
}
///////////////////////////////////////////
bool C_GSC::SetMapVWidth (int x,int y,int z,int v,int w)
{
    return 1;
}
///////////////////////////////////////////
bool C_GSC::SetMapVHeight(int x,int y,int z,int v,int h)
{
    return 1;
}
///////////////////////////////////////////
bool C_GSC::SetMapVColor (int x,int y,int z,int v,char r,char g,char b)
{
    return 1;
}

    bool        AddGUIStump     (char *name,int x,int y,int x2,int y2,int props,char *media);
    bool        DelGUIStump     (char *name,int x,int y,int x2,int y2,int props,char *media);
    bool        ModGUIStump     (char *name);
    bool        GUIStumpCaption (char *name,char *caption);
    bool        AddGUIControl   (char *stump,char *name,int type,int props,int x,int y,int x2,int y2,char *media,char *value);
    bool        DelGUIControl   (char *stump,char *name);
    int         ModGUIControl   (char *stump,char *name);
    bool        GUIControlValue (char *stump,char *name,char *value);




            case NETMSG_MODIFY_MAP: fold_block {

                inactivity_timer->Reset();
                /* if(pMap==NULL) break;

                cx=Recv->cRead();      // Type of modification
                cy=Recv->cRead();      // boolean

                ax=client->avatar->x;//Recv->iRead(); // x
                ay=client->avatar->y;//Recv->iRead(); // y
                az=client->avatar->z;//Recv->iRead(); // z

                pMap->LoadSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));

                Send.Reset();
                Send.Write((char)NETMSG_MODIFY_MAP);

                if(1) //!DoesHaveAccess(client->cGetAccess(),"map_build"))
                {
                    AdminToPlayer(client,"You don't have access to build the map.",255,0,0);
                    break;
                }

                switch(cx)
                {
                    case GMP_PROPERTY_LIQUID:

                        pMap->SetLiquid(CamCoord(ax),CamCoord(ay),(cy?true:false));  // keep microsoft happy
                        Send.Write((char)GMP_PROPERTY_LIQUID);
                        Send.Write((char)cy);

                        break;

                    case GMP_PROPERTY_BLOCKED:

                        pMap->SetBlocked(CamCoord(ax),CamCoord(ay),(cy?true:false));  // keep microsoft happy
                        Send.Write((char)GMP_PROPERTY_BLOCKED);
                        Send.Write((char)cy);

                        break;

                    case GMP_PROPERTY_INDOORS:

                        break;

                    case GMP_PROPERTY_TELEPORT:


                        Send.Write((char)GMP_PROPERTY_TELEPORT);
                        Send.Write((char)cy);

                        pTeleport=pFirstTeleport;
                        while(pTeleport)
                        {
                            if( (pTeleport->iSourceX  == ax) &&
                                (pTeleport->iSourceY  == ay) &&
                                (pTeleport->iSourceZ  == az) )
                                break;
                            pTeleport=pTeleport->pNext;
                        }

                        if(pTeleport)
                        {
                            if(cy)
                            {
                                strcpy(pTeleport->szName,Recv->pRead());
                                strcpy(pTeleport->szDestName,Recv->pRead());
                                pTeleport->iDestinationX=Recv->iRead();
                                pTeleport->iDestinationY=Recv->iRead();
                                pTeleport->iDestinationZ=Recv->iRead();
                                sprintf(szTemp,"teleports%c%d.cfg",PATH_SEP,pTeleport->iSourceX,pTeleport->iSourceY,pTeleport->iSourceZ);
                                pTeleport->bSave(szTemp);
                            }
                            else
                            {
                                sprintf(szTemp,"teleports%c%d-%d-%d.ini",PATH_SEP,pTeleport->iSourceX,pTeleport->iSourceY,pTeleport->iSourceZ);
                                pTeleport->bClear();
                                remove(szTemp);
                            }

                            pMap->SetTeleport(ax,ay,(cy?true:false));
                            break;
                        }

                        if(!cy)
                        {
                            pMap->SetTeleport(ax,ay,(cy?true:false));
                            break;
                        }

                        i=0;

                        pTeleport=pFirstTeleport;
                        while(pTeleport)
                        {
                            if(pTeleport->iKey==NOT_A_TELEPORT)
                                break;
                            i++;
                            pTeleport=pTeleport->pNext;
                        }

                        if(i>MAX_TELEPORTS)
                            break;

                        if(pTeleport)
                        {
                            pTeleport->iKey=i;

                            strcpy(pTeleport->szName,Recv->pRead());

                            pTeleport->iSourceX=ax;
                            pTeleport->iSourceY=ay;
                            pTeleport->iSourceZ=az;

                            strcpy(pTeleport->szDestName,Recv->pRead());

                            pTeleport->iDestinationX=Recv->iRead();
                            pTeleport->iDestinationY=Recv->iRead();
                            pTeleport->iDestinationZ=Recv->iRead();

                            sprintf(szTemp,"teleports%c%d-%d-%d.ini",PATH_SEP,pTeleport->iSourceX,pTeleport->iSourceY,pTeleport->iSourceZ);
                            pTeleport->bSave(szTemp);

                            pMap->SetTeleport(ax,ay,(cy?true:false));
                            pMap->SaveSector3D("map",bx,by,bz);

                            if(pTeleport->pNext)
                                break;

                            pTeleport->pNext = new CTeleport;
                        }
                        ////*
                        break;

                    default:
                        return;
                        break;
                }

                Send.Write((int)ax);
                Send.Write((int)ay);
                Send.Write((int)az);

                // Send to all players

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    other_client=other_client->pNext;
                }
                pMap->SaveSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(bz));
                */

                break;}

            case NETMSG_SET_TILE: fold_block {
                inactivity_timer->Reset();
                if(1)//!DoesHaveAccess(client->cGetAccess(),"map_build"))
                {
//                        AdminToPlayer(client,"You don't have access to build the map.",255,0,0);
                    // Log("%s(%s) tried to set map tile.",client->avatar->name,client->avatar->name);
                    break;
                }

                ax = Recv->iRead(); // x
                ay = Recv->iRead(); // y
                az = Recv->iRead(); // z
                i  = Recv->cRead(); // bank
                j  = Recv->cRead(); // tile

                // Log("%s(%s) set tile:%d %d %d %d %d %d [%d][%d]",client->avatar->name,client->avatar->name,ax,ay,az,bx,by,bz,i,j);

//                    pMap->LoadSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));
//                    pMap->SetTile(CamCoord(ax),CamCoord(ay),i,j);
//                    pMap->SaveSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));


/*
                Send.Reset();
                Send.Write((char)NETMSG_SET_TILE);
                Send.Write((int)ax);
                Send.Write((int)ay);
                Send.Write((int)az);
                Send.Write((char)i);
                Send.Write((char)j);

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    other_client=other_client->pNext;
                }
*/

                break;}
            case NETMSG_SET_OBJECT: fold_block {
                inactivity_timer->Reset();
                if(1) //!DoesHaveAccess(client->cGetAccess(),"map_build"))
                {
//                        AdminToPlayer(client,"You don't have access to build the map.",255,0,0);
                    break;
                }

                ax = Recv->iRead();  // x
                ay = Recv->iRead();  // y
                az = Recv->iRead();  // z
                i  = Recv->cRead(); // bank
                j  = Recv->cRead(); // object
                k  = Recv->cRead(); // layer

//                    pMap->LoadSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));
//                    pMap->SetObj(ax,ay,i,j,k);
//                    pMap->SaveSector3D("map",MapCoord(ax),MapCoord(ay),MapCoord(az));

/*
                Send.Reset();
                Send.Write((char)NETMSG_SET_OBJECT);
                Send.Write((int)ax);
                Send.Write((int)ay);
                Send.Write((int)az);
                Send.Write((char)i);
                Send.Write((char)j);
                Send.Write((char)k);

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    other_client=other_client->pNext;
                }
*/

                break; }
            case NETMSG_SET_VERTEX: fold_block {

                inactivity_timer->Reset();

                if(1) //!DoesHaveAccess(client->cGetAccess(),"map_build"))
                {
//                        AdminToPlayer(client,"You don't have access to build the map.",255,0,0);
                    break;
                }
/*
                if(!pMapBuffer)
                {
                    // Log("Attempt to set vertex on map that doesn't exist.");
                    break;
                }

                ax = Recv->iRead();  // x
                ay = Recv->iRead();  // y
                az = Recv->iRead();  // z

                cx = Recv->cRead(); // what type of change
                cy = Recv->cRead(); // which vertex
                vx = Recv->cRead(); // red element
                vy = Recv->cRead(); // green element
                vz = Recv->cRead(); // blue element

                LoadMap(ax,ay,az);

                switch(cx)
                {
                    case 0: // height
                        pMapBuffer->SetVertexHeight(CamCoord(ax),CamCoord(ay),cy,vx);
                        break;

                    case 1: // color/lighting
                        pMapBuffer->SetVertexColor(CamCoord(ax),CamCoord(ay),cy,vx,vy,vz);
                        break;

                    case 2: // width
                        pMapBuffer->SetVertexWidth(CamCoord(ax),CamCoord(ay),cy,vx);
                        break;

                    case 3: // relight the entire map sector with this color
                        pMapBuffer->ClearVertexColors(vx,vy,vz);
                        break;

                    default:
                        break;
                }

                SaveMap(ax,ay,az);

                Send.Reset();
                Send.Write((char)NETMSG_SET_VERTEX);
                Send.Write((int)ax);
                Send.Write((int)ay);
                Send.Write((int)az);
                Send.Write((char)cx); // what type of change
                Send.Write((char)cy); // which vertex
                Send.Write((char)vx);
                Send.Write((char)vy);
                Send.Write((char)vz);

                other_client=fmgs->pFirstPlayer;
                while(other_client)
                {
                    other_client->pSocket->SendUnreliableMessage((CPacket *)&Send);
                    other_client=other_client->pNext;
                }
*/

                break; }



                // given a starting location A
                // and   an ending  location B
                // the server will generate a path ((x,y) waypoints)
                // and return it to the client
                // if there is no path to the destination,
                // the function will return 0
                // location A and location B must be visible on the client's screen

                /*
                // Request for movement
                // Store current client location here.

                inactivity_timer->Reset();

                ax = client->avatar->x;
                ay = client->avatar->y;
                az = client->avatar->z;

                // Read requested move from data packet.

                dx = Recv->cRead();

                switch(dx)
                {
                    case FM_NORTH:
                        client->avatar->y=client->avatar->y-1;
                        client->avatar->direction=FM_NORTH;
                        break;

                    case FM_SOUTH:
                        client->avatar->y=client->avatar->y+1;
                        client->avatar->direction=FM_SOUTH;
                        break;

                    case FM_EAST:
                        client->avatar->x=client->avatar->x+1;
                        client->avatar->direction=FM_EAST;
                        break;

                    case FM_WEST:
                        client->avatar->x=client->avatar->x-1;
                        client->avatar->direction=FM_WEST;
                        break;

                    case FM_NORTHWEST:
                        client->avatar->y=client->avatar->y-1;
                        client->avatar->x=client->avatar->x-1;
                        client->avatar->direction=FM_NORTHWEST;
                        break;

                    case FM_SOUTHWEST:
                        client->avatar->x=client->avatar->x-1;
                        client->avatar->y=client->avatar->y+1;
                        client->avatar->direction=FM_SOUTHWEST;
                        break;

                    case FM_NORTHEAST:
                        client->avatar->x=client->avatar->x+1;
                        client->avatar->y=client->avatar->y-1;
                        client->avatar->direction=FM_NORTHEAST;
                        break;

                    case FM_SOUTHEAST:
                        client->avatar->x=client->avatar->x+1;
                        client->avatar->y=client->avatar->y+1;
                        client->avatar->direction=FM_SOUTHEAST;
                        break;

                    default:
                        Log("%s(%s) Move Error! %d",client->avatar->name,client->avatar->name,dx);
                        break;
                }

                // Get map sector for checking stuff

                dy = pMap->LoadSector3D("map",  MapCoord(client->avatar->x),
                                                MapCoord(client->avatar->y),
                                                MapCoord(client->avatar->z));

                dx=0; // teleport disable for now

                // Check if tile is blocked
                if( (client->game_state==PLAY) &&
                    (pMap->bIsBlocked(CamCoord(client->avatar->x),CamCoord(client->avatar->y))) )
                {
                        // restore previous client location
                        client->avatar->x=ax;
                        client->avatar->y=ay;
                        client->avatar->z=az;
                        break;
                }
                else
                {
                    MovePlayer(client,client->avatar->x,client->avatar->y,client->avatar->z,dx);
                }

                /*
                dx = 0; // no teleports allowed in this server!
                if(root->pSystem->iGetProperty("teleports"))
                {
                    dx = pMap->bIsTeleport( client->avatar->x,
                                            client->avatar->y); // Check if on a teleport location here

                    if(dx) // reference teleport list to get the destination teleport location
                    {
                        pTeleport=pFirstTeleport;
                        while(pTeleport)
                        {
                            if( (pTeleport->iSourceX) == (client->pObjectRef->iGetProperty("x")) &&
                                (pTeleport->iSourceY) == (client->pObjectRef->iGetProperty("y")) &&
                                (pTeleport->iSourceZ) == (client->pObjectRef->iGetProperty("z")) )
                                break;

                            pTeleport=pTeleport->pNext;
                        }

                        if(pTeleport)
                        {
                            client->pObjectRef->SetProperty("x",pTeleport->iDestinationX);
                            client->pObjectRef->SetProperty("y",pTeleport->iDestinationY);
                            client->pObjectRef->SetProperty("z",pTeleport->iDestinationZ);
                        }
                    }
                }*/




///////////////////////////////////////////
bool C_GSC::SetDayLight(float r,float g,float b)
{
/*
    SendPacket->Reset();
    SendPacket->Write((char)NETMSG_FVM);
    //SendPacket->Write((char)FVM_SET_DAYLIGHT);
    SendPacket->Write((float)r);
    SendPacket->Write((float)g);
    SendPacket->Write((float)b);
    SendUnreliableMessage(SendPacket);
    */
    return 1;
}
///////////////////////////////////////////
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
    SendUnreliableMessage(SendPacket);
    */
    return 1;
}
///////////////////////////////////////////
bool C_GSC::MoveCamTo   (int x,int y,int z)
{
    return 1;
}
///////////////////////////////////////////
bool C_GSC::ZoomCamTo   (int zoom)
{
    return 1;
}
///////////////////////////////////////////
bool C_GSC::ScrollCamTo (int x,int y,int z)
{
    return 1;
}
