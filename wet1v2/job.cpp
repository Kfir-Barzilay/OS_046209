#include <iostream>
#include <cstring>
#include <string>
#include <ctime>

#include "job.hpp"

using namespace std;

#define STOPPED 3


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


job::job(const char* command,
        const char** args, 
        const int job_id, 
        const pid_t pid,
        const bool is_background,
        const bool is_stopped
        )
{
    //---set command
    this->command = string_cpy(command);
    //---set args 
    if (args == NULL) {
        this->args = NULL;
    }
    else {
        this->args = new char*[MAX_ARG];
        for (int i = 0; i < MAX_ARG ; i++) {
            this->args[i] = string_cpy(args[i]);
        }
    }
    //---set the rest;
    this->job_id = job_id; 
    this->pid = pid;
    time(&this->start_time);
    this->is_background = is_background;
    this->is_stopped = is_stopped;
}

job::job(const job& a);s
{   
    //---set command
    this->command = a.get("command");
    this->args = a.get("args");
    this->job_id = a.get("job_id"); 
    this->pid = a.get("pid");
    this->start_time = a.get("start_time");
    this->is_back_ground = a.get("is_back_ground");
    this->is_stopped = a.get("is_stopped");
}

job::~job 
{
    delete(this->command);
    for (int i = 0; i < MAX_ARG ; i++) {
        if (this->args[i] != NULL) {
            delete(this->args[i]);
        }
    }
    delete(this->args)
}

char* job::get_command()
{
    char *str_cpy = string_cpy(this->command);
    return str_cpy;
}


//char** always args
char** job::get_args()
{ 
    if (this->args == NULL) {
        return = NULL;
    }
    else {
        char**args_cpy = new char*[MAX_ARG];
        for (int i = 0; i < MAX_ARG ; i++) {
            args_cpy[i] = string_cpy(this->args[i]);
        }
    }
    return args_cpy;
}

int job::get_job_id() 
{   
    return this->job_id;
}

pid_t job::get_pid() 
{   
    return this->pid;
}

time_t job::get_start_time() 
{
    return this->start_time;
}

bool job::get_is_background()
{
    return this->is_background;
}

bool job::get_is_stopped()
{
    return this->is_stopped;
}

bool job::get_is_background()
{
    return this->is_background;
}

bool job::get_is_stopped()
{
    return this->is_stopped;
}

bool job::set_is_background(bool value)
{
    this->background = value;
}

bool job::set_is_stopped(bool value)
{
    this->stopped = value;
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
