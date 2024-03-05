#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ATM.cpp"
typedef class ATM
{
public:
    int ATM_id;

    ATM(int ATM_id);
    ~ATM();
    int open_account(&bank, int id, int password, int initial_sum); 
    int deposit(int id, int password, int amount);
    int withdraw(int id, , int password, int amount);
    int balance_check(int id, int password);
    int quit_account(int id, int password);
    int transaction(int id_sender,
                int password_sender, 
                int id_receiver, 
                int password_receiver, 
                int amount);
    void printError(string error);
};



void sysError(string sys_call);


