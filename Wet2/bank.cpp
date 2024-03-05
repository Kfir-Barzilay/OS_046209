#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <random>
#include "bank.hpp"
#define FAIL 1

using namespace std;

bank::bank()
{
    this->list<account_t> accounts = {};
    this->bank_account = account::account(0,0,0);
}

int main(int argc, char *argv[])
{
    /* ------------ init bank -----------*/
    bank_t bank = bank();
    pthread_t bank_thread;
    if(pthread_create(&bank_thread) != 0)
    {
        sys_err("pthread_create");
        return FAIL;
    }
    if(/* statement that check that we in bank_thread*/)
    {
        while(true)
        {
            /* enter critical section*/
            take_money();
            /*exit critical section*/
            sleep(3);
            if(/* some statment to end the bank thread*/)
                break;
        }
        pthread_exit(&bank_thread);
    }

    /*------- take care of the inputs ----*/

    int n = argc;// number of ATMs

    for(int i = 0; i < n; n++)
    {
        //*** need to enter a new thread here ***//
        if(pthread_create() != 0)
        {
            sys_err("pthread_create");
            return FAIL;
        }
        if(/* statement for entering the loop*/)
        {
            while(true){
                ifstream file(argv[i]);
                // Check if the file was opened successfully
                if (!file.is_open()) {
                    cerr << "Error opening file: " << argv[i] << endl;
                    continue;  // Move on to the next file
                }
                while (getline(file, line)) {
                    // Create a stringstream from the line
                    stringstream ss(line);
                    string word;
                    vector<string> words;  // Vector to store separated words

                    // Split the line into words based on spaces
                    while (ss >> word) {
                        words.push_back(word);
                    }

                    string operation = *(words.front());
                    words.erase(0);
                    int account = atoi(words.front());
                    words.erase(0);
                    int password = atoi(words.front());
                    words.erase(0);
                    int amount = atoi(words.front());
                    words.erase(0);

                    switch operation
                    {
                        case 'O':
                            /*------ open acount -------*/
                        case 'D':
                            /*----- make a deposite ----*/
                        case 'W':
                            /*----- make a withdraw ----*/
                        case 'B':
                            /*------ balance check -----*/
                        case 'Q':
                            /*----- closing account ----*/
                        case 'T':
                            /*---- make a transaction---*/ 
                        case default:
                            cerr << "got: " << operation << "we don't support" << endl;
                            return FAIL;
                    }
                    //go to sleep for 100 ms
                    this_thread::sleep_for(chrono::milliseconds(100));
                }
                if(/* finish to run ATM_i */)
                {
                    break;
                }
            }
            pthread_exit();
        }
    }
}

/***********************
desc: take a random precent from each account (maybe also update if it finished running)
everything here is critical section
***********************/
bank::take_money()
{
    //get random num between 1 to 5
    random_device rd;
    mt19937 gen(rd());

    // Define the range [1, 5]
    uniform_int_distribution<> distribution(1, 5);

    // Generate a random number within the specified range
    int tax = distribution(gen);
    account_t *account = this->accounts.head();
    while(account != NULL)
    {
        float money = float(*account.read_balance());
        money = (money*tax)/100;
        int roundedUpValue = static_cast<int>(std::ceil(money));
        this->bank_account.write_balance(roundUpValue);
        account.write_balance(roundUpValue*(-1));
        account = *account.next();
    }

}