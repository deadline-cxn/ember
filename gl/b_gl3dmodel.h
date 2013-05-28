
#ifndef B4_MODEL
#define B4_MODEL


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include <vector>

using namespace std;
#include "b_gltexture.h"



// These are the needed defines for the max values when loading .MD2 files

#ifdef __linux__
typedef unsigned char byte;
#endif

#define MD2_MAX_TRIANGLES       4096
#define MD2_MAX_VERTICES        2048
#define MD2_MAX_TEXCOORDS       2048
#define MD2_MAX_FRAMES          512
#define MD2_MAX_SKINS           32
#define MD2_MAX_FRAMESIZE       (MD2_MAX_VERTICES * 4 + 128)

// This is our 3D point class.  This will be used to store the vertices of our model.
class CVector3 
{
public:
    float x, y, z;
};
class CColor3
{
public:
    float r, g, b;
};

// This is our 2D point class.  This will be used to store the UV coordinates.
class CVector2 
{
public:
    float x, y;
};
struct tFace
{
    int vertIndex[3];           // indicies for the verts that make up this triangle
    int coordIndex[3];          // indicies for the tex coords to texture this face
};

class t3DObject
{
public:
    t3DObject();
    ~t3DObject();
    int  numOfVerts;            // The number of verts in the model
    int  numOfFaces;            // The number of faces in the model
    int  numTexVertex;          // The number of texture coordinates
    int  materialID;            // The texture ID to use, which is the index into our texture array
    bool bHasTexture;           // This is TRUE if there is a texture map for this object
    char strName[255];          // The name of the object
    CVector3  *pVerts;          // The object's vertices
    CVector3  *pNormals;        // The object's normals
    CVector2  *pTexVerts;       // The texture's UV coordinates
    tFace *pFaces;              // The faces information of the object
};
//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This holds our information for each animation of the Quake model.
// A STL vector list of this structure is created in our t3DModel structure below.
struct tAnimationInfo
{
    char strName[255];          // This stores the name of the animation (Jump, Pain, etc..)
    int startFrame;             // This stores the first frame number for this animation
    int endFrame;               // This stores the last frame number for this animation
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.

struct tMaterialInfo
{
    char  strName[255];         // The texture name
    char  strFile[255];         // The texture file name (If this is set it's a texture map)
    unsigned char color[3];             // The color of the object (R, G, B)
    int   texureId;             // the texture ID
    float uTile;                // u tiling of texture  (Currently not used)
    float vTile;                // v tiling of texture  (Currently not used)
    float uOffset;              // u offset of texture  (Currently not used)
    float vOffset;              // v offset of texture  (Currently not used)
    CGLTexture  texture;
} ;


// We added 4 new variables to our model structure.  These will help us handle
// the current animation.  As of now, the current animation will continue to loop
// from it's start from to it's end frame until we right click and change animations.
class t3DModel 
{
public:
    t3DModel();
    ~t3DModel();
    int numOfObjects;                   // The number of objects in the model
    int numOfMaterials;                 // The number of materials for the model
    int numOfAnimations;                // The number of animations in this model (NEW)
    int currentAnim;                    // The current index into pAnimations list (NEW)
    int currentFrame;                   // The current frame of the current animation (NEW)

    CVector3 loc;
    CVector3 rot;
    CVector3 scale;
    CColor3 color;
    bool     transparent;


    vector<tAnimationInfo>  pAnimations;// The list of animations (NEW)
    vector<tMaterialInfo>   pMaterials; // The list of material information (Textures and colors)
    vector<t3DObject>       pObject;    // The object list for our model
};


// This stores the speed of the animation between each key frame
#define kAnimationSpeed         5.0f

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


// This holds the header information that is read in at the beginning of the file
struct tMd2Header
{ 
   int magic;                   // This is used to identify the file
   int version;                 // The version number of the file (Must be 8)
   int skinWidth;               // The skin width in pixels
   int skinHeight;              // The skin height in pixels
   int frameSize;               // The size in bytes the frames are
   int numSkins;                // The number of skins associated with the model
   int numVertices;             // The number of vertices (constant for each frame)
   int numTexCoords;            // The number of texture coordinates
   int numTriangles;            // The number of faces (polygons)
   int numGlCommands;           // The number of gl commands
   int numFrames;               // The number of animation frames
   int offsetSkins;             // The offset in the file for the skin data
   int offsetTexCoords;         // The offset in the file for the texture data
   int offsetTriangles;         // The offset in the file for the face data
   int offsetFrames;            // The offset in the file for the frames data
   int offsetGlCommands;        // The offset in the file for the gl commands data
   int offsetEnd;               // The end of the file offset
};


// This is used to store the vertices that are read in for the current frame
struct tMd2AliasTriangle
{
   byte vertex[3];
   byte lightNormalIndex;
};

// This stores the normals and vertices for the frames
struct tMd2Triangle
{
   float vertex[3];
   float normal[3];
};

// This stores the indices into the vertex and texture coordinate arrays
struct tMd2Face
{
   short vertexIndices[3];
   short textureIndices[3];
};

// This stores UV coordinates
struct tMd2TexCoord
{
   short u, v;
};

// This stores the animation scale, translation and name information for a frame, plus verts
struct tMd2AliasFrame
{
   float scale[3];
   float translate[3];
   char name[16];
   tMd2AliasTriangle aliasVertices[1];
};

// This stores the frames vertices after they have been transformed
struct tMd2Frame
{
   char strName[16];
   tMd2Triangle *pVertices;
};

// This stores a skin name
typedef char tMd2Skin[64];


// This class handles all of the loading code
class CLoadMD2
{

public:
    CLoadMD2();
    ~CLoadMD2() {} ;
    bool ImportMD2(t3DModel *pModel, char *strFileName, char *strTexture);
    void ReadMD2Data();
    void ParseAnimations(t3DModel *pModel);
    void ConvertDataStructures(t3DModel *pModel);
    void CleanUp();
    FILE *m_FilePointer;

    tMd2Header              m_Header;           // The header data
    tMd2Skin                *m_pSkins;          // The skin data
    tMd2TexCoord            *m_pTexCoords;      // The texture coordinates
    tMd2Face                *m_pTriangles;      // Face index information
    tMd2Frame               *m_pFrames;         // The frames of animation (vertices)
};


class CGLModel
{
public:
    CGLModel();
    ~CGLModel();

    char name[64];

    bool Load(char *filename,char *texture);
    bool Draw(void);
    bool RenderSceneDraw(void);

    void Locate(float nx, float ny, float nz) { if(!Model) return; Model->loc.x=nx;   Model->loc.y=ny;   Model->loc.z=nz; }
    void Rotate(float nx, float ny, float nz) { if(!Model) return; Model->rot.x=nx;   Model->rot.y=ny;   Model->rot.z=nz; }
    void Scale(float nx,float ny,float nz)    { if(!Model) return; Model->scale.x=nx; Model->scale.y=ny; Model->scale.z=nz; }
    void Color(float nr,float ng,float nb)    { if(!Model) return; Model->color.r=nr; Model->color.g=ng; Model->color.b=nb; }

    float ReturnCurrentTime(t3DModel *pModel, int nextFrame);
    float elapsedTime;
    float lastTime;
    char skin[1024];

    CLoadMD2 *MD2;
    t3DModel *Model;
    CGLModel *next;
};

#endif//B4_MODEL
