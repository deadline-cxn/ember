
#include "b_gltexture.h"
#include "c_data.h"
#include "c_util.h"
#include "b_glerrors.h"
#include "fm_util.h"

CGLTexture::CGLTexture()
{
    next=0;
    bmap=0;
    mask=0;
    usemask=0;
    memset(tfilename,0,1024);
    memset(name,0,64);
}

CGLTexture::CGLTexture(char *fname)
{
    next=0;
    bmap=0;
    mask=0;
    usemask=0;
    memset(tfilename,0,1024);
    memset(name,0,64);
    Load(fname,0);

}
CGLTexture::CGLTexture(char *fname, bool fmask)
{
    next=0;
    bmap=0;
    mask=0;
    usemask=fmask;
    memset(tfilename,0,1024);
    memset(name,0,64);
    Load(fname,0);
}
/****************************************************************************************************/
CGLTexture::~CGLTexture()
{
    glDEL(bmap);
    glDEL(mask);
}
bool CGLTexture::Create(int x, int y)
{
    width=x;
    height=y;
    glDEL(bmap);
    bmap=0;
    unsigned int* data;
    data = (unsigned int*)new GLuint[((width * height)* 4 * sizeof(unsigned int))];
    memset(data,((width * height)* 4 * sizeof(unsigned int)),0);
    glGenTextures(1, &bmap);
    glBindTexture(GL_TEXTURE_2D, bmap);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    delete [] data;
    DLog("CGLTexture::Create() -> created a texture!");
    return 1;
}
bool CGLTexture::Clear(u_char R,u_char G,u_char B)
{
//    glCopyTexImage2D()


    //glLoadIdentity();
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0,0,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,bmap);
    glBegin(GL_QUADS);
    glColor3ub(R,G,B);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(0),      float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(width),  float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(width),  float(height), 1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(0),      float(height), 1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return 1;
}

bool CGLTexture::ClearMask(u_char R,u_char G,u_char B)
{
    //glLoadIdentity();
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0,0,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,mask);
    glBegin(GL_QUADS);
    glColor3ub(R,G,B);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(0),      float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(width),  float(0),      1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(width),  float(height), 1.0f);
    glColor3ub(R,G,B);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(0),      float(height), 1.0f);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    return 1;
}

bool CGLTexture::Transparent(bool trans)
{
    if(trans)
    {
        if((width==0)||(height==0)) {width=1024;height=1024;}
        mask=0;
        unsigned int* data;
        data = (unsigned int*)new GLuint[((width * height)* 4 * sizeof(unsigned int))];
        memset(data,((width * height)* 4 * sizeof(unsigned int)),0);
        glGenTextures(1, &mask);
        glBindTexture(GL_TEXTURE_2D, mask);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        delete [] data;
        usemask=1;
        DLog("CGLTexture::Transparent() set texture to use transparency...");
    }
    else
    {
        glDEL(mask);
        usemask=0;
        DLog("CGLTexture::Transparent() set texture to not use transparency...");
    }

    return 1;
}

