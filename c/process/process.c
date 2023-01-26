#include "../include/process.h"
#include "../include/buffer.h"
#include "../include/utils.h"
#include "../include/view.h"
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

void process_input(process_t *process, view_t *view, buffer_t *buffer) {
  int updated_view = 0;
  int key_pressed = 0;

  struct pollfd fds;

  fds.fd = 0;
  fds.events = POLLIN;

  update_view();
  while (1) {
    key_pressed = poll(&fds, 1, 0);

    if (key_pressed) {
      update_view();
    }

    if (key_pressed == 1) {
      char ch = getchar();

      if (process->mode == NORMAL) {
        int quit = process_normal(process, ch, buffer);
        if (quit) {
          return;
        }
      } else if (process->mode == INSERT) {
        process_insert(process, ch, buffer);
      }
    }
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
