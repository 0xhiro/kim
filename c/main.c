#include "include/args.h"
#include "include/buffer.h"
#include "include/process.h"
#include "include/utils.h"
#include "include/view.h"
#include "include/const.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char **argv) {

  kim_args_t kim_args = PARSE_ARGS(argc, argv);

  if (kim_args.help) {
    printf(HELP);
    exit(0);
  }
  if (kim_args.version) {
    printf(VERSION);
    exit(0);
  }
  if (kim_args.no_args) {
    printf(HELP);
    exit(0);
  }

  clear_logs();

  kim_log("starting "
          "execution..........................................................."
          "....");

  
  buffer_t *buffer = init_buffer();
  read_file_to_buffer(buffer, kim_args.filename);

  view_t *view = init_view();

  process_t *process = init_process();

  buffer->oldt = view->oldt;
  process->oldt = view->oldt;

  main_process(process, view, buffer);

  exit_view(view);

  kim_log("stopping "
          "execution..........................................................."
          ".....");

  // dump_logs();
  dump_file(kim_args.filename);
  kim_log("\033c");

  return 0;
}
