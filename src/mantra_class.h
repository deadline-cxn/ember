#ifndef _MANTRA_CLASS
#define _MANTRA_CLASS

#include "mantra_common.h"

class CMClass{
public:
    CMClass();
    ~CMClass();
    int     id;
    char    name[MANTRA_TEXT_LEN];
};

#endif
