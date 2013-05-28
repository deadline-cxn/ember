/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#include "c_serverinfo.h"
#include "dlstorm.h"

//////////////////////////////////////////////////////
CServerInfo::CServerInfo()
{
	Clear();
	Load();
}
CServerInfo::~CServerInfo()
{
	Save();
	Clear();
}

void CServerInfo::Clear()
{
	memset(name,0,256);
	memset(ip_address,0,256);
	memset(media,0,256);
	memset(resource_id,0,256);
	memset(owner,0,256);
	memset(owner_email,0,256);
	memset(owner_website,0,256);
	memset(port,0,256);
	memset(maximum_players,0,256);
	memset(players,0,256);
	memset(character_slots,0,256);
	memset(inactivity_timeout,0,256);
	memset(world_save_timer,0,256);
	memset(allow_new_users,0,256);
	memset(allow_downloads,0,256);
	memset(teleports ,0,256);
	memset(ftp_ip_address,0,256);
	memset(minimum_char_age,0,256);
	memset(start_time,0,256);
	memset(operating_system,0,256);
	memset(version,0,256);
	memset(revision,0,256);
	memset(net_revision,0,256);
	memset(locked,0,256);
	memset(user,0,256);
	memset(password,0,256);

	strcpy(name,"127.0.0.1");
	strcpy(name,"user");
	strcpy(name,"pass");
	strcpy(name,"db");
	strcpy(name,"ember_server");
	strcpy(ip_address,"localhost");
	strcpy(port,"40403");
	strcpy(media,"localhost");
	strcpy(owner,"ember admin");
	strcpy(owner_email,"seth_coder@hotmail.com");
	strcpy(owner_website,"http://www.defectiveminds.com/ember/");
	strcpy(maximum_players,"100");
	strcpy(character_slots,"8");
	strcpy(allow_new_users,"yes");
	strcpy(world_save_timer,"15");


}
void CServerInfo::Save()
{

}
void CServerInfo::Load()
{

	FILE *fp;
	char In[256];
	char *Entry;

	fp=fopen("config.ini","r");
	if(!fp) return;
	while(1)
	{
		if(!fgets(In,255,fp)) break;

		Entry = strtok(In,"=,[];");

		if(dlcs_strcasecmp(Entry,"mysql_ip"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(name,Entry); else
			strcpy(name,"127.0.0.1"); continue;
		}
		if(dlcs_strcasecmp(Entry,"mysql_name"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(name,Entry); else
			strcpy(name,"user"); continue;
		}
		if(dlcs_strcasecmp(Entry,"mysql_pass"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(name,Entry); else
			strcpy(name,"pass"); continue;
		}
		if(dlcs_strcasecmp(Entry,"mysql_db"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(name,Entry); else
			strcpy(name,"db"); continue;
		}



		if(dlcs_strcasecmp(Entry,"name"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(name,Entry); else
			strcpy(name,"ember_server"); continue;
		}
		if(dlcs_strcasecmp(Entry,"ip_address"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(ip_address,Entry); else
			strcpy(ip_address,"localhost"); continue;
		}
		if(dlcs_strcasecmp(Entry,"port"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(port,Entry); else
			strcpy(port,"40403"); continue;
		}
		if(dlcs_strcasecmp(Entry,"media"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(media,Entry); else
			strcpy(media,"localhost"); continue;
		}
		if(dlcs_strcasecmp(Entry,"owner"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(owner,Entry); else
			strcpy(owner,"ember admin"); continue;
		}
		if(dlcs_strcasecmp(Entry,"owner_email"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(owner_email,Entry); else
			strcpy(owner_email,"seth_coder@hotmail.com"); continue;
		}
		if(dlcs_strcasecmp(Entry,"owner_website"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(owner_website,Entry); else
			strcpy(owner_website,"http://www.defectiveminds.com/ember/"); continue;
		}
		if(dlcs_strcasecmp(Entry,"maximum_players"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(maximum_players,Entry); else
			strcpy(maximum_players,"100"); continue;
		}
		if(dlcs_strcasecmp(Entry,"character_slots"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(character_slots,Entry); else
			strcpy(character_slots,"8"); continue;
		}
		if(dlcs_strcasecmp(Entry,"allow_new_users"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(allow_new_users,Entry); else
			strcpy(allow_new_users,"yes"); continue;
		}
		if(dlcs_strcasecmp(Entry,"world_save_timer"))
		{
			Entry=strtok(NULL,"\n");if( Entry != NULL )
			strcpy(world_save_timer,Entry); else
			strcpy(world_save_timer,"15"); continue;
		}

/*
	resource_id

	inactivity_timeout

	allow_downloads
	teleports
	ftp_ip_address
	minimum_char_age
	start_time
	operating_system
	version
	revision
	net_revision
	locked
	user
	password
	*/


	}
	fclose(fp);

	//Log("Server information loaded...");
}
