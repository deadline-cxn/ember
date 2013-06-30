/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_UTILITIES
#define _EMBER_UTILITIES

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C_FM_GUI_STUMP;
class C_FM_GUI_CONTROL_DATA;
class CLog;

#ifdef __cplusplus
extern "C"
{
#endif
    void    Log(char *fmt,...);
    void    LogC(char *fmt,...);
    void    DLog(char *fmt,...);
    int     OnScreenXCoord(int coordinate);
    int     OnScreenYCoord(int coordinate);
    unsigned long  GetSerialNumber(bool);
    void    FillFragMaps(void);
    void    FillpMapBuffer(void);
    void    SaveMap(int x, int y, int z);
    void    LoadMap(int x, int y, int z);
    void    RemoveAccount(char *szName);
	void	ClearGUIs(void);
	void	ClearGUIData(void);
	void	ParseGUIVars(char *text);
#ifdef __cplusplus
}
#endif

extern CLog *pLog;
extern bool bLog;
extern C_FM_GUI_CONTROL_DATA *pFirstGUIData;
extern C_FM_GUI_CONTROL_DATA *pGUIData;

#ifdef _EMBER_SERVER_
extern bool bQuiet;
#endif

#endif // _EMBER_UTILITIES


