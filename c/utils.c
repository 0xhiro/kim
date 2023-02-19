#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "include/utils.h"
#include "include/view.h"

void write_file(char *filename, char *content) {
  FILE *file = fopen(filename, "w"); // open file for writing

  if (file == NULL) {
    printf("Error opening file!\n");
    return;
  }

  fprintf(file, "%s", content); // write contents to file
  fclose(file);                 // close the file
}

void write_lines_to_file(char *filename, char **content, int lines_count) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    kim_log("Error opening file\n");
    exit(1);
  }

  for (int i = 0; i < lines_count; i++) {
    fprintf(file, "%s", content[i]);
  }

  fclose(file);
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

void kim_error(struct termios *oldt, const char *format, ...) {
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

  tcsetattr(STDIN_FILENO, TCSANOW, oldt);

  set_color(DEFAULT); // Reset text color to default
  set_background_color(DEFAULT);

  clear_view();

  exit(1);
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

line_collection_t read_file_to_lines(char *filename) {
  // Open the file for reading
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  // Read each line of the file into the array
  char **lines = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int num_lines = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    num_lines++;
    lines = (char **)realloc(lines, num_lines * sizeof(char *));
    lines[num_lines - 1] = line;
    line = NULL;
  }

  free(line);

  // Close the file
  fclose(fp);

  line_collection_t collection = {.lines = lines, .lines_count = num_lines};

  return collection;
}

int file_has_extension(const char *file_name, const char *extension) {
  char *dot = strrchr(file_name, '.');
  if (dot && !strcmp(dot, extension)) {
    return 1;
  }
  return 0;
}
