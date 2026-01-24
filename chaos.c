#ifndef CHAOS_H_
#define CHAOS_H_

#ifndef CHAOSDEF
#define CHAOSDEF
#endif

#ifndef CHAOS_REALLOC
#include <stdlib.h>
#define CHAOS_REALLOC realloc
#endif

#ifndef CHAOS_FREE
#include <stdlib.h>
#define CHAOS_FREE free
#endif

#ifndef CHAOS_ASSERT
#include <assert.h>
#define CHAOS_ASSERT assert
#endif

#define CHAOS_TODO(message) do { fprintf(stderr, "%s:%d TODO: %s\n", __FILE__, __LINE__, message); abort(); } while(0)
#define CHAOS_ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))

#ifndef CHAOS_DA_INIT_CAP
#define CHAOS_DA_INIT_CAP 256
#endif

#define chaos_da_reserve(da, expected_capacity)                                            \
    do {                                                                                   \
        if ((expected_capacity) > (da)->capacity) {                                        \
            if ((da)->capacity == 0) {                                                     \
                (da)->capacity = CHAOS_DA_INIT_CAP;                                        \
            }                                                                              \
            while ((expected_capacity) > (da)->capacity) {                                 \
                (da)->capacity *= 2;                                                       \
            }                                                                              \
            (da)->items = CHAOS_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items)); \
            CHAOS_ASSERT((da)->items != NULL && "Buy more RAM lol");                         \
        }                                                                                  \
    } while (0)

#define chaos_da_append(da, item)              \
    do {                                       \
        chaos_da_reserve((da), (da)->count + 1); \
        (da)->items[(da)->count++] = (item);   \
    } while (0)

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

typedef struct {
  char* items;
  size_t count;
  size_t capacity;
} Chaos_String_Builder;

typedef struct {
  const char *data;
  size_t count;
} Chaos_String_View;

typedef struct {
  Chaos_String_View* items;
  size_t count;
  size_t capacity;
} Chaos_cmd_arr;
/*
  ======== CONSTANTS ========
*/

#define CHAOS_TMP_BUF_SIZE 8192
#define CHAOS_TMP_BUF_COUNT 20


/*
  ======== FILE RELATED UTILITIES =========
*/

CHAOSDEF bool chaos_read_file(char* file_name, Chaos_String_Builder *sb)
{ 
  FILE *f = fopen(file_name, "rb");

  if (!f) {
    fprintf(stderr, "Cannot open file: <%s>\n", file_name);
    return false;
  }

  size_t new_count = 0;
  
  if (fseek(f, 0, SEEK_END) < 0) return false;
  
  size_t m = ftell(f);

  new_count = sb->count + m;

  if (new_count > sb->capacity){
    sb->items = CHAOS_REALLOC(sb->items, new_count * sizeof(char));
    sb->capacity = new_count;
  }
  
  fseek(f, 0, SEEK_SET);
  fread(sb->items + sb->count, 1, m, f);

  if (fclose(f) != 0) {
    fprintf(stderr, "Cannot close file <%s>\n", file_name);
    return false;
  }
  
  sb->count = new_count;
  return true;
}


CHAOSDEF bool chaos_write_file(char* file_name, Chaos_String_Builder *sb)
{ 
  FILE *f = fopen(file_name, "wb");

  if (!f) {
    fprintf(stderr, "Cannot open file <%s>\n", file_name);
    return false;
  }
  
  size_t written = fwrite(sb->items, 1, sb->count, f);
  if (written != sb->count){
    fprintf(stderr, "Failed to write file <%s>\n", file_name);
    return false;
  }

  if (fclose(f) != 0){
    fprintf(stderr, "Cannot close file <%s>\n", file_name);
    return false;
  }
  return true;
}

CHAOSDEF bool chaos_does_file_exist(char *filename){
  if (access(filename, F_OK) == 0) return true;
  return false;
}

// CHAOSDEF bool chaos_did_file_change(char *filename){
//   char* old_file = chaos_temp_sprintf("%s.old", filename);
//   if (chaos_does_file_exist(old_file)) return true;
//   return false;
// }

/*
  ======== STRING RELATED UTILITIES =========
*/

CHAOSDEF char* chaos_temp_sprintf(const char *fmt, ...) {
    static char bufs[CHAOS_TMP_BUF_COUNT][CHAOS_TMP_BUF_SIZE];
    static size_t idx = 0;

    char *out = bufs[idx++ % CHAOS_TMP_BUF_COUNT];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(out, CHAOS_TMP_BUF_SIZE, fmt, ap);
    va_end(ap);

    return out;
}

CHAOSDEF Chaos_String_View chaos_sv_from_parts(const char* data, size_t count){
  Chaos_String_View sv;
  sv.data = data;
  sv.count = count;
  return sv;
}


CHAOSDEF Chaos_String_View chaos_split_by_delim(Chaos_String_View *sv, char delim){
  size_t i = 0;

  while (i < sv->count && sv->data[i] != delim){
    i++;
  }
  
  Chaos_String_View result = chaos_sv_from_parts(sv->data, i);

  if (i < sv->count){
    sv->count -= i+1;
    sv->data  += i+1;
  } else {
    sv->count -= i;
    sv->data  += i;
  }

  return result;
}

