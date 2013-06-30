#ifndef _CUD_MANTRA_
#define _CUD_MANTRA_

#include "dlstorm.h"
#include <algorithm>
#include "mantra_common.h"

class C_UserData
{
public:

    C_UserData();
    ~C_UserData();

    // data to be saved

    char    session_id[MANTRA_TEXT_LEN];

	u_char  access;         // server access (0-255)
    char    username[MANTRA_TEXT_LEN];
    char    password[MANTRA_TEXT_LEN];

    bool    timeout_override;

    void    init(void);
    bool    load_userdata(void);
    bool    save_userdata(void);

    vector <string> chat_channel;
    void    leave_channel(const char *channel);
    void    join_channel(const char *channel);
};
#endif
