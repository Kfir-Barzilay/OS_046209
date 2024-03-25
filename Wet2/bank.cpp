#include "bank.hpp"
#define DEBUG 0
#define CLEAR "\033[2J"
#define START "\033[1;1H"
#define RESET   "\033[0m"
#define MAGENTA "\033[35m"
extern pthread_mutex_t log_mutex;
extern ofstream *outputFile;
/***********************
desc: take a random precent from each account (maybe also update if it finished running)
everything here is critical section
***********************/
bank::bank(int num_of_atms)
{
    this->bank_account = account(0, MASTER_PASSWORD ,0);
    this->ATMs_alive = num_of_atms;
    this->comm_alive = false;
    this->status_alive = false;
}

int bank::commission()
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
        account->write_lock();
        float money = float(account->read_balance(MASTER_PASSWORD));
        money = (money*tax)/100;
        int roundedValue = int(round(money));
        this->bank_account.write_balance(MASTER_PASSWORD ,roundedValue);
        account->write_balance(MASTER_PASSWORD , (-1) * roundedValue);

        string msg = "commissions of ";
        msg += to_string(tax);
        msg += " % were charged, the bank gained ";
        msg += to_string(roundedValue);
        msg += " $ from account ";
        msg += to_string(it->first);
        this->printLog(msg);
        account->write_unlock();
    }
    this->read_up();
    return SUCCESS;
}

void bank::status()
{
    this->read_down();
    printf(CLEAR);
    printf(START);
    cout << "Current Bank Status" << endl;
    for (auto it = this->accounts_map.begin() ; it != this->accounts_map.end() ; it++)
    {
        account_t* account = &(it->second); 
        account->write_lock();
        int balance = account->read_balance(MASTER_PASSWORD);
        cout << "Account " << it->first << ": Balance - " << balance; 
        cout<< " $, Account Password - " << account-> get_password() << endl;
        account->write_unlock();
    }
    int bank_balance = this->bank_account.read_balance(MASTER_PASSWORD);
    cout << "The bank has " << bank_balance << " $\n";

    this->read_up();
}
bool bank::isAlive()
{
    return this->ATMs_alive;
}


void bank::printLog(string msg)
{
   // if(DEBUG) cout<< "---------- entering log --------" << endl;
    pthread_mutex_lock(&log_mutex);
    if (outputFile->is_open())
    {
        *outputFile << "Bank: " << msg << endl;
    }
    //outputFile.close();
    pthread_mutex_unlock(&log_mutex);
}

/*void printError(string msg)
{
    
}*/

void bank::read_down()
{
    if (DEBUG) cout << MAGENTA << "bank read down" << RESET << endl;
    this->lock.read_down();
}

void bank::read_up()
{
    if (DEBUG) cout << MAGENTA<<"bank read up" << RESET << endl;
    this->lock.read_up();
}

void bank::write_lock()
{
    if (DEBUG) cout << MAGENTA<< "bank write lock"<<RESET << endl;
    this->lock.write_lock();
}


void bank::write_unlock()
{
    if (DEBUG) cout << MAGENTA<<"bank write unlock"<<RESET << endl;
    this->lock.write_unlock();
}