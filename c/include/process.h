#ifndef PROCESS_H
#define PROCESS_H

#include "buffer.h"
#include "view.h"

typedef enum {
  NORMAL,
  INSERT,
  SELECT
} process_mode_t;

typedef struct PROCESS_STRUCT {
  process_mode_t mode;
} process_t;

process_t *init_process();

void free_process(process_t *buffer);

void process_input(process_t* process, view_t *view, buffer_t *buffer);

int process_normal(process_t* process, char c, buffer_t *buffer);

int process_insert(process_t* process, char c, buffer_t *buffer);

#endif