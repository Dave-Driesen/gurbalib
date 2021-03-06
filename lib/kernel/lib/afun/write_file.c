int write_file(string file, string str, varargs int offset) {
   if (!valid(file, MODE_WRITE)) {
#ifdef ENABLE_STACK_SECURITY
      console_msg("write_file: Write access to " + file + " denied\n");
      return 0;
#else
      console_msg("write_file: Write access to " + file +
         " would have been denied, ignoring...\n");
#endif
   }

   return::write_file(file, str, offset);
}
