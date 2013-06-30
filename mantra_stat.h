#ifndef _MANTRA_STAT
#define _MANTRA_STAT

#include "dlstorm.h"
#include "mantra_common.h"

class CMStat{
public:
    CMStat();
    ~CMStat();

    void stat_init(void);

    int     id;
    char    name[MANTRA_TEXT_LEN];
    int     current_value;
    int     modified_value;
    int     maximum_value;

    CMStat *next;
    CMStat *prev;

};

#endif
