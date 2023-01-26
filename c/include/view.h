#ifndef VIEW_H
#define VIEW_H

#include <termios.h>

typedef struct VIEW_STRUCT {
  struct termios oldt;
  struct termios newt;
  char *footer;
  char *info;
  int view_height;
  int view_width;
} view_t;

typedef enum {
  BLACK,
  GREEN,
  YELLOW,
  MAGENTA,
  CYAN,
  WHITE,
  BLUE,
  RED,
  DEFAULT
} color_t;

struct winsize get_view_size();

void window_resize_handler(int sig);

void set_color(color_t color);

void reset_background_color();

void reset_color();

void set_background_color(color_t color);

void exit_view(view_t *view);

void clear_view();

view_t *init_view();

void draw_footer();

void draw_line_numbers();

int update_view();

#endif