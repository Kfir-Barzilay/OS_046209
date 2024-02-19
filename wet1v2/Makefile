# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall
CCLINK = $(CC)
OBJS = job.o smash.o commands.o signals.o 
RM = rm -f
# Creating the  executable
main: $(OBJS)
	$(CCLINK) -o main $(OBJS)
# Creating the object files
commands.o: commands.cpp commands.hpp
smash.o: smash.cpp commands.hpp
signals.o: signals.cpp signals.hpp
job.o: job.cpp job.hpp
main.o: main.cpp 
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

