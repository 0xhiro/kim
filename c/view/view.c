#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "../include/utils.h"
#include "../include/view.h"

view_t *init_view() {
  clear_view();

  view_t *view = calloc(1, sizeof(struct VIEW_STRUCT));

  // disable buffering of input stream
  tcgetattr(STDIN_FILENO, &view->oldt);
  view->newt = view->oldt;
  view->newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &view->newt);

  view->footer = (void *)0;
  view->info = (void *)0;
  return view;
}

int update_view() {
  kim_log("updating view");
  clear_view();

  draw_footer();

  return 1;
}

void draw_footer() {
  kim_log("drawing footer");

  set_color(RED);
  set_background_color(WHITE);
  for (int i = 0; i < 20; i++) {
    putchar(' ');
  }
  reset_color();
  reset_background_color();
}

void draw_line_numbers() {}

void set_color(color_t color) {
  switch (color) {

  case BLACK:
    printf("\033[30m");
    break;

  case GREEN:
    printf("\033[32m");
    break;

  case YELLOW:
    printf("\033[33m");
    break;

  case MAGENTA:
    printf("\033[35m");
    break;

  case CYAN:
    printf("\033[36m");
    break;

  case WHITE:
    printf("\033[37m");
    break;

  case RED:
    printf("\033[31m");
    break;
  case BLUE:
    printf("\033[34m");
    break;

  case DEFAULT:
    printf("\033[0m");
    break;
  }
}

void reset_color() {
  set_color(DEFAULT);
}

void reset_background_color() {
  set_background_color(DEFAULT);
}

void set_background_color(color_t color) {
  switch (color) {

  case BLACK:
    printf("\033[40m");
    break;

  case GREEN:
    printf("\033[42m");
    break;

  case YELLOW:
    printf("\033[43m");
    break;

  case MAGENTA:
    printf("\033[45m");
    break;

  case CYAN:
    printf("\033[46m");
    break;

  case WHITE:
    printf("\033[47m");
    break;

  case RED:
    printf("\033[41m");
    break;
  case BLUE:
    printf("\033[44m");
    break;

  case DEFAULT:
    printf("\033[0m");
    break;
  }
}

void clear_view() {
  printf("\033c"); // clear the terminal
}

void exit_view(view_t *view) {
  tcsetattr(STDIN_FILENO, TCSANOW, &view->oldt);
  
  set_color(DEFAULT); // Reset text color to default
  set_background_color(DEFAULT);

  free(view);

  // Clear the screen
  clear_view();
}
