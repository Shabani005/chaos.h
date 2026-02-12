#include <chaos.h>
#include <ctype.h>
#include <inttypes.h>

typedef enum {
  TOK_SQLB = '[',
  TOK_SQRB = ']',
} Tokens;

typedef struct {
  char *key;
  char *value;
} Ini_Pair;

typedef struct {
  char *name;
  Ini_Pair *items;
  size_t count;
  size_t capacity;
} Ini_Section;

typedef struct {
  Ini_Section *items;
  size_t count;
  size_t capacity;
} Ini;

CHAOSDEF void chaos_parse_key_value(String_Builder sb, size_t *idx, Ini *ini,size_t current_section);
CHAOSDEF bool chaos_parse_brackets(String_Builder sb, size_t *idx,String_Builder *out);
CHAOSDEF void chaos_parse_ini(String_Builder sb, Ini *ini);
CHAOSDEF char *chaos_get_ini(Ini *ini, const char *section, const char *key);
CHAOSDEF void chaos_print_ini(Ini *ini);
CHAOSDEF float chaos_get_ini_float(Ini *ini, const char *section, const char *key, float fallback);
CHAOSDEF int chaos_get_ini_int(Ini *ini, const char *section, const char *key, int fallback);

#ifndef CHAOS_ADD_PREFIX
  #define parse_ini       chaos_parse_ini
  #define get_ini         chaos_get_ini
  #define get_ini_float   chaos_get_ini_float
  #define get_ini_int     chaos_get_ini_int
  #define print_ini       chaos_print_ini
#endif

#ifdef CHAOS_IMPLEMENTATION
CHAOSDEF void chaos_parse_key_value(String_Builder sb, size_t *idx, Ini *ini,
                                    size_t current_section) {

  String_Builder key = {0};
  String_Builder value = {0};

  while (*idx < sb.count && sb.items[*idx] != '=' && sb.items[*idx] != '\n') {
    sb_appendf(&key, "%c", sb.items[*idx]);
    (*idx)++;
  }

  if (*idx >= sb.count || sb.items[*idx] != '=') {
    return;
  }

  (*idx)++;

  while (*idx < sb.count && sb.items[*idx] != '\n') {
    sb_appendf(&value, "%c", sb.items[*idx]);
    (*idx)++;
  }

  sb_append_null(&key);
  sb_append_null(&value);

  String_View ksv = sv_from_cstr(key.items);
  String_View vsv = sv_from_cstr(value.items);

  ksv = trim(&ksv);
  vsv = trim(&vsv);

  char *k = sv_to_cstr(&ksv);
  char *v = sv_to_cstr(&vsv);

  Ini_Pair pair = {.key = k, .value = v};

  da_append(&ini->items[current_section], pair);

  CHAOS_FREE(key.items);
  CHAOS_FREE(value.items);
}

CHAOSDEF bool chaos_parse_brackets(String_Builder sb, size_t *idx,
                                   String_Builder *out) {
  while (*idx < sb.count) {
    char c = sb.items[*idx];

    if (c == '\n') {
      fprintf(stderr, "Did not close bracket\n");
      return false;
    }

    if (c == TOK_SQRB) {
      (*idx)++;
      return true;
    }

    sb_appendf(out, "%c", c);
    (*idx)++;
  }

  fprintf(stderr, "Unexpected EOF while parsing bracket\n");
  return false;
}

CHAOSDEF void chaos_parse_ini(String_Builder sb, Ini *ini) {
  size_t current_section = 0;

  Ini_Section global = {0};
  global.name = sv_to_cstr(&(String_View){.data = "", .count = 0});
  da_append(ini, global);

  for (size_t i = 0; i < sb.count;) {
    char c = sb.items[i];

    if (isspace((unsigned char)c)) {
      i++;
      continue;
    }

    if (c == ';' || c == '#') {
      while (i < sb.count && sb.items[i] != '\n')
        i++;
      continue;
    }

    if (c == TOK_SQLB) {
      i++;

      String_Builder section_name = {0};

      if (!chaos_parse_brackets(sb, &i, &section_name)) {
        CHAOS_FREE(section_name.items);
        return;
      }

      sb_append_null(&section_name);

      String_View sv = sv_from_cstr(section_name.items);
      sv = trim(&sv);

      Ini_Section sec = {0};
      sec.name = sv_to_cstr(&sv);

      da_append(ini, sec);
      current_section = ini->count - 1;

      CHAOS_FREE(section_name.items);
      continue;
    }

    chaos_parse_key_value(sb, &i, ini, current_section);
    i++;
  }
}

CHAOSDEF char *chaos_get_ini(Ini *ini, const char *section, const char *key) {
  if (!ini || !section || !key)
    return NULL;

  for (size_t s = 0; s < ini->count; s++) {
    Ini_Section *sec = &ini->items[s];

    if (sec->name && strcmp(sec->name, section) == 0) {
      for (size_t p = 0; p < sec->count; p++) {
        Ini_Pair *pair = &sec->items[p];
        if (pair->key && strcmp(pair->key, key) == 0)
          return pair->value;
      }
      return NULL;
    }
  }

  return NULL;
}

CHAOSDEF void chaos_print_ini(Ini *ini) {
  for (size_t s = 0; s < ini->count; s++) {

    if (ini->items[s].name[0] == '\0' && ini->items[s].count == 0)
      continue;

    printf("[%s]\n", ini->items[s].name);

    for (size_t p = 0; p < ini->items[s].count; p++) {
      printf("%s = %s\n", ini->items[s].items[p].key,
             ini->items[s].items[p].value);
    }

    printf("\n");
  }
}

CHAOSDEF float chaos_get_ini_float(Ini *ini, const char *section, const char *key, float fallback){
  char *value = chaos_get_ini(ini, section, key);
  char *endptr;

  if (!value) return fallback;
  
  float v = strtof(value, &endptr);
  
  if (endptr != value){
    return v;
  } else return fallback;
}

CHAOSDEF int chaos_get_ini_int(Ini *ini, const char *section, const char *key, int fallback){
  char *value = chaos_get_ini(ini, section, key);
  char *endptr;

  if (!value) return fallback;

  long v = strtol(value, &endptr, 10);
  
  if (endptr != value){
    return (int)v;
  } else return fallback;
}
#endif // CHAOS_IMPLEMENTATION
