#ifndef PROCESS_H
#define PROCESS_H

#include "buffer.h"
#include "view.h"

typedef enum { NORMAL, INSERT, SELECT } process_mode_t;

typedef struct PROCESS_STRUCT {
  process_mode_t mode;

  char *info;

  struct termios oldt;
} process_t;

void move_right(buffer_t *buffer);

void move_left(buffer_t *buffer);

void move_up(buffer_t *buffer);

void move_down(buffer_t *buffer);

process_t *init_process();

void update_info(process_t *process, char *message);

void free_process(process_t *buffer);

void main_process(process_t *process, view_t *view, buffer_t *buffer);

int process_normal(process_t *process, char c, buffer_t *buffer);

int process_insert(process_t *process, char c, buffer_t *buffer);

#endif