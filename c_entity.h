#ifndef _C_ENTITY
#define _C_ENTITY

#include "c_log.h"
#include "c_gaf.h"
#include "c_gfx.h"
#include "c_gl3dmodel.h"

enum tEntityType
{
    ENTITY_STATIC,
    ENTITY_PLAYER,
    ENTITY_NPC,
    ENTITY_INTERACT,
};

class C_Entity
{
public:
    C_Entity();
    C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX);

    C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX, CGLModel *pInModel);

    ~C_Entity();

    char    szName[1024];

    int     type; // what type of entity is this

    void    Initialize(void);
    void    Draw(void);

    C_Entity *pNext;
    C_Entity *pPrev;

    CVector3 Pos;       // position of the entity
    CVector3 Dir;       // direction vector (which way the entity is facing)
    CVector3 Scale;     // vector for the scale matrix

    CGLModel *pModel;   // pointer to model data to use

    CLog    *pLog; // pointer to log
    CGAF    *pGAF; // pointer to GAF
    C_GFX   *pGFX; // pointer to GFX

};


#endif
