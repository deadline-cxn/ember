
#ifndef B4_GL_MOUSE
#define B4_GL_MOUSE

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

class CGLTexture;


// Mouse pointer cursor definitions

#define MP_NORTHWEST	0
#define MP_MENU			0
#define MP_NORTH		1
#define	MP_NORTHEAST	2
#define MP_WEST			3
#define MP_SYSTEMBUSY   4
#define MP_EAST         5
#define MP_SOUTHWEST	6
#define MP_SOUTH        7
#define MP_SOUTHEAST    8
#define MP_TEXTINPUT    9
#define MP_TARGET       10
#define MP_ITEM         11
#define MP_NONE         MAX_MOUSE_CURSORS

class CGLMouse
{
public:
    CGLMouse();//          { memset(filename,0,1024); Cursor=0; };
    CGLMouse(char *fn);//  { memset(filename,0,1024); Cursor=0; load(fn); };
    ~CGLMouse();// { kill(); };
    char    filename[1024]; // file name of the cursor
    unsigned char  r;            // default r color
    unsigned char  g;            // default g color
    unsigned char  b;            // default b color
    int     x;
    int     y;
    char    x_offset;
    char    y_offset;
    char    x_hotspot;
    char    y_hotspot;

    GLvoid reload();
    GLvoid kill();
    GLvoid load(char *file);
    GLvoid draw(void);
    CGLTexture *Cursor;
};

//typedef 
struct CGLMouseList
{
    CGLMouse *cursor;
};



#endif//B4_GL_MOUSE
