
#include "commands.hpp"
#include <iostream>
#include <cctype>
#include <bits/stdc++.h>
#include <string.h>
#define SMASH_ERROR "smash error: "
#define SUCCESS 0
#define FAILURE 1
using namespace std;

extern int smash_pid;
extern job jobs[MAXJOBS];
extern string last_path = "";


bool is_all_digits(const std::string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
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
int ExeCmd(char *lineSize,
		    char *cmdString)
{
	char *cmd; 
	char *args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command 
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
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
	
	if (!strcmp(cmd, "showpid"))  
	{
		cout << "smash pid is " << smash_pid;
		return SUCCESS;
	}
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		char* pwd = getcwd(NULL,0);
		if (pwd == NULL)  //getcwd failed
			return FAILURE; 
		
		cout << pwd << endl;
		return SUCCESS;
	}
	/*************************************************/
	else if (!strcmp(cmd, "cd") ) 
	{
		//---arg check
		if(num_arg > 1) {//too much arguments passed
            cerr << "smash error: cd: too many arguments" << endl;
			return FAILURE;
		}

		char* current_path = getcwd(NULL,0);

		if (current_path == NULL) {  //getcwd failed
			perror("smash error: getcwd failed");
			return FAILURE;
		}
        
        if (*args[1] == '-') //changing to last cd
        {
			if (last_path == "") {
				cerr << SMASH_ERROR << "cd: OLDPWD not set" << endl;
				free(current_path);
                return FAILURE;
			}
            else if(chdir(last_path.c_str())) { //chdir faild
				perror("smash error: chdir failed");
                free(current_path);
				return FAILURE;
			}
			last_path(current_path);
            free(current_path);
			return SUCCESS;
        } 

		//changing to new cd------------------------------------------------------------------check
		if (chdir(args[1])) { //faild
			cerr << "faild updating the current pwd \""<< current_path << '"';
            free(current_path);
			return FAILURE;
		}
        last_path(current_path);
        free(current_path);
        return SUCCESS;
	} 
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		print_all_jobs();
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		//-------checks the input
		if (num_arg != 2) { //invalid args num
			cerr << SMASH_ERROR << "kill: invalid arguments" << endl;
			return FAILURE;
		}
		if (!is_all_digits(args[1]) || !is_all_digits(args[2])) {
			cerr << SMASH_ERROR << "kill: invalid arguments" << endl;
			return FAILURE;
		}
		int job_id = atoi(args[1]);
		int signal = atoi(args[2]);
		//-------get job pid
        pid_t job_pid = get_job_pid(job_id);
		if (job_pid == INVALID)
		{ //job doesnt exist
			cerr << SMASH_ERROR << "kill: job-id " << args[2];
			cerr << " does not exist" << endl;
			return FAILURE;
		}
		//------send signal
		if (kill(job_pid , signal) == 0)
		{
			cout << "signal number " << args[2] << " was sent to pid ";
			cout << job_pid << endl;
			return SUCCESS;
		}
		//-----undefiend error
		return FAILURE;
	} 
	/*************************************************/	

	else if (!strcmp(cmd, "fg")) 
	{
		char* error_print[4] = {"smash error: fg: ",
							 	"invalid arguments ",
							 	"jobs list is empty ",
							 	"does not exist "};
		if (num_arg > 1 || !is_all_digits(args[1]))
		{
			cerr << error_print[0] << error_print[1] << endl;
		}
		else if (num_arg == 0 )
		{
			int largest_id = max_job_id();
			if (largest_id == INVALID) 
			{
				cerr << error_print[0] << error_print[2] << endl;
				return FAILURE;
			}
			pid_t pid = jobs[job_index(largest_id)].pid;
			string command =jobs[job_index(largest_id)].command; 
			cout << command << pid << endl;
			int status;
			int wait = waitpid(pid, &status, WUNTRACED);
			// do we need wuntraced or something else?
			if(wait == -1)
			{
				perror("smash error: waitpid failed");
				return FAILURE;
			} 
			if(WIFSTOPPED(status))/* return true if the process stoped,
									 otherwise false*/
			{
                jobs[job_index(largest_id)].is_stopped = true;
			}
			remove_by_id(largest_id);
		}
		else if (*args[1] > max_job_id())
		{
			cerr<< error_print[0] <<"job-id "<<args[1] <<error_print[3] <<endl;
		}
		else if(num_arg == 1 && is_all_digits(args[1])) /* execute the job_id */
		{
			int job_id = atoi(args[1]);
			if (job_index(job_id) == INVALID) 
			{
				cerr << error_print[0] << error_print[2] << endl;
				return FAILURE;
			}
			string command = jobs[job_index(job_id)].command;
			cout << command << job_id << endl;
			int status;
			// do we need wuntraced or something else?
			pid_t pid = jobs[job_index(job_id)].pid;
			int wait = waitpid(pid, &status, WUNTRACED);
			if(wait == -1)
			{
				perror("smash error: waitpid failed");
				return FAILURE;
			} 
			if(WIFSTOPPED(status))/* return true if the process stoped,
									 otherwise false */
			{
                jobs[job_index(job_id)].is_stopped = true;
			}
			remove_by_id(job_id);
			return SUCCESS;
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		char* errors[5] = {": invalid arguments", 
						   ": there are no jobs to resume",
						   " does not exist",
						   "is already running in the background",
						   ": job id ",
						   };
  		if(num_arg > 1)
		{
			cerr << SMASH_ERROR << cmd << errors[0] << endl;
			return FAILURE;
		}
		if(num_arg == 0)
		{
			int job_id = highest_stopped_id();
			if(job_id == -1)
			{
				cerr << SMASH_ERROR << cmd << errors[1] << endl;
				return FAILURE;
			}
			pid_t pid = jobs[job_index(job_id)].pid;
			int signal = kill(pid, SIGCONT);
			if(signal == -1)
			{
				perror("smash error: kill failed");
				return FAILURE;
			}
            jobs[job_index(job_id)].is_stopped() = false;
			string command = jobs[job_index(job_id)].command;
			cout << command << ": " << job_id << endl;
			int kill_p = kill(pid, SIGCONT);
			if(kill_p == -1)
			{
				perror("smash error: kill failed");
				return FAILURE;
			}
		}
		else if(num_arg == 1 && is_all_digits(args[1]))
		{
			int job_id = atoi(args[1]);
			if(job_index(job_id) == INVALID)
			{
				cerr<<SMASH_ERROR<<cmd<<errors[4]<<job_id << errors[2] << endl;
				return FAILURE;
			}
			if(!jobs[job_index(job_id)].is_stopped)//not stopped
			{
				cerr<<SMASH_ERROR<<cmd<<errors[4]<<job_id << errors[3] << endl;
				return FAILURE;
			}
			string command = jobs[job_index(job_id)].command;
			cout << command << ": " << job_id << endl;
            jobs[job_index(job_id)].is_stopped = false;
			pid_t pid = jobs[job_index(job_id)].pid;
			int kill_p = kill(pid, SIGCONT);
			if(kill_p == -1)
			{
				perror("smash error: kill failed");
				return FAILURE;
			}
		}
		return SUCCESS;
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		//-----kill all processes if needed
		if (args[1] == "kill") { //-------------------------------------------------need help
			for (int i = 0; i < MAXJOBS; i++) {
                if (jobs[i].job_id == INVALID) {
                }
                else
                {
                    pid_t job_pid = jobs[i].pid;
                    time_t sig_time = time(NULL);
                    cout << "Sending SIGTERM"; 
                    int kill_p = kill(job_pid , SIGTERM);
                    if(kill_p == -1)
                    {
                        perror("smash error: kill failed");
                        return FAILURE;
                    }
                    //time_t current_time = time();
                    while ((time(NULL) - sig_time)  < 5);
                }
			}
		}
		//finishing smash
   		delete(&smash);
		p_smash = NULL;
		return SUCCESS;
	} 
	/*************************************************/
	
	else if (!strcmp(cmd, "diff"))
	{
		if (num_arg != 3)
		{
			cerr << "smash error: diff: invalid arguments" << endl;
		}
		ifstream file1(args[1]);
    	ifstream file2(args[2]);
		if (!file1.is_open() || !file2.is_open()) 
		{
			cerr << "Error opening files." << std::endl;
			return FAILURE;
		}

		string line1, line2;
		int lineNumber = 1;
		while (getline(file1, line1) && getline(file2, line2)) 
		{
			if (line1 != line2)
			{
				cout << "Files differ at line " << lineNumber << endl;//not needed
				file1.close();
				file2.close();
				return FAILURE;
			}
			lineNumber++;
		}

		if (!file1.eof() || !file2.eof()) 
		{
			cout << "Files have different lengths." << endl;//not needed
			file1.close();
			file2.close();
			return FAILURE;
		} 
		else 
		{
			cout << "Files are identical." << endl;//not needed
		}

		file1.close();
		file2.close();

		return SUCCESS;
	}
			
 
	/*************************************************/	
	else // external command
	{
 		ExeExternal(args, cmdString, &smash);
	 	return SUCCESS;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return FAILURE;
	}
    return SUCCESS;
}

void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					perror("smash error: fork failed");
					exit(1);
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        execv(args[0], args); /*running on cmd the command,
										  return only if got error*/ 
					perror("smash error: execv failed");
					exit(1);
			
			default:
                	// Add your code here
					int status;
					int wait_pid =waitpid(pID, &status, WUNTRACED | WCONTINUED);
					if(wait_pid == -1)
					{
						perror("smash error: waitpid failed");
					} 
					if(WIFSTOPPED(status))// only occur when SIGSTOP sent
					{
						//do we need to enter the arguments also?
						smash.job_insert(cmdString, args, pID,true, status);
					}
					
	}
}
//*****************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//*****************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
		char cmd[MAX_LINE_SIZE];
		strcpy(cmd, lineSize);
		lineSize[strlen(lineSize) - 1] = '\0';
		Command = strtok(lineSize, delimiters);
		if(Command == NULL)
		{
			return SUCCESS;
		}			
		args[0] = Command;
		for(int i =0; i < MAX_ARG; i++)
		{
			//NULL indicates using the same string
			args[i] = strtok(NULL, delimiters);
		}
		ExeExternal(args, cmd, jobs);
		
	}
	return -1;
}


