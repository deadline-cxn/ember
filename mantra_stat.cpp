
#include "mantra_stat.h"

CMStat::CMStat(){
    stat_init();
}
CMStat::~CMStat(){
}
void CMStat::stat_init(void){
    id=0;
    memset(name,0,MANTRA_TEXT_LEN);
    current_value=0;
    modified_value=0;
    maximum_value=0;
    next=0;
    prev=0;
}
