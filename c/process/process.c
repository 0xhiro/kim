#include "../include/buffer.h"
#include "../include/view.h"
#include <stdio.h>

void process_input(view_t *view, buffer_t *buffer) {
  int updated_view = 0;
  int content_changed = 1;

  char c;
  int pos = 0;
  char buff[1024];

  while (1) {
    if (content_changed) {
      updated_view = update_view();
      content_changed = 0;
    }

    c = getchar();
    switch (c) {
    case 'q':
      return;
    case 'w':
      write_buffer_to_file(buffer);
      break;
    case '\n':
      // buff[pos] = '\0';
      // printf("\n%s\n", buff);
      // pos = 0;
      break;
    case '\b':
      // if (pos > 0) {
      // pos--;
      // printf("\b \b");
      // }
      break;
    default:
      buff[pos] = c;
      pos++;
      putchar(c);
      break;
    }
  }

  updated_view = 0;
}
