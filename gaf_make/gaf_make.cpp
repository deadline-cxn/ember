// GAF TOOL

#define GAFMAKEVERSION "4.0.1"
#include "gaf_make.h"

CGAF *pGAF;

GAF_SCANCALLBACK dir_callback(GAFFile_ElmHeader *ElmInfo,LPSTR FullPath)
{
    switch(ElmInfo->Type)
    {
        case GAFELMTYPE_FILE:
            printf("%20d %-25s \n",ElmInfo->FileSize,ElmInfo->Name);
            break;

        case GAFELMTYPE_DIR:
            printf("====== DIR: %-25s \n",ElmInfo->Name);
            // pGAF->ScanTree(ElmInfo->Name, (GAF_SCANCALLBACK) dir_callback);
            break;

        default:
            break;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char file[1024]; memset(file,0,1024); strcpy(file,"gaf.gaf");
    char res[1024];  memset(res,0,1024);
    unsigned int i;


    pGAF=new CGAF();

    // remove(file);

    if(argc<3) {
        print_help();
        return 1;
    }

    strcpy(file,argv[1]);

    switch(argv[2][1])
    {

        case 'a':
            for(i=3;i<strlen(argv[2]);i++)
            {
                res[i-3]=argv[2][i];
            }
            printf("Adding file %s to %s...\n",res,file);
            pGAF->Open(file);
            pGAF->AddFile(res,res);
            pGAF->Close();
            break;

        case 'z':
            for(i=3;i<strlen(argv[2]);i++)
            {
                res[i-3]=argv[2][i];
            }
            printf("Adding directory %s to %s...\n",res,file);
            pGAF->Open(file);
            pGAF->AddDir(res);
            pGAF->Close();
            break;

        case 'd':
            for(i=3;i<strlen(argv[2]);i++)
            {
                res[i-3]=argv[2][i];
            }
            printf("Removing file %s from %s...\n",res,file);
            pGAF->Open(file);
            pGAF->RemoveFile(res);
            pGAF->Close();
            break;

        case 'c':
            for(i=3;i<strlen(argv[2]);i++)
            {
                res[i-3]=argv[2][i];
            }
            printf("Removing directory %s from %s...\n",res,file);
            pGAF->Open(file);
            pGAF->RemoveDir(res);
            pGAF->Close();
            break;

        case 'x':
            printf("EXTRACT\n");
            break;

        case 'r':
            printf("REBUILD\n");
            break;

        case 'l':
            printf("LIST CONTENTS\n");

            pGAF->Open(file);
            pGAF->ScanTree("", (GAF_SCANCALLBACK) dir_callback);
            pGAF->Close();




            break;

        default:
            printf("??\n");
            break;
    }



    dlcsm_delete(pGAF);

    return 0;
}


void print_help(void)
{
    printf("\n");

    printf("GAF Game Archive File by Seth Parson sethcoder.com\n");
    printf("=======================================================\n");
    printf("VERSION: %s compiled %s @ %s \n",GAFMAKEVERSION,__DATE__,__TIME__);
    printf("=======================================================\n");
    printf("USAGE:\n");
    printf("gaf <gaf file> [-a:<filename>] [-z:<dir>] [-d:<file>] [-c:<dir>] [-x] [-r]\n");
    printf("-a add a file to the GAF\n");
    printf("-z add directory to the GAF\n");
    printf("-d remove a file from the GAF\n");
    printf("-c remove a directory from the GAF\n");
    printf("-x extract GAF into the working directory\n");
    printf("-l list contents\n");
    printf("-r rebuild the gaf\n");
}
