#ifndef VIEW_H
#define VIEW_H

#include <termios.h>

typedef struct VIEW_STRUCT {
  struct termios oldt;
  struct termios newt;
  char *footer;
  char *info;
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