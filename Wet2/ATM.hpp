#ifndef ATM_CLASS_H
#define ATM_CLASS_H
/*-----------------------------includes--------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <algorithm>
#include <pthread.h>

#include "rwlock.hpp"
#include "account.hpp"
#include "bank.hpp"



/*-----------------------------defines---------------------------------*/
#define SUCCESS 0
#define FAIL -1
#define LOG_FILE "log.txt"

using namespace std;

typedef class ATM
{
public:
    /**
     * @param ATM_id - The ATMs id
    */
    int ATM_id;
    
    /**
     * @brief Constractor
     * @param ATM_id - the ATM's id
    */
    ATM(int ATM_id);
    
    /**
     * @brief opening account
     * @param bank - Refernce to the bank.
     * @param id - The id of the new account.
     * @param password - The password of the new account.
     * @param initial_sum - The initial amount of money in the account.
     * @return - '0' on success, '1' if Failed. The func will log print.
    */
    int open_account(bank_t& bank, int id, int password, int initial_sum); 
   
    /**
     * @brief make a deposit to account 'id' with access password and the 
     *        amount to add
     * @param id - the account i want to make the deposit
     * @param password - the password the user entered to enter the account
     * @param amount - the amount of money you want to add to the account
     * @return - '0' on success and '-1' if failed. The func will log print if needed.
    */
    int deposit(bank_t& bank, int id, int password, int amount);
    
    /**
     * @brief make a withdraw from account 'id' with password and the amount.
     * @param id - the account i want to make the withdraw
     * @param password - the password the user entered to enter the account,
     * @param amount - the amount of money you want to withdraw
     * @return - '0' on success and '-1' if failed. The func will log print.
    */
    int withdraw(bank_t& bank, int id, int password, int amount);
    
    /**
     * @brief check the balance in account 'id' and the entering password
     * @param id - the id account i want access
     * @param password - the password need for access
     * @return - the balance in account, '-1' if failed. The func will log print.
    */
    int balance_check(bank_t& bank,int id, int password);
    
    /**
     * @brief closing the account on the ATM
     * @param id - closing account id
     * @param password - the password needed to access the account
     * @param return - '0' on success and '-1' if failed. The func will log print.
    */
    int quit_account(bank_t& bank,int id, int password);
    
    /**
     * @brief make a transaction from account 'id-sender' to account 'id-receiver'
     * @param id_sender - id-sender account to make a withdraw
     * @param password_sender - id-sender password to enter
     * @param id_receiver - id_receiver account to make a deposit
     * @param password_receiver - id_receiver password to enter
     * @param amount - the amount of money to take from id-sender account to 
     *                 give the id-receiver account
     * @return - '0' on success or '-1' if failed
    */
    int transaction(bank_t& bank,
                int id_sender,
                int password_sender, 
                int id_receiver,  
                int amount);
    
    /**
    * @brief Print error that matches the specific ATM
    * @param error - The error message to print. 
    */
    void printError(string error);

    /**
    * @brief Print error that matches the specific ATM
    * @param error - The error message to print. 
    */
    void printLog(string msg);
} ATM_t;

void sleep_ms(int sleep_time_ms);

bool isInVector(const vector<int>& vec, int value);

#endif /*ATM_CLASS_H*/




