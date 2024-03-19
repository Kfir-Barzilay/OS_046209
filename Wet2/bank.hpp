#ifndef BANK_CLASS_H
#define BANK_CLASS_H
/*-----------------------------includes--------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <cmath>
#include "rwlock.hpp"
#include "account.hpp"


/*-----------------------------defines---------------------------------*/
#define SUCCESS 0
#define FAIL -1

using namespace std;

typedef class bank
{
public:
    /**
     * @field accounts_map - Map struct that holds all the accounts.
     * @field bank_account - The account the commisions are taken to.
     */
    map<int id, account_t> accounts_map;
    account_t bank_account;
    rwlock_t lock;

    /**
     * @brief - Constructor
    */
    bank();

    /**
     * @brief - Destructor
    */
    //~bank(); using default destructor

    /**
     * @brief - inserts the account in the map_accounts.
     * @return 0 if succeded, -1 if account is already exist.
    */
    //int insert_account(account_t account);

    /**
     * @brief - deletes the account in the map_accounts.
     * @return 0 if succeded, -1 if account is already exist.
    */
    //int delete_account(account_t account);

    /**
     * @brief the bank take every x time a random tax from each account
     * @return '0' on success and '-1' if failed
    */
    int commissions();

    /**
     * @brief print to the screen the current status fro each account
     * @return '0' on success and '-1' if failed
    */
    int status();

    /**
     * @brief print to the log Bank: msg.
    */
    void printLog(string msg);
}bank_t;



#endif /*BANK_CLASS_H*/
