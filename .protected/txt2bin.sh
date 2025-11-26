#!/bin/bash

infile="$1"
outfile="$2"

: > "$outfile"

# ignore all lines that start with #
for n in $(grep -v '^#' "$infile" | tr -s '[:space:]' ' '); do
    dec=$((10#$n))
    printf '%02X' "$dec" | xxd -r -p >> "$outfile"
done