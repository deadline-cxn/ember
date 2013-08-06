#ifndef C_VAR_H
#define C_VAR_H

#include "dlstorm.h"
#include <map>
using namespace std;

enum cv_type
{
C_VAR_NULL=0,
C_VAR_BOOL,
C_VAR_STRING,
C_VAR_CHAR,
C_VAR_UCHAR,
C_VAR_INT,
C_VAR_UINT,
C_VAR_LONG,
C_VAR_ULONG,
C_VAR_FLOAT,
};


class c_var
{
    public:
    c_var();
    ~c_var();

    void                add(const char *name, void *value);

    int                 get_cvartype(const char *s);
    const char *       get_cvartype_string(int t);
    const char *       get_cvarformatted(const char *f,void *cv);
    map <string, int>   cvar_type_map;

    char *              get_cvarformat(int t);
    map <int, string>   cvar_type_format_map;

    typedef void *strvar_t;
    typedef map<string, strvar_t> strvarmap_t;
    map <string, strvar_t>::iterator svm_i;
    strvarmap_t varmap;
};

#endif // C_VAR_H
