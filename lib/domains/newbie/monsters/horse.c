#include "../domain.h"

inherit "/std/monster";
inherit "/std/modules/m_rideable";

void setup() {
   set_id("horse");
   set_race("horse");
   /* Leave out the gender specification, get a random one */
   /*  set_gender( "male" ); */
   add_adj("beatific");
   set_short("A horse");
   set_long("The grey horse, running around.");
   set_level(6);
   set_hit_skill("combat/unarmed");
   set_skill("combat/unarmed", 50);
   set_skill("combat/defense", 100);
   
   passengers = ({ });
}

