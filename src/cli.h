#ifndef CLI_H
#define CLI_H

typedef struct {
  int digits;
  char output_file[256];
  int help;
} Args;

Args parse_args(int argc, char *argv[]);
void print_help(void);

#endif
