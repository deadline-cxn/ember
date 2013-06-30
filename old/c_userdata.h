#ifndef _CUD_MANTRA_
#define _CUD_MANTRA_

#include "dlstorm.h"
#include <algorithm>

class C_UserData
{
public:

    C_UserData();
    ~C_UserData();

    // data to be saved

    char    session_id[64]; // md5(username)(32) . timestamp(10) . random(100000)(6)

	u_char  access; // server access (0-255)
    char    username[32];
    char    password[32];

    bool    timeout_override;

    void    init(void);
    bool    load_userdata(void);
    bool    save_userdata(void);

    ///void    create_session_id(void);

    vector <string> chat_channel;
    void    leave_channel(const char *channel);
    void    join_channel(const char *channel);



};
#endif