/****************************************************************************************************/
bool   CGLTexture::Loaded(void)
{
    if(bmap)
    {
        if(usemask) if(!mask) return 0;
        return 1;
    }
    return 0;
}
/****************************************************************************************************/
GLuint CGLTexture::Load(char *filename,bool which)
{
    char filename2[1024]; memset(filename2,0,1024);
    char maskfile[1024];  memset(maskfile,0,1024);
    char ax,ay,az;
    GLenum err;
    if(!strlen(filename)) return 0;

    glDEL(bmap);
    glDEL(mask);

    ax=filename[strlen(filename)-3];
    ay=filename[strlen(filename)-2];
    az=filename[strlen(filename)-1];
    strcpy(filename2,filename);
    filename2[strlen(filename2)-4]=0;
    sprintf(maskfile,"%smask.%c%c%c",filename2,ax,ay,az);

    Image *image=(Image *) malloc(sizeof(Image));
    if(!image) {
        DLog("CGLTexture::Load() Can't allocate resources for [%s]",filename);
        return 0; }
    if(!ImageLoad(filename, image))
    {   FREE(image);
        DLog("CGLTexture::ImageLoad() (called from) CGLTexture::Load() -> did not load:\n [%s]",filename);
        return 0; }

    strcpy(tfilename,filename);
    DLog("CGLTexture::Load() (loaded [%s])",filename);

    if(image)
    {
        GL_ClearErrors();
        glDEL(bmap);
        err=glGetError();
        if(err!=GL_NO_ERROR)
        {
            DLog("error[%d] bmap[%d] glError[%s]",err,bmap,GL_Error(err));
            FREE(image);
            glDEL(bmap);
            return 0;
        }

        GL_ClearErrors();
        glGenTextures(1, &bmap);
        err=glGetError();
        if(err!=GL_NO_ERROR)
        {
            DLog("error[%d] bmap[%d] glError[%s]",err,bmap,GL_Error(err));
            FREE(image);
            glDEL(bmap);
            return 0;
        }
        else
        {
            GL_ClearErrors();
            glBindTexture(GL_TEXTURE_2D, bmap);
            err=glGetError();
            if(err!=GL_NO_ERROR)
            {
                DLog("error[%d] bmap[%d] glError[%s]",err,bmap,GL_Error(err));
                FREE(image);
                glDEL(bmap);
                return 0;
            }
            glTexImage2D    (GL_TEXTURE_2D, 0, 3, image->width, image->height, 0, image->type, GL_UNSIGNED_BYTE, image->data);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFlush();
        }
        FREE(image);
    }
    else
        DLog("CGLTexture::Load() image [%s] returned null",filename);
    
    image = (Image *) malloc(sizeof(Image));
    if(!image) {
        DLog("CGLTexture::Load() Can't allocate resources for [%s]",maskfile);
        return 0; }
    if(!ImageLoad(maskfile, image))
    {   FREE(image);
        DLog("CGLTexture::ImageLoad() (called from) CGLTexture::Load() -> did not load:\n [%s]",maskfile);
        return 0; }
    if(image)
    {
        glDEL(mask);
        glGenTextures   (1, &mask);
        glBindTexture   (GL_TEXTURE_2D, mask);
        glTexImage2D    (GL_TEXTURE_2D, 0, 3, image->width, image->height, 0, image->type, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        FREE(image);
        DLog("Mask found: %s",maskfile);
        usemask=1;
    }
    else
        DLog("CGLTexture::Load() image [%s] returned null",maskfile);
    return 1;
}
/****************************************************************************************************/
bool CGLTexture::ReLoad(void)
{
    if(!strlen(tfilename))
    {
        DLog("CGLTexture::ReLoad() (could not produce tfilename)");
        return 0;
    }
    return (Load(tfilename,0)?0:1);
}
/****************************************************************************************************/
bool CGLTexture::ImageLoad(char *filename, Image *image)
{
    int i=strlen(filename);
    if( (filename[i-3]=='b')&&
        (filename[i-2]=='m')&&
        (filename[i-1]=='p') )
    {
        return LoadBMP(filename,image);
    }
    if( (filename[i-3]=='t')&&
        (filename[i-2]=='g')&&
        (filename[i-1]=='a') )
    {
        return LoadTGA(filename,image);
    }

    DLog("CGLTexture::ImageLoad() -> can't determine image type");

    return 0;
}
/****************************************************************************************************/
bool CGLTexture::LoadTGA(char* filename, Image *image)
{
    GLubyte     TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};    //Uncompressed TGA header
    GLubyte     TGAcompare[12];                 //Used to compare TGA header
    GLubyte     header[6];                      //The first six useful bytes from the header
    GLuint      bytesPerPixel;                  //Holds the bpp of the TGA
    GLuint      imageSize;                      //Used to store image size while in RAM
    GLuint      temp;                           //Temp variable
    image->type=GL_RGBA;
    FILE* file = fopen(filename, "rb");         // Open The TGA File
    if(file==NULL)
    {
        DLog("CGLTexture::LoadTGA() file [%s] not found",filename);
        return false;
    }
    if(fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) )
    {
        DLog("CGLTexture::LoadTGA() size not correct [%s]",filename);
        fclose(file);
        return false;
    }
    if(memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0)
    {
        DLog("CGLTexture::LoadTGA() incorrect header size [%s]",filename);
        fclose(file);
        return false;
    }
    if(fread(header,1,sizeof(header),file)!=sizeof(header))
    {
        DLog("CGLTexture::LoadTGA() can not read header info [%s]",filename);
        fclose(file);
        return false;
    }
    image->width = header[1] * 256 + header[0];     // Determine The TGA image->width   (highbyte*256+lowbyte)
    image->height= header[3] * 256 + header[2];     // Determine The TGA image->height  (highbyte*256+lowbyte)
    if( (image->width    <= 0) || (image->height   <= 0) || ((header[4] != 24) && (header[4] != 32)) )
    {
        DLog("CGLTexture::LoadTGA() [%s] (height or width < 0) or (TGA is not 24 or 32 bits)", filename);
        fclose(file);        
        return false;
    }
    image->bpp   = header[4]; bytesPerPixel= image->bpp/8;
    if(image->bpp==24) image->type=GL_RGB;
    imageSize    = image->width*image->height*bytesPerPixel;
    image->data  = (char *)new GLubyte[imageSize];
    if(image->data==NULL)
    {
        DLog("CGLTexture::LoadTGA() image data null [%s]",filename);
        fclose(file);
        return false;
    }
    if(fread(image->data, 1, imageSize, file)!=imageSize)
    {
        DLog("CGLTexture::LoadTGA() incomplete image data [%s]",filename);
        FREE(image->data);
        fclose(file);
        return false;
    }
    for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)
    {
        temp = image->data[i];
        image->data[i] = image->data[i + 2];
        image->data[i+2] = temp;
    }
    fclose (file);
    return true;
}
/****************************************************************************************************/
bool CGLTexture::LoadBMP(char* filename, Image *image)
{
    image->type=GL_RGB;
    FILE *file;
    unsigned long size;                     // size of the image in bytes.
    unsigned long i;                        // standard counter.
    unsigned short int planes;              // number of planes in image (must be 1) 
    char temp;                              // temporary color storage for bgr-rgb conversion.

    file = fopen(filename, "rb");
    if(!file)
    {
        DLog("CGLTexture::LoadBMP() file [%s] not found",filename);
        return 0;
    }
    
    fseek(file, 18, SEEK_CUR);

    if((i = fread(&image->width, 4, 1, file)) != 1)
    {
        DLog("Error reading width from %s.", filename);
        fclose(file);
        return 0;
    }
    if((i = fread(&image->height, 4, 1, file)) != 1)
    {
        DLog("Error reading height from %s.", filename);
        fclose(file);
        return 0;
    }
    size = image->width * image->height * 3;
    
    if((fread(&planes, 2, 1, file)) != 1)
    {
        DLog("Error reading planes from %s.", filename);
        fclose(file);
        return 0;
    }
    if(planes != 1)
    {
        DLog("Planes from %s is not 1: %u", filename, planes);
        fclose(file);
        return 0;
    }
    if((i = fread(&image->bpp, 2, 1, file)) != 1)
    {
        DLog("Error reading bpp from %s.", filename);
        fclose(file);
        return 0;
    }
    switch(image->bpp)
    {
           
        case 24:
            fseek(file, 24, SEEK_CUR); // seek past the rest of the bmap header.
            image->data = (char *) malloc(size); // read the data. 
            if(image->data == NULL)
            {
                DLog("Error allocating memory for color-corrected image data");
                fclose(file);
                return 0;
            }
            if((i = fread(image->data, size, 1, file)) != 1)
            {
                DLog("Error reading image data from %s.", filename);
                fclose(file);
                return 0;
            }
            for (i=0;i<size;i+=3) // reverse all of the colors. (bgr -> rgb)
            { temp = image->data[i]; image->data[i] = image->data[i+2]; image->data[i+2] = temp; }
            image->type=GL_RGB;
            fclose(file);
            return 1; // we're done.
            
        case 32:
            image->type=GL_RGBA;
     
        case 16:
        case 8:
        default:
            DLog("CGLTextureData::LoadBMP(%s) not 24 BPP it is [%d]",filename,image->bpp);
            fclose(file);
            return 0;
    }
    fclose(file);
    return true;
}
bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b)
{
    return Draw(x,y,x2,y2,r,g,b,255,255,255);
}

