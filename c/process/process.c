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

  update_view(view, buffer); // initial view render
  while (1) {
    key_pressed = poll(&fds, 1, 0);

    if (key_pressed) {
      update_view(view, buffer);
    }

    // FIXME: fix continuos rendering to fix flicker
    // update_view(view, buffer);

    if (key_pressed) {
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

    if (key_pressed) {
      update_view(view, buffer);
    }
  }
}

int process_normal(process_t *process, char ch, buffer_t *buffer) {
  if (ch == '\033') {
    getchar(); // skip the [
    switch (getchar()) {
    case 'A':
      move_up(buffer); // up arrow
      return 0;
    case 'B':
      move_down(buffer); // down arrow
      return 0;
    case 'C':
      move_right(buffer); // right arrow
      return 0;
    case 'D':
      move_left(buffer); // left arrow
      return 0;
    }
  }

  switch (ch) {
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

void move_right(buffer_t *buffer) { buffer->cursor++; }

void move_left(buffer_t *buffer) {
  if (buffer->cursor > 1) {
    buffer->cursor--;
  }
}

void move_up(buffer_t *buffer) {
  if (buffer->line > 1) {
    buffer->line--;
  }
}

void move_down(buffer_t *buffer) { buffer->line++; }

int process_insert(process_t *process, char ch, buffer_t *buffer) {
  if (ch == '\033') {
    getchar(); // skip the [
    switch (getchar()) {
    case 'A':
      move_up(buffer); // up arrow
      return 0;
    case 'B':
      move_down(buffer); // down arrow
      return 0;
    case 'C':
      move_right(buffer); // right arrow
      return 0;
    case 'D':
      move_left(buffer); // left arrow
      return 0;
    }
  }

  switch (ch) {
  case ESCAPE_CHAR:
    kim_log("changing to normal mode");
    process->mode = NORMAL;
    return 1;
  }

  write_char_to_buffer(buffer, ch, buffer->cursor);

  return 0;
}

process_t *init_process() {
  process_t *process = calloc(1, sizeof(struct PROCESS_STRUCT));
  process->mode = NORMAL;

  return process;
}

void free_process(process_t *process) { free(process); }
