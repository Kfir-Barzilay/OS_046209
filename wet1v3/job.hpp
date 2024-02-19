
#include <iostream>
#include <string>
#define MAXJOBS 100
#define MAXARGS 20
#define INVALID -1
using namespace std;



class job
{
public:
    int job_id;
    pid_t pid;
    string command;
    string args[MAXARGS];
    time_t start_time;// we can use time(pointer) return secondes passed from midnight or clock() return the clock cycles passed
    bool is_background;
    bool is_stopped;

    job();
    job(int job_id
        pid_t pid, 
        string command, 
        string args[MAXARGS], 
        bool is_background, 
        bool is_stopped);
    job(const job& a);
    void print_job(time_t current_time);
};

bool compareByJob_id(const job& a, const job& b);
bool compareByPid(const job& a, const job& b);
int job_index(int job_id);
int max_job_id();
int highest_stopped_id();
void refresh();

void insert_job(pid_t pid, 
            string command, 
            string args[MAX_ARG], 
            bool is_background, 
            bool is_stopped);
job pop_job(int job_id);
void remove_by_id(int job_id);
void remove_by_index(int index);

void print_all_jobs();
