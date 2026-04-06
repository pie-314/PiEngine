#include <stdio.h>
#define MPFR_USE_FILE
#include "chudnovsky.h"
#include "cli.h"
#include <gmp.h>
#include <mpfr.h>
#include <string.h>
#include <time.h>
int main(int argc, char *argv[]) {
  // Parse command-line arguments
  Args args = parse_args(argc, argv);

  // Show help if requested
  if (args.help) {
    print_help();
    return 0;
  }

  // Set output stream (stdout or file)
  FILE *output = stdout;
  if (strlen(args.output_file) > 0) {
    output = fopen(args.output_file, "w");
    if (output == NULL) {
      fprintf(stderr, "Error: unable to open output file '%s'\n",
              args.output_file);
      return 1;
    }
  }

  // Initialize timing
  Timer total_timer = timer_start();

  if (args.verbose) {
    fprintf(stderr, "=== PiEngine Computation ===\n");
    fprintf(stderr, "Target digits: %d\n", args.digits);
    fprintf(stderr, "Output to: %s\n\n",
            strlen(args.output_file) > 0 ? args.output_file : "stdout");
    fprintf(stderr, "Starting computation...\n");
  }

  // Set precision for MPFR
  set_precision(args.digits);

  // Initialize and compute pi
  mpfr_t pi;
  mpfr_init(pi);

  Timer compute_timer = timer_start();
  compute_pi(pi, args.digits);
  timer_stop(&compute_timer);

  // Write pi to file (no screen output)
  mpfr_fprintf(output, "%.*Rf\n", args.digits, pi);
  fflush(output);

  timer_stop(&total_timer);

  // Print timing information
  if (args.verbose) {
    fprintf(stderr, "\n=== Timing Results ===\n");
    fprintf(stderr, "Computation time: %.4f seconds\n",
            timer_elapsed_seconds(compute_timer));
    fprintf(stderr, "Total time: %.4f seconds\n",
            timer_elapsed_seconds(total_timer));
  }

  // Cleanup
  mpfr_clear(pi);
  if (strlen(args.output_file) > 0) {
    fclose(output);
    if (args.verbose) {
      fprintf(stderr, "Output written to: %s\n", args.output_file);
    }
  }

  return 0;
}
