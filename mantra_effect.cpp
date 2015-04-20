
#include "mantra_effect.h"

CMEffect::CMEffect(){
    next=0;
    prev=0;
    timer=0;
    ef_init();
}

CMEffect::~CMEffect(){
}

void CMEffect::ef_init(void){
    id=0;
    memset(name,0,MANTRA_TEXT_LEN);
    type=0;
    icon=0;
    duration=0;
    timer=0;
    stat_list=0;
}
