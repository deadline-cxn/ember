/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "fm_util.h"
#include "dlstorm.h"
#include "c_log.h"

#ifdef _EMBER_CLIENT_
#include "c_console.h"
#include "c_util.h"
#include "c_data.h"
#include "b_cam.h"
#include "c_map.h"
#include "c_maputil.h"
#include "b_gui.h"
#endif

#ifdef _EMBER_SERVER_
#include "ember_game_server.h"
#include "b_map.h"
#endif

CLog *pLog;
bool bLog;
C_FM_GUI_CONTROL_DATA *pFirstGUIData;
C_FM_GUI_CONTROL_DATA *pGUIData;

/**************************************
 ** Clears the GUI completely        **
 **************************************/

void ClearGUIs(void)
{
#ifdef _EMBER_CLIENT_
	focus_stump=0;
	focus_control=0;
	DEL(pGUI);
	pGUI=new C_FM_GUI();
	pGUI->cab_loading=true;
#endif
}


void ClearGUIData(void)
{
#ifdef _EMBER_CLIENT_
	//Log("ClearGUIData(); 1");

	C_FM_GUI_CONTROL_DATA *pDelGUIData=0;

	pGUIData=pFirstGUIData;

	while(pGUIData)
	{
		pDelGUIData=pGUIData;
		pGUIData=pGUIData->next;
		DEL(pDelGUIData);
	}

	DEL(pDelGUIData);
	DEL(pFirstGUIData);

	pFirstGUIData=0;

	//Log("ClearGUIData(); 2");
#endif
}

/*****************************************
 ** Extract the GUI form data into list **
 *****************************************/

void ParseGUIVars(char *text)
{
#ifdef _EMBER_CLIENT_

//	Log("ParseGUIVars(); 1");

	ClearGUIData();
	

	char *szInput;
	char temp[1024]; memset(temp,0,1024);

	szInput=strtok(text,"=");

//	Log("ParseGUIVars(); 3");

	while(szInput)
	{
		strcpy(temp,szInput);
		Log(" ParseGUIVars()-N-[%s]",temp);
		szInput=strtok(NULL,"|\n\r");
		if(szInput)
		{
			Log(" ParseGUIVars()-V-[%s]",szInput);

			pGUIData=pFirstGUIData;

			if(!pGUIData)
			{
				pGUIData=new C_FM_GUI_CONTROL_DATA();
				pFirstGUIData=pGUIData;
			}
			else
			{
				while(pGUIData->next)
				{
					pGUIData=pGUIData->next;
				}
				pGUIData->next=new C_FM_GUI_CONTROL_DATA();
				pGUIData=pGUIData->next;
			}
			strcpy(pGUIData->name,temp);
			strcpy(pGUIData->value,szInput);
		}
		else
		{
			Log(" ParseGUIVars()-V-[(null)]");
		}
		szInput=strtok(NULL,"=\n\r");
	}
#endif
	
}

/**************************************
 ** Log text to console and log file **
 **************************************/
void Log(char *fmt, ...)
{
    if(!pLog) return;
    char ach[16384];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
	if(ach[strlen(ach)-1]!='\n')
    strcat(ach,"\n");
    pLog->AddEntry(ach);
////////////////////////
#ifdef _EMBER_SERVER_

    if(!bQuiet) printf(ach);
#endif
////////////////////////
#ifdef _EMBER_CLIENT_
    ConsoleBufferAdd(ach);
#endif
////////////////////////
#ifdef _EMBER_MASTER_
    if(!bQuiet) printf(ach);
#endif
////////////////////////
}
/**************************************
 ** Log one char to the logfile      **
 **************************************/
void LogC(char *fmt, ...)
{
    if(!pLog) return;
    char ach[16384];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
    printf(ach);
    pLog->AddEntryNoTime(ach);
}
/**************************************
 ** Log text to console and log file **
 **************************************/
void DLog(char *fmt, ...)
{
    if(!pLog) return;
#ifdef _EMBER_CLIENT_
    if(pClientData) if(pClientData->cDebug!=2) return;
#endif
    char ach[16384];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
    strcat(ach,"\n");
#ifdef _EMBER_CLIENT_
    if(pClientData)
        if(pClientData->cDebug>0)
            ConsoleBufferAdd(ach);
    if(pClientData)
    {
        if(pClientData->cDebug>1)
            pLog->AddEntry(ach);
    }
    else
    {
        pLog->AddEntry("(No ClientData):%s",ach);
    }

#endif
#ifdef _EMBER_MASTER_
    if(!bQuiet) printf(ach);
    pLog->AddEntry(ach);
#endif
#ifdef _EMBER_SERVER_
    printf(ach);
    pLog->AddEntry(ach);
#endif
}



