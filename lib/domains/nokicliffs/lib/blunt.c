#include "../domain.h"

inherit "/std/weapons/blunt";

void create(void) {
   ::create();

   set_adj("");
   set_min_damage(NOKICLIFFS_MIN_DAMAGE);
   set_max_damage(NOKICLIFFS_MAX_DAMAGE);
   set_hit_bonus(NOKICLIFFS_HIT_BONUS);
   set_blunt_skill("medium");
   set_wield_type("single");
}
