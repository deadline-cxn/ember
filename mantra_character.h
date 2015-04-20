#ifndef _MANTRA_BASE_CHARACTER
#define _MANTRA_BASE_CHARACTER

#include "mantra_common.h"
#include "mantra_class.h"
#include "mantra_stat.h"
#include "mantra_effect.h"
#include "mantra_skill.h"

class CMCharacter {
public:
    CMCharacter();
    ~CMCharacter();
    void char_init(void);
    char name[MANTRA_TEXT_LEN];
    CMClass *m_class;
    char    gender;
    CMStat *health;
    CMStat *mana;
    CMStat *intel;
    CMStat *str;
    CMStat *spi;
    CMStat *agi;
    CMEffect *first_effect;
};

#endif
