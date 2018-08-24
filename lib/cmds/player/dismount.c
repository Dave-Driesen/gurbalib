inherit M_COMMAND;

string *usage(void) {
   string *lines;

   lines = ({ "Usage: dismount [-h] [all|OBJ]" });
   lines += ({ "" });
   lines += ({ "Allows you to dismount a rideable object that you are  " +
      "currently riding." });
   lines += ({ "" });
   lines += ({ "Options:" });
   lines += ({ "\t-h\tHelp, this usage message." });
   lines += ({ "\tall\tStop riding all mounts." });
   lines += ({ "Examples:" });
   lines += ({ "\tdismount horse" });
   lines += ({ "\tdismount all" });

   lines += get_alsos();

   return lines;
}

void setup_alsos() {
   add_also("player", "mount");
}

static void do_dismount(object obj, int loud) {
   string slot;
   object worn;
   int wielded;


   this_player()->do_unride(obj);
   this_player()->targeted_action(obj->query_unride_message(), nil, obj);
   obj->after_unride(this_player(), "dismount");
}

static void main(string str) {
   object obj, *is_riding;
   int i, max;

   if (!alsos) {
      setup_alsos();
   }

   if (sscanf(str, "-%s", str)) {
      this_player()->more(usage());
      return;
   }

   if (this_player()->is_dead()) {
      write("You can not do that when your are not among the living.\n");
      return;
   }

   /*if (empty_str(str) || (str == "all")) {*/
      is_riding = this_player()->query_riding();
      max = sizeof(is_riding);
      for (i = 0; i < max; i++) {
         do_dismount(is_riding[i], 0);
      }
      /*return;
   }

   obj = this_player()->present(lowercase(str));
   do_dismount(obj, 1);*/
}
