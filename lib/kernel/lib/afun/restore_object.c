int restore_object(string file) {
   if (!valid(file, MODE_READ)) {
#ifdef ENABLE_STACK_SECURITY
      error("restore_object: Read access to " + file + " denied");
#else
      console_msg("restore_object: Read access to " + file +
         " would have been denied, ignoring...\n");
#endif
   }

   return::restore_object(file);
}
