#include "mantra_item.h"

CMItem_DB::CMItem_DB(){
}
CMItem_DB::~CMItem_DB(){
}
void CMItem_DB::db_init(){
    memset(name,0,MANTRA_TEXT_LEN);
    memset(desc,0,MANTRA_TEXT_LEN);
    icon=0;
    prop=0;
    type=0;
    subtype=0;
    use_action=0;
    own_action=0;
    min_level=0;
    armor_base=0;
    dmg_l_base=0;
    dmg_h_base=0;
    bonus_stats=0;
    bonus_type.clear();
    bonus_value.clear();
    chakra_stats=0;
    max_stack=0;
}
///////////////////////////////////////
CMItem::CMItem(){
}
CMItem::~CMItem(){
}
void CMItem::it_init(){
}


