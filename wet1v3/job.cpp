#include <iostream>
#include <cstring>
#include <string>
#include <ctime>

#include "job.hpp"

extern job jobs[MAXJOBS];
using namespace std;

#define STOPPED 3

job::job()
{
    this->job_id = INVALID;
    this->pid = INVALID;
    this->command = "";
    for (int i = 0; i< MAXARGS; i++) {
        this->args = "";
    }
    time(&this->start_time);
    this->is_background = false;
    this->is_stopped = false;
}

job::job(const int job_id, 
        const pid_t pid,
        const string command,
        const string args[MAXARGS], 
        const bool is_background,
        const bool is_stopped
        )
{
    this->job_id = job_id; 
    this->pid = pid;
    this->command = command; 
    //set args
    for (int i; i < MAXARGS; i++){
        this->args[i] = args[i];
    }
    time(&this->start_time);
    this->is_background = is_background;
    this->is_stopped = is_stopped;
}

job::job(const job& other)
{   
    this->job_id = other.job_id; 
    this->pid = other.pid;
    this->command = other.command;
    for (int i; i < MAXARGS; i++){
        this->args[i] = other.args[i];
    }
    this->start_time = other.start_time;
    this->is_background = other.is_background;
    this->is_stopped = other.is_stopped;
}

void job::print_job(time_t current_time)
{
    int time_passed = difftime(current_time, this->start_time);
    cout <<"[" << this->job_id <<"] " << this->command << "& : ";
    cout << this->pid <<" " << time_passed << " secs";
    if(this->is_stopped){
        cout << " (stopped)";
    }
    cout << endl;
}

bool compareByJob_id(const job& a, const job& b)
{
    if (a.job_id == INVALID) {
        return false;
    }
    return (a.job_id < b.job_id);
}

bool compareByPid(const job& a, const job& b)
{
    if (a.job_id == INVALID) {
        return false;
    }
    return (a.pid < b.pid);
}

//finds job index, returns -1 if doesnt exist
int job_index(int job_id)
{
    for (int i = 0; i < MAXJOBS ; i++) {
        if jobs[i].job_id = job_id;
        return i;
    }
    return INVALID;
}

//return max job id, returns -1 if the list is empty
int max_job_id()
{
    int max = INVALID;
    for (int i = 0; i < MAXJOBS ; i++) {
        max = (jobs[i].job_id > max) ? jobs[i].job_id : max;
    }
    return max;
}

int highest_stopped_id()
{
   int max = INVALID;
    for (int i = 0; i < MAXJOBS ; i++) {
        if (jobs[i].is_stopped) {
            max = (jobs[i].job_id > max) ? jobs[i].job_id : max;
        }
    }
    return max; 
}

// sorts and removes dead programs-------------------------------------------
void refresh()
{
    //remove dead programs
    //--insert code
    //---------

    sort(begin(jobs), end(jobs), compareByJob_id)
}

int insert_job(pid_t pid, 
            string command, 
            string args[MAX_ARG], 
            bool is_background, 
            bool is_stopped)
{
    refresh();
    if (jobs[MAXJOBS - 1].job_id != INVALID) {//full list
        return FAILED;
    }
    int id = max_job_id() + 1;
    jobs[MAXJOBS - 1] = job(id,
                            pid,
                            command,
                            args,
                            is_background,
                            is_stopped)
    refresh();
    return SUCCESS;
}

job pop_job(int job_id);
{
    int index = job_index(job_id);
    job temp = jobs[index];
    jobs[index] = job(); //dead job
    refresh();
    return temp;
}

void remove_by_id(int job_id)
{
    int index = job_index(job_id);
    jobs[index] = job(); //dead job
    refresh();
}

void remove_by_index(int index) 
{
    jobs[index] = job();
    refresh();
}

void print_all_jobs()
{
    refresh();
    time_t curr_time;
    time(&curr_time);
    for (int i =0; i< MAX_JOBS; i++) {
        if ((this->jobs_array)[i] != NULL) {
           (*(this->jobs_array[i])).print_job(curr_time);
        }
    }
}