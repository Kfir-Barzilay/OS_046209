#include "ATM.hpp"
#define DEBUG 0
#define SLEEP_TIME 1000 
#define GREEN   "\033[32m"      /* Green */
#define RESET   "\033[0m"
extern pthread_mutex_t log_mutex;
extern ofstream* outputFile;

/*last min add to enable multi threding in transaction*/
vector<int> ids;
pthread_mutex_t mutex_ids;
pthread_mutex_init(&mutex_ids, nullptr);



ATM::ATM(int ATM_id)
{
    this->ATM_id = ATM_id; 
}

int ATM::open_account(bank_t &myBank, int account_id, int password, int initial_sum)
{
    if (DEBUG) cout << GREEN << "Openning" << RESET << endl;
    account_t new_account = account(    account_id,
                                        password, 
                                        initial_sum);
    myBank.write_lock();
    auto it = myBank.accounts_map.find(account_id);
    if (it != myBank.accounts_map.end()) {
        string err_msg = "account with the same id exists";
        this->printError(err_msg); 
        sleep_ms(SLEEP_TIME);
        myBank.write_unlock();
    }
    else {
        myBank.accounts_map.insert(make_pair(account_id, new_account));
        std::string msg = "New account id is ";
        msg += to_string(account_id);
        msg += " with password ";
        msg += to_string(password);
        msg += " and initial balance ";
        msg += to_string(initial_sum);
        this->printLog(msg);
        sleep_ms(SLEEP_TIME);
        myBank.write_unlock();
    }
    return SUCCESS;
}

int ATM::deposit(bank_t &myBank, int id, int password, int amount)
{
    if (DEBUG) cout << GREEN << "Depositing" << RESET <<endl;
    myBank.read_down();
    auto it = myBank.accounts_map.find(id);
    if(it == myBank.accounts_map.end());
    {
        string err_msg = "account id ";
        err_msg += to_string(id);
        err_msg += " does not exist";
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        myBank.read_up();
        return FAIL;
    }
    account_t *account = &(it->second); //kfir says refrence isnt needed
    
   
    account->write_lock();
    int status = account->write_balance(password, amount);
    if(status == FAIL)
    {
        string err_msg = "password for account id ";
        err_msg += to_string(id);
        err_msg += " is incorrect";
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        account->write_unlock();
        myBank.read_up();
        return FAIL;
    }
    string msg = " Account";
    msg += to_string(id);
    msg += " new balance is ";
    msg += to_string(status);
    msg += " after ";
    msg += to_string(amount);
    msg += " $ was deposited";
    this->printLog(msg);
    sleep_ms(SLEEP_TIME);
    account->write_unlock();
    myBank.read_up();
    return SUCCESS;
}

int ATM::withdraw(bank_t &myBank, int id ,int password, int amount)
{
    if (DEBUG) cout << GREEN << "Withdrawing" << RESET << endl;
    if(amount < 0) {
        return FAIL;
    }
    myBank.read_down();
    auto it = myBank.accounts_map.find(id);    
    if(it == myBank.accounts_map.end())
    {
        string err_msg = "account id ";
        err_msg += to_string(id);
        err_msg += " does not exist";
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        myBank.read_up();
        return FAIL;
    }
    account_t *account = &(it->second);
    account->write_lock();
    int status = account->write_balance(password, amount);
    if(status == -1)
    {
        string err_msg = "password for account id ";
        err_msg += to_string(id);
        err_msg += " is incorrect";
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        account->write_unlock();
        myBank.read_up();
        return FAIL;
    }
    else if(status == -2)
    {
        string err_msg = "account id ";
        err_msg += to_string(id);
        err_msg += " balance is lower than ";
        err_msg += to_string(amount);
        this->printError(err_msg);
        sleep_ms(SLEEP_TIME);
        account->write_unlock();
        myBank.read_up();
        return FAIL;
    }
    else
    {
        string msg = "Account ";
        msg += to_string(id);
        msg += " new balance is ";
        msg += to_string(status);
        msg += " after ";
        msg += to_string(amount);
        msg += " $ was withdrew";
        this->printLog(msg);
        sleep_ms(SLEEP_TIME);
        account->write_unlock();
        myBank.read_up();
        return SUCCESS;
    }

}

