#include "c_userdata.h"

C_UserData::C_UserData(){

}

C_UserData::~C_UserData(){

}

void C_UserData::init(){

    memset(username,0,32);
    memset(password,0,32);
    memset(session_id,0,64);
    timeout_override=0;
    chat_channel.clear();
}
bool C_UserData::save_userdata(void)
{
	FILE *fout;

	char Temp[256];
	char filename[_MAX_PATH];

	strcpy(filename,va("users%c%s",PATH_SEP,username));
	fout = fopen(va("%s.tmp",filename),"wt");
	if(!fout) return false;

	fputs("** Mantra Server User Data\n",fout);
	fputs(va("password=%s\n",password),fout);
	fputs(va("access=%d\n",access),fout);
	fputs(va("timeout_override=%d\n",timeout_override),fout);

	fclose(fout);

	remove(va("%s.ini",filename));
	rename(va("%s.tmp",filename),va("%s.ini",filename));
	return true;
}

bool C_UserData::load_userdata(){

	FILE *fp;
	char In[256];
	vector <string> v;
    float f;

	fp=fopen(va("users%c%s.ini",PATH_SEP,username),"rt");
	if(!fp) return false;
	while(1)
	{
		if(!fgets(In,255,fp)) break;
		v=explode("=",In);

		if(v.size()==2)
		{
            if(dscc(v[0].c_str(),"password"))           strcpy(password,v[1].c_str());
            if(dscc(v[0].c_str(),"access"))             access=atoi(v[1].c_str());
            if(dscc(v[0].c_str(),"timeout_override"))   timeout_override=atoi(v[1].c_str());
		}

    }
    fclose(fp);
    return true;
}

void C_UserData::leave_channel(const char *ch){
    chat_channel.erase( remove( chat_channel.begin(), chat_channel.end(), ch ), chat_channel.end() );
}

void C_UserData::join_channel(const char *ch){
    chat_channel.push_back(ch);
}
