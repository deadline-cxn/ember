#include "b_glerrors.h"

extern "C" char *GL_Error(GLenum err)
{
    char der[1024]; memset(der,0,1024);
    switch(err)
    {
        case GL_NO_ERROR:
            strcpy(der,"GL_NO_ERROR");
            break;
        case GL_INVALID_ENUM:
            strcpy(der,"GL_INVALID_ENUM");
            break;
        case GL_INVALID_VALUE:
            strcpy(der,"GL_INVALID_VALUE");
            break;
        case GL_INVALID_OPERATION:
            strcpy(der,"GL_INVALID_OPERATION");
            break;
        case GL_STACK_OVERFLOW:
            strcpy(der,"GL_STACK_OVERFLOW");
            break;
        case GL_STACK_UNDERFLOW:
            strcpy(der,"GL_STACK_UNDERFLOW");
            break;
        case GL_OUT_OF_MEMORY:
            strcpy(der,"GL_OUT_OF_MEMORY");
            break;
        default:
            strcpy(der,"unknown error");
            break;
    }
    return strdup(der);
}

extern "C" void GL_ClearErrors(void)
{
    GLenum err = GL_NO_ERROR;
    err=glGetError();
    while(err!=GL_NO_ERROR)
    {
        err=glGetError();
    }
}