int ATM::balance_check(bank_t &myBank,int id, int password)
{
    if (DEBUG) cout << GREEN << "Checking"  << RESET <<endl;
    myBank.read_down();
    auto it = myBank.accounts_map.find(id);
    if(it == myBank.accounts_map.end())
    {
        string err_msg = "account id ";
        err_msg += to_string(id);
        err_msg += " does not exist";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank.read_up();
        return FAIL;
    }
    account_t *account = &(it->second);
    account->read_down();
    int balance = account->read_balance(password);
    if(balance == FAIL)
    {
        string err_msg = "password for account id ";
        err_msg += to_string(id);
        err_msg += " is incorrect";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        account->read_up();
        myBank.read_up();
        return FAIL;
    }
    else
    {
        string msg = "Account ";
        msg += to_string(id);
        msg += " balance is ";
        msg += to_string(balance);
        this->printLog(msg);
        sleep_ms(SLEEP_TIME);
        account->read_up();
        myBank.read_up();
        return SUCCESS;
    }

}
int ATM::quit_account(bank_t &myBank, int id, int password)
{
    if (DEBUG) cout << GREEN << "Quiting"  << RESET <<endl;
    myBank.write_lock();
    auto it = myBank.accounts_map.find(id);
    if(it == myBank.accounts_map.end())
    {
        string err_msg = "account id ";
        err_msg += to_string(id);
        err_msg += " does not exist";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank.write_unlock();
        return FAIL;
    }
    int balance = it->second.read_balance(password);
    if(balance == FAIL)
    {
        string err_msg = "password for account id ";
        err_msg += to_string(id);
        err_msg += " is incorrect";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank.write_unlock();
        return FAIL;
    }
   
    myBank.accounts_map.erase(it);
    string msg = "Account ";
    msg += to_string(id);
    msg += " is now closed. Balance was ";
    msg += to_string(balance);
    this->printLog(msg);

    sleep_ms(SLEEP_TIME);
    myBank.write_unlock();
    return SUCCESS;
}

