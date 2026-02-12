#define CHAOS_IMPLEMENTATION
#include <chaos.h>


int main(int argc, char **argv){
  rebuild(argc, argv, __FILE__);

  cmd_arr cmd = {0};

  cmd_append(&cmd, "gcc");
  cmd_append(&cmd, "-o");
  cmd_append(&cmd, "example");
  cmd_append(&cmd, "./examples/example.c");
  cmd_run(&cmd);  
  return 0;
}
