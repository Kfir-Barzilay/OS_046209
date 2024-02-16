
#define MAX_JOBS 100
#include <iostream>
#include <unistd.h>
#include <string>
#include "smash.hpp"

using namespace std;

int find_job_index(int job_id);

int find_job_index(int job_id)
{
int index = 0;
    job* p_curret_job = this->jobs_array[index];
    if (p_curret_job == NULL) {
        //empty arr, job doesnt exist
        return -1;
    }
    while ((*p_curret_job).get_job_id() != job_id) {
        index++;
        if (index == MAX_JOBS){
            return -1; //ran on all the array
        }
        p_curret_job = this->jobs_array[index];         
        if (p_curret_job == NULL) {
            //job doesnt exist, 
            return -1;
        }
    } //quits the loop when the job id is identical
    return index;
}

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

//return job* of the job. return NULL in not exist
job* smash_t::job_access(int job_id)
{
    int index = find_job_index(job_id);
    if(index == -1)
    {
        return NULL;
    }
    job* return_p_job = this->jobs_array[index];
    return return_p_job;
} 

job* smash_t::job_get(int job_id) 
{
    //gives the user the job, he needs to free the memory
    int index = find_job_index(job_id);
    job* return_p_job = this->jobs_array[index];
    this->jobs_array[index] = NULL;
    this->job_counter--;
    //sorts the jobs
    this->jobs_sort_id(index);
    //sets the max job id to the highest id
    index = MAX_JOBS - 1;
    while (this->jobs_array[index] == NULL && index > 0) {
        index--;
    }
    if (this->jobs_array[index] == NULL) {
        this->max_job_id = 0;
    }
    else {
        this->max_job_id = *(this->jobs_array[index])->get_job_id();
    }
    //return the wanted output
    return return_p_job;
}

void smash_t::job_remove(int job_id)
{
    job* job_to_remove = this->job_get(job_id);
    if (job_to_remove == NULL) {
        return;
    }
    delete(job_to_remove);
    return;
}

//NULL is returned if not exist or invalid ID


void smash_t::print_jobs()
{
    time_t curr_time;
    time(&curr_time);
    for (int i =0; i< MAX_JOBS; i++) {
        if ((this->jobs_array)[i] != NULL) {
           (*(this->jobs_array[i])).print_job(curr_time);
        }
    }
}

void smash_t::jobs_sort_id(int place_in_array)
{
    for(int i = 0; i < MAX_JOBS; i++)
    {
        int j = i + 1;
        if(this->jobs_array[i] == NULL && this->jobs_array[j] != NULL)
        {
            jobs_array[i] = jobs_array[j];
            jobs_array[j] = NULL;
        }
    }
}



/*this func need to return a job_id with status value of stopped 
(we defined stopped as 3), check from the highest job_id to the lowest. 
if there is no job with this status return -1, for not exist)*/
int smash::highest_stopped_job()
{
    index = this->jobs_counter - 1;
    while (index > 0) {
        job* p_curr_job = this->jobs_array[index]
        if (p_curr_job == NULL) {
            return -1;
        }

        if ((*p_curr_job).get_status() == 3) {
            return (*p_curr_job).get_id();
        }
        index--;
    }

    return -1;
}