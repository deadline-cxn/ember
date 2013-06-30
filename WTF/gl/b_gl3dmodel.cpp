
#include "b_gl3dmodel.h"
#include "global.h"
#include "fm_util.h"
#include "b_cam.h"
#include "SDL.h"

extern C_FM_CAMERA *camera; 
using namespace std;

CGLModel::CGLModel()
{
    memset(name,0,64);
    memset(skin,0,1024);
    next=0;
    MD2=0;
    Model=0;
}

CGLModel::~CGLModel()
{
    DEL(MD2);
    DEL(Model);
}

bool CGLModel::Load(char *filename,char *texture)
{
    strcpy(skin,texture);
    DEL(MD2);
    DEL(Model);
    MD2=new CLoadMD2();
    Model=new t3DModel();
    if(!MD2->ImportMD2(Model,filename,texture)) return 0;
    Model->currentAnim=0;
    Model->currentFrame=0;
    
    Color(1.0f,1.0f,1.0f);
    return 1; 
}

bool CGLModel::RenderSceneDraw(void)
{
    if(!Model) return 0;
    if(!Model->pMaterials.begin()->texture.bmap) Model->pMaterials.begin()->texture.Load(skin,0);
    if(Model->pObject.size() <= 0) return 1;
    tAnimationInfo *pAnim = &(Model->pAnimations[Model->currentAnim]);
    if(Model->currentFrame>pAnim->endFrame) Model->currentFrame=pAnim->startFrame;
    int nextFrame = (Model->currentFrame + 1) % pAnim->endFrame;
    if(nextFrame == 0) nextFrame=pAnim->startFrame;
    if(Model->pObject.size() < nextFrame)
    {
        Model->currentFrame=pAnim->startFrame + 1;
        nextFrame = (Model->currentFrame + 1) % pAnim->endFrame;
    }
    t3DObject *pFrame       = &Model->pObject[Model->currentFrame];
    t3DObject *pNextFrame   = &Model->pObject[nextFrame];
    t3DObject *pFirstFrame  = &Model->pObject[0];
    float t = ReturnCurrentTime(Model, nextFrame);

    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    //glLoadIdentity();
	//gluPerspective( 80.0f, 1.333, 1.0f, 9000.0);
	//gluLookAt(Model->loc.x-100, Model->loc.y-100, Model->loc.z , Model->loc.x, Model->loc.y, Model->loc.z ,Model->loc.x-100, Model->loc.y-100, Model->loc.z-1 );
	//gluLookAt(camera->px,camera->py,camera->pz,camera->vx,camera->vy,camera->vz,camera->ux,camera->uy,camera->uz	); 
    //glTranslatef(0,0,Model->loc.z);
    //glRotatef(Model->rot.x, 1.0f , 0 ,0 );
    //glRotatef(Model->rot.y, 0, 1.0f , 0 );
    //glRotatef(Model->rot.z, 0, 0, 1.0f  );
    //glScalef(Model->scale.x, Model->scale.y, Model->scale.z);
	//glScalef(12.44,12.44,12.44);
    //glTranslatef(0.0f,0.0f,-6.0f);
	//gluLookAt(Model->loc.x,Model->loc.y,1, Model->loc.x, Model->loc.y, Model->loc.z ,  0, 1, 0 ); // WORKS!!!
	//glColor3f(Model->color.r, Model->color.g, Model->color.b);

	Log("Hi!");
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, (Model->pMaterials.begin()->texture.bmap));
    glBegin(GL_TRIANGLES);
        for(int j = 0; j < pFrame->numOfFaces; j++)
        {
            for(int whichVertex = 0; whichVertex < 3; whichVertex++)
            {
                int vertIndex = pFirstFrame->pFaces[j].vertIndex[whichVertex];
                int texIndex  = pFirstFrame->pFaces[j].coordIndex[whichVertex];
                if(pFirstFrame->pTexVerts) 
                {
                    glTexCoord2f(pFirstFrame->pTexVerts[ texIndex ].x, 
                                 pFirstFrame->pTexVerts[ texIndex ].y);
                }
                CVector3 vPoint1 = pFrame->pVerts[ vertIndex ];
                CVector3 vPoint2 = pNextFrame->pVerts[ vertIndex ];
                glVertex3f(vPoint1.x + t * (vPoint2.x - vPoint1.x), // Find the interpolated X
                           vPoint1.y + t * (vPoint2.y - vPoint1.y), // Find the interpolated Y
                           vPoint1.z + t * (vPoint2.z - vPoint1.z));// Find the interpolated Z
            }
        }
    glEnd();
    return 1;
}


