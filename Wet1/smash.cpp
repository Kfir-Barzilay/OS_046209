
#define MAX_JOBS 100
#define MAX_LINE_SIZE 80
#define MAXARGS 20
#include "commands.h"
#include "signals.h"

int counter = 0;
class job
{
private:
    int job_id;
    int pid;
    int start_time;// we can use time(pointer) return secondes passed from midnight or clock() return the clock cycles passed
    int status; // 0 - ready, 1 - use, 2 - wait, 3- zombie/dead
public:
    job();
    ~job();
    compareByPID (const job& a , const job& b);
    compareByJob_ID (const job& a , const job& b);
    get_job_id(const job& a);
    get_start_time(const job& a);
    get_start_time(const job& a);
    get_status(const job& a);
    set_status(const job& a, int status);
};

job::job(/* args */)
{
    this->job_id = counter++;
    this->pid = 0;//need to be change
    time(*this->start_time);
    this->status = 0;
}

job::~job()
{
}

bool compareByJob_ID (const job& a , const job& b) {
    if a == nullptr {
        return false;
    }
    else if b == nullptr {
        return true;
    }
    return a.job_id < b.job_id;
}   

bool compareByPID (const job& a , const job& b) {
    if a == nullptr {
        return false;
    }
    else if b == nullptr {
        return true;
    }
    return a.pid < b.pid;
}   
int get_job_id(const job& a){
    return a.job_id;
}
int get_pid(const job& a){
    return a.pid;
}
int get_start_time(const job& a){
    return a.start_time;
}
int get_status(const job& a){
    return a.status;
}
void set_status(const job& a, int status){
    a.status = status;
}

class smash_t
{
private:
    int pid_num;
    job* jobs_array[MAX_JOBS];
    char* last_path;
    char* curr_id;
    int job_counter;
public:
    smash();
    ~smash();
    get_pid_num(const smash_t& s);
    get_job_counter(const smash_t& s);
    get_last_path(const smash_t& s);
    get_curr_path(const smash_t& s);
    set_job_counter(const smash_t& s, int add);
    set_last_path(const smash_t& s);
    set_curr_path(const smash_t& s);
};

smash_t::smash(/* args */)
{
    this->pid_num = getpid();
    this->jobs_array = new *job[MAX_JOBS];
    this->last_path = nullptr;
    this->curr_path = nullptr;
    this->job_counter = 0;
    
}

smash_t::~smash()
{
}
int get_pid_num(const smash_t& s){
    return s.pid_num;
}
int get_job_counter(const smash_t& s){
    return s.job_counter;
}
char* get_last_path(const smash_t& s){
    return s.last_path;
}
char* get_curr_path(const smash_t& s){
    return s.curr_path;
}
/*
need to had access for 
*/
void set_job_counter(const smash_t& s, int add)
{
    s.job_counter += add;
}
void set_last_path(const smash_t& s)
{
    s.last_path = s.curr_path;
}
void set_curr_path(const smash_t& s)
{
    s.curr_path = getcwd();
}
/*
setter for jobs array
*/

char* L_Fg_Cmd;
char lineSize[MAX_LINE_SIZE]; 

int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   

	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals 


	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// background command	
	 	if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

