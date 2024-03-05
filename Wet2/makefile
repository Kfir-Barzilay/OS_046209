# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall -std=c++11
CCLINK = $(CC)
OBJS = bank.o ATM.o account.o rwlock.o
RM = rm -f
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) -o Bank $(OBJS)
# Creating the object files
ATM.o :ATM.cpp ATM.hpp
account.o: account.cpp account.hpp
rwlock.o: rwlock.cpp rwlock.hpp
bank.o: bank.cpp bank.hpp

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.* Bank
