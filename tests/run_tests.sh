#!/bin/bash
set -e
echo "Compiling..."
gcc -Wall -o student studentrecord2.c
echo "Running program with sample input..."
printf "1\n101\nAman\n85\n2\n3\n" | ./student
echo "All tests passed!"