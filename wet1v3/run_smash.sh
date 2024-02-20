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

# Run the compiled executable
echo "Running ./smash..."
./smash

