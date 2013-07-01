/***************************************************************
 **      EMBER                                                **
 ***************************************************************/

#ifndef _EMBER_SERVERINFO_H
#define _EMBER_SERVERINFO_H

class CServerInfo {
public:
	CServerInfo();
	~CServerInfo();

	void	Save();
	void	Load();
	void	Clear();

    char mysql_ip            [256];
    char mysql_name          [256];
    char mysql_pass          [256];
    char mysql_db            [256];


    char name                [256];
    char ip_address          [256];
    char media               [256];
    char resource_id         [256];
    char owner               [256];
    char owner_email         [256];
    char owner_website       [256];
    char port                [256];
    char maximum_players     [256];
    char players             [256];
    char character_slots     [256];
    char inactivity_timeout  [256];
    char world_save_timer    [256];

    char allow_new_users     [256];
    char allow_downloads     [256];
    char teleports           [256];
    char ftp_ip_address      [256];
    char minimum_char_age    [256];
    char start_time          [256];
    char operating_system    [256];
    char version             [256];
    char revision            [256];
    char net_revision        [256];
    char locked              [256];
    char user                [256];
    char password            [256];

	/*
	char	cFactions;
	char	*Faction;

	char	cRaces;
	CRace	*pFirstRace;
	CRace	*pRace;

	char	cClasses;
	CClass	*pFirstClass;
	CClass	*pClass;

	char	*MoneyName;

	int		iLevelMax;
	*/

};

#endif
