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
job.o: job.cpp job.hpp
	$(CC) -c $(CFLAGS) job.cpp -o job.o

commands.o: commands.cpp commands.hpp
	$(CC) -c $(CFLAGS) commands.cpp -o commands.o

smash.o: smash.cpp commands.hpp
	$(CC) -c $(CFLAGS) smash.cpp -o smash.o

signals.o: signals.cpp signals.hpp
	$(CC) -c $(CFLAGS) signals.cpp -o signals.o

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.* smash



