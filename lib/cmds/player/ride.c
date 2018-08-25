inherit M_COMMAND;

string *usage(void) {
   string *lines;

   lines = ({ "Usage: ride [-h] THING" });
   lines += ({ "" });
   lines += ({ "Allows you to ride rideable things." });
   lines += ({ "" });
   lines += ({ "Options:" });
   lines += ({ "\t-h\tHelp, this usage message." });
   lines += ({ "Examples:" });
   lines += ({ "\tride sign" });

   lines += get_alsos();

   return lines;
}

void setup_alsos() {
   add_also("player", "dismount");
   add_also("player", "remove");
}

static int do_ride(object obj, int loud) {
   string slot;
   object *riding;

   if (!obj) {
      if (loud) {
         write("Maybe you should get one first?");
      }
      return 0;
   }

   if (!obj->is_rideable()) {
      if (loud) {
         write("You can't ride that.");
      }
      return 0;
   }
   
   if (obj->query_ridden()) {
      if (loud) {
         write("Someone is already riding that.");
      }
      return 0;
   }

   riding = this_player()->query_riding();

    if (sizeof(riding) != 0) {
        write("You are already riding something else.");
        return 0;
    }
   
   this_player()->do_ride(obj);
   this_player()->targeted_action(obj->query_ride_message(), nil, obj);
      obj->after_ride(this_player());
   return 1;
}

static void main(string str) {
   object ob;
   string *lines;

   if (!alsos) {
      setup_alsos();
   }

   if (empty_str(str)) {
      this_player()->more(usage());
      return;
   }
   if (sscanf(str, "-%s", str)) {
      this_player()->more(usage());
      return;
   }

   ob = this_environment()->present(str);

   if (ob && ob->is_rideable() && ob->can_ride(this_player())) {
      do_ride(ob,TRUE);
   } else {
      write("You can't ride that.");
   }
}
