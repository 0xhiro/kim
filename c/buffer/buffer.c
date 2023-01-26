#include "../include/buffer.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

buffer_t *init_buffer() {
  buffer_t *buffer = calloc(1, sizeof(struct BUFFER_STRUCT));
  buffer->content = (void *)0;
  buffer->file_path = (void *)0;
  buffer->cursor = 1;
  buffer->line = 1;
  return buffer;
}

void free_buffer(buffer_t *buffer) { free(buffer); }

void write_char_to_buffer(buffer_t *buffer, char ch, int cursor) {
  int len = strlen(buffer->content);

  if (cursor > len) {
    // reallocate memory for the string
    buffer->content = (char *)realloc(buffer->content, cursor + 1);
    // set the new memory to 0
    memset(buffer->content + len, 0, cursor - len + 1);
  }
  // add the character to the string
  buffer->content[cursor] = ch;
}

void delete_char_from_buffer() {}

void overwrite_buffer(buffer_t *buffer, char *content) {
  buffer->content = (char *)malloc((strlen(content) + 1) * sizeof(char));
}

void write_buffer_to_file(buffer_t *buffer) {
  kim_log("writing buffer to file");
  write_file(buffer->file_path, buffer->content);
}

void read_file_to_buffer(buffer_t *buffer, char *file_path) {
  char *content = read_file_to_string(file_path);
  buffer->content = (char *)malloc((strlen(content) + 1) * sizeof(char));
  buffer->content = content;

  buffer->file_path = (char *)malloc((strlen(file_path) + 1) * sizeof(char));
  buffer->file_path = file_path;

  kim_log("reading file to buffer");
}

void dump_buffer(buffer_t *buffer) {
  kim_log("content: %s", buffer->content);

  kim_log("cursor: %d", buffer->cursor);
  kim_log("line: %d", buffer->line);
}