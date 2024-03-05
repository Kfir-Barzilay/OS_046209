#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef class account
{
    private:
    int id;
    int password;
    int balance;
    RWlock lock;
    
    public:
    account(int id, int password, int balance);
    ~account();
    bool isAccount(int id);
    bool isPassword(int password);
    int read_balance();
    int write_password();
};

int open_account(int id, int password, int initial_sum); 
int deposit(int id, int password, int amount);
int withdraw(int id, , int password, int amount);
int balance_check(int id, int password);
int quit_account(int id, int password);
int transaction(int id_sender,
                int password_sender, 
                int id_receiver, 
                int password_receiver, 
                int amount);



