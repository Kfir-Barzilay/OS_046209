
/*-----------------------------includes--------------------------------*/

#include "rwlock.hpp"

/*-----------------------------defines---------------------------------*/

#define EMPTY 0

rwlock::rwlock(int num_of_readers)
{
    this->read_counter = EMPTY;
    this->write_counter = EMPTY;
    sem_init(&(this->read_sem), 0, num_of_readers);
    sem_init(&(this->write_sem), 0, 1);
}

void rwlock::read_down()
{
    
}

void rwlock::read_up()
{

}

void rwlock::write_lock()
{
    

}

void rwlock::write_unlock()
{

}
