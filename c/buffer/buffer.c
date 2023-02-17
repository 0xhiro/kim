// #include "../include/buffer.h"
#include "../include/utils.h"
#include "../include/view.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

buffer_t *init_buffer() {
  buffer_t *buffer = calloc(1, sizeof(struct BUFFER_STRUCT));
  buffer->all_lines = (void *)0;
  buffer->file_path = (void *)0;
  buffer->col = 1;
  buffer->line = 1;
  return buffer;
}

void free_buffer(buffer_t *buffer) { free(buffer); }

void write_char_to_line(buffer_t *buffer, char ch) {
  buffer->all_lines[buffer->line - 1][buffer->col - 1] = ch;
}


void write_buffer_to_file(buffer_t *buffer) {
  kim_log("writing buffer to file");
  write_lines_to_file(buffer->file_path, buffer->all_lines, buffer->lines_count);
}

void read_file_to_buffer(buffer_t *buffer, char *file_path) {
  kim_log("reading file to buffer");

  line_collection_t collection = read_file_to_lines("./test/test2.txt");

  buffer->all_lines = collection.lines;
  buffer->lines_count = collection.lines_count;

  kim_log("there are %d lines", buffer->lines_count);

  buffer->file_path = (char *)malloc((strlen(file_path) + 1) * sizeof(char));
  buffer->file_path = file_path;
}

void dump_buffer(buffer_t *buffer) {
  kim_log("content: %s", buffer->all_lines);

  kim_log("cursor: %d", buffer->col);
  kim_log("line: %d", buffer->line);
}