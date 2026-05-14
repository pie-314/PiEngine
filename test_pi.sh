#!/bin/bash

# Default values
DIGITS=1000
OUTPUT_FILE="pi_calculated.txt"
REFERENCE_FILE="pi_reference.txt"

# Ensure we are in the OCaml directory
cd "$(dirname "$0")"

# Build the project
dune build

# Generate pi
echo "Generating $DIGITS digits of Pi..."
dune exec bin/main.exe -- --digits $DIGITS --output $OUTPUT_FILE

# Check if reference file exists, if not, create it with a different run or just warn
if [ ! -f "$REFERENCE_FILE" ]; then
    echo "Warning: Reference file $REFERENCE_FILE not found. Creating it from current run for future comparisons."
    cp "$OUTPUT_FILE" "$REFERENCE_FILE"
fi

# Compare the results
dune exec test/compare_pi.exe -- "$REFERENCE_FILE" "$OUTPUT_FILE"
