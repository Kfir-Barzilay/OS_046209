#include "account.hpp"

account::account(int id, int password, int balance)
{
    this->id = id;
    this->password = password;
    this->balance = balance;
    /*lock init*/

}
/*do we need to destruct locks?*/
~account();

bool account::isAccount(int id) 
{
    return (this->id == id);
}

bool account::isPassword(int password)
{
    return (this->password == password);
}

int account::read_balance() 
{
    return this->balance;
}

void account::write_balance(int amount)
{
    this->account += amount;
}
