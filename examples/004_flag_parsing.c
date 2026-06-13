#define CHAOS_IMPLEMENTATION
#include <chaos.h>

String_View SV_VSTAR(void *arg) {
  String_View *temparg = arg;
  return *temparg;
}

void out(void *arg) {
  String_View str = SV_VSTAR(arg)
 printf("flag was " S_FMT "\n", SV_PRINTER(str));
}

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

  printf("" S_FMT, SV_PRINTER(flags[1].value));
  if (!flags_parse(argc, argv, flags, ARRAY_LEN(flags)))
    return 1;
}
