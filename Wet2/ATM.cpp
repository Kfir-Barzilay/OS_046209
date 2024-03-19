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
    myBank->write_lock();
    auto it = myBank->accounts_map.find(id);
    if (it == myBank.end) {
        string err_msg = "account with the same id exists";
        this->printError(err_msg); 
        sleep_ms(SLEEP_TIME);
        bank->write_unlock();
    }
    else {
        myBank->account_map.insert(id, new_account)
        string msg = "New Account id is ";
        msg += id ;
        msg += " with password "
        msg += password;
        msg += " and initial balance ";
        msg += initial_sum;
        this->printLog(msg);
        sleep_ms(SLEEP_TIME);
        bank->write_unlock();
    }
    
}

int ATM::deposit(bank_t &myBank, int id, int password, int amount)
{
    myBank->read_down();
    auto it = myBank->accounts_map.find(id);
    if(it == myBank->accounts_map.end());
    {
        string err_msg = "account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        myBank.read_up();
        return FAIL;
    }
    account_t *account = it->second; //kfir says refrence isnt needed
    account_t *temp = &(it->second); //Roy says refrence is needed!!
   
    account.write_lock();
    int status = account->write_balance(password, amount);
    if(status == FAIL)
    {
        string err_msg = "password for account id ";
        err_msg += id;
        err_msg += " is incorrect";
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        account->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    string msg = " Account";
    msg += id;
    msg += " new balance is ";
    msg += status;
    msg += " after ";
    msg += amount;
    msg += " $ was deposited";
    sleep_ms(SLEEP_TIME);
    account->write_unlock();
    myBank.read_up();
    return SUCCESS;
}

int ATM::withdraw(bank_t &myBank, int id, , int password, int amount)
{
    if(amount < 0) {
        return FAIL;
    }

    myBank->read_down();
    auto it = myBank->accounts.map.find(id);    
    if(it == myBank->accounts.map.end())
    {
        string err_msg = "account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        myBank->read_up();
        return FAIL;
    }
    account_t *account = it->second;
    account->write_lock();
    int status = account->write_balance(password, amount);
    if(status == -1)
    {
        string err_msg = "password for account id ";
        err_msg += id;
        err_msg += " is incorrect";
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        account->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    else if(status == -2)
    {
        string err_msg = "account id ";
        err_msg += id;
        err_msg += " balance is lower than ";
        err_msg += amount;
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        account->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    else
    {
        string msg = " Account ";
        msg += id;
        msg += "new balance is ";
        msg += status;
        msg += " after ";
        msg += amount;
        msg += " $ was withdraw\n";
        this->printLog(msg);
        sleep_ms(SLEEP_TIME);
        account->write_unlock();
        myBank->read_up();
        return SUCCESS;
    }

}

int ATM::balance_check(bank_t &myBank,int id, int password)
{
    myBank->read_down();
    auto it = myBank->accounts_map.find(id);
    if(it == myBank->accounts_map.end();)
    {
        string err_msg = "account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank->read_up();
        return FAIL;
    }
    account_t *account = it->second;
    account.read_down();
    int balance = account->read_balance(password, amount);
    if(balance == FAIL)
    {
        string err_msg = "password for account id ";
        err_msg += id;
        err_msg += " is incorrect";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        account->read_up();
        myBank->read_up();
        return FAIL;
    }
    else
    {
        string msg = this->id;
        msg += ": Account ";
        msg += id;
        msg += " balance is ";
        msg += balance;

        sleep_ms(SLEEP_TIME);
        account->read_up();
        myBank->read_up();
        return SUCCESS;
    }

}
int quit_account(bank_t &myBank, int id, int password)
{
    myBank->write_lock();
    auto it = myBank->accounts_map.find(id);
    if(it == myBank->accounts_map.end())
    {
        string err_msg = "account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank->write_unlock();
        return FAIL;
    }
    int balance = it->second.read_balance(password);
    if(balance == FAIL)
    {
        string err_msg = "password for account id ";
        err_msg += id;
        err_msg += " is incorrect";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank->write_unlock();
        return FAIL;
    }
   
    myBank->accounts_map.erase(it);
    string msg += " Account ";
    msg += id;
    msg += "is now closed. balance was ";
    msg += balance;
    this->printLog(msg);

    sleep_ms(SLEEP_TIME);
    myBank->write_unlock();
    return SUCCESS;
}

int transaction(bank_t &myBank, 
                int id_sender,
                int password_sender, 
                int id_receiver, 
                int amount)
{
    if(amount < 0) {
        return FAIL;
    }
    //-------------- checking if exist ----------------//
    myBank->read_down();
    auto it_sender = myBank->accounts.find(id_sender);
    auto it_receiver = myBank->accounts.find(id_receiver);
    if(it_sender == myBank.accounts.end())
    {
        string err_msg = "account id ";
        err_msg += id_sender;
        err_msg += " does not exist";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank->read_up();
        return FAIL;
    }
    if (it_receiver == myBank.accounts.end())
    {
        string err_msg = "account id ";
        err_msg += id_receiver;
        err_msg += " does not exist";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank->read_up();
        return FAIL;
    }
    /*---------- withdrawing ------------------------*/
    account_t *account_sender = it_sender->second;
    account_sender->write_lock();
    int status_s = account_sender->write_balance(password, amount);
    if(status_s == -1)
    {
        string err_msg = "password for account id ";
        err_msg += id_sender;
        err_msg += " is incorrect";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        account_sender->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    else if(status_s == -2)
    {
        string err_msg = "account id ";
        err_msg += id_sender;
        err_msg += " balance is lower than ";
        err_msg += amount;
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        account_sender->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    //------------------ deposit ------------------//
    acount_t *account_receiver = it->second; //kfir says refrence isnt needed   
    account_receiver.write_lock();
    int status_r = account_receiver->write_balance(MASTER_PASSWORD , amount);
    if(status_r == FAIL) //shouldnt be here
    {
        string err_msg = "MASTER_PASSWORD doesnt works"
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        account->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    string msg = "Transfer ";
    msg += amount;
    msg += " from account ";
    msg += id_sender;
    msg += " to account ";
    msg += id_reciever;
    msg += " new account balance is ";
    msg += status_s;
    msg += " new target account balance is ":
    msg += status_r;
    printLog(msg);

    sleep_ms(SLEEP_TIME);
    account_receiver->write_unlock();
    account_sender->write_unlock();
    myBank.read_up();
}

void ATM::printError(string error)
{
    cerr << "Error " << this->id << ": Your transaction failed - ";
    cerr << error << endl; 
}

void ATM::printError(string error)
{
    cerr << this->id << ": " << error << endl; 
}

void sleep_ms (int sleep_time_ms) {
    this_thread::sleep_for(chrono::milliseconds(sleep_time_ms));
}