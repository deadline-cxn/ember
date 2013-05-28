/***************************************************************

 ***************************************************************/

#ifndef _EMBER_GAME_CLIENT
#define _EMBER_GAME_CLIENT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////
// Function declarations

#ifdef __cplusplus
extern "C"
{
#endif

	void MainGameLoop(void);
	bool doInit(void);
	void ShutDown(void);
	void DoSDLEvents(void);

#ifdef __cplusplus
}
#endif



#endif // _EMBER_GAME_CLIENT
