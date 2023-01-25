#include "include/buffer.h"
#include "include/process.h"
#include "include/utils.h"
#include "include/view.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_SIZE 1024

int main() {
  clear_logs();

  kim_log("starting execution....");

  char *file_path = "./test/test.txt";

  view_t *view = init_view();

  buffer_t *buffer = init_buffer();
  read_file_to_buffer(buffer, file_path);

  process_input(view, buffer);

  exit_view(view);
  free_buffer(buffer);

  kim_log("stopping execution....");
  dump_logs();

  return 0;
}
