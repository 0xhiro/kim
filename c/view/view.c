#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "../include/view.h"

void exit_view(view_t *view) {
  tcsetattr(STDIN_FILENO, TCSANOW, &view->oldt);
  printf("\033[0m"); // Reset text color to default

  free(view);

  // Clear the screen
  printf("\033c");

  printf("\033[0m"); // Reset text color to default
}

view_t *init_view() {
  printf("\033c");      // clear the terminal
  printf("\033[0;31m"); // Set text color to red

  view_t *view = calloc(1, sizeof(struct VIEW_STRUCT));

  // disable buffering of input stream
  tcgetattr(STDIN_FILENO, &view->oldt);
  view->newt = view->oldt;
  view->newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &view->newt);

  return view;
}
