
#define MAX_JOBS 100
#include <iostream>
#include <unistd.h>
#include <string>
#include "smash.hpp"

using namespace std;

smash_t::smash_t(/* args */)
{
    this->pid_num = getpid();
    this->last_path = "";
    this->job_counter = 0;
    this->max_job_id = 0;
    this->jobs_array = new job*[MAX_JOBS];
    for (int i = 0; i < MAX_JOBS; i++) {
        jobs_array[i] = NULL;
    }
}

smash_t::~smash_t()
{
    delete(this->jobs_array);
}

pid_t smash_t::get_pid_num()
{
    return this->pid_num;
}

string smash_t::get_last_path()
{
    return this->last_path;
}

void smash_t::set_last_path(const string path)
{
    this->last_path = path;
}

int smash_t::get_job_counter()
{
    return this->job_counter;
}


void smash_t::job_insert(string command, int status, int pID)
{
    if (this->get_job_counter() >= 100) { 
        cout << "too many jobs in array" << endl;
        return;
    }
    int job_ID = this->max_job_id + 1;
    this->max_job_id++;
    job* new_job = new job(command, job_ID , status, pID); 
    this->jobs_array[job_counter] = new_job;
    this->job_counter++;
}

void smash_t::job_remove(int job_id)
{
    int index = 0;
    while (this->jobs_array[index] != NULL && ) {
        //print job doesnt exist
        return;
    }
    delete(this->jobs_array[index]);
    this->jobs_array[index] = NULL;
    this->job_counter--;
    jobs_sort();
}

//NULL is returned if not exist or invalid ID
job smash_t::job_get(int job_id) ; 
{
    int index = 0;
    while (*(this->jobs_array[index]).get_) {
        
    }
    job a = *(this->jobs_array[])
    int index = job_id - 1;
    if (job_id > 100 or job_id < 1) {
        return NULL;
    }
    return (this->jobs_array)[index];
}

void smash_t::print_jobs()
{
    time_t curr_time;
    time(&curr_time);
    for (int i =0; i< MAX_JOBS; i++) {
        if ((this->jobs_array)[i] != NULL) {
           (*((this->jobs_array)[i])).print_job(curr_time);
        }
    }
}

void smash_t::jobs_sort(int place_in_array)
{
    for(int i = 0; i < MAX_JOBS; i++)
    {
        int j = i + 1;
        if(this->jobs_array[i] == NULL && this->jobs_array[j] != NULL)
        {
            jobs_array[i] = jobs_array[j];
        }
    }
}
