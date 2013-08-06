#include "c_var.h"
using namespace std;

c_var::c_var()
{

    varmap.clear();

    cvar_type_map.clear();
    cvar_type_map["C_VAR_NULL"]  = C_VAR_NULL;
    cvar_type_map["C_VAR_BOOL"]  = C_VAR_BOOL;
    cvar_type_map["C_VAR_STRING"]= C_VAR_STRING;
    cvar_type_map["C_VAR_CHAR"]  = C_VAR_CHAR;
    cvar_type_map["C_VAR_UCHAR"] = C_VAR_UCHAR;
    cvar_type_map["C_VAR_INT"]   = C_VAR_INT;
    cvar_type_map["C_VAR_UINT"]  = C_VAR_UINT;
    cvar_type_map["C_VAR_LONG"]  = C_VAR_LONG;
    cvar_type_map["C_VAR_ULONG"] = C_VAR_ULONG;
    cvar_type_map["C_VAR_FLOAT"] = C_VAR_FLOAT;

    cvar_type_format_map.clear();
    cvar_type_format_map[C_VAR_NULL]   = "-null-";
    cvar_type_format_map[C_VAR_BOOL]   = "%d";
    cvar_type_format_map[C_VAR_STRING] = "%s";
    cvar_type_format_map[C_VAR_CHAR]   = "%d";
    cvar_type_format_map[C_VAR_UCHAR]  = "%d";
    cvar_type_format_map[C_VAR_INT]    = "%d";
    cvar_type_format_map[C_VAR_UINT]   = "%d";
    cvar_type_format_map[C_VAR_LONG]   = "%d";
    cvar_type_format_map[C_VAR_ULONG]  = "%d";
    cvar_type_format_map[C_VAR_FLOAT]  = "%0.3f";
}

c_var::~c_var()
{

}

void c_var::add(const char *name,void *value)
{


/*
    vector <string> vs;
    int i,ivartype;
    char temp[1024]; memset(temp,0,1024);

    // ivartype=pGUI->pConsole->get_cvartype(s.c_str());

    if(ivartype==C_VAR_NULL)
    {
        //pLog->_Add("C_VAR: Can not determine cvar [%s] type",s);
        return;
    }

    //pLog->_Add("C_VAR: Type[%s]",pGUI->pConsole->get_cvartype_string(ivartype));

    string s;

    vs=explode("=",s.c_str());

    if(vs.size()>1)
    {
        for(i=0;i<vs.size();i++)
        {
            //pLog->_Add("C_VAR: exploded = vs[%d]=[%s]",i,vs[i].c_str());
        }

//        for( pGUI->pConsole->svm_i=pGUI->pConsole->varmap.begin(); pGUI->pConsole->svm_i!=pGUI->pConsole->varmap.end(); ++pGUI->pConsole->svm_i)
        {
   //         if( dscc( ((*pGUI->pConsole->svm_i).first.c_str()), vs[0].c_str()) )
//            break;
        }

        switch(ivartype)
        {
            case C_VAR_BOOL:
//                (*(bool *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case C_VAR_INT:
//                    (*(int *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case C_VAR_UINT:
//                    (*(unsigned int *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case C_VAR_CHAR:
//                    (*(char *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case C_VAR_UCHAR:
//                    (*(unsigned char *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case C_VAR_FLOAT:
//                    (*(float *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case C_VAR_LONG:
//                    (*(long *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;

            case C_VAR_ULONG:
//                    (*(unsigned long *)(*pGUI->pConsole->svm_i).second) = atoi(vs[1].c_str());
                break;


            case C_VAR_STRING:
//                    strcpy((char *)(*pGUI->pConsole->svm_i).second, vs[1].c_str());
                break;
        }

    }

//    if(pGUI->pConsole->varmap.find(s.c_str())!=pGUI->pConsole->varmap.end())
    {
        switch(ivartype)
        {
            case C_VAR_BOOL:
//                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
//                    (*(bool *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                break;

            case C_VAR_INT:
//                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
//                    (*(int *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case C_VAR_UINT:
//                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
//                    (*(unsigned int *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case C_VAR_CHAR:
//                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
//                    (*(char *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case C_VAR_UCHAR:
//                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
//                    (*(unsigned char *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case C_VAR_FLOAT:
//                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
//                    (*(float *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case C_VAR_LONG:
//                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
//                    (*(long *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case C_VAR_ULONG:
//                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
//                    (*(unsigned long *) pGUI->pConsole->varmap.find(s.c_str())->second)));
                    break;

            case C_VAR_STRING:
//                strcpy(temp, va(pGUI->pConsole->get_cvarformat(ivartype),
//                     ( pGUI->pConsole->varmap.find(s.c_str())->second)));
                break;

            default:
                strcpy(temp,"UNKNOWN");
            break;
        }
        //pLog->_Add("C_VAR: [%s]=[%s]",s.c_str(),temp);

    }
    */

}



char * c_var::get_cvarformat(int t)
{
    return (char *)cvar_type_format_map[t].c_str();
}
const char * c_var::get_cvarformatted(const char *f,void *cv)
{
    return va(f,cv);
}

const char * c_var::get_cvartype_string(int t)
{
    map <string, int>::iterator ii;
    for( ii=cvar_type_map.begin(); ii!=cvar_type_map.end(); ++ii)
    {
        if( ( (*ii).second ) == (t) )
        {
            return (*ii).first.c_str();
        }
    }
    return "null";
}

int c_var::get_cvartype(const char * s)
{
    vector <string> vt;
    vt=explode("_",s);
    if(vt.size()>1)
    {
        if(dscc(vt[0].c_str(),"b"))  return C_VAR_BOOL;
        if(dscc(vt[0].c_str(),"s"))  return C_VAR_STRING;
        if(dscc(vt[0].c_str(),"c"))  return C_VAR_CHAR;
        if(dscc(vt[0].c_str(),"uc")) return C_VAR_UCHAR;
        if(dscc(vt[0].c_str(),"i"))  return C_VAR_INT;
        if(dscc(vt[0].c_str(),"ui")) return C_VAR_UINT;
        if(dscc(vt[0].c_str(),"l"))  return C_VAR_LONG;
        if(dscc(vt[0].c_str(),"ul")) return C_VAR_ULONG;
        if(dscc(vt[0].c_str(),"f"))  return C_VAR_FLOAT;
    }
    return C_VAR_NULL;
}
/***********************************************/
