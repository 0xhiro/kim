#ifndef BUFFER_H
#define BUFFER_H

typedef struct CURSOR_STRUCT {
  int x;
  int y;
} cursor_t;

typedef struct BUFFER_STRUCT {
  char *content;
  char *file_path;
  cursor_t cursor;
} buffer_t;

buffer_t *init_buffer();

void free_buffer(buffer_t *buffer);

void write_char_to_buffer(buffer_t *buffer, char ch, int cursor);

void write_buffer_to_file(buffer_t *buffer);

void read_file_to_buffer(buffer_t *buffer, char *file_path);

#endif