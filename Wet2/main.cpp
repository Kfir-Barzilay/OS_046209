#ifndef MAIN_CPP
#define MAIN_CPP
#include "bank.hpp"
#include "ATM.hpp"
#include "account.hpp"
#include "rwlock.hpp"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <random>



using namespace std;

/* global */
pthread_mutex_t log_mutex;

struct ThreadArgs {
    bank_t &bank;
    char *file_name;
    int atm_id;
    // Add more arguments as needed
};

int main(int argc, char *argv[])
{
    /*---------- init new file ----------*/
    remove(LOG_FILE);
    ofstream outputFile(LOG_FILE);
    outputFile.close();

    /* ------------ init bank -----------*/
    bank_t bank(argc);
    ThreadArgs* args = new ThreadArgs;
    args->bank = &bank;
    args->file_name = NULL;
    args->atm_id = 0;
    /* ------------ init bank -----------*/
    pthread_t bank_commission_thread;
    if(pthread_create(&bank_commission_thread,NULL,bank_commisions_func, args) != 0)
    {
        sys_Error("pthread_create");
        return FAIL;
    }
   
    /*------------ print status -------------*/
    pthread_t bank_status_thread;
    if(pthread_create(&bank_status_thread,NULL,bank_commisions_func, args) != 0)
    {
        sys_Error("pthread_create");
        return FAIL;
    }
   
    
    /*------- take care of the inputs ----*/

    int n = argc;// number of ATMs
    pthread_t ATM_threads[n];
    for(int i = 0; i < n; n++)
    {
        args->file_name = argv[i];
        args->atm_id = i;
        if(pthread_create(&(ATM_threads[i]), NULL, ATM_func, args) != 0)
        {
            sys_err("pthread_create");
            return FAIL;
        }
    }

    while(bank->isAlive())

    return SUCCESS;
}

void* bank_status_func(void* args)//bank_t& bank
{
    while(((thread_args*)args)->bank->isAlive())
    {
        ((thread_args*)args)->bank->status();

        this_thread::sleep_for(chrono::milliseconds(500));
    }
    pthread_exit(NULL);
}

void* bank_commisions_func(void* args) //bank_t& bank
{
    while(((thread_args*)args)->bank->isAlive())
    {
        ((thread_args*)args)->bank->commissions();

        sleep(3);
    }
    pthread_exit(NULL);
}

void* ATM_func(void* args) //bank_t& bank , file , int id 
{
    bank_t& bank = ((thread_args*)args)->bank;
    ATM_t myATM = ATM(((thread_args*)args)->atm_id);
    string line;
    string word;
    ifstream file(((thread_args*)args)->file_name);
    if (!file.is_open()) { // Check if the file was opened successfully
        cerr << "Error opening file: " << argv[i] << endl;
        pthread_exit(NULL);
        return;
    }  
    
    while (getline(file, line)) {
        // Create a stringstream from the line
        stringstream ss(line);
        vector<string> words;  // Vector to store separated words

        // Split the line into words based on spaces
        while (ss >> word) {
            words.push_back(word);
        }
        int amount_trans=-1000000;
        int amount_or_sec_account=-10000000;
        string operation = *(words.front());
        words.erase(words.front());
        int account = stoi(words.front());
        words.erase(words.front());
        int password = stoi(words.front());
        words.erase(words.front());
        if(!words.empty())
        {
            amount_or_sec_account = stoi(words.front());//for transaction it will be account_reciever 
            words.erase(words.front());
        }
        if(!words.empty())
        {
            amount_trans = stoi(words.front());
            words.erase(words.front());
        }
        switch (operation[0])
        {
            case 'O':
                /*------ open acount -------*/
                myATM.open_account(bank, account, password, amount_or_sec_account);
            case 'D':
                /*----- make a deposite ----*/
                myATM.deposit(bank, account, password, amount_or_sec_account);
            case 'W':
                /*----- make a withdraw ----*/
                myATM.withdraw(bank, account, password, amount_or_sec_account);
            case 'B':
                /*------ balance check -----*/
                myATM.balance_check(bank, account, password);
            case 'Q':
                /*----- closing account ----*/
                myATM.quit_account(bank, account, password);
            case 'T':
                /*---- make a transaction---*/
                myATM.transaction(bank,
                                    account,
                                    password,
                                    amount_or_sec_account,
                                    amount_trans);
            default:
                cerr << "got: " << operation << "we don't support" << endl;
                bank.ATMs_alive--;
                pthread_exit(NULL);
                return NULL;
        }
        //go to sleep for 100 ms
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    bank.ATMs_alive--;
    pthread_exit(NULL);
    return NULL;
}


#endif /*MAIN_CPP*/