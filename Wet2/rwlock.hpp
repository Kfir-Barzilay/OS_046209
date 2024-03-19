#ifndef RWLOCK_CLASS_H
#define RWLOCK_CLASS_H
/*-----------------------------includes--------------------------------*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
/*-----------------------------defines---------------------------------*/
#define SUCCESS 0 
#define FAIL -1

using namespace std;
typedef class rwlock
{
public:
    /**
     * @feild rd_counter  -> count how many ATM's read from the account
     * @feild rd_lock - mutex lock for readers.
     * @feild rd_lock - writer lock for writers.
    */
    int rd_counter;
    pthread_mutex_t rd_lock;
    pthread_mutex_t wr_lock;

    /**
     * @brief Constructor.
    */
    rwlock();
    
    /**
     * @brief an ATM want to access read, so add him to counter
    */
    void read_down();

    /**
     * @brief an ATM finish to read from account
    */
    void read_up();

    /**
     * @brief an ATM want to write into account, so lock write
    */
    void write_lock();

    /**
     * @brief an ATM finished to write to tha account
    */
    void write_unlock();
} rwlock_t ;

#endif /*RWLOCK_CLASS_H*/


