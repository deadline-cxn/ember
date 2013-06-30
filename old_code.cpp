/*
//u_long   GetSerialNumber(bool);
u_long CServer::GetSerialNumber(bool bSave){
    static u_long iSerialNumber=0;
    char szSerialNumber[256];
    FILE *fp;
    if(iSerialNumber==0)  // only to save the serial number the for the first time
    {
        fp=fopen("serial.dat","rt");
        if(!fp)
        {
            // not found, create a new item.id file and put a 1
            fp=fopen("serial.dat","wt");
            if(!fp)
            {
                Log("Can't create serial.dat! #1");
                return 0;
            }
            iSerialNumber=1;
            Log("Serial number set to %d",iSerialNumber);
            sprintf(szSerialNumber,"%d",iSerialNumber);
            fputs(szSerialNumber,fp);
            fputs("=================================================\n",fp);
            fputs("Serial number tracking file. DO NOT erase or modify.\n",fp);

            fclose(fp);
            return iSerialNumber;
        }
        fgets(szSerialNumber, 100, fp);
        fclose(fp);
        iSerialNumber=atoi(szSerialNumber);
    }

    if(bSave) // this is called at the end of the program to save the state of iserialnumber
    {
        fp=fopen("serial.dat","wt");
        if(!fp)
        {
            Log("Can't create serial.dat! #2");
            return 0;
        }
        sprintf(szSerialNumber,"%d\n",iSerialNumber);
        fputs(szSerialNumber,fp);
        fputs("=================================================\n",fp);
        fputs("Serial number tracking file. DO NOT erase or modify.\n",fp);
        fclose(fp);
    }

    iSerialNumber++;
    // Log("Serial number incremented to %d",iSerialNumber);
    return iSerialNumber;
}
*/
