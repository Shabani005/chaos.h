#define CHAOS_IMPLEMENTATION
#include "chaos.h"


#define BUILD_FOLDER "./build/"

void cc_flags_run(cmd_arr *cmd, char* app){
  cmd_append(cmd, "cc");
  cmd_append(cmd, "-o");
  cmd_append(cmd, temp_sprintf("%s%s", BUILD_FOLDER, app));
  cmd_append(cmd, temp_sprintf("./examples/%s.c",app));
  cmd_append(cmd, "-L./");
  cmd_append(cmd, "-std=c99");
  cmd_run(cmd);
}

int main(int argc, char** argv){
  rebuild(argc, argv, __FILE__);

  cmd_arr cmd = {0};

  cmd_append(&cmd, "mkdir -p ./build");
  cmd_run(&cmd);
  
  cc_flags_run(&cmd, "001_simple");
  cc_flags_run(&cmd, "002_src_hello_builder");
  cc_flags_run(&cmd, "003_free_at_exit_gc");
  cc_flags_run(&cmd, "004_flag_parsing");
  return 0;
}
