
#include "commands.hpp"
#include <iostream>
#include <cstring>
#include <cctype>
#include <bits/stdc++.h>
#include <string.h>

#define SMASH_ERROR "smash error: "
#define SUCCESS 0
#define FAILURE 1

using namespace std;
extern bool isTest; 
extern pid_t current_pid;
extern pid_t smash_pid;
extern job jobs[MAXJOBS];
extern string last_path;


bool is_all_digits(const std::string& str) {
    for (std::string::size_type i = 0; i < str.size(); ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//********************************************************
int ExeCmd(char* lineSize,
		   char* cmdString)
{
	char* cmd; 
	char* args[MAXARGS];
	//char pwd[MAX_LINE_SIZE];
	string delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command 
    cmd = strtok(lineSize, delimiters.c_str());
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i < MAXARGS; i++)
	{
		args[i] = strtok(NULL, delimiters.c_str()); 
		if (args[i] != NULL) 
		{
			num_arg++;
		}
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	
	if (!strcmp(cmd, "showpid"))  //V
	{
		cout << "smash pid is " << smash_pid << endl;
		return SUCCESS;
	}
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) //V
	{
		char* pwd = getcwd(NULL,0);
		if (pwd == NULL)  //getcwd failed
		{
			sys_err(GETCWD);
			return FAILURE; 
		}

		
		cout << pwd << endl;
		return SUCCESS;
	}
	/*************************************************/
	else if (!strcmp(cmd, "cd") ) 
	{
		//---arg check
		if(num_arg > 1) {//too much arguments passed
            smash_err("cd" , "too many arguments");
			return FAILURE;
		}

		char* current_path = getcwd(NULL,0);

		if (current_path == NULL) {  //getcwd failed
			sys_err(GETCWD);
			return FAILURE;
		}
        
        if (args[1] != NULL && *args[1] == '-') //changing to last cd
        {
			if (last_path == "") {
				smash_err("cd", "OLDPWD not set");
				free(current_path);
                return FAILURE;
			}
            else if(chdir(last_path.c_str())) { //chdir faild
				sys_err(CHDIR);
                free(current_path);
				return FAILURE;
			}
			last_path = current_path;
            free(current_path);
			return SUCCESS;
        } 

		//changing to new cd
		if (chdir(args[1])) { //faild
			sys_err(CHDIR);
            free(current_path);
			return FAILURE;
		}
        last_path = current_path;
        free(current_path);
        return SUCCESS;
	} 
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		refresh();
		print_all_jobs();
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		refresh();
		//-------checks the input
		if (num_arg != 2 || args[1] == NULL || args[2] == NULL) {
			smash_err(cmd, "invalid arguments");
			return FAILURE;
		}
		char* sig_num_str = args[1] + 1;
		bool valid_sig =((*(args[1])) == '-') && is_all_digits(sig_num_str);
		if (!valid_sig || !is_all_digits(args[2])) {
			smash_err(cmd, "invalid arguments");
			return FAILURE;
		}
		int job_id = atoi(args[2]);
		int signal = atoi(sig_num_str);
		//-------get job pid
		if (job_index(job_id) == INVALID)
		{ //job doesnt exist
			string err_msg = "job-id ";
			err_msg += args[2];
			err_msg += " does not exist";
			smash_err(cmd, err_msg);
			return FAILURE;
		}
		pid_t job_pid = jobs[job_index(job_id)].pid;
		//------send signal
		int kill_p = kill(job_pid,signal);
		if (kill_p == 0)
		{
			string err_msg = "signal number ";
			err_msg += sig_num_str;
			err_msg += " was sent to pid ";
			cout << err_msg << job_pid << endl;
			return SUCCESS;
		}
		sys_err(KILL);
		//-----undefiend error
		return FAILURE;
	} 
	/*************************************************/	

	else if (!strcmp(cmd, "fg")) 
	{
		refresh();
		/*
		string error_print[4] = {"smash error: fg: ",
							 	"invalid arguments ",
							 	"jobs list is empty ",
							 	" does not exist "};
		*/
		if (num_arg > 1 || (args[1] != NULL && !is_all_digits(args[1])))
		{
			smash_err(cmd , "invalid arguments");
			return FAILURE;
		}
		int job_id;
		if (num_arg == 0 ) 
		{
			job_id = max_job_id();
			if (job_id == INVALID) 
			{
				smash_err(cmd , "jobs list is empty");
				return FAILURE;
			}
		}
		else
		{
			job_id = atoi(args[1]);
		}
		if(job_index(job_id) == INVALID)
			{
				string err_msg = "job id ";
				err_msg += args[1];
				err_msg += " does not exist";
				smash_err(cmd, err_msg);
				return FAILURE;
			}
		job fg_job = pop_job(job_id);
		pid_t pid = fg_job.pid;
		if(fg_job.is_stopped)
		{
			int kill_pp = kill(pid, SIGCONT);
			if(kill_pp == INVALID)
			{
				sys_err(KILL);
				return FAILURE;
			}
		}
		fg_job.is_stopped = false;
		current_pid = pid;
		string command =fg_job.command; 
		cout << command << " : " << pid << endl;
		int status;
		int wait = waitpid(pid, &status, WUNTRACED); 
		if(wait == -1)
		{
			sys_err(WAITPID);
			return FAILURE;
		} 
		current_pid = smash_pid;
		if(WIFSTOPPED(status))/* return true if the process stopped,
									otherwise false*/
		{
			fg_job.is_stopped = true;
			if(insert_job(fg_job) == FAILURE) {
				return FAILURE;
			}
		}
		return SUCCESS;
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		refresh();
		string errors[5] = {": invalid arguments", 
						   ": there are no stopped jobs to resume",
						   " does not exist",
						   " is already running in the background",
						   ": job-id ",
						   };
		if (num_arg == 0) {}
		else if((num_arg > 1) || (args[1] == NULL))
		{
			smash_err(cmd , "invalid arguments");
			return FAILURE;
		}
		else if(!is_all_digits(args[1]))
		{
			smash_err(cmd , "invalid arguments");
			return FAILURE;
		}
		
		int job_id = INVALID;
		if(num_arg == 0) 
		{
			job_id = highest_stopped_id();
			if(job_id == INVALID)
			{
				smash_err(cmd , "there are no stopped jobs to resume");
				return FAILURE;
			}
		}
		else { // 1 arg is digit
			job_id = atoi(args[1]);
			if(job_index(job_id) == INVALID)
			{
				string err_msg = "job-id ";
				err_msg += args[1];
				err_msg += " does not exist";
				smash_err(cmd, err_msg);
				
				return FAILURE;
			}
		}
		if	(!jobs[job_index(job_id)].is_stopped)
		{
			string err_msg = "job-id ";
				err_msg += args[1];
				err_msg += " is already running in the background";
				smash_err(cmd, err_msg);
				return FAILURE;
		}

		pid_t pid = jobs[job_index(job_id)].pid;
		int signal = kill(pid, SIGCONT);
		if(signal == -1)
		{
			sys_err(KILL);
			return FAILURE;
		}
		jobs[job_index(job_id)].is_stopped = false;
		string command = jobs[job_index(job_id)].command;
		cout << command << " : " << pid << endl;
		int kill_p = kill(pid, SIGCONT);
		if(kill_p == -1)
		{
			sys_err(KILL);
			return FAILURE;
		}
		return SUCCESS;
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		refresh();
		//-----kill all processes if needed
		if (args[1] != NULL && !strcmp(args[1], "kill")) { 
			for (int i = 0; i < MAXJOBS; i++) {
                if (jobs[i].job_id == INVALID){}
                else
                {
                    pid_t job_pid = jobs[i].pid;
                    time_t sig_time;
					time(&sig_time);
                    cout <<  "Sending SIGTERM... "; 
                    int kill_p = kill(job_pid , SIGTERM);
                    if(kill_p == -1)
                    {
                        sys_err(KILL);
                        return FAILURE;
                    }
                    if ((time(NULL) - sig_time)  > 5) {
						kill_p = kill(job_pid, SIGKILL);
						if(kill_p == INVALID)
						{
							sys_err(KILL);
							return FAILURE;
						}
						cout << "(5 sec passed) Sending SIGKILL... ";
					}
					cout << "Done." << endl;
				}
			}
		}
		//finishing smash
		int kill_p = kill(smash_pid, SIGKILL);
		if(kill_p == INVALID)
		{
			sys_err(KILL);
			return FAILURE;
		}
		return SUCCESS;
	} 
	/*************************************************/
	
	else if (!strcmp(cmd, "diff"))
	{
		if (num_arg > 2){
			smash_err(cmd, "invalid arguments");
		}
  	ifstream f1(args[1], ifstream::binary|ifstream::ate);
  	ifstream f2(args[2], ifstream::binary|ifstream::ate);

  if (f1.fail() || f2.fail()) {
	cout << FAILED << endl;
    return false; //file problem
  }

  if (f1.tellg() != f2.tellg()) {
	cout << FAILED << endl;
	return SUCCESS;
     //size mismatch
  }

  //seek back to beginning and use std::equal to compare contents
  f1.seekg(0, ifstream::beg);
  f2.seekg(0, ifstream::beg);
  if(equal(istreambuf_iterator<char>(f1.rdbuf()),
                istreambuf_iterator<char>(),
                istreambuf_iterator<char>(f2.rdbuf())))
	{
		cout << SUCCESS << endl;
	}
	else{
		cout << FAILED << endl;
	}
	return SUCCESS;
}
 
	/*************************************************/	
	else // external command
	{
 		ExeExternal(args, cmdString, false);
	 	return SUCCESS;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return FAILURE;
	}
    return SUCCESS;
}

