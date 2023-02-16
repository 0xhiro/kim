#include "include/buffer.h"
#include "include/process.h"
#include "include/utils.h"
#include "include/view.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int main() {

  clear_logs();

  kim_log("starting "
          "execution..........................................................."
          "....");

  char *file_path = "./test/test.txt";

  buffer_t *buffer = init_buffer();
  read_file_to_buffer(buffer, file_path);

  view_t *view = init_view();

  // kimview();

  // for (;;) {
  // }

  process_t *process = init_process();

  main_process(process, view, buffer);

  exit_view(view);

  kim_log("stopping "
          "execution..........................................................."
          ".....");

  // dump_logs();
  dump_file(file_path);
  // kim_log("\033c");

  return 0;
}
