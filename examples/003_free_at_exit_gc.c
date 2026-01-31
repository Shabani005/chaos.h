#define CHAOS_IMPLEMENTATION
#define CHAOS_GC
#include <chaos.h>

/*
  defining CHAOS_GC gives us a predefined main()
  function which we can use that auto-frees and provides
  a default arena (which doesnt require specifying)

  default signature:
    void* arena_alloc(arena* a, size_t size_b);

  modified signature: (implicitly passes a default arena)
    void* arena_alloc(size_t size_b);
*/

main(int argc, char** argv){
  int *x = arena_alloc(10*sizeof(int));

  for (int i=0; i<10; ++i){
    x[i] = i*i;
  } 

  for (int i=0; i<10; ++i){
    char* str = arena_sprintf("i=%d", x[i]);
    printf("%s\n", str);
  }
  return 0;
}