void ExeExternal(char* args[MAXARGS], char* cmdString, bool is_background)
{
	int pID = fork();
    switch(pID) 
	{
    		case -1: 
					sys_err(FORK);
					exit(1);
        	case 0 :
                	// Child Process
               		setpgrp();
					if (args == NULL && cmdString == NULL)
					{
						exit(1);
					}
					
			        execvp(args[0], args); /*running on cmd the command,
										  return only if got error*/ 
					sys_err(EXECV);
					exit(1);
			
			default:
					if (args == NULL && cmdString == NULL)
					{
						return;
					}
					string args_str[MAXARGS];
					for (int i = 0 ; i < MAXARGS; i++) {
						if (args[i] != NULL) {
							args_str[i] = args[i];
						}
					}
                	if(!is_background)
					{
						int is_stopped = false;
						current_pid = pID;
						int wait_pid =waitpid(current_pid, 
											&is_stopped, 
											WUNTRACED | WCONTINUED);
						if(wait_pid == -1)
						{
							sys_err(WAITPID);
						} 
						
						if(WIFSTOPPED(is_stopped))// only occur when SIGSTOP sent
						{
							insert_job(current_pid,
										cmdString, 
										args_str ,
										false , 
										is_stopped);
						}
						
					}
					else
					{
						insert_job(pID, cmdString, args_str ,true, false);
					}
					current_pid = smash_pid;
					
	}
}
//*****************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//*****************************************************************************
int BgCmd(char* lineSize)
{
	char* Command;
	bool is_background = false;
	string delimiters = " \t\n";
	char* args[MAXARGS];
	
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		is_background = true;
		lineSize[strlen(lineSize)-2] = '\0';
		char cmd[MAX_LINE_SIZE];
		strcpy(cmd, lineSize);
		lineSize[strlen(lineSize) - 1] = '\0';
		Command = strtok(cmd, delimiters.c_str());
		if(Command == NULL)
		{
			return SUCCESS;
		}			
		args[0] = Command;
		//cout << "command " << Command << endl; /////////////////////////////////////////
		for(int i = 1; i < MAXARGS; i++)
		{
			//NULL indicates using the same string

			args[i] = strtok(NULL, delimiters.c_str());
		}
		//cout << "lineSize " << lineSize << endl; ///////////////////////////////////////////
		ExeExternal(args, lineSize, is_background);
		return SUCCESS;
	}
	return -1;
}


