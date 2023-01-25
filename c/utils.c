#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "include/utils.h"

void write_file(char *filename, char *contents) {
  FILE *file = fopen(filename, "w"); // open file for writing

  if (file == NULL) {
    printf("Error opening file!\n");
    return;
  }

  fprintf(file, "%s", contents); // write contents to file
  fclose(file);                  // close the file
}

void kim_log(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *logs_path = "./target/kim-logs.txt";
  FILE *file = fopen(logs_path, "a"); // open file for writing

  if (file == NULL) {
    printf("Error opening file!\n");
    va_end(args);
    return;
  }

  vfprintf(file, format, args); // write contents to file
  fprintf(file, "%s", "\n");    // write newline to file

  fclose(file); // close the file
  va_end(args);
}

void dump_logs() {
  char *logs = read_file_to_string("./target/kim-logs.txt");
  puts(logs);
}

void dump_file(char *filename) {
  char *logs = read_file_to_string(filename);
  puts(logs);
}

void clear_logs() { write_file("./target/kim-logs.txt", ""); }

char *read_file_to_string(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("file error: Couldn't open file %s\n", filename);
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char *file_contents = (char *)malloc(sizeof(char) * (file_size + 1));
  if (file_contents == NULL) {
    fclose(file);
    return NULL;
  }

  fread(file_contents, sizeof(char), file_size, file);
  file_contents[file_size] = '\0';

  fclose(file);
  return file_contents;
}

int file_has_extension(const char *file_name, const char *extension) {
  char *dot = strrchr(file_name, '.');
  if (dot && !strcmp(dot, extension)) {
    return 1;
  }
  return 0;
}
