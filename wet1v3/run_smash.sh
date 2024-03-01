#!/bin/bash

# Run 'make clean'
echo "Running make clean..."
make clean

# Run 'make'
echo "Running make..."
make

# Compile and link the source files
echo "Compiling and linking source files..."
g++ -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG *.cpp -o smash

# Add newline characters to the input file if not present
#echo "Adding newline characters to input1.txt if needed..."
#sed -i -e '/.*[^[:space:]]$/!s/$/\n/' input2.txt

# Run the compiled executable with input redirection
#echo "Running ./smash with input from input1.txt..."
#./smash < input2.txt >> outputs_tests.txt 2>&1
#./smash