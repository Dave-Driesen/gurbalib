void usage() {
  write("Usage: muzzle [-h] PLAYER\n");
  write("Muzzle a player so they can no longer shout.\n");
  write("If PLAYER is already muzzled it unmuzzles them.\n");
  write("Options:\n");
  write("\t-h\tHelp, this usage message.\n");
}

void main( string str ) {
  int val;
  object *usrs;
  object usr;

  if (!str || (str == "")) {
     write("You need to specify a player name.\n");
  } else {
     if (sscanf(str, "-%s",str)) {
        usage();
     } else if (usr = USER_D->find_user(str) ) {
	val = usr->query_player()->toggle_muzzle();
        if (val) {
	   write(capitalize(str) + " muzzled.\n");
	   usr->query_player()->message( "You have been muzzled by:" +
              capitalize(this_player()->query_name()) + "\n");
        } else {
	   write(capitalize(str) + " unmuzzled.\n");
	   usr->query_player()->message( "You have been unmuzzled by:" +
              capitalize(this_player()->query_name()) + "\n");
        }
     } else {
        write("Who are you looking for?  I can't find: " + str + "!\n");
     }
  }
}

