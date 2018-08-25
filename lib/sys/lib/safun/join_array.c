string resolve_name(object obj) {
   
   string result;
   
   result=obj->query_Name();
   if (!result) result=obj->query_short();
   
   return result;
}

string join_array_human_readable(object* objects){
   
   int i;
   string object_list, entry;
   object_list="";

   if (nilp(objects)) objects = ({ });

   switch (sizeof(objects)) {
      case 0:
         return "";
         break;
      case 1: 
         object_list+=resolve_name(objects[0]);
         break;
      case 3:
         for (i = 0; i < sizeof(objects)-2; i++) {
            object_list+=resolve_name(objects[i]) + ", ";
         }
      case 2: 
         object_list+=resolve_name(objects[sizeof(objects)-2]) + " and ";
         object_list+=resolve_name(objects[sizeof(objects)-1]);
         break;
   }

   return object_list;
}
