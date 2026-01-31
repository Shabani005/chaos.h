/*
  This is an example of a program which does
  nothing except rebuilding and re-running
  itself after it is edited.
*/

/*
  #define CHAOS_ADD_PREFIX
  adds a chaos_ prefix to all functions and types
  useful when there are namespace collisions.  
*/

#define CHAOS_IMPLEMENTATION

/*
  without defining CHAOS_IMPLEMENTATION,
  chaos.h acts like a regular header file
  with no implementations (like libraries
  that require linking).
*/

#include <chaos.h>

/*
  program needs to accept argc and argv to know
  the name of the program so it can re-run itself
  after rebuilding.
*/

int main(int argc, char** argv){
  rebuild(argc, argv, __FILE__);
}
