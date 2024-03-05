#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef class rwlock
{
public:
    int atm_id; 
    int counter;
    sem_t sem;

    rwlock();
    ~rwlock();
    lock();
    unlock();
};


