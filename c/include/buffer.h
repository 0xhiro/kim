#ifndef BUFFER_H
#define BUFFER_H

typedef struct BUFFER_STRUCT {
  char **all_lines;
  int lines_count;
  
  char *file_path;
  int line;
  int col;
} buffer_t;

buffer_t *init_buffer();

void delete_char_in_line(buffer_t *buffer);

void write_newline_to_line(buffer_t *buffer);

void dump_buffer(buffer_t *buffer);

void free_buffer(buffer_t *buffer);

void write_buffer_to_file(buffer_t *buffer);

void read_file_to_buffer(buffer_t *buffer, char *file_path);

void write_char_to_line(buffer_t *buffer, char ch);

#endif