#ifndef B4_GL_FONT
#define B4_GL_FONT

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

class CGLFont
{
public:

    CGLFont();
    CGLFont(char *fn);
    ~CGLFont();

    bool Load(char *file);
    GLvoid Reload();
    GLvoid Kill();

    GLvoid PrintSolid(GLint x, GLint y, char *string, u_char r, u_char g, u_char b); // will print solid color ignoring ^# codes
    GLvoid Print(GLint x, GLint y,char *string,int set); // will print with ^# codes

    GLvoid SinPrint(GLint x, GLint y, char *string,int set, u_char r, u_char g, u_char b);

    GLvoid Stuff(GLenum target, GLint x, GLint y, char *string, int set, u_char r, u_char g, u_char b);
#ifdef _WIN32
    __forceinline
#endif 
    GLvoid RawPrint(GLint x, GLint y, char *string,int set, u_char r, u_char g, u_char b);
    GLvoid BoldPrint(GLint x,GLint y, char *string,int set, u_char r, u_char g, u_char b);
    GLvoid WhatPrint(GLint x,GLint y, char *string,int set, u_char r, u_char g, u_char b);

    char szFile[1024];          // file name of the font
    char Set1[64];              // name of font 0
    char Set2[64];              // name of font 1
    GLfloat width;              // specify the width of one drawn character
    GLfloat height;             // specify the height of one drawn character
    u_char r;                   // default r color
    u_char g;                   // default g color
    u_char b;                   // default b color

    GLuint      pFontList;          // Base Display List For The Font
    GLuint      pFontMaskList;      // Base Display List For The Font
    CGLTexture *pFontTex;
    //GLuint  pFontTexture;       // Storage For Our Font Texture
    //GLuint  pFontMaskTexture;   // Storage For Our Font Texture

};

//typedef 
struct CGLFontList { CGLFont *ob; };

extern "C" int CGLFont_StrLen(char *string);

#endif//B4_GL_FONT


