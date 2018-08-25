/* Created by Dave Driesen on Fri Aug  24 12:24:42 2018 */

static object *riding;

void do_ride(object ob) {
   
   if (nilp(riding)) riding = ({ });
   
   if (ob->has_passenger(this_player())) {
      write("You are already riding that.");
      return;
   }
   
   if (sizeof(riding) != 0) {
      return;
   } else {
      riding += ( { ob } );
      ob->add_passenger(this_player());
   }
}

void do_unride(object ob) {
   int i;

   if (nilp(riding)) riding = ({ });
   
   if (!ob) return;
   
   if (!ob->has_passenger(this_player())) {
      write("You are not riding that.");
      return;
   }

   /*write("\nUnriding " + ob->query_short());*/
   for (i = 0; i < sizeof(riding); i++) {
      if (riding[i] == ob) {
         riding -= ( { ob } );
         ob->remove_passenger(this_player());
      }
   }
}

void clear_mounts() {
   int i;

   if (nilp(riding)) riding = ({ });
   
   while (sizeof(riding)>0) {
      riding[0]->remove_passenger(this_player());
      riding -= ( { riding[0] } );
   }
}

object *query_riding(void) {
   
   if (nilp(riding)) riding = ({ });
   
   return riding;
}

int is_riding(void) {
   
   if (nilp(riding)) riding = ({ });

   return (sizeof(riding) != 0);
}

