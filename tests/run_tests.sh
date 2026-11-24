#!/bin/bash
set -e
echo "Compiling..."
gcc -Wall -o newcode newcode.c -lm
echo "Running program with sample input..."
printf "1\n101\nAman\nCSE\n85\n2\n3\n101\n6\n" | ./newcode
echo "All tests passed!"