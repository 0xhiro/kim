#include "../include/buffer.h"
#include "../include/view.h"
#include <stdio.h>

void process_input(view_t *view, buffer_t *buffer) {
  char c;
  int pos = 0;
  char *buff;

  while (1) {
    c = getchar();
    switch (c) {
    case 'q':
      return;
    case '\n':
      buff[pos] = '\0';
      printf("\n%s\n", buff);
      pos = 0;
      break;
    case '\b':
      if (pos > 0) {
        pos--;
        printf("\b \b");
      }
      break;
    default:
      buff[pos] = c;
      pos++;
      putchar(c);
      break;
    }
  }
}
