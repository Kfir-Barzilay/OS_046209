
/*-----------------------------includes--------------------------------*/
#include "rwlock.hpp"

/*-----------------------------defines---------------------------------*/

#define EMPTY 0

rwlock::rwlock()
{
    this->rd_counter = EMPTY;
}

void rwlock::read_down()
{
    this->rd_lock.lock();
    rd_counter++;
    if (rd_counter == 1) {
        this->wr_lock.lock();
    }
    this->rd_lock.unlock();
}

void rwlock::read_up()
{
    this->rd_lock.lock();
    rd_counter--;
    if (rd_counter == 0) {
        this->wr_lock.unlock();
    }
    this->rd_lock.unlock();
}

void rwlock::write_lock()
{
    wr_lock.lock();
}

void rwlock::write_unlock()
{
    wr_lock.unlocked();
}
