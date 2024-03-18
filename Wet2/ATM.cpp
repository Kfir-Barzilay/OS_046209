
#include "ATM.hpp"

ATM::ATM(ATM_id); 
{
    this->ATM_id = ATM_id;
}

int ATM::open_account(bank_t &myBank, int account_id, int password, int initial_sum)
{
    myBank->write_lock();
    account_t new_account = account::account(   id,
                                                password, 
                                                initial_sum);
    /*lock to myBank add mutex*/
    new_account.write_lock();
    int status = myBank->insert_account(new_account);
    auto it = myBank->accounts_map.find(id);
    if (status || it == myBank.end) {
        string err_msg = "account with the same id exists";
        this->printError(err_msg);
    }
    else {
        string msg = "New Account id is ";
        msg += id ;
        msg += " with password "
        msg += password;
        msg += " and initial balance ";
        msg += initial_sum;
        this->printLog(msg);
        it->second->write_unlock();
    }
    bank->write_unlock
}

int ATM::deposit(bank_t &myBank, int id, int password, int amount)
{
    myBank->read_down();
    auto it = myBank->accounts_map.find(id);
    if(it == myBank->accounts_map.end());
    {
        string err_msg = "Your transaction failed - account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);
        myBank.read_up();
        return FAIL;
    }
    account_t *account = it->second; //kfir says refrence isnt needed
    account_t *temp = &(it->second); //Roy says refrence is needed!!
   
    account.write_lock();
    int status = account->write_balance(password, amount);
    if(status == FAIL)
    {
        string err_msg = "Your transaction failed - password for account id ";
        err_msg += id;
        err_msg += " is incorrect";
        this->printError(err_msg);
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
        string err_msg = "Your transaction failed - account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);
        myBank->read_up();
        return FAIL;
    }
    account_t *account = it->second;
    account->write_lock();
    int status = account->write_balance(password, amount);
    if(status == -1)
    {
        string err_msg = "Your transaction failed - password for account id ";
        err_msg += id;
        err_msg += " is incorrect";
        this->printError(err_msg);
        account->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    else if(status == -2)
    {
        string err_msg = "Your transaction failed - account id ";
        err_msg += id;
        err_msg += " balance is lower than ";
        err_msg += amount;
        this->printError(err_msg);
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
        string err_msg = "Your transaction failed - account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);
        myBank->read_up();
        return FAIL;
    }
    account_t *account = it->second;
    account.read_down();
    int balance = account->read_balance(password, amount);
    if(balance == FAIL)
    {
        string err_msg = "Your transaction failed - password for account id ";
        err_msg += id;
        err_msg += " is incorrect";
        this->printError(err_msg);
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
        string err_msg = "Your transaction failed - account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);
        myBank->write_unlock();
        return FAIL;
    }
    int balance = it->second.read_balance(password);
    if(balance == FAIL)
    {
        string err_msg = "Your transaction failed - password for account id ";
        err_msg += id;
        err_msg += " is incorrect";
        this->printError(err_msg);
        myBank->write_unlock();
        return FAIL;
    }
   
    myBank->accounts_map.erase(it);
    string msg += " Account ";
    msg += id;
    msg += "is now closed. balance was ";
    msg += balance;
    this->printLog(msg);
    myBank->write_unlock();
    return SUCCESS;
}

int transaction(bank_t &myBank, 
                int id_sender,
                int password_sender, 
                int id_receiver, 
                int password_receiver, 
                int amount)
{
    if(amount < 0) {
        return FAIL;
    }
    //-------------- now withdraw ----------------//
    myBank->read_down();
    auto it = myBank->accounts.find(id_sender);
    if(it != myBank.accounts.end())
    {
        string err_msg = "Your transaction failed - account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);
        myBank->read_up();
        return FAIL;
    }
    account_t *account = it->second;
    account->write_lock();
    int status = account->write_balance(password, amount);
    if(status == -1)
    {
        string err_msg = "Your transaction failed - password for account id ";
        err_msg += id_sender;
        err_msg += " is incorrect";
        this->printError(err_msg);
        account->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    else if(status == -2)
    {
        string err_msg = "Your transaction failed - account id ";
        err_msg += id_sender;
        err_msg += " balance is lower than ";
        err_msg += amount;
        this->printError(err_msg);
        account->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    account->write_unlock();
    //------------------ deposit ------------------
    auto it = myBank->accounts_map.find(id_reciever);
    if(it == myBank->accounts_map.end());
    {
        string err_msg = "Your transaction failed - account id ";
        err_msg += id;
        err_msg += " does not exist";
        this->printError(err_msg);
        myBank.read_up();
        return FAIL;
    }
    *account = it->second; //kfir says refrence isnt needed
    account_t *temp = &(it->second); //Roy says refrence is needed!!
   
    account.write_lock();
    int status = account->write_balance(password, amount);
    if(status == FAIL)
    {
        string err_msg = "Your transaction failed - password for account id ";
        err_msg += id_reciever;
        err_msg += " is incorrect";
        this->printError(err_msg);
        account->write_unlock();
        myBank->read_up();
        return FAIL;
    }
    string msg = ""
    account->write_unlock();
    myBank.read_up();



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