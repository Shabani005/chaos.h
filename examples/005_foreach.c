
#define CHAOS_IMPLEMENTATION
#include <chaos.h>

typedef struct {
  String_View* items;
  size_t count;
  size_t capacity;
} Strings;

int main(void){

  Strings ss = {0};

  da_append(&ss, sv_from_cstr("hi"));
  da_append(&ss, sv_from_cstr("from"));
  da_append(&ss, sv_from_cstr("Chaos!"));
  
  chaos_da_foreach(x, &ss){
    printf(S_FMT"\n", SV_PRINTER(*x));
  }

  return 0;
}
