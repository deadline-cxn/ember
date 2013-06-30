
#ifndef B4_GL_TEXTURE
#define B4_GL_TEXTURE
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
#include <winsock2.h>
#ifndef _MMEDIA_
#define _MMEDIA_
#include <mmsystem.h>
#include <mmreg.h>
#endif//_MMEDIA
#endif//_WIN32
#include <GL/gl.h>
#include <GL/glu.h>

struct Image { u_long width; u_long height; int bpp; int type; char *data; }; // Image;
#define glDEL(x) if(glIsTexture(x)) { glDeleteTextures(1,&x); x=0; }

class CGLTexture
{
public:

    CGLTexture();
    CGLTexture(char *fname);
    CGLTexture(char *fname,bool fmask);
    ~CGLTexture();

    char    name[64];

    bool    Create(int x,int y);
    bool    Transparent(bool trans);
    bool    Clear(u_char R,u_char G,u_char B);
    bool    ClearMask(u_char R,u_char G,u_char B);
    
    //bool    Draw(int x,int y,int x2,int y2,long color);
    
    bool    Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b);
    bool    Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2);
    bool    DrawRaw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2);

    bool    ImageLoad(char *filename, Image *image);
    bool    LoadTGA(char* filename, Image *image);
    bool    LoadBMP(char* filename, Image *image);

    GLuint  Load(char *filename,bool which); // 0
    // GLuint  LoadMask(char *filename);
    bool    ReLoad(void);

    bool    Loaded(void);
    
    GLuint  bmap;
    GLuint  mask;

    int     width;
    int     height;

    bool    usemask;

    char    tfilename[1024];

    CGLTexture *next;
};

//typedef
 struct CGLTextureList { CGLTexture *texture; };

#endif//B4_GL_TEXTURE
