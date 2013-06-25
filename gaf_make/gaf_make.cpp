// GAF TOOL

#include "gaf_make.h"
#define VERSION "1.2.3.4"

int main(int argc, char *argv[])
{
    char file[1024]; memset(file,0,1024); strcpy(file,"gaf.gaf");
    char res[1024];  memset(res,0,1024);
    unsigned int i;



    // remove(file);

    if(argc<3) {
        print_help();
        return 1;
    }

    strcpy(file,argv[1]);

    CGAF *pGAF;
    pGAF=new CGAF(file,GAFCOMP_BEST);

    switch(argv[2][1]) {

        case 'a':
            for(i=3;i<strlen(argv[2]);i++) {
                res[i-3]=argv[2][i];
            }
            printf("Adding file %s to %s...\n",res,file);
            pGAF->AddFile(res,res);
            pGAF->Close();
            break;

        case 'z':
            for(i=3;i<strlen(argv[2]);i++) {
                res[i-3]=argv[2][i];
            }
            printf("Adding directory %s to %s...\n",res,file);
            pGAF->AddDir(res,res,1);
            pGAF->Close();
            break;

        case 'd':
            for(i=3;i<strlen(argv[2]);i++) {
                res[i-3]=argv[2][i];
            }
            printf("Removing file %s from %s...\n",res,file);

            pGAF->RemoveFile(res);
            pGAF->Close();
            break;

        case 'c':
            for(i=3;i<strlen(argv[2]);i++)
            {
                res[i-3]=argv[2][i];
            }
            printf("Removing directory %s from %s...\n",res,file);

            pGAF->RemoveDir(res);
            pGAF->Close();
            break;

        case 'x':
            printf("EXTRACT\n");
            break;

        case 'r':
            printf("REBUILD\n");
            break;

        default:
            printf("??\n");
            break;
    }



    DEL(pGAF);

    return 0;
}


void print_help(void)
{
    printf("\n");
    printf("GAF Game Archive File %s (c)2013 Seth Parson sethcoder.com\n",VERSION);
    printf("=======================================================\n");
    printf("USAGE:\n");
    printf("gaf <gaf file> [-a:<filename>] [-z:<dir>] [-d:<file>] [-c:<dir>] [-x] [-r]\n");
    printf("-a add a file to the GAF\n");
    printf("-z add directory to the GAF\n");
    printf("-d remove a file from the GAF\n");
    printf("-c remove a directory from the GAF\n");
    printf("-x extract GAF into the working directory\n");
    printf("-r rebuild the gaf\n");
}
