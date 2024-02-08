
#include "commands.h"
using namespace std;



//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//********************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "showpid")) 
	{
		cout << "smash pid is " << smash.pid;
	}
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		char* pwd = getcwd();
		if (pwd == nullptr) {
			cout << 'didnt catch that pwd';
			return 1; 
		}
		cout << pwd;
	}
	/*************************************************/
	else if (!strcmp(cmd, "cd") ) 
	{
		if(num_arg > 1)//too much arguments passed
        {
            cout << "smash error: cd: too many arguments" ;
            return 1;
        }
        if (args[1] == '-')
        {
			if (smash.last_path == nullptr) {
				cout << 'smash error: cd: OLDPWD not set'
				return 1
			}
            else if(chdir(smash.last_path)) { //chdir faild
				cout << 'faild returning to last pwd "' << smash.last_path <<'"';
				return 1;
			}
        }
		else 
		{
			smash.last_path = smash.curr_path();
			smash.curr_path = arg[1];
			if chdir(smash.curr_path) { //faild
				cout << 'faild updating the current pwd "'<< smash.curr_path << '"';
				return;
			}
		}
		
        return ;
	} 
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		int current_time = time()
 		std::sort(smash.jobs_array[0],
				smash.jobs_array[MAX_JOBS - 1],
				job.compareByJob_ID);
		for (int i ; i = 0; i < smash.job_counter ; i++)
			job.print_job(smash.jobs_array[i] , current_time);
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
   		
	} 
	/*************************************************/	
	else if (!strcmp(cmd, "diff"))
	{
   		
	} 
	/*************************************************/	
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					
					/* 
					your code
					*/
	}
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}



int find_command(string command, )