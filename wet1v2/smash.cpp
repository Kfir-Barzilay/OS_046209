
#define MAX_JOBS 100
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include "smash.hpp"

using namespace std;

char* string_cpy(const char* str)
{
    if (str == NULL) {
        return NULL;
    }
    size_t str_len = strlen(str) + 1;
    char* str_cpy = new char[str_len];
    strcpy(str_cpy, str);
    return str_cpy;
}

int smash_t::find_job_index(int job_id)
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
    this->pid = getpid();
    this->last_path = NULL;
    this->jobs_counter = 0;
    this->max_job_id = 0;
    this->jobs_array = new job*[MAX_JOBS];
    for (int i = 0; i < MAX_JOBS; i++) {
        jobs_array[i] = NULL;
    }
}

smash_t::~smash_t()
{
    if (this->last_path != NULL){
        delete(last_path);
    }
    for (int i = 0; i < MAX_JOBS; i++) {
        if(jobs_array[i] != NULL) {
            delete(jobs_array[i]);
        }
    }
    delete(this->jobs_array);
}

pid_t smash_t::get_pid() const
{
    return this->pid;
}

char* smash_t::get_last_path() const
{
    char* last_path_cpy = string_cpy(this->last_path);
    return last_path_cpy;
}
     
int smash_t::get_jobs_counter() const
{
    return this->jobs_counter;
}

int smash_t::get_max_job_id() const
{
    return this->max_job_id;
}

void smash_t::set_last_path(char* str)
{
    this->last_path = string_cpy(str);
}

void smash_t::set_jobs_counter(int num)
{
    this->jobs_counter = num;
}

void smash_t::set_max_job_id(int num)
{
    this->max_job_id = num;
}

void smash_t::job_insert(const char* command,
                        const char** args,
                        pid_t pid,
                        bool is_background,
                        bool is_stopped)
{
    if (this->jobs_counter >= 100) { 
        cerr << "too many jobs in array" << endl;
        return;
    }
    int job_id = this->max_job_id + 1;
    this->max_job_id++;
    job* new_job = new job(command,
                        args, 
                        job_id, 
                        pid, 
                        is_background, 
                        is_stopped); 
    this->jobs_array[jobs_counter] = new_job;
    this->jobs_counter++;
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
    if (index < 0) {
        return NULL;
    }
    job* return_p_job = this->jobs_array[index];
    this->jobs_array[index] = NULL;
    this->jobs_counter--;
    //sorts the jobs
    this->jobs_sort_id(index);
    //sets the max job id to the highest id
    index = MAX_JOBS - 1;
    while (this->jobs_array[index] == NULL && index > 0) { //stops at the first element
        index--;
    }
    if (this->jobs_array[index] == NULL) { //empty list
        this->max_job_id = 0;
    }
    else {
        this->max_job_id = (*(this->jobs_array[index])).get_job_id();
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

/*this func need to return a job_id with status value of stopped,
  check from the highest job_id to the lowest. 
  if there is no job with this status return -1, for not exist)*/
int smash_t::highest_stopped_job()
{
    int index = this->jobs_counter - 1;
    while (index > 0) {
        job* p_curr_job = this->jobs_array[index];
        if (p_curr_job == NULL) {
            return -1;
        }

        if ((*p_curr_job).get_is_stopped()) {
            return (*p_curr_job).get_job_id();
        }
        index--;
    }

    return -1;
}