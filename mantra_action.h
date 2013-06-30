#ifndef _MANTRA_ACTION
#define _MANTRA_ACTION

#include "mantra_common.h"

class CMAction{
public:
    CMAction();
    ~CMAction();
    int     id;
    char    name[MANTRA_TEXT_LEN];

    int     type;

};


#endif
