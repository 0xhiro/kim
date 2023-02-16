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

void *view_process(void *arg) {
  struct ThreadArgs *args = (struct ThreadArgs *)arg;
  double seconds = args->frame_rate;

  time_t next_time = time(NULL);

  while (1) {
    time_t now = time(NULL);
    if (now >= next_time) {
      render_view(args->view, args->buffer);
      next_time = now + seconds;
    }
    int sleepTime = (int)(seconds * 1000000);
    usleep(sleepTime);
  }
  return NULL;
}

void main_process(process_t *process, view_t *view, buffer_t *buffer) {
  pthread_t thread_id;
  int ret;

  struct ThreadArgs args = {
      .view = view, .buffer = buffer, .frame_rate = 0.07}; // 30fps

  // Create a new thread to run the function
  ret = pthread_create(&thread_id, NULL, view_process, (void *)&args);
  if (ret) {
    kim_log("Error creating thread\n");
    exit(1);
  }

  int key_pressed = 0;

  struct pollfd fds;

  fds.fd = 0;
  fds.events = POLLIN;

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

void move_right(buffer_t *buffer) {
  if (strlen(buffer->content) + 1 > buffer->cursor + 1) {
    buffer->cursor++;
  }
}

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
