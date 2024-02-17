
#include "commands.hpp"
#include "smash.hpp"
#include <iostream>
#define SMASH_ERROR "smash error: "
#define SUCCESS 0
#define FAILURE 1
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
			return FAILURE;
		}
		cout << "smash pid is " << smash.pid;
		return SUCCESS;
	}
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		char* pwd = getcwd();
		if (pwd == nullptr)  //getcwd failed
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

		char* current_path = getcwd()
		if (current_path == nullptr) {  //getcwd failed
			perror("smash error: getcwd failed");
			return FAILURE;
		}
		
        
        if (args[1] == '-') //changing to last cd
        {
			char *last_path = smash.get_last_path();
			if (last_path == nullptr) {
				cerr << SMASH_ERROR << 'cd: OLDPWD not set' << endl;
				delete(last_path);
				return FAILURE;
			}
            else if(chdir(last_path)) { //chdir faild
				perror("smash error: chdir failed");
				delete(last_path);
				return FAILURE;
			}
			smash.set_last_path(current_path);
			return SUCCESS
        } 

		//changing to new cd------------------------------------------------------------------check
		smash.set_last_path(current_path);
		if chdir(args[1]) { //faild
			cerr << 'faild updating the current pwd "'<< current_path << '"';
			return FAILURE;
		}
		
        return SUCCESS;
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
			return FAILURE;
		}
		if (!isdigit(args[1]) || !isdigit(args[2])) {
			cerr << SMASH_ERROR << "kill: invalid arguments" << endl;
			return FAILURE;
		}
		pid_t job_id = atoi(args[1]);
		int signal = atoi(args[2]);
		//-------get job pid
		job* kill_job = smash.job_access(job_id)
		if (kill_job == NULL)
		{ //job doesnt exist
			cerr << SMASH_ERROR << "kill: job-id " << args[2];
			cerr << " does not exist" << endl;
			return FAILURE;
		}
		pid_t job_pid = (*kill_job).get_pid();
		//------send signal
		if (kill(kill_job , signal) == 0)
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
							 	"does not exist "}
		if (num_arg > 1 || !isdigit(args[1])))
		{
			cerr << error_print[0] << error_print[1] << endl;
		}
		else if (num_arg == 0 )
		{
			int largest_id = smash.get_max_job_id();
			job* job_p = smash.job_access(largest_id);
			if (job_p == NULL) 
			{
				cerr << error_print[0] << error_print[2] << endl;
				return FAILURE;
			}
			pid_t pid = (*job_p).get_pid();
			char *command = (*job_p).get_command(); 
			cout << command << pid << endl;
			delete(command);
			int status;
			pid_t pid = (*job_p).get_pid();
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
				*(job_p).set_is_stopped(true);

			}
			smash.job_remove(largest_id);
		}
		else if (args[1] > smash.get_max_job_id())
		{
			cerr<< error_print[0] <<"job-id "<<args[1] <<error_print[3] <<endl;
		}
		else if(num_arg == 1 && isdigit(arg[1])) /* execute the job_id */
		{
			int job_id = atoi(arg[1]);
			job* job_p = smash.job_access(job_id);
			if (job_p == NULL) 
			{
				cerr << error_print[0] << error_print[2] << endl;
				return FAILURE;
			}
			char* command = (*job_p).get_command();
			cout << command << job_id << endl;
			delete(command);
			int status;
			// do we need wuntraced or something else?
			pid_t pid = (*job_p).get_pid();
			int wait = waitpid(pid, &status, WUNTRACED);
			if(wait == -1)
			{
				perror("smash error: waitpid failed");
				return FAILURE;
			} 
			if(WIFSTOPPED(status))/* return true if the process stoped,
									 otherwise false */
			{
				*(job_p).set_is_stopped(true);

			}
			smash.job_remove(job_id);
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
						   }
  		if(num_arg > 1)
		{
			cerr << SMASH_ERROR << cmd << errors[0] << endl;
			return FAILURE;
		}
		if(num_arg == 0)
		{
			int job_id = highest_stopped_job();
			if(job_id == -1)
			{
				cerr << SMASH_ERROR << cmd << errors[1] << endl;
				return FAILURE;
			}
			job *job_p = smash.job_access(job_id);
			pid_t pid = job_p.get_pid();
			int signal = kill(pid, SIGCONT);
			if(signal == -1)
			{
				perror("smash error: kill failed");
				return FAILURE;
			}
			job_p.set_is_stopped(false);
			char *command = job_p.get_command();
			cout << command << ": " << job_id << endl;
			delete(command);
			pid_t pid = job_p.get_pid();
			int kill = kill(pid, SIGCONT);
			if(kill == -1)
			{
				perror("smash error: kill failed");
				return FAILURE;
			}
		}
		else if(arg_num == 1 && isdigit(arg[1]))
		{
			int job_id = atoi(arg[1]);
			job *job_p = smash.job_access();
			if(job_p == NULL)
			{
				cerr<<SMASH_ERROR<<cmd<<errors[4]<<job_id << errors[2] << endl;
				return FAILURE;
			}
			if(!(job_p.get_is_stopped))//not stopped
			{
				cerr<<SMASH_ERROR<<cmd<<errors[4]<<job_id << errors[3] << endl;
				return FAILURE;
			}
			char *command = job_p.get_command();
			cout << command << ": " << job_id << endl;
			delete(command);
			job_p.set_is_stopped( false);
			pit_t pid = job_p.get_pid();
			int kill = kill(pid, SIGCONT);
			if(kill == -1)
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
			for (int i = 0; i < smash.job_counter; i++) {
				pid_t job_pid = (*(smash.jobs_array[i])).get_pid();
				time_t sig_time = time();
				cout << "Sending SIGTERM"; 
				int kill = kill(job_pid , SIGTERM);
				time_t current_time = time();
				while (time_diff(current_time , sig_time) < 5)
					
				if(kill == -1)
				{
					cout << SMASH_ERROR << "kill failed" <<endl;
					return FAILURE;
				}
			}
		}
		//finishing smash
   		delete[smash];
		*pp_smash = NULL;
		return SUCCESS;
	} 
	/*************************************************/
	
	else if (!strcmp(cmd, "diff"))
	{
		if (num_arg != 3)
		{
			cerr << "smash error: diff: invalid arguments" << endl;
		}
		ifstream file1(argv[1]);
    	ifstream file2(argv[2]);
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
 		ExeExternal(args, cmdString);
	 	return SUCCESS;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return FAILURE;
	}
    return SUCCESS;
}

void ExeExternal(char *args[MAX_ARG], char* cmdString, Job *jobs)
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
						smash.job_insert(cmdString, args, pID, status);
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
			args[i] = strtok(NULL, delimeters);
		}
		ExeExternal(args, cmd, *jobs);
		
	}
	return -1;
}


