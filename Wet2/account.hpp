#ifndef ACCOUNT_CLASS_H
#define ACCOUNT_CLASS_H
/*-----------------------------includes--------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rwlock.hpp"

/*-----------------------------defines---------------------------------*/
#define DEBUG 0
#define MASTER_PASSWORD -1


using namespace std;

/**
* @brief Class that handles each account 
*/
typedef class account
{
    private:
    /**
     * @field password  - password to acces the accout.
     * @field balnce - Account's balance.  
     * @field valid - '1' if the data relevant , '0' not relevant
     * @field lock - Read / write lock.
    */
    int password;
    int balance;
    int valid;
    rwlock_t lock;
    public:
    /**
    * @field id - Key that is uniqe to each account.
    */
    int id;

    /**
    * @brief constructs new acccount.
    * @param id - The constructed account key value.
    * @param password - The constructed account password.
    * @param balance - The constructed account default balance. 
    */
    account(int id, int password, int balance);

    /**
    * @brief destructor to account.
    */  
    //~account(); using default destructor
    

    /**
     * @brief - read account current balance if password is correct
     *          or master password is used.
     * @param password - The password the user entered.
     * @return - The current balance in the account , -1 if failed (invalid paasword).
    */
    int read_balance(int password);

    /**
    * @brief - Deposit or withdraw money from the account if password is correct
     *          or master password is used.
    * @param password - the password the user entered.
    * @param password - The amount of money to deposit(positive), withdraw(negative).
    * @return - 0 if succeded, -1 for invalid password, -2 for insufficient amount of money.
    */
    int write_balance(int password, int amount);

    /**
    * @brief - Read down the account.
    */
    void read_down();

    /**
    * @brief - Read up the account.
    */
    void read_up();

    /**
    * @brief - write lock the account.
    */
    void write_lock();

    /**
    * @brief - write unlock the account.
    */
    void write_unlock();

    /**
     * @return - the password of account 
    */
    int get_password();
} account_t;

/**
* @brief - Print function for failed sys calls.
* @param sys_call - The failed system call function.
*/
void sys_Error(string msg);

#endif /*ACCOUNT_CLASS_H*/

