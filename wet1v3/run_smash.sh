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

#Run the compiled executable with input redirection
echo "Running ./smash with input1 from inputs"
#./smash < input1.txt >> our_outputs1.txt 2>&1
#echo "Running ./smash with input2 from inputs"
#./smash < input2.txt >> our_outputs2.txt 2>&1