bool CGLModel::Draw(void)
{

    if(!Model) return 0;
    if(!Model->pMaterials.begin()->texture.bmap) 
        Model->pMaterials.begin()->texture.Load(skin,0);
    
    if(Model->pObject.size() <= 0) return 1;

    tAnimationInfo *pAnim = &(Model->pAnimations[Model->currentAnim]);
    
    int nextFrame = (Model->currentFrame + 1) % pAnim->endFrame;
    
    if(nextFrame == 0) nextFrame=pAnim->startFrame;    
    
    if(Model->pObject.size() < nextFrame)
    {
        Model->currentFrame=pAnim->startFrame + 1;
        nextFrame = (Model->currentFrame + 1) % pAnim->endFrame;
    }
    
    t3DObject *pFrame       = &Model->pObject[Model->currentFrame];
    t3DObject *pNextFrame   = &Model->pObject[nextFrame];
    t3DObject *pFirstFrame  = &Model->pObject[0];

    float t = ReturnCurrentTime(Model, nextFrame);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);//|GL_LIGHTING);

    // glMatrixMode(GL_PROJECTION);
    // glPushMatrix();
    // glLoadIdentity();

    // gluOrtho2D(0,SCREEN_WIDTH,0,SCREEN_HEIGHT);
    
    // gluPerspective( 80.0f, 1.333, 1, 9000.0);

    // gluLookAt( Model->loc.x,Model->loc.y,Model->loc.z-360, Model->loc.x, Model->loc.y, Model->loc.z , 0, 1, 0);
    
    //gluLookAt( 0,0,-360, Model->loc.x, Model->loc.y, Model->loc.z ,  0, 2, 0 ); // WORKS!!!

    //gluLookAt( Model->loc.x,Model->loc.y,0, Model->loc.x, Model->loc.y, Model->loc.z ,  0, 2, 0 ); // WORKS!!!

    //glTranslated(camera->ox,camera->oy,camera->oz);
    //glRotatef(camera->ax,1.0f,0,0);
    //glRotatef(camera->ay,0,1.0f,0);
    //glRotatef(camera->az,0,0,1.0f);

    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    //glLoadIdentity();
    //glTranslatef( -Model->loc.x , -Model->loc.y , 260);//Model->loc.z );

    //glRotatef (Model->rot.x, 1.0f , 0 ,0 );
    //glRotatef (Model->rot.y, 0, 1.0f , 0 );
    //glRotatef (Model->rot.z, 0, 0, 1.0f  );
    //glScalef  (Model->scale.x, Model->scale.y, Model->scale.z);

    glBindTexture(GL_TEXTURE_2D, (Model->pMaterials.begin()->texture.bmap));
   
    glColor3f (Model->color.r, Model->color.g, Model->color.b);

    glBegin(GL_TRIANGLES);
        for(int j = 0; j < pFrame->numOfFaces; j++)
        {
            for(int whichVertex = 0; whichVertex < 3; whichVertex++)
            {
                int vertIndex = pFirstFrame->pFaces[j].vertIndex[whichVertex];
                int texIndex  = pFirstFrame->pFaces[j].coordIndex[whichVertex];
                if(pFirstFrame->pTexVerts) 
                {
                    glTexCoord2f(pFirstFrame->pTexVerts[ texIndex ].x, 
                                 pFirstFrame->pTexVerts[ texIndex ].y);
                }
                CVector3 vPoint1 = pFrame->pVerts[ vertIndex ];
                CVector3 vPoint2 = pNextFrame->pVerts[ vertIndex ];
                glVertex3f(vPoint1.x + t * (vPoint2.x - vPoint1.x), // Find the interpolated X
                           vPoint1.y + t * (vPoint2.y - vPoint1.y), // Find the interpolated Y
                           vPoint1.z + t * (vPoint2.z - vPoint1.z));// Find the interpolated Z
            }
        }
    glEnd();

    //glMatrixMode(GL_PROJECTION);
    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();
    
    return 1;
}

