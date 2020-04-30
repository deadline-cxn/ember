#ifndef _MANTRA_PCHARACTER
#define _MANTRA_PCHARACTER

#include "c_sqlitedb.h"
#include "manserver.client.h"
#include "mantra_character.h"

class C_GSC;
class CMPCharacter : public CMCharacter {
   public:
    CMPCharacter();
    CMPCharacter(C_GSC *x);
    ~CMPCharacter();
    char     username[MANTRA_TEXT_LEN];
    CMSkill *skill;

    C_GSC *pOwner;

    void load(C_SQLite *pSQLite);
    void save(C_SQLite *pSQLite);

    void p_char_init(void);
};

#endif
