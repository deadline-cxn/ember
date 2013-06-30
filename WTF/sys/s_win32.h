/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_WIN32_
#define _EMBER_WIN32_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32

#include "dlstorm.h"

extern BOOL WINAPI HandleCTRL_C(DWORD dwCtrlType);
extern void ConsoleSetup(void);
extern void ConsoleShutDown(void);


#endif 

#endif

