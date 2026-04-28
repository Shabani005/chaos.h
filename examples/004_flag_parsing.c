#define CHAOS_IMPLEMENTATION
#include <chaos.h>

void out() { printf("YOU CALLED OUT\n"); }

int main(int argc, char **argv) {
  Flag flags[] = {
      {
          .long_name = chaos_sv_from_cstr("run"),
          .short_name = chaos_sv_from_cstr("r"),
          .desc = chaos_sv_from_cstr("Run the executable"),
      },

      {.long_name = chaos_sv_from_cstr("out"),
       .short_name = chaos_sv_from_cstr("o"),
       .desc = chaos_sv_from_cstr("Output binary name"),
       .dispatcher = out},
  };


  if (!flags_parse(argc, argv, flags, ARRAY_LEN(flags))) return 1;
}
