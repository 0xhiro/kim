#ifndef VIEW_H
#define VIEW_H

#include <termios.h>

typedef struct VIEW_STRUCT {
  struct termios oldt;
  struct termios newt;
} view_t;

void exit_view(view_t *view);

view_t *init_view();

#endif