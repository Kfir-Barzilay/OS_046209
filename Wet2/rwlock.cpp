
/*-----------------------------includes--------------------------------*/
#include "rwlock.hpp"

/*-----------------------------defines---------------------------------*/
#define DEBUG 0
#define EMPTY 0

rwlock::rwlock()
{
    // Initialize mutexes
    pthread_mutex_init(&rd_lock, nullptr);
    pthread_mutex_init(&wr_lock, nullptr);
    rd_counter = EMPTY;
}

void rwlock::read_down()
{
    if (DEBUG) cout << "read down" << endl;
    pthread_mutex_lock(&rd_lock);
    rd_counter++;
    if (rd_counter == 1) {
        pthread_mutex_lock(&wr_lock);
    }
    pthread_mutex_unlock(&rd_lock);
}

void rwlock::read_up()
{
    if (DEBUG) cout << "read up" << endl;
    pthread_mutex_lock(&rd_lock);
    rd_counter--;
    if (rd_counter == 0) {
        pthread_mutex_unlock(&wr_lock);
    }
    pthread_mutex_unlock(&rd_lock);
}

void rwlock::write_lock()
{
    if (DEBUG) cout << "write lock" << endl;
    pthread_mutex_lock(&wr_lock);
}

void rwlock::write_unlock()
{
    if (DEBUG) cout << "write unlock" << endl;
    pthread_mutex_unlock(&wr_lock);
}