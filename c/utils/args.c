#include <getopt.h>
#include <string.h>

#include "../include/args.h"
#include "../include/utils.h"

kim_args_t PARSE_ARGS(int argc, char **argv) {
  int option;

  static int help_flag = 0;
  static int version_flag = 0;

  char *non_opt_arg = NULL;
  char *non_opt_arg2 = NULL;
  char *e_arg = NULL;
  char *filename = NULL;

  if (argc > 1 && argv[1][0] != '-') {
    non_opt_arg = argv[1];
  }

  if (argc > 2 && argv[2][0] != '-') {
    non_opt_arg2 = argv[2];
  }

  static struct option long_options[] = {
      {"help", no_argument, &help_flag, 1},
      {"version", no_argument, &version_flag, 1},
      {0, 0, 0, 0}};

  while ((option = getopt_long(argc, argv, "e:hv", long_options, NULL)) != -1) {
    switch (option) {
    case 'e':
      e_arg = optarg;
      break;
    case 'h':
      help_flag = 1;
      break;
    case 'v':
      version_flag = 1;
      break;
    case '?':
      kim_error(NULL, "unknown option");
    }
  }

  int no_args = 0;
  if (argc == 1) {
    no_args = 1;
  }

  if (non_opt_arg) {
    char *command = non_opt_arg;

    if (strcmp(command, "help") == 0) {
      help_flag = 1;
    } else {
      filename = command;
    }
  }

  kim_args_t kim_args = {.filename = filename,
                         .help = help_flag,
                         .version = version_flag,
                         .start = e_arg, // unused
                         .no_args = no_args};

  return kim_args;
}
