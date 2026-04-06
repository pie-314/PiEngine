#ifndef CLI_H
#define CLI_H

#include <time.h>

typedef struct {
  int digits;
  char output_file[256];
  int help;
  int verbose;
} Args;

typedef struct {
  clock_t start;
  clock_t end;
} Timer;

Args parse_args(int argc, char *argv[]);
void print_help(void);
Timer timer_start(void);
void timer_stop(Timer *t);
double timer_elapsed_seconds(Timer t);

#endif