////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _EMBER_SERVER_
/////////////////////////////////////////////////////////
void RemoveAccount(char *szName)
{
    char szTemp[1024];
    FILE *fp1;
    FILE *fp2;
    char In[1024];
    char *Tok;
    char Save[1024];
    int i;

    sprintf(szTemp,"users%c%s",PATH_SEP,szName);
    strcpy(Save,szTemp);
    sprintf(Save,"%s%c%s.fcd",szTemp,PATH_SEP,szName);
    remove(Save);

    for(i=0;i<9;i++)
    {
        sprintf(Save,"%s%c%s.cs%d",szTemp,PATH_SEP,szName,i);
        remove(Save);
        sprintf(Save,"%s%c%s.it%d.can",szTemp,PATH_SEP,szName,i);
        remove(Save);
    }
    rmdir(szTemp);

    sprintf(szTemp,"users%cpassword.dat",PATH_SEP);
    fp1=fopen(szTemp,"rt");
    sprintf(szTemp,"users%cpassword.tmp",PATH_SEP);
    fp2=fopen(szTemp,"wt");
    if(fp1)
    {
        while(fgets(In,255,fp1))
        {
            strcpy(Save,In);
            Tok=strtok(In,"(,)");
            if(!strcmp(Tok,"Data"))
            {
                Tok=strtok(NULL,"(,)");
                if(!strcmp(Tok,szName))
                {
                    continue;
                }
            }
            fputs(Save,fp2);
        }
        fclose(fp1);
        fclose(fp2);
    }
    sprintf(szTemp,"users%cpassword.dat",PATH_SEP);
    remove(szTemp);
    sprintf(In,"users%cpassword.tmp",PATH_SEP);
    rename(In,szTemp);

}
/******************************************************************
 ** Increment the global serial number counter and return it     **
 ******************************************************************/
u_long GetSerialNumber(bool bSave)
{
    static u_long iSerialNumber=0;
    char szSerialNumber[256];
    FILE *fp;

    if(iSerialNumber==0)  // only to save the serial number the for the first time
    {
        fp=fopen("serial.dat","rt");
        if(!fp)
        {
            // not found, create a new item.id file and put a 1
            fp=fopen("serial.dat","wt");
            if(!fp)
            {
                Log("Can't create serial.dat! #1");
                return 0;
            }
            iSerialNumber=1;
            Log("Serial number set to %d",iSerialNumber);
            sprintf(szSerialNumber,"%d",iSerialNumber);
            fputs(szSerialNumber,fp);
            fputs("=================================================\n",fp);
            fputs("Serial number tracking file. DO NOT erase or modify.\n",fp);

            fclose(fp);
            return iSerialNumber;
        }
        fgets(szSerialNumber, 100, fp);
        fclose(fp);
        iSerialNumber=atoi(szSerialNumber);
    }

    if(bSave) // this is called at the end of the program to save the state of iserialnumber
    {
        fp=fopen("serial.dat","wt");
        if(!fp)
        {
            Log("Can't create serial.dat! #2");
            return 0;
        }
        sprintf(szSerialNumber,"%d\n",iSerialNumber);
        fputs(szSerialNumber,fp);
        fputs("=================================================\n",fp);
        fputs("Serial number tracking file. DO NOT erase or modify.\n",fp);
        fclose(fp);
    }

    iSerialNumber++;
    // Log("Serial number incremented to %d",iSerialNumber);
    return iSerialNumber;
}
#endif

#ifndef _EMBER_MASTER_
#ifndef _EMBER_MS_WEB_
#ifndef _EMBER_UE_

#ifdef _EMBER_CLIENT_
/******************************************************************
 ** Convert real coordinate to on screen coordinate              **
 ******************************************************************/
int OnScreenXCoord(int coordinate) 
{ 
    // camera is positioned at 45, 45
    // return int based on cam position
    return camera->x;

}
/******************************************************************
 ** Convert real coordinate to on screen coordinate              **
 ******************************************************************/
int OnScreenYCoord(int coordinate) 
{
    // camera is positioned at 45, 45
    // return int based on cam position
    return camera->y;
}
#endif

/**************************************************
 ** Fill the FragMaps with pMapBuffer data       **
 **************************************************/

