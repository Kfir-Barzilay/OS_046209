
#include <iostream>
#include <string>
#define MAX_ARG 20
using namespace std;

class job
{
private:
    char* command;
    char *args[MAX_ARG];
    int job_id;
    pid_t pid;
    time_t start_time;// we can use time(pointer) return secondes passed from midnight or clock() return the clock cycles passed
    bool is_background;
    bool is_stopped;
public:
    job(const char* command,
        const char** args,  
        const int job_id, 
        const pid_t pid, 
        const bool is_background, 
        const bool is_stopped
    );
    job(const job& a);
    ~job();
    char* get_command();
    char** get_args();
    int get_job_id();
    pid_t get_pid();
    time_t get_start_time();
    bool get_is_background();
    bool get_is_stopped();
    void set_is_background(bool value);
    void set_is_stopped(bool value);
    void print_job(time_t current_time);
};