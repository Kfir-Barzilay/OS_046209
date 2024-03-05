#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <cmath>
#include "account.hpp"
#include "ATM.hpp"
#include "bank.hpp"

using namespace std 

typedef class bank
{
public:
    /**/
    list<account_t> accounts;
    /*mutex lock for adding members*/
    account_t bank_account;
    bank(/* args */);
    ~bank();
    take_money();
}bank_t;

bank::bank(/* args */)
{
}

bank::~bank()
{
}
