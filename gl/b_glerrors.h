
#ifndef B4_GL_ERRORS
#define B4_GL_ERRORS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
#include <winsock2.h>
//#include <io.h>
//#include <direct.h>
//#include <conio.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>
#endif//_MMEDIA
#endif//_WIN32
#include <GL/gl.h>
#include <GL/glu.h>
// #include "SDL.h"

extern "C" char *GL_Error(GLenum err);
extern "C" void GL_ClearErrors(void);

#endif
