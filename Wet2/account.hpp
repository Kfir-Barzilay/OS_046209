#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "account.cpp"
typedef class account
{
    private:
    int id;
    int password;
    int balance;
    int valid;
    rwlock::rwlock lock;
    
    public:
    account(int id, int password, int balance);
    ~account();
    bool isAccount(int id);
    bool isPassword(int password);
    int read_balance();
    int write_balance(int amount);
} account_t;




