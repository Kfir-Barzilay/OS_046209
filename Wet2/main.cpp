#include "bank.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <random>

#define FAIL 1

using namespace std;

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
    if(pthread_equal(pthread_self(), bank_thread))
    {
        while(true)
        {
            bank.take_money();

            sleep(3);
            //if(/* some statment to end the bank thread*/)
            //    break;
        }
        pthread_exit(&bank_thread);
    }

    /*------------ print status -------------*/
    pthread_t bank_status_thread;
    if(pthread_create(&bank_status_thread) != 0)
    {
        sys_err("pthread_create");
        return FAIL;
    }
    if(pthread_equal(pthread_self(), bank_status_thread))
    {
        while(true)
        {
            bank.status();

            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }

    /*------- take care of the inputs ----*/

    int n = argc;// number of ATMs
    ATM atms [n];
    pthread_t ATM_threads[n];
    for(int i = 0; i < n; n++)
    {
        atms[i] = atm(i);
        //*** need to enter a new thread here ***//
        if(pthread_create(&ATM_threads[i]) != 0)
        {
            sys_err("pthread_create");
            return FAIL;
        }

        if(pthread_equal(pthread_self(), ATM_threads[i]))
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
                    if(!words.empty())
                    {
                        int amount_or_sec_account = atoi(words.front());//for transaction it will be account_reciever 
                        words.erase(0);
                    }
                    if(!words.empty())
                    {
                        int amount_trans = atoi(words.front());
                        words.erase(0);
                    }
                    switch operation
                    {
                        case 'O':
                            /*------ open acount -------*/
                            atms[i].open_account(&bank, account, password, amount_or_sec_account);
                        case 'D':
                            /*----- make a deposite ----*/
                            atms[i].deposit(&bank, account, password, amount_or_sec_account);
                        case 'W':
                            /*----- make a withdraw ----*/
                            atms[i].withdraw(&bank, account, password, amount_or_sec_account);
                        case 'B':
                            /*------ balance check -----*/
                            atms[i].balance_check(&bank, account, password);
                        case 'Q':
                            /*----- closing account ----*/
                            atms[i].quit_account(&bank, account, password);
                        case 'T':
                            /*---- make a transaction---*/
                            atms[i].transaction(&bank,
                                                account,
                                                password,
                                                amount_or_sec_account,
                                                amount_trans);
                        case default:
                            cerr << "got: " << operation << "we don't support" << endl;
                            return FAIL;
                    }
                    //go to sleep for 100 ms
                    this_thread::sleep_for(chrono::milliseconds(100));
                }
                //if(/* finish to run ATM_i */)
                //{
                //    break;
                //}
                pthread_exit(&ATM_threads[i]);
            }
           
        }
    }
    return SUCCESS;
}