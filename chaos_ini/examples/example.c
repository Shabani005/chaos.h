#define CHAOS_IMPLEMENTATION
#include <chaos_ini.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file.ini>\n", argv[0]);
    return 1;
  }

  String_Builder sb = {0};
  read_file(argv[1], &sb);

  Ini ini = {0};

  parse_ini(sb, &ini);

  float FOV = get_ini_float(&ini, "Settings", "FOV", 80);
  printf("%f\n", FOV);

  return 0;
}

