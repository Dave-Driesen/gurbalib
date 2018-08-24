/* Created by Dave Driesen */

static string rideable_text;

static object *passengers;
string ride_message;
string unride_message;

int has_passenger(object m) {
   int i;
   
   if (nilp(passengers)) passengers = ({ });
   
   if (m) {
      for (i = 0; i < sizeof(passengers); i++) {
         if (passengers[i] == m) {
            return TRUE;
         }
      }
    }
   
   return FALSE;
}

void add_passenger(object m) {
   
   if (m) {
      if (has_passenger(m)) {
         return;
      }
      passengers += ( { m } );
    }
}

void remove_passenger(object m) {
   int i;
   
   if (nilp(passengers)) passengers = ({ });
   
   if (m) {
      for (i = 0; i < sizeof(passengers); i++) {
         if (passengers[i] == m) {
            passengers -= ( { m } );
            return;
         }
      }
      /* object wqas not a passnger of this vehicle */
    }
}

int is_rideable(void) {
   return TRUE;
}

int can_ride(object who) {
   return TRUE;
}


void set_ride_message(string str) {
   ride_message = str;
}

string query_ride_message(void) {
   if (!ride_message) {
      ride_message = "$N $vride $o.";
   }
   return ride_message;
}

void set_unride_message(string str) {
   unride_message = str;
}

string query_unride_message(void) {
   if (!unride_message) {
      unride_message = "$N $vstop riding $o.";
   }
   return unride_message;
}

object* get_passengers() {

   if (nilp(passengers)) passengers = ({ });
   
   return passengers;
}

string* query_passengers(void) {
   int i;
   
   string* result;
   
   if (nilp(passengers)) passengers = ({ });
   
   for (i = 0; i < sizeof(passengers); i++) {
      result += passengers[i]->query_Name();
   }
   return result;
}

string query_passengers_human_readable(){
   
   int i;
   string passenger_list;
   passenger_list="";

   if (nilp(passengers)) passengers = ({ });
   
   switch (sizeof(passengers)) {
      case 0:
         return "";
         break;
      case 1: 
        passenger_list=passengers[0]->query_Name();
        break;
      case 2: 
         passenger_list=passengers[0]->query_Name() + " and " + passengers[1]->query_Name();
         break;
      case 3:
         for (i = 0; i < sizeof(passengers)-2; i++) {
            passenger_list+=passengers[i]->query_Name() + ", ";
         }
         passenger_list += passengers[sizeof(passengers)-2]->query_Name() + " and " + passengers[sizeof(passengers)-1]->query_Name();
   }

   return passenger_list;
}

int query_passenger_count(void) {
   
   if (nilp(passengers)) passengers = ({ });
   
   return sizeof(passengers);
}

int has_passengers(void) {
   
   write ("passenger count = " + query_passenger_count());
   return (query_passenger_count()>0);
}

int do_be_ridden(object who) {

   add_passenger(who);
   return 1;
}

void clear_passengers(void) {
    
   if (nilp(passengers)) passengers = ({ });
   
   while (sizeof(passengers)>0) {
      passengers[0]->do_unride(this_object());
      passengers[0]->targeted_action(query_unride_message(), nil, this_object());
      passengers[0]->after_unride(passengers[0], "dismount");
      passengers -= ( { passengers[0] } );
   }
}
