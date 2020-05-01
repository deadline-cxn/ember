#include "mantra_player_character.h"

CMPCharacter::CMPCharacter() {
    p_char_init();
    pOwner = 0;
}
CMPCharacter::CMPCharacter(C_GSC *x) {
    p_char_init();
    pOwner = x;
}

CMPCharacter::~CMPCharacter() { DEL(skill); }

void CMPCharacter::p_char_init(void) {
    memset(username, 0, MANTRA_TEXT_LEN);
    skill = new CMSkill;
}

void CMPCharacter::load() {  // add dlcs_cpp
    // CMEffect *     ef;
    vector<string> v1;
    vector<string> v2;

    strcpy(username, pOwner->username);

    /*
        pSQLite->db_query("SELECT * from characters where username = '%s'", username);

        strcpy(name, pSQLite->db_getvalue("username", username, ("name")).c_str());
        gender                = atoi(pSQLite->db_getvalue("username", username, ("gender")).c_str());
        health->maximum_value = atoi(pSQLite->db_getvalue("username", username, ("health_max")).c_str());
        mana->maximum_value   = atoi(pSQLite->db_getvalue("username", username, ("mana_max")).c_str());
        intel->maximum_value  = atoi(pSQLite->db_getvalue("username", username, ("int_max")).c_str());
        str->maximum_value    = atoi(pSQLite->db_getvalue("username", username, ("str_max")).c_str());
        spi->maximum_value    = atoi(pSQLite->db_getvalue("username", username, ("spi_max")).c_str());
        agi->maximum_value    = atoi(pSQLite->db_getvalue("username", username, ("agi_max")).c_str());
        m_class->id           = atoi(pSQLite->db_getvalue("username", username, ("class")).c_str());
        skill->id             = atoi(pSQLite->db_getvalue("username", username, ("skill")).c_str());

        // effects stored as EFFECT_ID,SECONDS_LEFT|EFFECT_ID,SECONDS_LEFT|...

        v1 = dlcs_explode("|", pSQLite->db_getvalue("username", username, ("effects")).c_str());
        for (unsigned int i = 0; i < v1.size(); i++) {
            v2 = dlcs_explode(",", v1[i].c_str());
            // dLog("v2 size[%d]",v2.size());
            if (v2.size() > 1) {
                ef                     = new CMEffect();
                ef->id                 = atoi(v2[0].c_str());
                ef->timer->currenttick = dlcs_get_tickcount() - atoi(v2[1].c_str()) * 1000;
            }
        }
        */
}

void CMPCharacter::save() {}
