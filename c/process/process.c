#include "../include/process.h"
#include "../include/buffer.h"
#include "../include/utils.h"
#include "../include/view.h"
#include <stdio.h>
#include <stdlib.h>

void process_input(process_t *process, view_t *view, buffer_t *buffer) {
  int updated_view = 0;
  int content_changed = 1;
  

  int pos = 0;
  char buff[1024];

  while (1) {
    if (content_changed) {
      updated_view = update_view();
      content_changed = 0;
    }

    char ch = getchar();

    if (process->mode == NORMAL) {
      int quit = process_normal(process, ch, buffer);
      if (quit) {
        return;
      }
    } else if (process->mode == INSERT) {
      process_insert(process, ch, buffer);
    }

    updated_view = 0;
  }
}

int process_normal(process_t *process, char c, buffer_t *buffer) {
  switch (c) {
  case 'q':
    kim_log("quiting...");
    return 1;
  case 'w':
    kim_log("saving buffer");
    write_buffer_to_file(buffer);
    break;
  case 'i':
    kim_log("changing to insert mode");
    process->mode = INSERT;
    break;
  default:
    kim_log("unrecognized normal key");
    break;
  }

  return 0;
}

void process_insert(process_t *process, char ch, buffer_t *buffer) {
  if (ch == ESCAPE_CHAR) {
    kim_log("changing to normal mode");
    process->mode = NORMAL;
    return;
  }

  putchar(ch);
}

process_t *init_process() {
  process_t *process = calloc(1, sizeof(struct PROCESS_STRUCT));
  process->mode = NORMAL;

  return process;
}

void free_process(process_t *process) { free(process); }
