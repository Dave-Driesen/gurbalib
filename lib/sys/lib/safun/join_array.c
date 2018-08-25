string join_array_human_readable(object* objects){
   
   int i;
   string object_list;
   object_list="";

   if (nilp(objects)) objects = ({ });
   
   switch (sizeof(objects)) {
      case 0:
         return "";
         break;
      case 1: 
        object_list=objects[0]->query_Name();
        break;
      case 2: 
         object_list=objects[0]->query_Name() + " and " + objects[1]->query_Name();
         break;
      case 3:
         for (i = 0; i < sizeof(objects)-2; i++) {
            object_list+=objects[i]->query_Name() + ", ";
         }
         object_list += objects[sizeof(objects)-2]->query_Name() + " and " + objects[sizeof(objects)-1]->query_Name();
   }

   return object_list;
}
