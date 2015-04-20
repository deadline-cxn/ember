#ifndef _MANTRA_SKILL
#define _MANTRA_SKILL

#include "mantra_common.h"

class CMSkill {
public:
    CMSkill();
    ~CMSkill();
    int  id;
    char name[MANTRA_TEXT_LEN];
};

#endif
