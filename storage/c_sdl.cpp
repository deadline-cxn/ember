#include "dlstorm.h"
#include "c_sdl.h"
#include "sdl_image.h"

bool bLeftDown,bMiddleDown,bRightDown;
bool bLeftRelease,bMiddleRelease,bRightRelease;
bool bLeftDblClick,bMiddleDblClick,bRightDblClick;
bool bWheelUp, bWheelDown;
long dwLeftDblClickTimer,dwMiddleDblClickTimer,dwRightDblClickTimer;
int ix,iy;

///////////////////////////////////////////////////////////////////////
void InitializeInput(void) {
    bLeftDown             = 0;
    bMiddleDown           = 0;
    bRightDown            = 0;
    bLeftDblClick         = 0;
    bMiddleDblClick       = 0;
    bRightDblClick        = 0;
    bWheelUp              = 0;
    bWheelDown            = 0;
    dwLeftDblClickTimer   = getticks();
    dwMiddleDblClickTimer = getticks();
    dwRightDblClickTimer  = getticks();
}
///////////////////////////////////////////////////////////////////////
void ShutDownInput(void) { } // Obsolete
///////////////////////////////////////////////////////////////////////
void RefreshMouse(void){
    bool bLeft,bRight,bMiddle;
    int butt;
    butt       = SDL_GetMouseState(&ix,&iy);
    bLeft      = TOBOOL((SDL_BUTTON(SDL_BUTTON_LEFT)      & butt));
    bMiddle    = TOBOOL((SDL_BUTTON(SDL_BUTTON_MIDDLE)    & butt));
    bRight     = TOBOOL((SDL_BUTTON(SDL_BUTTON_RIGHT)     & butt));
    bWheelUp   = TOBOOL((SDL_BUTTON(SDL_BUTTON_WHEELUP)   & butt));
    bWheelDown = TOBOOL((SDL_BUTTON(SDL_BUTTON_WHEELDOWN) & butt));
    bLeftRelease=0;
    bRightRelease=0;
    bMiddleRelease=0;
    bLeftDblClick=0;
    bMiddleDblClick=0;
    bRightDblClick=0;
    if( (bLeftDown)   & (!bLeft))
    {
        bLeftRelease   = 1;
        if(getticks()-dwLeftDblClickTimer<300) bLeftDblClick = 1;
        dwLeftDblClickTimer=getticks();
    }
    if( (bRightDown)  & (!bRight))
    {
        bRightRelease  = 1;
        if(getticks()-dwRightDblClickTimer<300) bRightDblClick = 1;
        dwRightDblClickTimer=getticks();
    }
    if( (bMiddleDown) & (!bMiddle))
    {
        bMiddleRelease = 1;
        if(getticks()-dwMiddleDblClickTimer<300) bMiddleDblClick = 1;
        dwMiddleDblClickTimer=getticks();
    }
    bLeftDown=bLeft;
    bRightDown=bRight;
    bMiddleDown=bMiddle;
}
///////////////////////////////////////////////////////////////////////
bool GetMouseRelease(int iWhich){
    switch(iWhich)
    {
        case SDL_BUTTON_LEFT:
            return bLeftRelease;
        case SDL_BUTTON_MIDDLE:
            return bMiddleRelease;
        case SDL_BUTTON_RIGHT:
            return bRightRelease;
        default:
            return 0;
            break;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////
void SetMouseRelease(int iWhich,bool set){
    switch(iWhich)
    {
        case SDL_BUTTON_LEFT:
            bLeftRelease=set;
            return;
        case SDL_BUTTON_MIDDLE:
            bMiddleRelease=set;
            return;
        case SDL_BUTTON_RIGHT:
            bRightRelease=set;
            return;
        default:
            break;
    }
}
///////////////////////////////////////////////////////////////////////
bool GetMouseDown(int iWhich){
    switch(iWhich)
    {
        case SDL_BUTTON_LEFT:
            return bLeftDown;
        case SDL_BUTTON_RIGHT:
            return bRightDown;
        case SDL_BUTTON_MIDDLE:
            return bMiddleDown;
        default:
            return 0;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////
bool GetMouseDblClick(int iWhich){
    switch(iWhich)
    {
        case SDL_BUTTON_LEFT:
            return bLeftDblClick;
        case SDL_BUTTON_RIGHT:
            return bRightDblClick;
        case SDL_BUTTON_MIDDLE:
            return bMiddleDblClick;
        default:
            return 0;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////
int  GetMouseX(void){ return ix; }
///////////////////////////////////////////////////////////////////////
void SetMouseX(int x) { ix=x; }
///////////////////////////////////////////////////////////////////////
int  GetMouseY(void){return iy;}
///////////////////////////////////////////////////////////////////////
void SetMouseY(int y) { iy=y; }
///////////////////////////////////////////////////////////////////////
bool MouseInX(int x1,int x2){
	if((GetMouseX()>x1) && (GetMouseX()<x2)) return true;
	return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseInY(int y1,int y2){
	if((GetMouseY()>y1) && (GetMouseY()<y2)) return true;
	return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseIn( int x1,int y1,int x2,int y2) { if((MouseInX(x1,x2))&&(MouseInY(y1,y2))) return true; return false; }
/*
///////////////////////////////////////////////////////////////////////
bool MouseInXPct(float fx1,float fx2)
{
	if((GetMouseX()>(pClientData->ScreenWidth*fx1)) && (GetMouseX()<(pClientData->ScreenWidth*fx2)))
		return true;
	return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseInYPct(float fy1,float fy2)
{
	if((GetMouseY()>(pClientData->ScreenHeight*fy1)) && (GetMouseY()<(pClientData->ScreenHeight*fy2)))
		return true;
	return false;
}
///////////////////////////////////////////////////////////////////////
bool MouseInPct( float fx1,float fy1,float fx2,float fy2)
{
	if((MouseInXPct(fx1,fx2))&&(MouseInYPct(fy1,fy2)))
		return true;
	return false;
}
&*/
///////////////////////////////////////////////////////////////////////
void SetMouseWheelUp(bool m) { bWheelUp=m; }
///////////////////////////////////////////////////////////////////////
int  GetMouseWheelUp(void)   { return bWheelUp;   }
///////////////////////////////////////////////////////////////////////
void SetMouseWheelDown(bool m) { bWheelDown=m; }
///////////////////////////////////////////////////////////////////////
int  GetMouseWheelDown(void) { return bWheelDown; }
///////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSDL_Wrap Class

CSDL_Wrap::CSDL_Wrap(){
	GAF=0;
	Log=0;
	SDL_Surface *s;
	s=0;
	videoflags = SDL_HWSURFACE|SDL_HWPALETTE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_NOFRAME;
    Begin(0,800,600,32,s);
}

CSDL_Wrap::CSDL_Wrap(char *appname,Uint32 nw,Uint32 nh,Uint32 nb){
	GAF=0;
	Log=0;
	SDL_Surface *s;
	s=0;
	videoflags = SDL_HWSURFACE|SDL_HWPALETTE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_NOFRAME;
    Begin(appname,nw,nh,nb,s);
 }

CSDL_Wrap::CSDL_Wrap(char *appname,Uint32 nw,Uint32 nh,Uint32 nb,SDL_Surface *icon){
	GAF=0;
	Log=0;
	videoflags = SDL_HWSURFACE|SDL_HWPALETTE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_NOFRAME;
	Begin(appname,nw,nh,nb,icon);
}

CSDL_Wrap::CSDL_Wrap(char *appname,Uint32 nw,Uint32 nh,Uint32 nb,char *icon){
	GAF=0;
	Log=0;
	videoflags = SDL_HWSURFACE|SDL_HWPALETTE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_NOFRAME;
	Begin(appname,nw,nh,nb,icon);
}
CSDL_Wrap::CSDL_Wrap(char *an,Uint32 w,Uint32 h,Uint32 b, Uint16 f, char *icon){

	GAF=0;
	Log=0;
	videoflags = f;
	Begin(an,w,h,b,icon);
}

CSDL_Wrap::~CSDL_Wrap(){
    DEL(GAF);
    DEL(Log);
    SDL_Quit();
}
bool CSDL_Wrap::Begin(char *appname,Uint32 nw, Uint32 nh, Uint32 np, char *icon){
    if(!Log) Log=new CLog("gfx.log");
    Log->AddEntry("CSDL_Wrap::Begin(1)...");
    // Icon=LoadGAFSurface(icon);
    // if(Icon==0) { InitSuccess=0; return 0; }
    return Begin(appname,nw,nh,np,Icon);
}
bool CSDL_Wrap::Begin(char *appname,Uint32 nw, Uint32 nh, Uint32 np, SDL_Surface *icon){
    if(!Log) Log=new CLog("gfx.log");
    Log->AddEntry("CSDL_Wrap::Begin(2)...");
    w=nw; h=nh; b=np;
    RebuildGAF();
    if(!GAF) GAF = new CGAF("gfx.gaf",GAFCOMP_BEST);
    Icon=icon;
    strcpy(APP_NAME,appname);
    InitSuccess=Init2D(w,h,b,Icon);
    return InitSuccess;
}

void CSDL_Wrap::RebuildGAF(void){
    Log->AddEntry("Rebuilding gfx.gaf GAF...");
    remove("gfx.gaf");
    CGAF *gaf;
    gaf=new CGAF("newgfx.gaf",GAFCOMP_BEST);
    gaf->AddDir("gfx");
    DEL(gaf);
    rename("newgfx.gaf","gfx.gaf");
}

bool CSDL_Wrap::Init2D(int width, int height, int bpp, SDL_Surface *icon){
	if(screen) return true;
    Log->AddEntry(va("CSDL_Wrap::Init2D(%d,%d,%d,(SDL_Surface *)%d)",width,height,bpp,icon));
    if(SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if(icon) SDL_WM_SetIcon(icon, 0);
    if(strlen(APP_NAME)) SDL_WM_SetCaption(APP_NAME,APP_NAME);
    if(videoflags==0) videoflags = SDL_HWSURFACE|SDL_HWPALETTE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_NOFRAME;
    screen = CreateScreen(width, height, bpp, videoflags);
    if(!screen)
    {
       // videoflags=SDL_SWSURFACE|SDL_FULLSCREEN|SDL_NOFRAME;
        //screen = CreateScreen(width, height, bpp, videoflags);
        if(!screen) return false;
    }
    LoadFont("gfx/font.bmp");
    Log->AddEntry("SDL Video started...");
    return true;
}

SDL_Surface *CSDL_Wrap::LoadGAFSurface(char *fn){
    Log->AddEntry(va("CSDL_Wrap::LoadGAFSurface(\"%s\")...",fn));
    if(!GAF) GAF=new CGAF("gfx.gaf",GAFCOMP_BEST);
    Log->AddEntry(va("GAF [%d]",GAF));
    SDL_Surface *what;
    what=0;
    SDL_RWops *rw;
    // unsigned char * fi;
    if(GAF->GetFileSize(fn)) {
        rw = SDL_RWFromMem(GAF->GetFile(fn).fb,GAF->GetFileSize(fn));
        FILE *fb; fb=fopen("TEST.GIF","wb");
        if(fb) {
            fwrite(rw,sizeof(rw),1,fb);
        }
        fclose(fb);
        what = LoadMemSurface(rw);
        SDL_FreeRW(rw);
        if(what) {
            SDL_SetColorKey(what, SDL_SRCCOLORKEY,SDL_MapRGB(what->format, 0, 0, 0));
            Log->AddEntry(va("CSDL_Wrap::LoadGAFSurface(\"%s\") - [%s] loaded to surface [%d]",fn,fn,what));
        }
    }
    else
    {
        Log->AddEntry(va("CSDL_Wrap::LoadGAFSurface(\"%s\") - [%s] doesn't appear to be a valid resource.",fn,fn));
    }
    return what;
}

SDL_Surface *CSDL_Wrap::LoadMemSurface(SDL_RWops *rw){
    Log->AddEntry(va("CSDL_Wrap::LoadMemSurface((SDL_RWops *)%d)...",rw));
    SDL_Surface *what;
    what=0;
    if(!rw) return 0;
    if(IMG_isBMP(rw)) what=IMG_LoadBMP_RW(rw);
    if(IMG_isGIF(rw)) what=IMG_LoadGIF_RW(rw);
    if(IMG_isJPG(rw)) what=IMG_LoadJPG_RW(rw);
    if(IMG_isLBM(rw)) what=IMG_LoadLBM_RW(rw);
    if(IMG_isPCX(rw)) what=IMG_LoadPCX_RW(rw);
    if(IMG_isPNG(rw)) what=IMG_LoadPNG_RW(rw);
    if(IMG_isPNM(rw)) what=IMG_LoadPNM_RW(rw);
    if(IMG_isTIF(rw)) what=IMG_LoadTIF_RW(rw);
    if(IMG_isXCF(rw)) what=IMG_LoadXCF_RW(rw);
    if(IMG_isXPM(rw)) what=IMG_LoadXPM_RW(rw);
    if(IMG_isXV(rw))  what=IMG_LoadXV_RW(rw);
    if(what==0)       what=IMG_LoadTGA_RW(rw);
    Log->AddEntry(va("WHAT = %d",what));
    Log->AddEntry(va("CSDL_Wrap::LoadSurface(SDL_RWops *)%d) - end",rw));
    return what;
}

void CSDL_Wrap::LoadFont(char *fn){
    Log->AddEntry(va("CSDL_Wrap::LoadFont(\"%s\")...",fn));
    Font=LoadGAFSurface(fn);
}

SDL_Surface *CSDL_Wrap::CreateScreen(Uint16 w, Uint16 h, Uint8 bpp, Uint32 flags){
    Log->AddEntry(va("CSDL_Wrap::CreateScreen(%d,%d,%d,%d)...",w,h,bpp,flags));
    SDL_Surface *screen;
    int i;
    SDL_Color palette[NUM_COLORS];
    screen = SDL_SetVideoMode(w, h, bpp, flags);
    if(!screen) return screen;
    if (bpp==8)
    {
        for ( i=0; i < NUM_COLORS; ++i )
        {
            palette[i].r = (NUM_COLORS-1)-i * (256 / NUM_COLORS);
            palette[i].g = (NUM_COLORS-1)-i * (256 / NUM_COLORS);
            palette[i].b = (NUM_COLORS-1)-i * (256 / NUM_COLORS);
        }
        SDL_SetColors(screen, palette, 0, NUM_COLORS);
    }
    return(screen);
}

bool CSDL_Wrap::ToggleFullScreen(){
    int videoflags;
    videoflags = screen->flags;
    videoflags ^= SDL_FULLSCREEN;
    screen = CreateScreen( screen->w, screen->h, screen->format->BitsPerPixel, videoflags);
    if(screen == NULL ) return false;
    return true;
}
Uint32 CSDL_Wrap::GetPixel(int x, int y){
    if(x>(screen->w-1)) return 0; if(x<0) return 0; if(y>(screen->h-1)) return 0; if(y<0) return 0;
    int bpp = screen->format->BytesPerPixel; Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
    switch(bpp) {
    case 1: return *p;
    case 2: return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2];
        else                                return p[0] | p[1] << 8 | p[2] << 16;
    case 4: return *(Uint32 *)p;
    default:return 0;
    }
}
void CSDL_Wrap::PutPixel(int x, int y, Uint32 pixel){
    if(x>(screen->w-1)) return; if(x<0) return; if(y>(screen->h-1)) return;if(y<0) return;
    int bpp=screen->format->BytesPerPixel; Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
    switch(bpp)
    {
    case 1: *p = pixel; break;
    case 2: *(Uint16 *)p = pixel; break;
    case 3:
        if(SDL_BYTEORDER==SDL_BIG_ENDIAN){ p[0] = (pixel >> 16) & 0xff; p[1] = (pixel >> 8) & 0xff; p[2] = pixel & 0xff; }
        else        { p[0] = pixel & 0xff; p[1] = (pixel >> 8) & 0xff; p[2] = (pixel >> 16) & 0xff; }
        break;
    case 4: *(Uint32 *)p = pixel; break;
    }
}

void CSDL_Wrap::ClearScreen(Uint32 color){
    SDL_Rect barea2; barea2.x=0; barea2.y=0; barea2.w=screen->w;barea2.h=screen->h;
    SDL_FillRect(screen, &barea2, color);
}

void CSDL_Wrap::Flip(){
    if(videoflags&&SDL_DOUBLEBUF) SDL_Flip(screen);
}

void CSDL_Wrap::DrawRect(int x, int y, int w, int h, Uint32 color){
    SDL_Rect barea2; barea2.x=x; barea2.y=y; barea2.w=w; barea2.h=h;
    SDL_FillRect(screen, &barea2, color);
}

void CSDL_Wrap::DrawMap(){
    int x,y ;
	//int dt;

        //SDL_LockSurface(screen);
        for(y=h;y>-1;y--)
        {
            for(x=0;x<w;x++)
            {
                    int fartr=rand()%255;
                    //int fartg=rand()%255;
                    //int fartb=rand()%255;
                    //DrawRect(x*2,y*2,2,2,__RGB(fartr,0,0));
                    PutPixel(x,y,SDL_MapRGB(screen->format,0,0,fartr));
            }
        }
    //SDL_UnlockSurface(screen);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

C2DFont::C2DFont() { Font=0; }
C2DFont::C2DFont(char *file) { Load(file); }
C2DFont::~C2DFont() { }
void C2DFont::Load(char *file) { Font = SDL_LoadBMP(file); if(Font) SDL_SetColorKey(Font, SDL_SRCCOLORKEY,SDL_MapRGB(Font->format, 0, 0, 0)); }
void C2DFont::Write(int x, int y, char *string,int bank){
    char c;
    int getx;
    int gety;
    bool draw;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    src_rect.h=15;
    src_rect.w=15;
    dst_rect.x=x;
    dst_rect.y=y;
    for(unsigned int i=0;i<strlen(string);i++)
    {
        c=string[i];
        draw=0;
        if((c>32) && (c<128))
        {
            getx=(c-32)*16;
            gety=0;

            while(getx>255)
            {
                gety+=16;
                getx-=256;
            }
            draw=1;
        }
        if(draw) {

            src_rect.x=getx;
            src_rect.y=gety+(bank*128);
            if(Font)
            SDL_BlitSurface(Font, &src_rect, SDL_GetVideoSurface(), &dst_rect);
        }
        dst_rect.x+=10;
    }
}

