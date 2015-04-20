#ifndef _MANTRA_Pc_CHARACTER
#define _MANTRA_Pc_CHARACTER

#include "mantra_character.h"
#include "manserver.client.h"
#include "c_sqlitedb.h"

class C_GSC;
class CMPCharacter : public CMCharacter {
public:
    CMPCharacter();
    CMPCharacter(C_GSC *x);
    ~CMPCharacter();
    char username[MANTRA_TEXT_LEN];
    CMSkill *skill;

    C_GSC* pOwner;

    void load(void);
    void save(void);

    void p_char_init(void);
};


#endif
