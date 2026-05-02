#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Args parse_args(int argc, char *argv[]) {
  Args args = {0};
  args.digits = 1000; // Default digits
  args.help = 0;
  args.verbose = 0;
  strcpy(args.output_file, ""); // No output file by default

  if (argc != 1) {

    for (int i = 1; i < argc; i++) {
      // Check for help
      if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
        args.help = 1;
        return args;
      }
      // Check for verbose
      else if (strcmp(argv[i], "--verbose") == 0 ||
               strcmp(argv[i], "-v") == 0) {
        args.verbose = 1;
      }
      // Check for digits
      else if (strcmp(argv[i], "--digits") == 0 || strcmp(argv[i], "-d") == 0) {
        if (i + 1 < argc) {
          args.digits = atoi(argv[++i]);
          if (args.digits <= 0) {
            fprintf(stderr, "Error: digits must be a positive integer\n");
            exit(1);
          }
        } else {
          fprintf(stderr, "Error: --digits requires a value\n");
          exit(1);
        }
      }
      // Check for output file
      else if (strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0) {
        if (i + 1 < argc) {
          i++;
          if (strlen(argv[i]) >= 256) {
            fprintf(stderr, "Error: output filename too long\n");
            exit(1);
          }
          strcpy(args.output_file, argv[i]);
        } else {
          fprintf(stderr, "Error: --output requires a value\n");
          exit(1);
        }
      } else {
        fprintf(stderr, "Error: unknown argument '%s'\n", argv[i]);
        print_help();
        exit(1);
      }
    }
  } else {
    printf("No args passed");
  }

  return args;
}

void print_help(void) {
  printf("\n=== PiEngine - High Precision Pi Calculator ===\n\n");
  printf("Usage: piengine [OPTIONS]\n\n");
  printf("Options:\n");
  printf("  -d, --digits NUM      Number of digits of pi to compute (default: "
         "1000)\n");
  printf("  -o, --output FILE     Write output to file (default: stdout)\n");
  printf("  -v, --verbose         Show timing and performance information\n");
  printf("  -h, --help            Show this help message\n\n");
  printf("Examples:\n");
  printf("  piengine                              # Compute 1000 digits to "
         "stdout\n");
  printf("  piengine --digits 10000              # Compute 10000 digits\n");
  printf("  piengine -d 5000 -o pi.txt           # Save to file\n");
  printf("  piengine --digits 100000 --verbose   # Show timing info\n\n");
}

Timer timer_start(void) {
  Timer t;
  t.start = clock();
  return t;
}

void timer_stop(Timer *t) { t->end = clock(); }

double timer_elapsed_seconds(Timer t) {
  return (double)(t.end - t.start) / CLOCKS_PER_SEC;
}
