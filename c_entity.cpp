
#include "c_entity.h"

C_Entity::C_Entity()
{
    Initialize();
}
C_Entity::C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX)
{
    Initialize();
    pLog=pInLog;
    pGAF=pInGAF;
    pGFX=pInGFX;
}

C_Entity::C_Entity(CLog *pInLog, CGAF *pInGAF, C_GFX *pInGFX, CGLModel *pInModel)
{
    Initialize();
    pLog=pInLog;
    pGAF=pInGAF;
    pGFX=pInGFX;
    pModel=pInModel;
}


void C_Entity::Initialize(void)
{
    memset(szName,0,1024);
    type=ENTITY_STATIC;    /*       ENTITY_STATIC,
                                    ENTITY_PLAYER,
                                    ENTITY_NPC,
                                    ENTITY_INTERACT,        */
    pNext=0;
    pPrev=0;
    pLog=0;
    pGAF=0;
    pGFX=0;
    pModel=0;
}

C_Entity::~C_Entity()
{
}

void C_Entity::Draw(void)
{
}

/*  CVector3 Pos;       // position of the entity
    CVector3 Dir;       // direction vector (which way the entity is facing)
    CVector3 Scale;     // vector for the scale matrix

    CGLModel *pModel;   // pointer to model data to use
    CLog    *pLog; // pointer to log
    CGAF    *pGAF; // pointer to GAF
    C_GFX   *pGFX; // pointer to GFX    */