int ATM::transaction(bank_t &myBank, 
                int id_sender,
                int password_sender, 
                int id_receiver, 
                int amount)
{
    if (id_sender == id_receiver) {
        /*illigal transaction*/
        cout << "Transaction: Account cant transfer to himself,";
        cout << " aborting" << endl; 
        exit(1);
    }
    if (DEBUG) cout << GREEN << "transfering" << RESET <<endl;
    if(amount < 0) {
        return FAIL;
    }
    myBank.read_down();
    //-------------- checking if exist ----------------//
    auto it_sender = myBank.accounts_map.find(id_sender);
    auto it_receiver = myBank.accounts_map.find(id_receiver);
    if(it_sender == myBank.accounts_map.end())
    {
        string err_msg = "account id ";
        err_msg += to_string(id_sender);
        err_msg += " does not exist";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank.read_up();
        return FAIL;
    }
    if (it_receiver == myBank.accounts_map.end())
    {
        string err_msg = "account id ";
        err_msg += to_string(id_receiver);
        err_msg += " does not exist";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        myBank.read_up();
        return FAIL;
    }
    while(true)
    {
        pthread_mutex_lock(&mutex_ids);
        if (isInVector(ids, id_sender) || isInVector(ids, id_receiver))
        {
            pthread_mutex_unlock(&mutex_ids);
            continue;
        }
        ids.push_back(id_receiver);
        ids.push_back(id_sender);
        pthread_mutex_unlock(&mutex_ids);
        break;
    }
    /*---------- withdrawing ------------------------*/
    account_t *account_sender = &(it_sender->second);
    account_sender->write_lock();
    int status_s = account_sender->write_balance(MASTER_PASSWORD, -1*amount);
    if(status_s == -1)
    {
        string err_msg = "password for account id ";
        err_msg += to_string(id_sender);
        err_msg += " is incorrect";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        account_sender->write_unlock();
        pthread_mutex_lock(&mutex_ids);
        ids.erase(std::remove(ids.begin(), ids.end(), id_receiver), ids.end());
        ids.erase(std::remove(ids.begin(), ids.end(), id_sender), ids.end());
        pthread_mutex_unlock(&mutex_ids);
        myBank.read_up();
        return FAIL;
    }
    else if(status_s == -2)
    {
        string err_msg = "account id ";
        err_msg += to_string(id_sender);
        err_msg += " balance is lower than ";
        err_msg += to_string(amount);
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        account_sender->write_unlock();
        pthread_mutex_lock(&mutex_ids);
        ids.erase(std::remove(ids.begin(), ids.end(), id_receiver), ids.end());
        ids.erase(std::remove(ids.begin(), ids.end(), id_sender), ids.end());
        pthread_mutex_unlock(&mutex_ids);
        myBank.read_up();
        return FAIL;
    }
    //------------------ deposit ------------------//
    account_t *account_receiver = &(it_receiver->second);    
    account_receiver->write_lock();
    int status_r = account_receiver->write_balance(MASTER_PASSWORD , amount);
    if(status_r == FAIL) //shouldnt be here
    {
        string err_msg = "MASTER_PASSWORD doesnt works";
        this->printError(err_msg);

        sleep_ms(SLEEP_TIME);
        account_receiver->write_unlock();
        pthread_mutex_lock(&mutex_ids);
        ids.erase(remove(ids.begin(), ids.end(), id_receiver), ids.end());
        ids.erase(remove(ids.begin(), ids.end(), id_sender), ids.end());
        pthread_mutex_unlock(&mutex_ids);
        myBank.read_up();
        return FAIL;
    }
    string msg = "Transfer ";
    msg += to_string(amount);
    msg += " from account ";
    msg += to_string(id_sender);
    msg += " to account ";
    msg += to_string(id_receiver);
    msg += " new account balance is ";
    msg += to_string(status_s);
    msg += " new target account balance is ";
    msg += to_string(status_r);
    this->printLog(msg);

    sleep_ms(SLEEP_TIME);
    account_receiver->write_unlock();
    account_sender->write_unlock();
    pthread_mutex_lock(&mutex_ids);
    ids.erase(remove(ids.begin(), ids.end(), id_receiver), ids.end());
    ids.erase(remove(ids.begin(), ids.end(), id_sender), ids.end());
    pthread_mutex_unlock(&mutex_ids);
    
    myBank.read_up();
    return SUCCESS;
}

void ATM::printError(string error)
{
    //if (DEBUG) cout << "-----------Error log print ---------------" << endl;
    pthread_mutex_lock(&log_mutex);
    if (outputFile->is_open())
    {
        *outputFile << "Error " << this->ATM_id << ": Your transaction failed - ";
        *outputFile << error << endl; 
    }
    pthread_mutex_unlock(&log_mutex);
}

void ATM::printLog(string msg) /*not completed write fo log is required*/
{
    //if (DEBUG) cout << "----------- log print ---------------" << endl;
    pthread_mutex_lock(&log_mutex);
    if (outputFile->is_open())
    {
        *outputFile << this->ATM_id << ": " << msg << endl; 
    } 
    pthread_mutex_unlock(&log_mutex);
}

void sleep_ms (int sleep_time_ms) {
    usleep(1000 * sleep_time_ms);
}

bool isInVector(const vector<int>& vec, int value) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == value) {
            return true; // Found the value in the vector
        }
    }
    return false; // Value not found in the vector
}