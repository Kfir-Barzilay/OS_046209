
#include <iostream>
#include <string>

using namespace std;

class job
{
private:
    string command;
    int job_id;
    int pid;
    time_t start_time;// we can use time(pointer) return secondes passed from midnight or clock() return the clock cycles passed
    int status; // 1- foreground, 2 - background, 3 - stopped
public:
    job(const string str, const int job_id, const int status, const int pID);
    job(const job& a);
    //no heap allocations no need for destructor
    string get_command();
    int get_job_id();
    int get_pid();
    int get_start_time();
    int get_status();
    void set_status(int status);
    void print_job(time_t current_time);
    bool lowerByPID (const job* a);
};