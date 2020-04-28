
#ifndef _MANTRA_ITEM_CLASS
#define _MANTRA_ITEM_CLASS

#include "dlstorm.h"
#include "mantra_common.h"
#include <map>

#define MI_MAX_BONUS_STATS 6
#define MI_MAX_CHAKRA 3

#define MI_PROP_NONE 0
#define MI_PROP_TRADEABLE 1
#define MI_PROP_HACKABLE  2
#define MI_PROP_CONSUMED  4
#define MI_PROP_STACKABLE 8

class CMItem_DB { // the base data from which the item will be created
public:
    CMItem_DB();
    ~CMItem_DB();

    void    db_init(void);

    char    name[MANTRA_TEXT_LEN];
    char    desc[MANTRA_TEXT_LEN];
    int     icon; // pointer to base gfx
    int     prop;
    int     type;
    int     subtype;
    int     use_action; // pointer to action table
    int     own_action; // pointer to action table
    int     min_level;  // minimum level
    int     armor_base;
    int     dmg_l_base;
    int     dmg_h_base;
    int     bonus_stats; // up to 6 bonus stats, default 0
    std::map     <int, int> bonus_type;
    std::map     <int, int> bonus_value;
    int     chakra_stats;// how many chakras can be applied
    int     max_stack;
};

class CMItem : public CMItem_DB {
public:
    CMItem();
    ~CMItem();

    void    it_init(void);

    int     armor_adj;
    int     dmg_l_adj;
    int     dmg_h_adj;

    char    made_by[MANTRA_TEXT_LEN];
    int     hack; // which hack is applied to the item
    std::map <int, int> chakra;  // which chakra is applied
};

#endif
