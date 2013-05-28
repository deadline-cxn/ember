/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

// Make win32 system funcs for *nix so I don't have to recode 5000 lines of code...


#include "s_gnu.h"

#ifndef _WIN32

int peek_character;
bool bKeyboardInitialized;
struct termios initial_settings, new_settings;

extern "C" long GetTickCount(void)
{
    long            curtime;
    struct timeval  tp;
    struct timezone tzp;
    static long     secbase;
       
    gettimeofday(&tp,&tzp);

	if (!secbase)
	{
		secbase = tp.tv_sec;
		return tp.tv_usec/1000;
	}

	curtime = (tp.tv_sec - secbase)*1000 + tp.tv_usec/1000;	
	return curtime;
}

extern "C" void Sleep(int ms)
{
    int ms2 = ms * 100;
    usleep(ms2);
}

extern "C" int _kbhit(void)
{
    if(!bKeyboardInitialized)
    {
        tcgetattr(0,&initial_settings);
        new_settings = initial_settings;
        new_settings.c_lflag &= ~ICANON;
        new_settings.c_lflag &= ~ECHO;
        new_settings.c_lflag &= ~ISIG;
        new_settings.c_cc[VMIN] = 1;
        new_settings.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, &new_settings);
        bKeyboardInitialized=true;
    }
    char ch;
    int nread;
    if(peek_character != -1)
    return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1)
    {
        peek_character = ch;
        return 1;
    }
    return 0;
}

extern "C" int _getch(void)
{
    char ch;
    if(peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}

extern "C" void close_keyboard(void)
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

#endif

