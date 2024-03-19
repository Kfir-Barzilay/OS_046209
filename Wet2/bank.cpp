#include "bank.hpp"

extern pthread_mutex_t log_mutex;

/***********************
desc: take a random precent from each account (maybe also update if it finished running)
everything here is critical section
***********************/
bank::bank()
{
    this->map<int ,account_t> accounts_map;
    this->bank_account = account::account(0, MASTER_PASSWORD ,0);
    this->lock = rwlock();
}

bank::commission()
{
    //get random num between 1 to 5
    random_device rd;
    mt19937 gen(rd());

    // Define the range [1, 5]
    uniform_int_distribution<> distribution(1, 5);

    // Generate a random number within the specified range
    int tax = distribution(gen);
    
    this->read_down();
    for (auto it = this->accounts_map.begin() ; it != this->accounts_map.end() ; it++)
    {
        account_t* account = &(it->second); 
        account.write_lock();
        float money = float(account->read_balance(MASTER_PASSWORD));
        money = (money*tax)/100;
        int roundedValue = int(round(money));
        this->bank_account.write_balance(MASTER_PASSWORD ,roundedValue);
        account.write_balance(MASTER_PASSWORD , (-1) * roundedValue);

        string msg = "commission of ";
        msg += tax;
        msg += " % were charged, the bank gained ";
        msg += money;
        msg += " $ from account ";
        msg += it->first;
        this->printLog(msg);

        account.write_unlock();
    }
    this->read_up();
}

void bank::status()
{
    this->read_down();
    cout << "Current Bank Status" << endl;
    for (auto it = this->accounts_map.begin() ; it != this->accounts_map.end() ; it++)
    {
        account_t* account = &(it->second); 
        account.write_lock();
        int balance = account->read_balance(MASTER_PASSWORD);
        cout << "Account " << it->first << ": Balance - " << balance; 
        cout<< " $, Account Password - " << account-> get_password() << endl;
    }
    int bank_balance = this->bank_account.read_balance(MASTER_PASSWORD);
    cout << "The bank has " << bank_balance << " $\n";

    this->read_up():
}

void printLog(string msg)
{
    log_mutex.lock();
    ofstream outputFile(LOG_FILE);
    if (outputFile.is_open())
    {
        outputFile << "Bank: " << msg << endl;
    }
    outputFile.close();
    log_mutex.unlock();
}