bool CGLTexture::Draw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2)//Draw(int x,int y,int x2,int y2,long color)
{    
    if(!bmap) { Load(tfilename,1); return 0; }
    if(usemask) if(!mask) { Load(tfilename,1); return 0; }

    int x3=(x2-x);
    int y3=(y2-y);

    x=x/2;
    y=(-y/2)+(pClientData->ScreenHeight/2);

    if(usemask)
    {
        ////////////////////////////////////////////////////////
        // draw mask

        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR,GL_ZERO);
        glDisable(GL_DEPTH_TEST);
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();  // <------------------- push matrix
        glLoadIdentity();
        gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();  // <------------------- push matrix
        glLoadIdentity();

        glTranslated(x,y,0);
        glBindTexture(GL_TEXTURE_2D,mask);
        glEnable(GL_TEXTURE_2D);
        glColor3ub(r2,g2,b2);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),      float(y-y3),    1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),   float(y-y3),    1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),   float(y),       1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),      float(y),       1);
        glEnd();

        ////////////////////////////////////////////////////////
        // draw bmap

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslated(x,y,0);
        glBindTexture(GL_TEXTURE_2D,bmap);
        glEnable(GL_TEXTURE_2D);
        glColor3ub(r,g,b);


        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),      float(y-y3),    1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),   float(y-y3),    1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),   float(y),       1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),      float(y),       1);
        glEnd();

        ////////////////////////////////////////////////////////
        // pop matrices

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glEnable(GL_DEPTH_TEST);


    }
    else
    {
        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        
        gluOrtho2D(0,pClientData->ScreenWidth,0,pClientData->ScreenHeight);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glTranslated(x,y,0);
        glBindTexture(GL_TEXTURE_2D,bmap);
        glColor3ub(r,g,b);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),      float(y-y3),    1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),   float(y-y3),    1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),   float(y),       1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),      float(y),       1);
        glEnd();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glEnable(GL_DEPTH_TEST);
    }

    return 1;
}

bool CGLTexture::DrawRaw(int x,int y,int x2,int y2,u_char r,u_char g,u_char b,u_char r2,u_char g2,u_char b2)
{
    int x3=(x2-x);
    int y3=(y2-y);

    x=x/2;
    y=(-y/2)+(pClientData->ScreenHeight/2);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(float(x),      float(y-y3),    1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(float(x+x3),   float(y-y3),    1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(float(x+x3),   float(y),       1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(float(x),      float(y),       1);
    glEnd();

    return 1;

}
