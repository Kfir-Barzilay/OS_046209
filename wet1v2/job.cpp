#include <iostream>
#include <string>
#include <ctime>

#include "job.hpp"

using namespace std;

#define STOPPED 3

job::job(const string str, const int job_id, const int status, const int pID)
{
    this->command = str;
    this->job_id = job_id; 
    this->pid = pID;
    time(&this->start_time);
    this->status = status;
}

job(const job& a);
{
    this->command = a.get_command();
    this->job_id = a.get_job_id(); 
    this->pid = a.get_pid();
    this->start_time = a.get_start_time();
    this->status = a.get_status();
}

string job::get_command()
{
    return this->command;
}

int job::get_job_id()
{
    return this->job_id;
}

int job::get_pid()
{
    return this->pid;
}

int job::get_start_time()
{
    return this->start_time;
}

int job::get_status()
{
    return this->status;
}

void job::set_status(int status)
{
    this->status = status;
}

void job::print_job(time_t current_time)
{
    int time_passed = difftime(current_time, this->start_time);
    cout <<"[" << this->job_id <<"] " << this->command << " : ";
    cout << this->pid <<" " << time_passed << " secs";
    if(status == STOPPED){
        cout << " (stopped)";
    }
    cout << endl;
}

bool job::lowerByPID (const job* a) {
    if (a == NULL) { //null pointer is the greatest
        return true;
    }
    return this->pid < (*a).pid;
}   