CHAOSDEF Chaos_String_View chaos_trim_left(Chaos_String_View *sv){
  Chaos_String_View result = {0};

  result.data = sv->data;
  result.count = sv->count;

  size_t i=0;

  while (i < result.count && result.data[i] == ' '){
    i++;
  }

  result.count -=i;
  result.data +=i;

  return result;
}

CHAOSDEF Chaos_String_View chaos_trim_right(Chaos_String_View *sv) {
  Chaos_String_View result = {0};

  result.data  = sv->data;
  result.count = sv->count;

  size_t i = result.count;

  while (i > 0 && result.data[i - 1] == ' ') {
    --i;
  }

  result.count = i;

  return result;
}

CHAOSDEF Chaos_String_View chaos_trim(Chaos_String_View *sv){
  Chaos_String_View tmp = chaos_trim_left(sv);
  return chaos_trim_right(&tmp);
}

CHAOSDEF void chaos_sb_append_null(Chaos_String_Builder *sb){
  chaos_da_append(sb, '\0');  
}

CHAOSDEF void chaos_sb_append_cstr(Chaos_String_Builder *sb, char* s){
  while (*s){
    chaos_da_append(sb, *s++);
  }
}

CHAOSDEF void chaos_sb_appendf(Chaos_String_Builder *sb, const char* fmt, ...){
  CHAOS_TODO("NOT IMPLEMENTED");
}
/*
  ================= Build System Utils ===============
*/
CHAOSDEF void chaos_cmd_append(Chaos_cmd_arr *arr, char* value){
  size_t len = strlen(value);

  Chaos_String_View sv = {0};
  sv.data = value;
  sv.count = len;
  
  chaos_da_append(arr, sv);
}


CHAOSDEF bool chaos_cmd_run(Chaos_cmd_arr *arr) {
  #if !defined(__GNUC__) || defined(__clang__)
    fprintf(stderr, "cmd_run does not support windows for now\n");
    return false;
  #endif

  if (arr->count < 1) {
    fprintf(stderr, "USAGE: provide more parameters\n");
    return false;
  }

  size_t total_len = 0;
    
  for (size_t i = 0; i < arr->count; i++) {
    total_len += arr->items[i].count;
    if (i + 1 < arr->count) total_len += 1;
  }

  char *cmd = malloc(total_len + 1);

  if (!cmd) {
    fprintf(stderr, "Allocation failed in cmd_run\n");
    return false;
  }

  size_t pos = 0;

  for (size_t i = 0; i < arr->count; i++) {
    const char *s = arr->items[i].data;
    size_t n = arr->items[i].count;

    for (size_t j=0; j<n; ++j){
      cmd[pos++] = s[j];
    }
    
    if (i+1 < arr->count) cmd[pos++] = ' ';
  }

  cmd[pos] = '\0';

  printf("[CMD] %s\n", cmd);

  int ret = system(cmd);
  if (ret == -1) perror("system");
  free(cmd);

  return ret == 0; 
}

// void nb_rebuild(int argc, char **argv){
//   char *filename = "builder.c";
//   char* cloned_file = chaos_temp_sprintf("%s.old", filename);

//   if (chaos_does_file_exist(cloned_file)){
//     // printf("%s does exist\n", cloned_file);
//     if (chaos_did_file_change(filename)){
//       printf("[Rebuilding]\n");
//       nb_copy_file(filename, cloned_file);

//       Chaos_cmd_arr cmd = {0};
//       char *fname = chaos_temp_sprintf("%s.old", filename);

//       chaos_cmd_append(&cmd, "gcc");
//       chaos_cmd_append(&cmd, "-o");
//       chaos_cmd_append(&cmd, fname);
//       chaos_cmd_append(&cmd, filename);
      
//       chaos_cmd_run(&cmd);

//       printf("[INFO] rebuilt %s\n\n", filename);

//       for (size_t i=0; i<argc; ++i){
//         chaos_cmd_append(&cmd, argv[i]);
//       }
//       chaos_cmd_run((&cmd);
//             exit(1);

//   } else {
//     // printf("file did not change\n");
//     }
//   }else{
//     nb_copy_file(filename, cloned_file);
//   }
// }

/*
  =================== MISC Utilities ===================
*/

CHAOSDEF bool chaos_is_float(char* v){
  size_t len = strlen(v);
  size_t dots = 0;
  size_t sign = 0;
  
  if (len==0) return false;

  if (v[0] == '.' || v[len-1] == '.') return false;

  if (v[0] == '-' || v[0] == '+') sign++;
  
  for (size_t i=0; i<len; ++i){
    if (dots > 1 || sign > 1) return false;
    if (v[i] == '.') {
      dots++;
      continue;
    } else if (v[i] == '-' || v[i] == '+') {
      sign++;
      continue;
    }
    if ((unsigned char)v[i] < '0' || (unsigned char)v[i] > '9') return false;
  }
  return true;
}

CHAOSDEF bool chaos_is_int(char* v){
  size_t len = strlen(v);
  size_t sign = 0;

  if (len==0) return false;

  if (v[0] == '+' || v[0] == '-') sign++;
  
  for (size_t i=0; i<len; ++i){
    if (sign > 1) return false;
    if ((unsigned char)v[i] < '0' || (unsigned char)v[i] > '9') return false;
    else if (v[0] == '+' || v[0] == '-') sign++;
  }
  return true;
}
#endif // CHAOS_H_
 
