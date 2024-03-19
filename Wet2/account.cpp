#include "account.hpp"
#include <string.h>

using namespace std;

account::account(int id, int password, int balance)
{
    this->id = id;
    this->password = password;
    this->balance = balance;
    this->lock = rwlock();
}
/*do we need to destruct locks?*/

int account::read_balance(int password) 
{
    if (password != this->password || password != MASTER_PASSWORD)
        return -1;
    return this->balance;
}

int account::write_balance(int password, int amount)
{
    if (password != this->password || password != MASTER_PASSWORD)
        return -1;
    else if (this->balance - amount < 0 )
        return -2;
    else
        this->balnce += amount ; 
    return this->balance;
}

void account::read_down() 
{
    this->lock.read_down();
}

void account::read_up() 
{
    this->lock.read_up();
}

void account::write_lock() 
{
    this->lock.write_lock();
}

void account::write_unlock() 
{
    this->lock.write_unlock();
}

int account::get_password()
{
    return this->password;
}

void sys_Error(string msg)
{
    string error = "Bank error: ";
    error += msg;
    error += " failed";
    perror(error);
}