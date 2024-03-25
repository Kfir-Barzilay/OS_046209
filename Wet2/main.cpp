#ifndef MAIN_CPP
#define MAIN_CPP
#include "bank.hpp"
#include "ATM.hpp"
#include "account.hpp"
#include "rwlock.hpp"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>
#include <vector>
#include <random>

#define BLUE    "\033[34m"      /* Blue */
#define RESET   "\033[0m"
#define PURPULE "\033[1;44m"
#define DEBUG 0
/* -------- threads functions -------- */
void* bank_commission_func(void* args);
void* bank_status_func(void* args);
void* ATM_func(void* args);



using namespace std;

/* global */
pthread_mutex_t log_mutex;
extern pthread_mutex_t mutex_ids;

/*---------- init new file ----------*/

ofstream* outputFile;

typedef struct ThreadArgs {
    bank_t *p_bank;
    ifstream *files;
    int atm_id;
    
    // Add more arguments as needed
}thread_args;

int main(int argc, char *argv[])
{
    
    int n = argc - 1;// number of ATMs
    if (n == 0) {//no params
        cerr << "Bank error: illegal arguments"<< endl;
        return FAIL;
    }
    thread_args** args = new thread_args*[n + 1];
    args[0] = new thread_args;
    for (int i = 1 ; i <= n ; i++)
    {
        args[i] = new thread_args;
        args[i]->files = new ifstream(argv[i]);
        if (!args[i]->files->is_open()) { // Check if the file was opened successfully
        cerr << "Bank error: illegal arguments"<< endl;
        return FAIL;
        }
    }  
    remove(LOG_FILE);
    ofstream outputFile1(LOG_FILE);
    outputFile = &outputFile1;

    /* ------------ init bank -----------*/
    if (DEBUG) cout << "generating Bank" << endl;
    bank_t myBank = bank(n);
    for (int i = 0 ; i <= n ; i++)
    {
        args[i]->p_bank = &myBank;
    }  
    /* ------------ init bank commission -----------*/
    pthread_t bank_commission_thread;
    if(pthread_create(&bank_commission_thread,NULL,bank_commission_func, args[0]) != 0)
    {
        sys_Error("pthread_create");
        return FAIL;
    }
    if (DEBUG) cout << "comm thread was created" << endl;
    /*------------ print status -------------*/
    pthread_t bank_status_thread;
    if(pthread_create(&bank_status_thread,NULL,bank_status_func, args[0]) != 0)
    {
        sys_Error("pthread_create");
        return FAIL;
    }
    if (DEBUG) cout << "status thread was created" << endl;
    
    /*------- take care of the inputs ----*/
    pthread_t *ATM_threads = new pthread_t[n + 1];
    for(int i = 1; i <= n; i++)
    {
        args[i]->atm_id = i;
        if(pthread_create(&(ATM_threads[i]), NULL, ATM_func, args[i]) != 0)
        {
            sys_Error("pthread_create");
            return FAIL;
        }
    }

    while(myBank.isAlive() || myBank.status_alive || myBank.comm_alive){}
    if (DEBUG) cout << "Bank is Dead" << endl;
    outputFile1.close();
    for (int i = 0; i <= n ; i++){
        delete(args[i]);
    }
    delete(args);
    delete(ATM_threads);
    return SUCCESS;
}

void* bank_status_func(void* args)//bank_t& bank
{
    ((thread_args*)args)->p_bank->status_alive = true;
    while(((thread_args*)args)->p_bank->isAlive())
    {
        ((thread_args*)args)->p_bank->status();

        usleep(500000);
    }
    ((thread_args*)args)->p_bank->status_alive = false;
    pthread_exit(NULL);
}

void* bank_commission_func(void* args) //bank_t& bank
{
    ((thread_args*)args)->p_bank->comm_alive = true;
    while(((thread_args*)args)->p_bank->isAlive())
    {
        ((thread_args*)args)->p_bank->commission();

        sleep(3);
    }
    ((thread_args*)args)->p_bank->comm_alive = false;
    pthread_exit(NULL);
}

void* ATM_func(void* args) //bank_t& bank , file , int id 
{
    int ATM_id = ((thread_args*)args)->atm_id;
    bank_t* p_bank = ((thread_args*)args)->p_bank;
    ifstream& file = *(((thread_args*)args)->files);
    ATM_t myATM = ATM(ATM_id);
    string line;
    string word;
    if (DEBUG) cout << BLUE << "ATM" << ATM_id << " was generated" << RESET << endl;
    while (getline(file, line)) {
       if(DEBUG) cout << PURPULE << line << RESET <<endl;
        // Create a stringstream from the line
        stringstream ss(line);
        vector<string> words;  // Vector to store separated words

        // Split the line into words based on spaces
        while (ss >> word) {
            words.push_back(word);
        }
        int amount_trans=-1000000;
        int amount_or_sec_account=-10000000;
        string operation = (words.front());
        words.erase(words.begin());
        int account = atoi(words.front().c_str());
        words.erase(words.begin());
        int password = atoi(words.front().c_str());
        words.erase(words.begin());
        if(!words.empty())
        {
            amount_or_sec_account = atoi(words.front().c_str());//for transaction it will be account_reciever 
            words.erase(words.begin());
        }
        if(!words.empty())
        {
            amount_trans = atoi(words.front().c_str());
            words.erase(words.begin());
        }

        char op = operation[0];
        if(op == 'O'){
            /*------ open acount -------*/
            myATM.open_account(*p_bank, account, password, amount_or_sec_account);
        }
        else if(op == 'D'){
            /*----- make a deposite ----*/
            myATM.deposit(*p_bank, account, password, amount_or_sec_account);
        }
        else if(op =='W'){
            /*----- make a withdraw ----*/
            myATM.withdraw(*p_bank, account, password, amount_or_sec_account);
        }
        else if(op == 'B'){
            /*------ balance check -----*/
            myATM.balance_check(*p_bank, account, password);
        }
        else if(op =='Q'){
            /*----- closing account ----*/
            myATM.quit_account(*p_bank, account, password);
        }
        else if(op =='T'){
            /*---- make a transaction---*/
            myATM.transaction(*p_bank,
                                account,
                                password,
                                amount_or_sec_account,
                                amount_trans);
        }
        else{
            cerr << "got: " << operation << " we don't support" << endl;
            p_bank->ATMs_alive--;
            pthread_exit(NULL);
            return NULL;
        }
        //go to sleep for 100 ms
        usleep(100000);
    }
    if (DEBUG) cout << BLUE << "ATM" << ATM_id << "is destroyed" << RESET << endl;
    p_bank->ATMs_alive--;
    pthread_exit(NULL);
    return NULL;
}


#endif /*MAIN_CPP*/