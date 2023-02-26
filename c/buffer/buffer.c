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

void write_char_to_line(buffer_t *buffer, int line, int col, char ch) {
  char *content = buffer->all_lines[line - 1];

  int len = strlen(content);

  int index = col - 1;

  if (index > len) {
    kim_error(&buffer->oldt, "index is greater");
  }

  buffer->all_lines[line - 1] = realloc(content, len + 2);

  content = buffer->all_lines[line - 1];

  memmove(content + index + 1, content + index, len - index + 1);

  content[index] = ch;
}

void write_str_to_line(buffer_t *buffer, int line, int col, char *str) {
  if (line > buffer->lines_count) {
    kim_error(&buffer->oldt, "line is greater");
  }

  // char *content = buffer->all_lines[line - 1];

  int i = 0;
  int j = col;

  int len = strlen(str);

  while (i < len - 1) {
    char current_char = str[i];

    write_char_to_line(buffer, line, j, current_char);

    i++;
    j++;
  }
}

void remove_line_from_buffer(buffer_t *buffer, int line) {
  int index = line - 1;

  if (index < 0 || index >= buffer->lines_count) {
    kim_error(&buffer->oldt, "cannot delete invalid line");
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

void delete_char_in_line(buffer_t *buffer, int line, int col) {
  if (line > buffer->lines_count) {
    kim_error(&buffer->oldt, "cannot delete more than existing lines");
  }

  char *current_line = buffer->all_lines[line - 1];

  int len = strlen(current_line);

  // kim_log("del col is: %d", col);
  // kim_log("del len is: %d", len);

  if (col > len) {
    kim_error(&buffer->oldt, "col is greater than len");
  }

  if (col > 1) {
    int index = col - 2;

    memmove(current_line + index, current_line + index + 1, len - index);

    buffer->all_lines[line - 1] = realloc(current_line, len);
  } else if (col == 1 && line > 1) {
    int thecol = strlen(buffer->all_lines[line - 2]);

    write_str_to_line(buffer, line - 1, thecol, buffer->all_lines[line - 1]);

    remove_line_from_buffer(buffer, line);
    buffer->line = line - 1;

    buffer->col = thecol + 1;
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

    int main_col = buffer->col;

    write_char_to_line(buffer, buffer->line + 1, i + 1,
                       buffer->all_lines[buffer->line - 1][buffer->col - 1]);

    delete_char_in_line(buffer, buffer->line, main_col + 1);

    i++;
  }

  buffer->line++;

  buffer->col = 1;
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