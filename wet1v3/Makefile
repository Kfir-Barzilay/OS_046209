# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall -std=c++11
CCLINK = $(CC)
OBJS = commands.o signals.o job.o smash.o
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
job.o :job.cpp job.hpp
commands.o: commands.cpp commands.hpp
smash.o: smash.cpp commands.hpp
signals.o: signals.cpp signals.hpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*


