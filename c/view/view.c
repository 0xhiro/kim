#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  view_cords_t view_size = get_view_size();
  view->view_height = view_size.y;
  view->view_width = view_size.x;

  view->footer = (void *)0;
  view->info = (void *)0;

  return view;
}

int render_view(view_t *view, buffer_t *buffer) {
  kim_log("rendering view");
  clear_view();

  draw_info(view, buffer);

  draw_footer(view, buffer);

  print_content(view, buffer);

  set_cursor(buffer->cursor, buffer->line);

  flush_view();

  return 1;
}

int window_resized(int former_row, int former_col) {
  view_cords_t size = get_view_size();

  if (former_row != size.x || former_col != size.y) {
    return 1;
  }

  return 0;
}

void draw_info(view_t *view, buffer_t *buffer) {
  view_cords_t view_size = get_view_size();

  set_color(WHITE);

  put_str(0, view_size.y, "some random info");

  reset_color();
  reset_background_color();
}

void put_char(int row, int col, char ch) {
  set_cursor(row, col);
  putchar(ch);
}

void put_str(int row, int col, char *str) {
  set_cursor(row, col);
  printf("%s", str);
}

void draw_footer(view_t *view, buffer_t *buffer) {
  view_cords_t view_size = get_view_size();

  set_color(BLACK);
  set_background_color(GREEN);

  for (int i = 0; i < view_size.x + 1; i++) {
    if (i == 2) {
      put_str(i, view_size.y - 1, "NOR  ");
      i += 5;

      put_str(i, view_size.y - 1, buffer->file_path);

      i += strlen(buffer->file_path) - 1;

    } else {
      put_char(i, view_size.y - 1, ' ');
    }
  }

  reset_color();
  reset_background_color();
}

void print_content(view_t *view, buffer_t *buffer) {
  set_cursor(0, 0);
  printf("%s", buffer->content);
}

void set_cursor(int x, int y) { printf("\033[%d;%dH", y, x); }

void set_cursor_shape(cursor_shape_t shape) {
  switch (shape) {
  case SHAPE_DEFAULT:
    printf("\033[0;0H");
    break;

  case SHAPE_BAR:
    printf("\033[0;3H");
    break;

  case SHAPE_BLOCK:
    printf("\033[0;1H");
    break;

  case SHAPE_UNDERLINE:
    printf("\033[0;2H");
    break;
  }
}

void set_cursor_style(cursor_style_t style) {
  switch (style) {
  case STYLE_BLINKING:
    printf("\033[2m");
    break;

  case STYLE_STEADY:
    printf("\033[0;1m");
    break;

  case STYLE_INVISIBLE:
    printf("\033[0;2m");
    break;
  }
}

view_cords_t get_view_size() {
  struct winsize view_size;
  ioctl(0, TIOCGWINSZ, &view_size);

  view_cords_t cords = {view_size.ws_col, view_size.ws_row};

  return cords;
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

void reset_color() { set_color(DEFAULT); }

void reset_background_color() { set_background_color(DEFAULT); }

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

  flush_view();
}

void flush_view() { fflush(stdout); }

void exit_view(view_t *view) {
  tcsetattr(STDIN_FILENO, TCSANOW, &view->oldt);

  set_color(DEFAULT); // Reset text color to default
  set_background_color(DEFAULT);

  free(view);

  // Clear the screen
  clear_view();
}
