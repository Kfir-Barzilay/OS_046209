# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall -std=c++11
CCLINK = $(CC)
OBJS = main.o bank.o ATM.o account.o rwlock.o
RM = rm -f
TARGET = Bank

# Creating the executable
$(TARGET): $(OBJS)
	g++ -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -pthread *.cpp -o Bank
# Creating the object files
ATM.o : ATM.cpp ATM.hpp
	$(CC) $(CFLAGS) -c ATM.cpp

account.o: account.cpp account.hpp
	$(CC) $(CFLAGS) -c account.cpp

rwlock.o: rwlock.cpp rwlock.hpp
	$(CC) $(CFLAGS) -c rwlock.cpp

bank.o: bank.cpp bank.hpp
	$(CC) $(CFLAGS) -c bank.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*
