#ifndef RWLOCK_CLASS_H
#define RWLOCK_CLASS_H
/*-----------------------------includes--------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore>
#include <mutex>
/*-----------------------------defines---------------------------------*/
#define SUCCESS 0 
#define FAIL -1

using namespace std
typedef class rwlock
{
public:
    /**
     * @feild read_counter  -> count how many ATM's read from the account
     * @feild write_counter -> count the writers
     * @feild read_sem      -> semaphore lock specified for read, no one can 
     *                         write as long there are ATM's that read
     * @feild write_sem     -> semaphore lock specified for write, no one can 
     *                         read as long someone write
     * @feild mtx           -> mutex to make the semaphore actions as atomic
    */
    int read_counter;
    int write_counter;
    sem_t read_sem;
    sem_t write_sem;
    mutex mtx;
    /**
     * @brief Constructor.
     * @param num_of_readers - max number of readers that can read from account
    */
    rwlock(int num_of_readers);
    
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
};

#endif /*RWLOCK_CLASS_H*/


