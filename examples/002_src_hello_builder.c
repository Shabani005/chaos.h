/*
  This is an example of cross-platform builder
  which can run the executable its compiling
  (while rebuilding itself if changed) 
*/


#define CHAOS_IMPLEMENTATION
#include <chaos.h>

int main(int argc, char** argv)
{
  rebuild(argc, argv, __FILE__);

  cmd_arr cmd = {0};

  #ifdef _WIN32
    cmd_append(&cmd, "cl.exe");
  #else
    cmd_append(&cmd, "gcc");
  #endif

  cmd_append(&cmd, "-o");
  cmd_append(&cmd, "hello");
  cmd_append(&cmd, "./src_hello.c");
  cmd_run(&cmd);

  if (argc == 2 && strcmp(argv[1], "run") == 0){
    #ifdef _WIN32
      cmd_append(&cmd, "hello.exe");
    #else
      cmd_append(&cmd, "./hello");
    #endif

    cmd_run(&cmd);  
  }  
}
