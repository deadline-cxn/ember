
#include "b_glmouse.h"
#include "dlstorm.h"
#include "c_util.h"
//#include "global.h"
#include "c_data.h"
#include "fm_util.h"
#include "b_gltexture.h"

//////////////////////////////////////////////////////////////////////////////////////////
// CGLMouse Class 
//////////////////////////////////////////////////////////////////////////////////////////

CGLMouse::CGLMouse()
{
    memset(filename,0,1024);
    Cursor=0;
    DLog("Mouse cursor created");
}
//////////////////////////////////////////////////////////////////////////////////////////
CGLMouse::CGLMouse(char *fn)
{
    memset(filename,0,1024);
    Cursor=0;
    load(fn);
    DLog("Mouse cursor created");
}
//////////////////////////////////////////////////////////////////////////////////////////
CGLMouse::~CGLMouse()
{
    kill();
    DLog("Mouse cursor destroyed");
}
//////////////////////////////////////////////////////////////////////////////////////////
GLvoid CGLMouse::kill()
{
    DEL(Cursor);
}
//////////////////////////////////////////////////////////////////////////////////////////
GLvoid CGLMouse::load(char *file)
{
    if(!strlen(file)) return;
    strcpy(filename,file);
    //Log("Loading mouse cursor %s",filename);
    FILE *fp;
    char temp[32];
    DEL(Cursor);
    Cursor=new CGLTexture();
    if(!Cursor) return;
    Cursor->bmap=0;
    Cursor->mask=0;
    Cursor->usemask=1;
    Cursor->Load(va("%s.bmp",file),0);
    fp=fopen(va("%s.ini",file),"rt");
    if(fp)
    {
        DLog("Found mouse cursor settings %s",va("%s.ini",file));
        fgets(temp,32,fp); x_offset  = atoi(temp); DLog(" \tx_offset =%d",x_offset);
        fgets(temp,32,fp); y_offset  = atoi(temp); DLog(" \ty_offset =%d",y_offset);
        fgets(temp,32,fp); x_hotspot = atoi(temp); DLog(" \tx_hotspot=%d",x_hotspot);
        fgets(temp,32,fp); y_hotspot = atoi(temp); DLog(" \ty_hotspot=%d",y_hotspot);
        fclose(fp);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
GLvoid CGLMouse::draw(void)
{
    if(!Cursor) return;
    if(!Cursor->bmap) load(filename);
    if(!Cursor->mask) load(filename);
    if(!Cursor->bmap) return;
    if(!Cursor->mask) return;

    x=x/2;
    y=(-y/2)+(pClientData->ScreenHeight/2);

    glLoadIdentity();
    glColor3f(1.0f,1.0f,1.0f);  
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR,GL_ZERO);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x,y,0);
    glBindTexture(GL_TEXTURE_2D,Cursor->mask);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f,1.0f,1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x ),    float(y - 64),    1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+64),  float(y - 64),    1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+64),  float(y ),        1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x ),    float(y ),        1);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x,y,0);
    glBindTexture(GL_TEXTURE_2D,Cursor->bmap);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f,1.0f,1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x ),    float(y - 64),    1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+64),  float(y - 64),    1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+64),  float(y ),        1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x ),    float(y ),        1);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

}

