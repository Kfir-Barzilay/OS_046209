
#include "commands.hpp"
#include "smash.hpp"
#include <iostream>
#define SMASH_ERROR "smash error: "
#define STOPPED 3
using namespace std;



//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//********************************************************
int ExeCmd(smash_t** pp_smash, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
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
	if (*pp_smash == NULL) {
		return 0;
	}
	smash_t smash = **pp_smash;

	if (!strcmp(cmd, "showpid"))  
	{
		if (smash.pid == nullptr) { //should not happen
			return 1;
		}
		cout << "smash pid is " << smash.pid;
		return 0;
	}
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		char* pwd = getcwd();
		if (pwd == nullptr)  //getcwd failed
			return 1; 
		
		cout << pwd;
		return 0;
	}
	/*************************************************/
	else if (!strcmp(cmd, "cd") ) 
	{
		//---arg check
		if(num_arg > 1) {//too much arguments passed
            cout << "smash error: cd: too many arguments" ;
			return 1;
		}

		char* current_path = getcwd()
		if (current_path == nullptr) {  //getcwd failed
			return 1;
		}
		
        
        if (args[1] == '-') //changing to last cd
        {
			if (smash_t::get_last_path() == nullptr) {
				cout << 'smash error: cd: OLDPWD not set';
				return 1;
			}
            else if(chdir(smash_t::get_last_path())) { //chdir faild
				return 1;
			}
			smash.last_path = current_path;
			return 0;
        } 

		smash.last_path = current_path;
		if chdir(smash.curr_path) { //faild
			cout << 'faild updating the current pwd "'<< smash.curr_path << '"';
			return;
		}
		
        return ;
	} 
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		smash.print_jobs();
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		//-------checks the input
		if (num_arg != 2) { //invalid args num
			cerr << SMASH_ERROR << "kill: invalid arguments" << endl;
			return 1;
		}
		if (!isdigit(args[2])) {
			cerr << SMASH_ERROR << "kill: invalid arguments" << endl;
			return 1;
		}
		int signal = atoi(args[2]);
		//-------get job pid
		job* kill_job = smash.job_access(args[1])
		if (kill_job == NULL) { //job doesnt exist
			cerr << SMASH_ERROR << "kill: job-id " << args[2];
			cerr << " does not exist" << endl;
			return 1;
		}
		pid_t job_pid = (*kill_job).get_pid();
		//------send signal
		if (kill(kill_job , signal) == 0) {
			cout << "signal number " << args[2] << " was sent to pid ";
			cout << job_pid << endl;
			return 0;
		}
		//-----undefiend error
		return 1;
	} 
	/*************************************************/	

	else if (!strcmp(cmd, "fg")) 
	{
		char* error_print[4] = {"smash error: fg: ",
							 	"invalid arguments ",
							 	"jobs list is empty ",
							 	"does not exist "}
		if (num_arg > 1 || !isdigit(args[1])))
		{
			cout << error_print[0] << error_print[1] << endl;
		}
		else if (num_arg == 0 )
		{
			int largest_id = smash.get_job_counter();
			job* job_p = smash.job_access(largest_id);
			if (job_p == NULL) 
			{
				cout << error_print[0] << error_print[2] << endl;
				return 1;
			}
			cout << (*job_p).get_command() << (*job_p).get_job_id() << endl;
			int status;
			int wait = waitpid((*job_p).get_pid(), &status, WUNTRACED);// do we need wuntraced or something else?
			if(wait == -1)
			{
				cout << "smash error: waitpid failed" << endl;
				return 1;
			} 
			if(WIFSTOPPED(status))//return true if the process stoped, otherwise false
			{
				*(job_p).set_status(STOPPED);

			}
			smash.job_remove(largest_id);
		}
		else if (args[1] > smash.get_job_counter())
		{
			cout<< error_print[0] <<"job-id "<<args[1] <<error_print[3] <<endl;
		}
		else if(num_arg == 1 && isdigit(arg[1])) /* execute the job_id */
		{
			int job_id = atoi(arg[1]);
			job* job_p = smash.job_access(job_id);
			if (job_p == NULL) 
			{
				cout << error_print[0] << error_print[2] << endl;
				return 1;
			}
			cout << (*job_p).get_command() << job_id << endl;
			int status;
			int wait = waitpid((*job_p).get_pid(), &status, WUNTRACED);// do we need wuntraced or something else?
			if(wait == -1)
			{
				cout << "smash error: waitpid failed" << endl;
				return 1;
			} 
			if(WIFSTOPPED(status))//return true if the process stoped, otherwise false
			{
				*(job_p).set_status(STOPPED);

			}
			smash.job_remove(job_id);
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		if(num_arg > 1)
		{
			cout << SMASH_ERROR << cmd <<": invalid arguments" << endl;
			return 1;
		}
		if(num_arg == 0)
		{
			int job_id = highest_stopped_job();
			if(job_id == -1)
			{
				cout << SMASH_ERROR << cmd << ": there are no jobs to resume" << endl;
				return 1;
			}
			job *job_p = smash.job_access(job_id);
			int signal = kill(job_p.get_pid(), SIGCONT);
			if(signal == -1)
			{
				cout << "smash error: kill failed" << emdl;
				return 1;
			}
			job_p.set_status(1);//need to check the status numbers
			cout << job_p.get_command() << ": " << job_id << endl;
			int kill = kill(job_p.get_pid(), SIGCONT);
			if(kill == -1)
			{
				perror("smash error: kill failed");
				return 1;
			}
		}
		else if(arg_num == 1 && isdigit(arg[1]))
		{
			int job_id = atoi(arg[1]);
			job *job_p = smash.job_access();
			if(job_p == NULL)
			{
				cout << SMASH_ERROR << cmd << ": job id " << job_id << " does not exist" << endl;
				return 1;
			}
			if(job_p.get_status() != 3)//not stopped
			{
				cout << SMASH_ERROR << cmd <<": job id" << job_id << "is already running in the background" << endl;
				return 1;
			}
			cout << job_p.get_command() << ": " << job_id << endl;
			job_p.set_status(STOPPED);
			int kill = kill(job_p.get_pid(), SIGCONT);
			if(kill == -1)
			{
				perror("smash error: kill failed");
				return 1;
			}
		}
		return 0;
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		//-----kill all processes if needed
		if (args[1] == "kill") { //-------------------------------------------------need help
			for (int i = 0; i < smash.job_counter; i++) {
				job_pid = (*(smash.jobs_array[i])).get_pid();
				time_t sig_time = time();
				cout << "Sending SIGTERM"; 
				int kill = kill(job_pid , SIGTERM);
				time_t current_time = time();
				while (time_diff(current_time , sig_time) < 5)
					
				if(kill == -1)
				{
					cout << SMASH_ERROR << "kill failed" <<endl;
				}
			}
		}
		//finishing smash
   		delete[smash];
		*pp_smash = NULL;
		return 0;
	} 
	/*************************************************/
	
	else if (!strcmp(cmd, "diff"))
	{
		if (num_arg != 3)
		{
			cout << "smash error: diff: invalid arguments" << endl;
		}
		ifstream file1(argv[1]);
    	ifstream file2(argv[2]);
		if (!file1.is_open() || !file2.is_open()) 
		{
			cerr << "Error opening files." << std::endl;
			return 1;
		}

		string line1, line2;
		int lineNumber = 1;
		while (getline(file1, line1) && getline(file2, line2)) 
		{
			if (line1 != line2)
			{
				cout << "Files differ at line " << lineNumber << endl;
				file1.close();
				file2.close();
				return 1;
			}
			lineNumber++;
		}

		if (!file1.eof() || !file2.eof()) 
		{
			cout << "Files have different lengths." << endl;
			file1.close();
			file2.close();
			return 1;
		} 
		else 
		{
			cout << "Files are identical." << endl;
		}

		file1.close();
		file2.close();

		return 0;
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
					
					if(args[num_arg -1] == &)//enter job to the array or run it in the background or in the child process what the fuckkkkkkkkk
					{
						smash.job_insert(args[1] , 2, pID);
					}
					else //run it 
					{
						execv(cmdString, args);
						waitpid(pID);
					}
					
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


