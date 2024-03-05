
#define EMPTY -1
#define ERROR -1

rwlock::rwlock(/* args */)
{
    this->atm_id = EMPTY;
    counter = 0;
    int result = sem_init(&(this->sem) , 1);
    if (result == ERROR) {
        sys_error("sem_init");
        return FAILURE;
    }
}

rwlock::~rwlock()
{
}

rwlock::lock(char op) 
{
    if (accessing_atm_id == this->atm_id)
    {

    }
}