void FillFragMaps(void)
{
    int x;
    int y;

    int bank;
    int tile;
    int object;
    int priority;

    // Clear off the item data
    
    for(x=0;x<GMP_MAPSIZE;x++)
    {
        for(y=0;y<GMP_MAPSIZE;y++)
        {
            // Copy Tiles

            pFragMap=pFirstFragMap;

            bank = pMapBuffer->GetTile(x,y)&0XFF00;
            bank = bank/256;
            tile = pMapBuffer->GetTile(x,y)&0X00FF;
            pFragMap->SetTile(x,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank = pMapBuffer->GetTile(x+GMP_MAPSIZE,y)&0XFF00;
            bank = bank/256;
            tile = pMapBuffer->GetTile(x+GMP_MAPSIZE,y)&0X00FF;
            pFragMap->SetTile(x,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank = pMapBuffer->GetTile(x+GMP_MAPSIZE*2,y)&0XFF00;
            bank = bank/256;
            tile = pMapBuffer->GetTile(x+GMP_MAPSIZE*2,y)&0X00FF;
            pFragMap->SetTile(x,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank = pMapBuffer->GetTile(x,y+GMP_MAPSIZE)&0XFF00;
            bank = bank/256;
            tile = pMapBuffer->GetTile(x,y+GMP_MAPSIZE)&0X00FF;
            pFragMap->SetTile(x,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank = pMapBuffer->GetTile(x+GMP_MAPSIZE,y+GMP_MAPSIZE)&0XFF00;
            bank = bank/256;
            tile = pMapBuffer->GetTile(x+GMP_MAPSIZE,y+GMP_MAPSIZE)&0X00FF;
            pFragMap->SetTile(x,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank = pMapBuffer->GetTile(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE)&0XFF00;
            bank = bank/256;
            tile = pMapBuffer->GetTile(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE)&0X00FF;
            pFragMap->SetTile(x,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank = pMapBuffer->GetTile(x,y+GMP_MAPSIZE*2)&0XFF00;
            bank = bank/256;
            tile = pMapBuffer->GetTile(x,y+GMP_MAPSIZE*2)&0X00FF;
            pFragMap->SetTile(x,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank = pMapBuffer->GetTile(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2)&0XFF00;
            bank = bank/256;
            tile = pMapBuffer->GetTile(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2)&0X00FF;
            pFragMap->SetTile(x,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank = pMapBuffer->GetTile(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2)&0XFF00;
            bank = bank/256;
            tile = pMapBuffer->GetTile(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2)&0X00FF;
            pFragMap->SetTile(x,y,bank,tile);            

            // Copy Objects

            for(priority=0;priority<GMP_OBJECT_LAYERS;priority++)
            {
                pFragMap = pFirstFragMap;

                bank   = pMapBuffer->GetObj(x,y,priority)&0xFF00;
                bank   = bank/256;
                object = pMapBuffer->GetObj(x,y,priority)&0x00FF;
                pFragMap->SetObj(x,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank   = pMapBuffer->GetObj(x+GMP_MAPSIZE,y,priority)&0xFF00;
                bank   = bank/256;
                object = pMapBuffer->GetObj(x+GMP_MAPSIZE,y,priority)&0x00FF;
                pFragMap->SetObj(x,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank   = pMapBuffer->GetObj(x+GMP_MAPSIZE*2,y,priority)&0xFF00;
                bank   = bank/256;
                object = pMapBuffer->GetObj(x+GMP_MAPSIZE*2,y,priority)&0x00FF;
                pFragMap->SetObj(x,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank   = pMapBuffer->GetObj(x,y+GMP_MAPSIZE,priority)&0xFF00;
                bank   = bank/256;
                object = pMapBuffer->GetObj(x,y+GMP_MAPSIZE,priority)&0x00FF;
                pFragMap->SetObj(x,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank   = pMapBuffer->GetObj(x+GMP_MAPSIZE,y+GMP_MAPSIZE,priority)&0xFF00;
                bank   = bank/256;
                object = pMapBuffer->GetObj(x+GMP_MAPSIZE,y+GMP_MAPSIZE,priority)&0x00FF;
                pFragMap->SetObj(x,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank   = pMapBuffer->GetObj(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,priority)&0xFF00;
                bank   = bank/256;
                object = pMapBuffer->GetObj(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,priority)&0x00FF;
                pFragMap->SetObj(x,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank   = pMapBuffer->GetObj(x,y+GMP_MAPSIZE*2,priority)&0xFF00;
                bank   = bank/256;
                object = pMapBuffer->GetObj(x,y+GMP_MAPSIZE*2,priority)&0x00FF;
                pFragMap->SetObj(x,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank   = pMapBuffer->GetObj(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,priority)&0xFF00;
                bank   = bank/256;
                object = pMapBuffer->GetObj(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,priority)&0x00FF;
                pFragMap->SetObj(x,y,bank,object,priority);

                pFragMap=pFragMap->pNext;
        
                bank   = pMapBuffer->GetObj(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,priority)&0xFF00;
                bank   = bank/256;
                object = pMapBuffer->GetObj(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,priority)&0x00FF;
                pFragMap->SetObj(x,y,bank,object,priority);
            }                       

            // Copy Properties  29 Jul 00            

            pFragMap=pFirstFragMap;            
            pFragMap->SetProperties(x,y,pMapBuffer->cGetProperties(x,y));

            pFragMap=pFragMap->pNext;
            pFragMap->SetProperties(x,y,pMapBuffer->cGetProperties(x+GMP_MAPSIZE,y));            

            pFragMap=pFragMap->pNext;
            pFragMap->SetProperties(x,y,pMapBuffer->cGetProperties(x+GMP_MAPSIZE*2,y));

            pFragMap=pFragMap->pNext;
            pFragMap->SetProperties(x,y,pMapBuffer->cGetProperties(x,y+GMP_MAPSIZE));

            pFragMap=pFragMap->pNext;
            pFragMap->SetProperties(x,y,pMapBuffer->cGetProperties(x+GMP_MAPSIZE,y+GMP_MAPSIZE));            

            pFragMap=pFragMap->pNext;
            pFragMap->SetProperties(x,y,pMapBuffer->cGetProperties(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE));

            pFragMap=pFragMap->pNext;
            pFragMap->SetProperties(x,y,pMapBuffer->cGetProperties(x,y+GMP_MAPSIZE*2));

            pFragMap=pFragMap->pNext;
            pFragMap->SetProperties(x,y,pMapBuffer->cGetProperties(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2));

            pFragMap=pFragMap->pNext;
            pFragMap->SetProperties(x,y,pMapBuffer->cGetProperties(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2));

            // Copy Vertex Info (lighting(color) and height) 2 Sep 00

            for(int vertex=0;vertex<4;vertex++)
            {

                pFragMap=pFirstFragMap;
                pFragMap->SetSingleVertexHeight(x,y,vertex,pMapBuffer->GetVertexHeight(x,y,vertex));                
                pFragMap->SetSingleVertexWidth(x,y,vertex,pMapBuffer->GetVertexWidth(x,y,vertex));
                pFragMap->SetSingleVertexColor(x,y,vertex,
                                                pMapBuffer->GetVertexColorR(x,y,vertex),
                                                pMapBuffer->GetVertexColorG(x,y,vertex),
                                                pMapBuffer->GetVertexColorB(x,y,vertex));

                pFragMap=pFragMap->pNext;
                pFragMap->SetSingleVertexHeight(x,y,vertex,pMapBuffer->GetVertexHeight(x+GMP_MAPSIZE,y,vertex));
                pFragMap->SetSingleVertexWidth(x,y,vertex,pMapBuffer->GetVertexWidth(x+GMP_MAPSIZE,y,vertex));
                pFragMap->SetSingleVertexColor(x,y,vertex,
                                                pMapBuffer->GetVertexColorR(x+GMP_MAPSIZE,y,vertex),
                                                pMapBuffer->GetVertexColorG(x+GMP_MAPSIZE,y,vertex),
                                                pMapBuffer->GetVertexColorB(x+GMP_MAPSIZE,y,vertex));
            
                pFragMap=pFragMap->pNext;
                pFragMap->SetSingleVertexHeight(x,y,vertex,pMapBuffer->GetVertexHeight(x+GMP_MAPSIZE*2,y,vertex));
                pFragMap->SetSingleVertexWidth(x,y,vertex,pMapBuffer->GetVertexWidth(x+GMP_MAPSIZE*2,y,vertex));
                pFragMap->SetSingleVertexColor(x,y,vertex,
                                                pMapBuffer->GetVertexColorR(x+GMP_MAPSIZE*2,y,vertex),
                                                pMapBuffer->GetVertexColorG(x+GMP_MAPSIZE*2,y,vertex),
                                                pMapBuffer->GetVertexColorB(x+GMP_MAPSIZE*2,y,vertex));
                
                pFragMap=pFragMap->pNext;
                pFragMap->SetSingleVertexHeight(x,y,vertex,pMapBuffer->GetVertexHeight(x,y+GMP_MAPSIZE,vertex));
                pFragMap->SetSingleVertexWidth(x,y,vertex,pMapBuffer->GetVertexWidth(x,y+GMP_MAPSIZE,vertex));
                pFragMap->SetSingleVertexColor(x,y,vertex,
                                                pMapBuffer->GetVertexColorR(x,y+GMP_MAPSIZE,vertex),
                                                pMapBuffer->GetVertexColorG(x,y+GMP_MAPSIZE,vertex),
                                                pMapBuffer->GetVertexColorB(x,y+GMP_MAPSIZE,vertex));
            
                pFragMap=pFragMap->pNext;
                pFragMap->SetSingleVertexHeight(x,y,vertex,pMapBuffer->GetVertexHeight(x+GMP_MAPSIZE,y+GMP_MAPSIZE,vertex));
                pFragMap->SetSingleVertexWidth(x,y,vertex,pMapBuffer->GetVertexWidth(x+GMP_MAPSIZE,y+GMP_MAPSIZE,vertex));
                pFragMap->SetSingleVertexColor(x,y,vertex,
                                                pMapBuffer->GetVertexColorR(x+GMP_MAPSIZE,y+GMP_MAPSIZE,vertex),
                                                pMapBuffer->GetVertexColorG(x+GMP_MAPSIZE,y+GMP_MAPSIZE,vertex),
                                                pMapBuffer->GetVertexColorB(x+GMP_MAPSIZE,y+GMP_MAPSIZE,vertex));
                        
                pFragMap=pFragMap->pNext;
                pFragMap->SetSingleVertexHeight(x,y,vertex,pMapBuffer->GetVertexHeight(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,vertex));
                pFragMap->SetSingleVertexWidth(x,y,vertex,pMapBuffer->GetVertexWidth(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,vertex));
                pFragMap->SetSingleVertexColor(x,y,vertex,
                                                pMapBuffer->GetVertexColorR(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,vertex),
                                                pMapBuffer->GetVertexColorG(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,vertex),
                                                pMapBuffer->GetVertexColorB(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,vertex));

                pFragMap=pFragMap->pNext;
                pFragMap->SetSingleVertexHeight(x,y,vertex,pMapBuffer->GetVertexHeight(x,y+GMP_MAPSIZE*2,vertex));
                pFragMap->SetSingleVertexWidth(x,y,vertex,pMapBuffer->GetVertexWidth(x,y+GMP_MAPSIZE*2,vertex));
                pFragMap->SetSingleVertexColor(x,y,vertex,
                                                pMapBuffer->GetVertexColorR(x,y+GMP_MAPSIZE*2,vertex),
                                                pMapBuffer->GetVertexColorG(x,y+GMP_MAPSIZE*2,vertex),
                                                pMapBuffer->GetVertexColorB(x,y+GMP_MAPSIZE*2,vertex));
           
                pFragMap=pFragMap->pNext;
                pFragMap->SetSingleVertexHeight(x,y,vertex,pMapBuffer->GetVertexHeight(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,vertex));
                pFragMap->SetSingleVertexWidth(x,y,vertex,pMapBuffer->GetVertexWidth(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,vertex));
                pFragMap->SetSingleVertexColor(x,y,vertex,
                                                pMapBuffer->GetVertexColorR(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,vertex),
                                                pMapBuffer->GetVertexColorG(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,vertex),
                                                pMapBuffer->GetVertexColorB(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,vertex));
            
                pFragMap=pFragMap->pNext;
                pFragMap->SetSingleVertexHeight(x,y,vertex,pMapBuffer->GetVertexHeight(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,vertex));
                pFragMap->SetSingleVertexWidth(x,y,vertex,pMapBuffer->GetVertexWidth(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,vertex));
                pFragMap->SetSingleVertexColor(x,y,vertex,
                                                pMapBuffer->GetVertexColorR(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,vertex),
                                                pMapBuffer->GetVertexColorG(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,vertex),
                                                pMapBuffer->GetVertexColorB(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,vertex));
            }
        }
    }
}

/**************************************************
 ** Fill the pMapBuffer with FragMaps data        **
 **************************************************/

 void FillpMapBuffer(void)
{  
    int x;
    int y;

    int bank;
    int tile;
    int object;
    int priority;

    for(x=0;x<GMP_MAPSIZE;x++)
    {
        for(y=0;y<GMP_MAPSIZE;y++)
        {
            // Tile Copy

            pFragMap=pFirstFragMap;

            bank=pFragMap->GetTile(x,y)&0xFF00;
            bank=bank/256;
            tile=pFragMap->GetTile(x,y)&0x00FF;
            pMapBuffer->SetTile(x,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank=pFragMap->GetTile(x,y)&0xFF00;
            bank=bank/256;
            tile=pFragMap->GetTile(x,y)&0x00FF;
            pMapBuffer->SetTile(x+GMP_MAPSIZE,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank=pFragMap->GetTile(x,y)&0xFF00;
            bank=bank/256;
            tile=pFragMap->GetTile(x,y)&0x00FF;
            pMapBuffer->SetTile(x+GMP_MAPSIZE*2,y,bank,tile);

            pFragMap=pFragMap->pNext;

            bank=pFragMap->GetTile(x,y)&0xFF00;
            bank=bank/256;
            tile=pFragMap->GetTile(x,y)&0x00FF;
            pMapBuffer->SetTile(x,y+GMP_MAPSIZE,bank,tile);

            pFragMap=pFragMap->pNext;

            bank=pFragMap->GetTile(x,y)&0xFF00;
            bank=bank/256;
            tile=pFragMap->GetTile(x,y)&0x00FF;
            pMapBuffer->SetTile(x+GMP_MAPSIZE,y+GMP_MAPSIZE,bank,tile);

            pFragMap=pFragMap->pNext;

            bank=pFragMap->GetTile(x,y)&0xFF00;
            bank=bank/256;
            tile=pFragMap->GetTile(x,y)&0x00FF;
            pMapBuffer->SetTile(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,bank,tile);

            pFragMap=pFragMap->pNext;

            bank=pFragMap->GetTile(x,y)&0xFF00;
            bank=bank/256;
            tile=pFragMap->GetTile(x,y)&0x00FF;
            pMapBuffer->SetTile(x,y+GMP_MAPSIZE*2,bank,tile);

            pFragMap=pFragMap->pNext;

            bank=pFragMap->GetTile(x,y)&0xFF00;
            bank=bank/256;
            tile=pFragMap->GetTile(x,y)&0x00FF;
            pMapBuffer->SetTile(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,bank,tile);

            pFragMap=pFragMap->pNext;

            bank=pFragMap->GetTile(x,y)&0xFF00;
            bank=bank/256;
            tile=pFragMap->GetTile(x,y)&0x00FF;
            pMapBuffer->SetTile(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,bank,tile);            

            // Object Copy  25 Nov 99

            for(priority=0;priority<GMP_OBJECT_LAYERS;priority++)
            {
                pFragMap=pFirstFragMap;

                bank = pFragMap->GetObj(x,y,priority)&0xFF00;
                bank=bank/256;
                object=pFragMap->GetObj(x,y,priority)&0x00FF;
                pMapBuffer->SetObj(x,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank = pFragMap->GetObj(x,y,priority)&0xFF00;
                bank=bank/256;
                object=pFragMap->GetObj(x,y,priority)&0x00FF;
                pMapBuffer->SetObj(x+GMP_MAPSIZE,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank = pFragMap->GetObj(x,y,priority)&0xFF00;
                bank=bank/256;
                object=pFragMap->GetObj(x,y,priority)&0x00FF;
                pMapBuffer->SetObj(x+GMP_MAPSIZE*2,y,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank = pFragMap->GetObj(x,y,priority)&0xFF00;
                bank=bank/256;
                object=pFragMap->GetObj(x,y,priority)&0x00FF;
                pMapBuffer->SetObj(x,y+GMP_MAPSIZE,bank,object,priority);

                pFragMap=pFragMap->pNext;
                
                bank = pFragMap->GetObj(x,y,priority)&0xFF00;
                bank=bank/256;
                object=pFragMap->GetObj(x,y,priority)&0x00FF;
                pMapBuffer->SetObj(x+GMP_MAPSIZE,y+GMP_MAPSIZE,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank = pFragMap->GetObj(x,y,priority)&0xFF00;
                bank=bank/256;
                object=pFragMap->GetObj(x,y,priority)&0x00FF;
                pMapBuffer->SetObj(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank = pFragMap->GetObj(x,y,priority)&0xFF00;
                bank=bank/256;
                object=pFragMap->GetObj(x,y,priority)&0x00FF;
                pMapBuffer->SetObj(x,y+GMP_MAPSIZE*2,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank = pFragMap->GetObj(x,y,priority)&0xFF00;
                bank=bank/256;
                object=pFragMap->GetObj(x,y,priority)&0x00FF;
                pMapBuffer->SetObj(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,bank,object,priority);

                pFragMap=pFragMap->pNext;

                bank = pFragMap->GetObj(x,y,priority)&0xFF00;
                bank=bank/256;
                object=pFragMap->GetObj(x,y,priority)&0x00FF;
                pMapBuffer->SetObj(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,bank,object,priority);               
            }
            
            // Copy Properties  29 Jul 00            
           
            pFragMap=pFirstFragMap;
            pMapBuffer->SetProperties(x,y,pFragMap->cGetProperties(x,y));

            pFragMap=pFragMap->pNext;
            pMapBuffer->SetProperties(x+GMP_MAPSIZE,y,pFragMap->cGetProperties(x,y));

            pFragMap=pFragMap->pNext;
            pMapBuffer->SetProperties(x+GMP_MAPSIZE*2,y,pFragMap->cGetProperties(x,y));

            pFragMap=pFragMap->pNext;
            pMapBuffer->SetProperties(x,y+GMP_MAPSIZE,pFragMap->cGetProperties(x,y));

            pFragMap=pFragMap->pNext;
            pMapBuffer->SetProperties(x+GMP_MAPSIZE,y+GMP_MAPSIZE,pFragMap->cGetProperties(x,y));

            pFragMap=pFragMap->pNext;
            pMapBuffer->SetProperties(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,pFragMap->cGetProperties(x,y));

            pFragMap=pFragMap->pNext;
            pMapBuffer->SetProperties(x,y+GMP_MAPSIZE*2,pFragMap->cGetProperties(x,y));

            pFragMap=pFragMap->pNext;
            pMapBuffer->SetProperties(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,pFragMap->cGetProperties(x,y));

            pFragMap=pFragMap->pNext;
            pMapBuffer->SetProperties(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,pFragMap->cGetProperties(x,y));

            for(int vertex=0;vertex<4;vertex++)
            {
                pFragMap=pFirstFragMap;                
                pMapBuffer->SetSingleVertexHeight(x,y,vertex,pFragMap->GetVertexHeight(x,y,vertex));
                pMapBuffer->SetSingleVertexWidth(x,y,vertex,pFragMap->GetVertexWidth(x,y,vertex));
                pMapBuffer->SetSingleVertexColor(x,y,vertex,
                                                    pFragMap->GetVertexColorR(x,y,vertex),
                                                    pFragMap->GetVertexColorG(x,y,vertex),
                                                    pFragMap->GetVertexColorB(x,y,vertex));
           
                pFragMap=pFragMap->pNext;
                pMapBuffer->SetSingleVertexHeight(x+GMP_MAPSIZE,y,vertex,pFragMap->GetVertexHeight(x,y,vertex));
                pMapBuffer->SetSingleVertexWidth(x+GMP_MAPSIZE,y,vertex,pFragMap->GetVertexWidth(x,y,vertex));
                pMapBuffer->SetSingleVertexColor(x+GMP_MAPSIZE,y,vertex,
                                                    pFragMap->GetVertexColorR(x,y,vertex),
                                                    pFragMap->GetVertexColorG(x,y,vertex),
                                                    pFragMap->GetVertexColorB(x,y,vertex));
           
                pFragMap=pFragMap->pNext;
                pMapBuffer->SetSingleVertexHeight(x+GMP_MAPSIZE*2,y,vertex,pFragMap->GetVertexHeight(x,y,vertex));
                pMapBuffer->SetSingleVertexWidth(x+GMP_MAPSIZE*2,y,vertex,pFragMap->GetVertexWidth(x,y,vertex));
                pMapBuffer->SetSingleVertexColor(x+GMP_MAPSIZE*2,y,vertex,
                                                    pFragMap->GetVertexColorR(x,y,vertex),
                                                    pFragMap->GetVertexColorG(x,y,vertex),
                                                    pFragMap->GetVertexColorB(x,y,vertex));

                pFragMap=pFragMap->pNext;
                pMapBuffer->SetSingleVertexHeight(x,y+GMP_MAPSIZE,vertex,pFragMap->GetVertexHeight(x,y,vertex));
                pMapBuffer->SetSingleVertexWidth(x,y+GMP_MAPSIZE,vertex,pFragMap->GetVertexWidth(x,y,vertex));
                pMapBuffer->SetSingleVertexColor(x,y+GMP_MAPSIZE,vertex,
                                                    pFragMap->GetVertexColorR(x,y,vertex),
                                                    pFragMap->GetVertexColorG(x,y,vertex),
                                                    pFragMap->GetVertexColorB(x,y,vertex));
           
                pFragMap=pFragMap->pNext;
                pMapBuffer->SetSingleVertexHeight(x+GMP_MAPSIZE,y+GMP_MAPSIZE,vertex,pFragMap->GetVertexHeight(x,y,vertex));
                pMapBuffer->SetSingleVertexWidth(x+GMP_MAPSIZE,y+GMP_MAPSIZE,vertex,pFragMap->GetVertexWidth(x,y,vertex));
                pMapBuffer->SetSingleVertexColor(x+GMP_MAPSIZE,y+GMP_MAPSIZE,vertex,
                                                    pFragMap->GetVertexColorR(x,y,vertex),
                                                    pFragMap->GetVertexColorG(x,y,vertex),
                                                    pFragMap->GetVertexColorB(x,y,vertex));
                        
                pFragMap=pFragMap->pNext;
                pMapBuffer->SetSingleVertexHeight(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,vertex,pFragMap->GetVertexHeight(x,y,vertex));
                pMapBuffer->SetSingleVertexWidth(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,vertex,pFragMap->GetVertexWidth(x,y,vertex));
                pMapBuffer->SetSingleVertexColor(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE,vertex,
                                                    pFragMap->GetVertexColorR(x,y,vertex),
                                                    pFragMap->GetVertexColorG(x,y,vertex),
                                                    pFragMap->GetVertexColorB(x,y,vertex));

                pFragMap=pFragMap->pNext;
                pMapBuffer->SetSingleVertexHeight(x,y+GMP_MAPSIZE*2,vertex,pFragMap->GetVertexHeight(x,y,vertex));
                pMapBuffer->SetSingleVertexWidth(x,y+GMP_MAPSIZE*2,vertex,pFragMap->GetVertexWidth(x,y,vertex));
                pMapBuffer->SetSingleVertexColor(x,y+GMP_MAPSIZE*2,vertex,
                                                    pFragMap->GetVertexColorR(x,y,vertex),
                                                    pFragMap->GetVertexColorG(x,y,vertex),
                                                    pFragMap->GetVertexColorB(x,y,vertex));

                pFragMap=pFragMap->pNext;
                pMapBuffer->SetSingleVertexHeight(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,vertex,pFragMap->GetVertexHeight(x,y,vertex));
                pMapBuffer->SetSingleVertexWidth(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,vertex,pFragMap->GetVertexWidth(x,y,vertex));
                pMapBuffer->SetSingleVertexColor(x+GMP_MAPSIZE,y+GMP_MAPSIZE*2,vertex,
                                                    pFragMap->GetVertexColorR(x,y,vertex),
                                                    pFragMap->GetVertexColorG(x,y,vertex),
                                                    pFragMap->GetVertexColorB(x,y,vertex));
           
                pFragMap=pFragMap->pNext;
                pMapBuffer->SetSingleVertexHeight(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,vertex,pFragMap->GetVertexHeight(x,y,vertex));
                pMapBuffer->SetSingleVertexWidth(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,vertex,pFragMap->GetVertexWidth(x,y,vertex));
                pMapBuffer->SetSingleVertexColor(x+GMP_MAPSIZE*2,y+GMP_MAPSIZE*2,vertex,
                                                    pFragMap->GetVertexColorR(x,y,vertex),
                                                    pFragMap->GetVertexColorG(x,y,vertex),
                                                    pFragMap->GetVertexColorB(x,y,vertex));
            }
        }
    }
}



/**************************************************
 ** Save the MAP buffers                         **
 **************************************************/

void SaveMap(int x, int y, int z)
{
    if(!pFirstFragMap) return;
    x=MapCoord(x);
    y=MapCoord(y);
    z=MapCoord(z);
    char Directory[1024]; memset(Directory,0,1024);

    sprintf(Directory,"map");

    FillFragMaps();

    pFragMap=pFirstFragMap;
    pFragMap->SaveSector3D(Directory,x-1,y-1,z);
    
    pFragMap=pFragMap->pNext;
    pFragMap->SaveSector3D(Directory,x,y-1,z);
    
    pFragMap=pFragMap->pNext;
    pFragMap->SaveSector3D(Directory,x+1,y-1,z);
    
    pFragMap=pFragMap->pNext;
    pFragMap->SaveSector3D(Directory,x-1,y,z);
    
    pFragMap=pFragMap->pNext;
    pFragMap->SaveSector3D(Directory,x,y,z);
    
    pFragMap=pFragMap->pNext;
    pFragMap->SaveSector3D(Directory,x+1,y,z);
    
    pFragMap=pFragMap->pNext;
    pFragMap->SaveSector3D(Directory,x-1,y+1,z);
    
    pFragMap=pFragMap->pNext;
    pFragMap->SaveSector3D(Directory,x,y+1,z);
    
    pFragMap=pFragMap->pNext;
    pFragMap->SaveSector3D(Directory,x+1,y+1,z);
}


/**************************************************
 ** Load the MAP buffers with data               **
 **************************************************/

void LoadMap(int x, int y, int z)
{
    if(!pFirstFragMap) return;
    x=MapCoord(x);
    y=MapCoord(y);
    z=MapCoord(z);
    
    char Directory[1024]; memset(Directory,0,1024);

    sprintf(Directory,"map");

    pFragMap=pFirstFragMap;
    pFragMap->LoadSector3D(Directory,x-1,y-1,z);

    pFragMap=pFragMap->pNext;
    pFragMap->LoadSector3D(Directory,x,y-1,z);

    pFragMap=pFragMap->pNext;
    pFragMap->LoadSector3D(Directory,x+1,y-1,z);

    pFragMap=pFragMap->pNext;
    pFragMap->LoadSector3D(Directory,x-1,y,z);

    pFragMap=pFragMap->pNext;
    pFragMap->LoadSector3D(Directory,x,y,z);

    pFragMap=pFragMap->pNext;
    pFragMap->LoadSector3D(Directory,x+1,y,z);

    pFragMap=pFragMap->pNext;
    pFragMap->LoadSector3D(Directory,x-1,y+1,z);

    pFragMap=pFragMap->pNext;
    pFragMap->LoadSector3D(Directory,x,y+1,z);

    pFragMap=pFragMap->pNext;
    pFragMap->LoadSector3D(Directory,x+1,y+1,z);

    FillpMapBuffer();
}

#endif // _EMBER_UE_ 
#endif // _EMBER_MS_WEB_
#endif // _EMBER_MASTER_

