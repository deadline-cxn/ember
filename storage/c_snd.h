/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

    Must include the following libs: fmodvc dsound winmm msacm32

****************************************************************/

#ifndef _EMBER_SOUND_UTIL
#define _EMBER_SOUND_UTIL

#include "dlstorm.h"
#include "fmod.h"

#define MAX_CHANNELS 32

#ifdef _WINDOWS_
class C_Sound {
public:
    C_Sound();
    ~C_Sound();

    char *FMODVersion(void);
    char InitializeSound();
    void ShutDownSound(void);
    char PlayMusic(char* szFilename);
    void StopMusic(void);
    void StopAudio(void);
    void StopSound(void);
    void SetMusicVolume(float f);
    void SetSoundVolume(float f);
    int  PlaySample(char* szFilename);

    FMUSIC_MODULE * fmusic;

    typedef struct
            samplelist {

        FSOUND_SAMPLE *sptr;
    };

    samplelist *sample;

    unsigned char svol;
    unsigned char mvol;
    bool bfmod;
};
#endif


#endif

