
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <ctime>
#include <vector>
#include <iterator>
#include <cctype>
#include <stdio.h>
#include <errno.h>


#include "job.hpp"

extern job jobs[MAXJOBS];
extern pid_t smash_pid;

using namespace std;

#define STOPPED 3



job::job()
{
    this->job_id = INVALID;
    this->pid = INVALID;
    this->command = "";
    for (int i = 0; i< MAXARGS; i++) {
        this->args[i] = "";
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
    if (args == NULL) {
        for (int i=0; i < MAXARGS; i++){
            this->args[i] = "";
        }
    }
    else {
        for (int i=0; i < MAXARGS; i++){
            this->args[i] = args[i];
        }
    }
    time(&this->start_time);
    this->is_background = is_background;
    this->is_stopped = is_stopped;
}

job::job(const job& other)
{   
    this->job_id = other.job_id; 
    this->pid = other.pid;
    //cout << "1" << endl;
    this->command = other.command;
   // cout << "2" << endl;
    if (other.args == NULL) {
        for (int i = 0; i < MAXARGS; i++){
            this->args[i] = "";
        }
    }
    else {
        for (int i = 0; i < MAXARGS; i++){
            this->args[i] = other.args[i];
        }
    }
    //cout << "3" << endl;
    this->start_time = other.start_time;
    this->is_background = other.is_background;
    this->is_stopped = other.is_stopped;
    //cout << "4" << endl;
}

bool job::operator<(const job& b)// is a < b ?
{
    if (this->job_id == INVALID) { //a > b 
        return false;
    }
    else if (b.job_id == INVALID) {// a < b
        return true;
    }
    return (this->job_id < b.job_id);
}

void job::print_job(time_t current_time)
{
    int time_passed = difftime(current_time, this->start_time);
    if (is_background) {
        cout <<"[" << this->job_id <<"] " << this->command << " & : ";
        cout << this->pid <<" " << time_passed << " secs";
    }
    else {
        cout <<"[" << this->job_id <<"] " << this->command << " : ";
        cout << this->pid <<" " << time_passed << " secs";
    }
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
    else if (b.job_id == INVALID) {
        return true;
    }
    return (a.job_id < b.job_id);
}

bool compareByPid(const job& a, const job& b)
{
    if (a.pid == INVALID) {
        return false;
    }
    else if (b.pid == INVALID) {
        return true;
    }

    return (a.pid < b.pid);
}

//finds job index, returns -1 if doesnt exist
int job_index(int job_id)
{
    for (int i = 0; i < MAXJOBS ; i++) {
        if (jobs[i].job_id == job_id) {
            return i;
        }
    }
    return INVALID;
}


bool job_in_fg(pid_t pid)
{
    pid_t fg_pid = getpid();
    return pid == fg_pid;
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

//doing wait on all jobs... 
//if terminated then remove
//if stopped or working then update array
//sort 
void refresh()
{
    //update jobs 
    int status;
    for (int i = 0; i < MAXJOBS; i++)
    {
        if (jobs[i].pid > 0) {
            pid_t result = waitpid(jobs[i].pid, &status, WNOHANG | WUNTRACED);
            if (result == -1) {
                sys_err(WAITPID); //----------------------
                return;
            } else if (result > 0) {
                // Child process state has changed
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    remove_by_index(i);
                    break;
                } else if (WIFSTOPPED(status)) {
                    jobs[i].is_stopped = true;
                }   else if (WIFCONTINUED(status)) { //doesnt works :(
                jobs[i].is_stopped = false;
                }
            } else {
                // Child process is still running
            }
        }
    }
    //sort jobs (min sort)
    for (int i = 0; i < MAXJOBS; i++) {
        int min_index = i;
        job min_job = jobs[i]; 
        for (int j = i + 1; j < MAXJOBS; j++) {
            if (jobs[j] < jobs[min_index]) {
                min_index = j;
                min_job = jobs[j];
            }
        } 
        jobs[min_index] = jobs[i];
        jobs[i] = min_job;           
    }
   // current_pid =smash_pid; //no proc in the fg
}

int insert_job(const job& a)
{
    refresh();
    if (jobs[MAXJOBS - 1].job_id != INVALID) {//full list
        return FAILED;
    }
    jobs[MAXJOBS - 1] = job(a.job_id,
                            a.pid,
                            a.command,
                            a.args,
                            a.is_background,
                            a.is_stopped);
    refresh();
    return SUCCESS;
}
int insert_job(pid_t pid, 
            string command, 
            string args[MAXARGS], 
            bool is_background, 
            bool is_stopped)
{
    refresh();
    if (jobs[MAXJOBS - 1].job_id != INVALID) {//full list
        return FAILED;
    }
    int id = max_job_id();
    id = (id == INVALID) ? 1 : id + 1;
    jobs[MAXJOBS - 1] = job(id,
                            pid,
                            command,
                            args,
                            is_background,
                            is_stopped);
    refresh();
    return SUCCESS;
}

job pop_job(int job_id)
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
    for (int i =0; i< MAXJOBS; i++) {
        if (jobs[i].job_id != INVALID) {
            jobs[i].print_job(curr_time);
        }
    }
}

void sys_err(int sys_call)
{
    switch (sys_call)
    {
    case GETCWD:
        perror("smash error: getcwd failed");
        break;

    case CHDIR:
        perror("smash error: chdir failed");
        break;

    case KILL:
        perror("smash error: kill failed");
        break;

    case WAITPID:
        perror("smash error: waitpid failed");
        break;

    case FORK:
        perror("smash error: fork failed");
        break;
    
    case EXECV:
       perror("smash error: execv failed");
       break;
    
    case GETPID:
        perror("smash error: getpid failed");
        break;
        
    case FOPEN:
        perror("smash error: fopen failed");
        break;

    case FCLOSE:
        perror("smash error: fclose failed");
        break; 
    
    case SIGPROCMASK:
        perror("smash error: sigprocmask failed");
        break;
    
    case SIGFILLSET:
        perror("smash error: sigfillset failed");
        break;

    case SIGACT:
        perror("smash error: sigaction failed");
        break;

    default:
        cout << "something aint good for " << sys_call << endl; //debug line
        break;
    }
}

void smash_err (string cmd, string error_msg)
{
    cerr << "smash error: " << cmd << ": " << error_msg << endl;
}