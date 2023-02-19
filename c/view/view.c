#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include <unistd.h>

#include "../include/process.h"
#include "../include/utils.h"

view_t *init_view() {
  clear_view();

  view_t *view = calloc(1, sizeof(struct VIEW_STRUCT));

  // disable buffering of input stream
  tcgetattr(STDIN_FILENO, &view->oldt);
  view->newt = view->oldt;
  view->newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &view->newt);

  cords_t view_size = get_view_size();
  view->view_height = view_size.row;
  view->view_width = view_size.col;

  view->footer = (void *)0;
  view->info = (void *)0;

  return view;
}

int render_view(view_t *view, buffer_t *buffer, process_t *process) {
  // kim_log("rendering view");
  clear_view();

  draw_info(view, process);

  draw_footer(view, buffer, process);

  draw_content(view, buffer);

  set_cursor(buffer->line, buffer->col);

  update_info(process, "");

  if (process->mode == NORMAL) {
    set_cursor_shape(SHAPE_BLOCK);
    set_cursor_style(STYLE_STEADY);
  } else if (process->mode == INSERT) {
    set_cursor_shape(SHAPE_BAR);
    set_cursor_style(STYLE_BLINKING);
  }

  flush_view();

  return 1;
}

int window_resized(cords_t former_size) {
  cords_t size = get_view_size();

  if (former_size.row != size.row || former_size.col != size.col) {
    return 1;
  }

  return 0;
}

void draw_info(view_t *view, process_t *process) {
  cords_t view_size = get_view_size();

  set_color(WHITE);

  put_str(view_size.row, 0, process->info);
}

void draw_footer(view_t *view, buffer_t *buffer, process_t *process) {
  cords_t view_size = get_view_size();

  set_color(BLACK);
  set_background_color(GREEN);

  for (int i = 1; i < view_size.col + 1; i++) {
    put_char(view_size.row - 1, i, ' ');
  }

  int i = 2;

  if (process->mode == NORMAL) {
    put_str(view_size.row - 1, i, "NOR");
  } else if (process->mode == INSERT) {
    put_str(view_size.row - 1, i, "INS");
  }

  i += 5;

  put_str(view_size.row - 1, i, buffer->file_path);

  i += strlen(buffer->file_path);

  i += 2;

  char line_str[10];
  sprintf(line_str, "%d", buffer->line);

  put_str(view_size.row - 1, i, line_str);

  i += strlen(line_str);

  put_char(view_size.row - 1, i, ':');

  i += 1;

  char col_str[10];
  sprintf(col_str, "%d", buffer->col);

  put_str(view_size.row - 1, i, col_str);

  i += strlen(col_str) + 5;

  char len_str[10];
  sprintf(len_str, "%lu", strlen(buffer->all_lines[buffer->line - 1]) );
  put_str(view_size.row - 1, i, "len: ");
  i += 5;
  put_str(view_size.row - 1, i, len_str);
  i += strlen(len_str);

  reset_background_color();
}

void draw_content(view_t *view, buffer_t *buffer) {
  for (int i = 0; i < buffer->lines_count; i++) {
    // draw_line_number(i + 1);

    set_color(YELLOW);
    put_str(i + 1, 1, buffer->all_lines[i]);
  }
}

void draw_line_number(int line) {
  set_color(RED);

  put_char(line, 1, 'X');
}

void put_char(int line, int col, char ch) {
  set_cursor(line, col);
  putchar(ch);
}

void put_str(int line, int col, char *str) {
  set_cursor(line, col);
  printf("%s", str);
}

void set_cursor(int line, int col) { printf("\033[%d;%dH", line, col); }

void set_cursor_shape(cursor_shape_t shape) {
  switch (shape) {
  case SHAPE_DEFAULT:
    printf("\033[0 q");
    break;
  case SHAPE_BLOCK:
    printf("\033[2 q");
    break;
  case SHAPE_UNDERLINE:
    printf("\033[4 q");
    break;
  case SHAPE_BAR:
    printf("\033[6 q");
    break;
  default:
    printf("Invalid cursor shape");
  }
}

void set_cursor_style(cursor_style_t style) {
  switch (style) {
  case STYLE_BLINKING:
    printf("\033[?12h");
    break;
  case STYLE_STEADY:
    printf("\033[?12l");
    break;
  case STYLE_INVISIBLE:
    printf("\033[?25l");
    break;
  default:
    printf("Invalid cursor style");
  }
}

cords_t get_view_size() {
  struct winsize view_size;
  ioctl(0, TIOCGWINSZ, &view_size);

  cords_t cords = {view_size.ws_col, view_size.ws_row};

  return cords;
}

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
