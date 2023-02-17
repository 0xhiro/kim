#include "../include/process.h"
#include "../include/buffer.h"
#include "../include/utils.h"
#include "../include/view.h"
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

struct ThreadArgs {
  view_t *view;
  buffer_t *buffer;
  double frame_rate;
};

void main_process(process_t *process, view_t *view, buffer_t *buffer) {
  int key_pressed = 0;
  int former_row = 0;
  int former_col = 0;

  struct pollfd fds;

  fds.fd = 0;
  fds.events = POLLIN;

  cords_t former_size = get_view_size();

  update_info(process, "welkome to kim :)");

  render_view(view, buffer, process);

  for (;;) {
    key_pressed = poll(&fds, 1, 0);

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

      render_view(view, buffer, process);
    }

    if (window_resized(former_size)) {
      render_view(view, buffer, process);

      former_size = get_view_size();
    }
  }
}

void update_info(process_t *process, char *message) {
  process->info = realloc(process->info, strlen(message) + 1 * sizeof(char));
  strcpy(process->info, message);
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
    update_info(process, "saving buffer");
    write_buffer_to_file(buffer);
    break;
  case 'i':
    kim_log("changing to insert mode");
    update_info(process, "changing to insert mode");
    process->mode = INSERT;
    break;
  default:
    kim_log("unrecognized normal key");
    break;
  }

  return 0;
}

int process_insert(process_t *process, char ch, buffer_t *buffer) {
  if (ch == '\033') {
    kim_log("processing escape");
    
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
  // BUG: escape is not registered until three presses
  case ESCAPE_CHAR: {
    kim_log("changing to normal mode");
    update_info(process, "now in normal mode");
    process->mode = NORMAL;
    break;
  }

  case 'a': {
    kim_log("changing to normal mode");
    update_info(process, "now in normal mode");
    process->mode = NORMAL;
    break;
  }

  // BUG: backspace is not being recognized
  case 's': {
    delete_char_in_line(buffer);
    break;
  }

  case '\n': {
    write_newline_to_line(buffer);
    break;
  }

  default: {
    write_char_to_line(buffer, ch);
    break;
  }
  }

  return 1;
}

void move_right(buffer_t *buffer) {
  if (buffer->col < strlen(buffer->all_lines[buffer->line - 1])) {
    buffer->col++;
  } else if (buffer->line < buffer->lines_count) {
    buffer->line++;
    buffer->col = 1;
  }
}

void move_left(buffer_t *buffer) {
  if (buffer->col > 1) {
    buffer->col--;
  } else if (buffer->line != 1) {
    buffer->line--;
    buffer->col = strlen(buffer->all_lines[buffer->line - 1]);
  }
}

void move_up(buffer_t *buffer) {
  if (buffer->line != 1) {
    buffer->line--;

    if (strlen(buffer->all_lines[buffer->line - 1]) > buffer->col) {
      buffer->col = buffer->col;
    } else {
      buffer->col = strlen(buffer->all_lines[buffer->line - 1]);
    }
  }
}

void move_down(buffer_t *buffer) {
  if (buffer->line < buffer->lines_count) {
    buffer->line++;

    if (strlen(buffer->all_lines[buffer->line - 1]) > buffer->col) {
      buffer->col = buffer->col;
    } else {
      buffer->col = strlen(buffer->all_lines[buffer->line - 1]);
    }
  }
}

process_t *init_process() {
  process_t *process = calloc(1, sizeof(struct PROCESS_STRUCT));
  process->mode = NORMAL;
  process->info = (char *)malloc(2 * sizeof(char));
  strcpy(process->info, "H");

  return process;
}

void free_process(process_t *process) { free(process); }
