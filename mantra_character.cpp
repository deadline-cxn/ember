#include "mantra_character.h"

CMCharacter::CMCharacter(){
    char_init();
}

CMCharacter::~CMCharacter(){
    DEL(m_class);
    DEL(health);
    DEL(mana);
    DEL(intel);
    DEL(str);
    DEL(spi);
    DEL(agi);
}

void CMCharacter::char_init(void){
    m_class=new CMClass;
    health=new CMStat;
    mana=new CMStat;
    intel=new CMStat;
    str=new CMStat;
    spi=new CMStat;
    agi=new CMStat;
}
