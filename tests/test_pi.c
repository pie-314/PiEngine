#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000000
#define MAX_DIGITS 100000

typedef struct {
  char real_pi[MAX_DIGITS];
  char calculated_pi[MAX_DIGITS];
  int real_length;
  int calculated_length;
  int accurate_digits;
  int total_compared;
} ComparisonResult;

// remove space
void remove_whitespace(char *str) {
  int write = 0;
  for (int read = 0; str[read]; read++) {
    if (!isspace(str[read])) {
      str[write++] = str[read];
    }
  }
  str[write] = '\0';
}

int load_pi_from_file(const char *filename, char *buffer, int max_length) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
    return -1;
  }

  int total_length = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, sizeof(line), file) && total_length < max_length) {
    remove_whitespace(line);

    int line_length = strlen(line);
    if (line_length == 0)
      continue;

    if (total_length + line_length > max_length) {
      line_length = max_length - total_length;
    }

    strncpy(buffer + total_length, line, line_length);
    total_length += line_length;
  }

  fclose(file);
  buffer[total_length] = '\0';
  return total_length;
}

void compare_pi_values(ComparisonResult *result) {
  result->accurate_digits = 0;
  result->total_compared = (result->real_length < result->calculated_length)
                               ? result->real_length
                               : result->calculated_length;

  for (int i = 0; i < result->total_compared; i++) {
    if (result->real_pi[i] == result->calculated_pi[i]) {
      result->accurate_digits++;
    } else {
      break; /* Stop at first mismatch to count consecutive accurate digits */
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <real_pi.txt> <calculated_pi.txt>\n", argv[0]);
    fprintf(stderr, "Example: %s real_pi.txt calculated_pi.txt\n", argv[0]);
    return 1;
  }

  ComparisonResult result = {0};

  printf("   Pi Value Comparison Test\n\n");

  /* Load real pi values */
  printf("Loading real pi from: %s\n", argv[1]);
  result.real_length = load_pi_from_file(argv[1], result.real_pi, MAX_DIGITS);
  if (result.real_length < 0) {
    return 1;
  }
  printf("Loaded %d digits\n\n", result.real_length);

  /* Load calculated pi values */
  printf("Loading calculated pi from: %s\n", argv[2]);
  result.calculated_length =
      load_pi_from_file(argv[2], result.calculated_pi, MAX_DIGITS);
  if (result.calculated_length < 0) {
    return 1;
  }
  printf("Loaded %d digits\n\n", result.calculated_length);

  /* Compare values */
  compare_pi_values(&result);

  /* Display results */
  printf("   Comparison Results\n");

  printf("Real pi digits:       %d\n", result.real_length);
  printf("Calculated pi digits: %d\n", result.calculated_length);
  printf("Total compared:       %d\n", result.total_compared);
  printf("\n");

  if (result.accurate_digits == result.total_compared) {
    printf("ALL DIGITS MATCH! (%d/%d accurate)\n", result.accurate_digits,
           result.total_compared);
  } else {
    printf("Accurate digits: %d/%d (%.2f%%)\n", result.accurate_digits,
           result.total_compared,
           (result.total_compared > 0)
               ? (100.0 * result.accurate_digits / result.total_compared)
               : 0.0);

    if (result.accurate_digits < result.total_compared &&
        result.accurate_digits < 100) {
      printf("\nFirst mismatch at position %d:\n", result.accurate_digits);
      printf("  Real:       %c\n", result.real_pi[result.accurate_digits]);
      printf("  Calculated: %c\n",
             result.calculated_pi[result.accurate_digits]);
    }
  }

  return (result.accurate_digits == result.total_compared) ? 0 : 1;
}
