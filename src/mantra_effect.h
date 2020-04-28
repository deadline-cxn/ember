#ifndef _MANTRA_EFFECT
#define _MANTRA_EFFECT

#include "c_timer.h"
#include "mantra_common.h"
#include "mantra_stat.h"

class CMEffect{
public:
    CMEffect();
    ~CMEffect();

    void    ef_init();

    int     id;
    char    name[MANTRA_TEXT_LEN];
    int     icon;

    int     type;

    CMStat  *stat_list; // list of affected stats

    long    duration;
    CTimer  *timer;

    CMEffect *next;
    CMEffect *prev;
};

#endif
