inherit M_COMMAND;

string *usage(void) {
   string *lines;

   lines = ({ "Usage: name [-h] OBJECT NAME]" });
   lines += ({ " " });
   lines += ({ "Set a name for the given object."
      });
   lines += ({ " " });
   lines += ({ "Options:" });
   lines += ({ "\t-h\tHelp, this usage message." });
   lines += ({ "Examples:" });
   lines += ({ "\tname horse Stinky" });
   
   lines += get_alsos();

   return lines;
}

void setup_alsos() {
   add_also("wiz", "ban");
   add_also("wiz", "halt");
   add_also("wiz", "heal");
   add_also("wiz", "muzzle");
   add_also("wiz", "forcequit");
}

static void main(string str) {
   object target;
   int i, sz, perc, target_hp, target_damage;
   string who, target_name, old_name;

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

   if (sscanf(str, "%s %s", who, target_name) != 2) {
      this_player()->more(usage());
      return;
   }

   target = this_environment()->present(who);

   if (!target) {
      write("Cant find " + who + ".\n");
      return;
   }

   old_name=target->query_short();
   target->set_name(target_name);
   target->set_short(target_name);
   
   write(old_name + " is now known as " + target_name);
}
