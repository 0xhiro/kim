#ifndef KIM_ARGS_H
#define KIM_ARGS_H

typedef struct KIM_ARGS_STRUCT {
  char *filename;
  char *start;
  int help;
  int version;
  int no_args;
} kim_args_t;

kim_args_t PARSE_ARGS(int argc, char **argv);

#endif
