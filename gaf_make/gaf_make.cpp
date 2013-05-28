

#include "dlstorm.h"
#include "c_gaf.h"


int main(int argc, char *argv[])
{

    printf("%s\n", dlcs_getcwd());

    CGAF *pGAF;
    pGAF=new CGAF();

    printf("Rebuilding ember.gaf...\n");
    remove("ember.gaf");


    pGAF->Open("ember.gaf");


    printf("Adding base...\n");
    pGAF->AddDir("base");

    printf("Adding buttons...\n");
    pGAF->AddDir("buttons");

    printf("Adding fonts...\n");
    pGAF->AddDir("fonts");

    printf("Adding gumps...\n");
    pGAF->AddDir("gumps");

    //printf("Adding map_models...\n");	pGAF->AddDir("map_models");

    //printf("Adding misc...\n");	pGAF->AddDir("misc");

    //printf("Adding models...\n");	pGAF->AddDir("models");

    printf("Adding mouse...\n");
    pGAF->AddDir("mouse");

    //printf("Adding objects...\n");	pGAF->AddDir("objects");

    //printf("Adding map...\n");	pGAF->AddDir("map");

    //printf("Adding scripts...\n");	pGAF->AddDir("scripts");

    //printf("Adding snd...\n");
    //pGAF->AddDir("snd");

    //printf("Adding tiles...\n");
    //pGAF->AddDir("tiles");


    pGAF->Close();

    DEL(pGAF);

    printf("Created ember.gaf");

    return 0;
}
