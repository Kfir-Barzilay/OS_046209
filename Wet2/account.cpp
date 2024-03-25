#include "account.hpp"
#include <string.h>
#define DEBUG 0
#define RESET   "\033[0m"
#define CYAN    "\033[36m"
using namespace std;
account::account()
{
    this->id = -1;
    this->password = -1;
    this->balance = 0;
}

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
    if ((password == this->password) || (password == MASTER_PASSWORD)) {
        return this->balance;
    }
    return -1;
}

int account::write_balance(int password, int amount)
{
    if (password != this->password && password != MASTER_PASSWORD)
        return -1;
    else if (this->balance + amount < 0)
        return -2;
    else
        this->balance += amount ; 
    return this->balance;
}

void account::read_down() 
{
    if (DEBUG) cout <<CYAN<< "read down account: "<<RESET << this->id << endl;
    this->lock.read_down();
}

void account::read_up() 
{
    if (DEBUG) cout <<CYAN << "read up account: "<<RESET << this->id << endl;
    this->lock.read_up();
}

void account::write_lock() 
{
    if (DEBUG) cout <<CYAN<< "write lock account: "<<RESET << this->id << endl;
    this->lock.write_lock();
}

void account::write_unlock() 
{
    if (DEBUG) cout <<CYAN<< "write unlock account: "<<RESET << this->id << endl;
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
    perror(error.c_str());
}