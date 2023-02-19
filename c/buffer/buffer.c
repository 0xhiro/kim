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

void write_char_to_line(buffer_t *buffer, int line, char ch) {
  char *current_line = buffer->all_lines[line - 1];

  int len = strlen(current_line);

  int index = buffer->col - 1;

  kim_log("here!!");
  buffer->all_lines[line - 1] = realloc(current_line, len + 2);

  kim_log("here!!");
  memmove(current_line + index + 1, current_line + index, len - index + 1);

  current_line[index] = ch;

  buffer->col++;

  kim_log("here!!");
}

void write_str_to_line(buffer_t *buffer, int line, char *str) {
  char *current_line = buffer->all_lines[line];

  int len = strlen(current_line);
  int str_len = strlen(str);

  int index = buffer->col - 1;

  buffer->all_lines[line] = realloc(current_line, len + str_len + 1);

  memmove(current_line + index + str_len, current_line + index,
          len - index + 1);

  memcpy(current_line + index, str, str_len);

  buffer->col += str_len;
}

void remove_line_from_buffer(buffer_t *buffer, int line) {
  int index = line - 1;

  if (index < 0 || index >= buffer->lines_count) {
    // index is out of bounds, do nothing
    return;
  }

  // free the memory of the line being removed
  free(buffer->all_lines[index]);

  // shift the following lines to the left
  for (int i = index + 1; i < buffer->lines_count; i++) {
    buffer->all_lines[i - 1] = buffer->all_lines[i];
  }

  // decrease the line count
  buffer->lines_count--;
}

void delete_char_in_line(buffer_t *buffer, int line) {
  if (buffer->col > 1) {
    char *current_line = buffer->all_lines[line - 1];

    int len = strlen(current_line);

    int index = buffer->col - 2;

    memmove(current_line + index, current_line + index + 1, len - index);

    buffer->all_lines[line - 1] = realloc(current_line, len);

    buffer->col--;
  } else if (line > 1) {
    int thecol = strlen(buffer->all_lines[line - 2]);

    buffer->col = thecol;

    write_str_to_line(buffer, line - 2, buffer->all_lines[line - 1]);

    buffer->col = thecol;

    remove_line_from_buffer(buffer, line);
    buffer->line = line - 1;
  }
}

void add_line_to_buffer(buffer_t *buffer, int index) {
  if (index < 0 || index > buffer->lines_count) {
    // index is out of bounds, do nothing
    return;
  }

  // increase the size of the all_lines array
  buffer->all_lines =
      realloc(buffer->all_lines, (buffer->lines_count + 1) * sizeof(char *));

  // shift the existing lines to the right
  for (int i = buffer->lines_count; i > index; i--) {
    buffer->all_lines[i] = buffer->all_lines[i - 1];
  }

  // allocate memory for the new line
  char *new_line = malloc(2 * sizeof(char));

  // copy the line content to the new buffer
  strcpy(new_line, "\n");

  // insert the new line at the specified index
  buffer->all_lines[index] = new_line;

  // increase the line count
  buffer->lines_count++;
}

void write_newline_to_line(buffer_t *buffer) {
  // TODO: implement newlines

  add_line_to_buffer(buffer, buffer->line);

  int i = 0;
  int times = strlen(buffer->all_lines[buffer->line - 1]) - buffer->col;

  for (;;) {

    if (i >= times) {
      break;
    }

    int theline = buffer->line - 1;
    kim_log("line is %d", theline);
    write_char_to_line(buffer, theline, 'x');

    // kim_log("here!!");
    buffer->col++;
    delete_char_in_line(buffer, buffer->line);

    i++;

    kim_log("del");
  }

  buffer->line++;

  buffer->col = 1;

  kim_log("writing newline");
}

void replace_char_in_line(buffer_t *buffer, char ch) {
  buffer->all_lines[buffer->line - 1][buffer->col - 1] = ch;
}

void write_buffer_to_file(buffer_t *buffer) {
  kim_log("writing buffer to file");
  write_lines_to_file(buffer->file_path, buffer->all_lines,
                      buffer->lines_count);
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