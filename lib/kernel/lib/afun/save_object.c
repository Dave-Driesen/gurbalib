void save_object(string file) {
   if (!valid(file, MODE_READ)) {
#ifdef ENABLE_STACK_SECURITY
      error("save_object: Read access to " + file + " denied");
#else
      console_msg("save_object: Read access to " + file +
         " would have been denied, ignoring...\n");
#endif
   }

   ::save_object(file);
}
