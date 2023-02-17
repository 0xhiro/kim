#ifndef VIEW_H
#define VIEW_H

#include "buffer.h"
#include <termios.h>

typedef struct CURSOR_STRUCT {
  int col;
  int row;
} cords_t;

typedef struct LINE_COLLECTION {
  char** lines;
  int lines_count;
} line_collection_t;

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

typedef enum {
  SHAPE_DEFAULT,
  SHAPE_BLOCK,
  SHAPE_UNDERLINE,
  SHAPE_BAR
} cursor_shape_t;

typedef enum { STYLE_BLINKING, STYLE_STEADY, STYLE_INVISIBLE } cursor_style_t;



extern void kimview();

void put_char(int row, int col, char ch);

cords_t get_view_size();

int window_resized(cords_t former_size);

void draw_content(view_t *view, buffer_t *buffer);

void set_color(color_t color);

void reset_background_color();

void reset_color();

void set_background_color(color_t color);

void exit_view(view_t *view);

void clear_view();

void flush_view();

view_t *init_view();

void draw_footer(view_t *view, buffer_t *buffer);

void draw_info(view_t *view, buffer_t *buffer);

void draw_line_number(int i);

int render_view(view_t *view, buffer_t *buffer);

void set_cursor_shape(cursor_shape_t shape);

void set_cursor_style(cursor_style_t style);

void set_cursor(int row, int column);

void put_str(int row, int col, char *str);

#endif