float CGLModel::ReturnCurrentTime(t3DModel *pModel, int nextFrame)
{
    float time = SDL_GetTicks();
    this->elapsedTime = time - this->lastTime;
    float t = this->elapsedTime / (1000.0f / kAnimationSpeed);
    if (elapsedTime >= (1000.0f / kAnimationSpeed) )
    {
        pModel->currentFrame = nextFrame;
        this->lastTime = time;
    }
    return t;
}

t3DObject::t3DObject()
{
    numOfVerts=0;
    numOfFaces=0;
    numTexVertex=0;
    materialID=0;
    bHasTexture=0;
    memset(strName,0,255);
    pVerts=0;
    pNormals=0;
    pTexVerts=0;
    pFaces=0;
}
t3DObject::~t3DObject()
{
}

t3DModel::t3DModel()
{
    numOfObjects=0;
    numOfMaterials=0;
    numOfAnimations=0;
    currentAnim=0;
    currentFrame=0;
    
    scale.x=1.0f;
    scale.y=1.0f;
    scale.z=1.0f;

    pAnimations.clear();
    pMaterials.clear();
    pObject.clear();

}
t3DModel::~t3DModel()
{

}
////////////////////////////////////////////////////////////////////////
CLoadMD2::CLoadMD2()
{
    memset(&m_Header, 0, sizeof(tMd2Header));
    m_pSkins=NULL;
    m_pTexCoords=NULL;
    m_pTriangles=NULL;
    m_pFrames=NULL;
}
////////////////////////////////////////////////////////////////////////
bool CLoadMD2::ImportMD2(t3DModel *MD2, char *strFileName, char *strTexture)
{
    char strMessage[255] = {0};
    m_FilePointer = fopen(strFileName, "rb");
    if(!m_FilePointer) 
    {
        // Log("Unable to find the file: %s!", strFileName);
        return 0;
    }
    fread(&m_Header, 1, sizeof(tMd2Header), m_FilePointer);
    if(m_Header.version != 8)
    {
        Log("Invalid file format (Version not 8): %s!", strFileName);
        return 0;
    }
    ReadMD2Data();
    ConvertDataStructures(MD2);
    if(strTexture)
    {
        tMaterialInfo texture;
        strcpy(texture.strFile, strTexture);
        texture.texureId = 0;
        texture.uTile = texture.uTile = 1;
        MD2->numOfMaterials = 1;
        MD2->pMaterials.push_back(texture);
    }
    CleanUp();
    //Log("md2:%s",strTexture);
    MD2->pMaterials.begin()->texture.Load(strTexture,0);
    return true;
}
//////////////////////////////////////////////////////////////////
void CLoadMD2::ReadMD2Data()
{
    unsigned char buffer[MD2_MAX_FRAMESIZE];
    m_pSkins     = new tMd2Skin [m_Header.numSkins];
    m_pTexCoords = new tMd2TexCoord [m_Header.numTexCoords];
    m_pTriangles = new tMd2Face [m_Header.numTriangles];
    m_pFrames    = new tMd2Frame [m_Header.numFrames];
    fseek(m_FilePointer, m_Header.offsetSkins, SEEK_SET);
    fread(m_pSkins,      sizeof(tMd2Skin), m_Header.numSkins, m_FilePointer);
    fseek(m_FilePointer, m_Header.offsetTexCoords, SEEK_SET);
    fread(m_pTexCoords,  sizeof(tMd2TexCoord), m_Header.numTexCoords, m_FilePointer);
    fseek(m_FilePointer, m_Header.offsetTriangles, SEEK_SET);
    fread(m_pTriangles,  sizeof(tMd2Face), m_Header.numTriangles, m_FilePointer);
    fseek(m_FilePointer, m_Header.offsetFrames, SEEK_SET);
    for (int i=0; i <    m_Header.numFrames; i++)
    {
        tMd2AliasFrame *pFrame = (tMd2AliasFrame *) buffer;
        m_pFrames[i].pVertices = new tMd2Triangle [m_Header.numVertices];
        fread(pFrame, 1, m_Header.frameSize, m_FilePointer);
        strcpy(m_pFrames[i].strName, pFrame->name);
        tMd2Triangle *pVertices = m_pFrames[i].pVertices;
        for (int j=0; j < m_Header.numVertices; j++)
        {
            pVertices[j].vertex[0] = pFrame->aliasVertices[j].vertex[0] * pFrame->scale[0] + pFrame->translate[0];
            pVertices[j].vertex[2] = -1 * (pFrame->aliasVertices[j].vertex[1] * pFrame->scale[1] + pFrame->translate[1]);
            pVertices[j].vertex[1] = pFrame->aliasVertices[j].vertex[2] * pFrame->scale[2] + pFrame->translate[2];
        }
    }
}
//////////////////////////////////////////////////////////////////
void CLoadMD2::ParseAnimations(t3DModel *MD2)
{
    tAnimationInfo animation;
    string strLastName = "";
    for(int i = 0; i < MD2->numOfObjects; i++)
    {
        string strName  = m_pFrames[i].strName;
        int frameNum = 0;
        for(int j = 0; j < strName.length(); j++)
        {
            if( isdigit(strName[j]) && j >= strName.length() - 2)
            {
                frameNum = atoi(&strName[j]);
                strName.erase(j, strName.length() - j);
                break;
            }
        }
        if( (strName != strLastName) ||
            (i == MD2->numOfObjects - 1) )
        {
            if(strLastName != "")
            {
                strcpy(animation.strName, strLastName.c_str());
                animation.endFrame = i;
                MD2->pAnimations.push_back(animation);
                memset(&animation, 0, sizeof(tAnimationInfo));
                MD2->numOfAnimations++;
            }
            animation.startFrame = frameNum - 1 + i;
        }
        strLastName = strName;
    }
}
////////////////////////////////////////////////////////////////
void CLoadMD2::ConvertDataStructures(t3DModel *MD2)
{
    int j = 0, i = 0;
    memset(MD2, 0, sizeof(t3DModel));
    MD2->numOfObjects = m_Header.numFrames;
    ParseAnimations(MD2);
    for (i=0; i < MD2->numOfObjects; i++)
    {
        t3DObject *currentFrame = new t3DObject();// {0};
        currentFrame->numOfVerts   = m_Header.numVertices;
        currentFrame->numTexVertex = m_Header.numTexCoords;
        currentFrame->numOfFaces   = m_Header.numTriangles;
        currentFrame->pVerts    = new CVector3 [currentFrame->numOfVerts];
        for (j=0; j < currentFrame->numOfVerts; j++)
        {
            currentFrame->pVerts[j].x = m_pFrames[i].pVertices[j].vertex[0];
            currentFrame->pVerts[j].y = m_pFrames[i].pVertices[j].vertex[1];
            currentFrame->pVerts[j].z = m_pFrames[i].pVertices[j].vertex[2];
        }
        delete m_pFrames[i].pVertices;
        if(i > 0)
        {
            MD2->pObject.push_back(*currentFrame);
            continue;   // Go on to the next key frame
        }
        currentFrame->pTexVerts = new CVector2 [currentFrame->numTexVertex];
        currentFrame->pFaces    = new tFace [currentFrame->numOfFaces];
        for(j=0; j < currentFrame->numTexVertex; j++)
        {
            currentFrame->pTexVerts[j].x = m_pTexCoords[j].u / float(m_Header.skinWidth);
            currentFrame->pTexVerts[j].y = 1 - m_pTexCoords[j].v / float(m_Header.skinHeight);
        }
        for(j=0; j < currentFrame->numOfFaces; j++)
        {
            currentFrame->pFaces[j].vertIndex[0]  = m_pTriangles[j].vertexIndices[0];
            currentFrame->pFaces[j].vertIndex[1]  = m_pTriangles[j].vertexIndices[1];
            currentFrame->pFaces[j].vertIndex[2]  = m_pTriangles[j].vertexIndices[2];
            currentFrame->pFaces[j].coordIndex[0] = m_pTriangles[j].textureIndices[0];
            currentFrame->pFaces[j].coordIndex[1] = m_pTriangles[j].textureIndices[1];
            currentFrame->pFaces[j].coordIndex[2] = m_pTriangles[j].textureIndices[2];
        }
        MD2->pObject.push_back(*currentFrame);
    }
}
//////////////////////////////////////////
void CLoadMD2::CleanUp()
{
    fclose(m_FilePointer);                      // Close the current file pointer
    if(m_pSkins)     delete [] m_pSkins;        // Free the skins data
    if(m_pTexCoords) delete m_pTexCoords;       // Free the texture coord data
    if(m_pTriangles) delete m_pTriangles;       // Free the triangle face data
    if(m_pFrames)    delete m_pFrames;          // Free the frames of animation
}
