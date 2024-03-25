#ifndef BANK_CLASS_H
#define BANK_CLASS_H
/*-----------------------------includes--------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <map>
#include <cmath>
#include <random>
#include <fstream>
#include "rwlock.hpp"
#include "account.hpp"


/*-----------------------------defines---------------------------------*/
#define SUCCESS 0
#define FAIL -1
#define LOG_FILE "log.txt"


using namespace std;

typedef class bank
{
public:
    /**
     * @field accounts_map - Map struct that holds all the accounts.
     * @field bank_account - The account the commisions are taken to.
     */
    map<int, account_t> accounts_map;
    account_t bank_account;
    rwlock_t lock;
    int ATMs_alive;
    bool status_alive;
    bool comm_alive;

    /**
     * @brief - Constructor
     * @param num_of_atms - the number of ATMs
    */
    bank(int num_of_atms);

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
    int commission();

    /**
     * @brief print to the screen the current status fro each account
     * @return '0' on success and '-1' if failed
    */
    void status();

    /**
     * @brief print to the log.
     * @param msg - prints "Bank: " + msg.
    */
    void printLog(string msg);

    /**
     * @brief print to the log.
     * @param msg - prints "Bank: " + msg.
    */
    //void printError(string msg);
    

     /**
     * @brief gives status on bank.
     * @return true - if there is atm alive, false otherwise. 
    */
    bool isAlive();

    /**
     * @brief read locks the bank lock
     * 
    */
   void read_down();

   /**
    * @brief read unlocks the bank lock
   */
    void read_up();

    /**
     * @brief write locks the bank lock
     * 
    */
   void write_lock();

   /**
    * @brief write unlocks the bank lock
   */
    void write_unlock();

}bank_t;



#endif /*BANK_CLASS_H*/
