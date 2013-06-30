#ifndef _MANTRA_NPC_CHARACTER
#define _MANTRA_NPC_CHARACTER

#include "mantra_common.h"
#include "mantra_skill.h"
#include "mantra_character.h"
#include "c_log.h"

class CM_NPC : public CMCharacter
{
public:
    CM_NPC();
    ~CM_NPC();

    virtual void load(void);





    int loot_table;


    void npc_char_init(void);


};


#endif
