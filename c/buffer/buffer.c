#include "../include/buffer.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <string.h>

buffer_t *init_buffer() {
  buffer_t *buffer = calloc(1, sizeof(struct BUFFER_STRUCT));

  return buffer;
}

void free_buffer(buffer_t *buffer) { free(buffer); }

void write_char_to_buffer(buffer_t *buffer, char ch, int cursor) {
  buffer->content[cursor] = ch;
}

void overwrite_buffer(buffer_t *buffer, char *content) {
  buffer->content = (char *)malloc((strlen(content) + 1) * sizeof(char));
}

void write_buffer_to_file(buffer_t *buffer) {
  write_file(buffer->file_path, buffer->content);
}

void read_file_to_buffer(buffer_t *buffer, char *file_path) {
  char *content = read_file_to_string(file_path);
  buffer->content = (char *)malloc((strlen(content) + 1) * sizeof(char));
  buffer->file_path = (char *)malloc((strlen(file_path) + 1) * sizeof(char));
}
