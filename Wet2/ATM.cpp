
#include "ATM.hpp"

ATM::ATM(ATM_id); 
{
    this->ATM_id = ATM_id;
}

int ATM::open_account(bank_t &myBank, int account_id, int password, int initial_sum)
{
    account_t new_account = account::account(   id,
                                                password, 
                                                initial_sum);
    /*lock to myBank add mutex*/
    account_t iter = myBank.accounts.head();
    list<account_t>::iterator it_account = myBank.accounts.begin();
    while (it_account != myBank.accounts.end()) {
        if (it_account.id == account 
        string err_msg = "account with the same id exists";
        this->printError(err_msg);
    }
    else 
    {
        /*this is not new critical section because when the element is 
        is added the data is valid*/
        
        myBank.accounts.push_back(new_account);
    }
    /*unlock read myBank delete semaphore*/
    /*unlock myBank add mutex*/
}

int ATM::deposit(bank_t &myBank, int id, int password, int amount)
{
    account_t* account = find_account_pointer

    /*write lock account*/
    if 


}

int withdraw(bank_t &myBank, int id, , int password, int amount)
{
    /* check with locks that the id_account is secure */
    account_t *account = *myBank.accounts
}
int balance_check(int id, int password)
{

}
int quit_account(int id, int password)
{

}
int transaction(int id_sender,
                int password_sender, 
                int id_receiver, 
                int password_receiver, 
                int amount)
{
        
}

void sysError(string sys_call)
{
    string error = "Bank error: ";
    error += sys_call;
    error += " failed";
    perror(error);
}
void printError(int id, string error)
{
    cerr << "Error " << id << ": " << error << endl; 
}