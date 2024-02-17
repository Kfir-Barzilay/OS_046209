#include <iostream>
#include <string>
#include "job.hpp"

#define MAX_JOBS 100

using namespace std;

class smash_t
{
private:
    pid_t pid;
    char* last_path;
    int jobs_counter;
    int max_job_id;
    job** jobs_array;
public:
    smash_t();
    ~smash_t();
    pid_t get_pid();
    char* get_last_path();
    int get_jobs_counter();
    int get_max_job_id();
    void set_last_path(char* str);
    void set_jobs_counter(int num);
    void set_max_job_id( int num);

    void job_insert(char* command, int status, int pID);
    job* job_access(int job_id);
    job* job_get(int job_id); // the user needs to free the space
                            //jk memory loss isnt being tested. 
    void job_remove(int job_id);
    void jobs_sort_id(int place_in_array);
    int highest_stopped_job();
    void print_jobs();
};