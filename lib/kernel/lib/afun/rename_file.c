static int rename_file(string from, string to) {
   if (!valid(from, MODE_READ)) {
#ifdef ENABLE_STACK_SECURITY
      console_msg("rename_file: Read access to " + from + " denied\n");
      return 0;
#else
      console_msg("rename_file: Read access to " + from +
         " would have been denied, ignoring...\n");
#endif
   }

   if (!valid(to, MODE_WRITE)) {
#ifdef ENABLE_STACK_SECURITY
      console_msg("Access to " + to + " denied\n");
      return 0;
#else
      console_msg("rename_file: Write access to " + to + 
         " would have been denied, ignoring...\n");
#endif
   }

   return::rename_file(from, to);
}
