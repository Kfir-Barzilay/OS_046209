/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.hpp"

#define MAX_LINE_SIZE 80
#define MAXARGS 20
#define MAXJOBS 100

bool isTest = true;
pid_t current_pid = -1;
pid_t smash_pid = -1;
job jobs[MAXJOBS]; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
string last_path = "";

char* L_Fg_Cmd;
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	 
	struct sigaction control_c = {0};
	struct sigaction control_z = {0};

	control_c.sa_handler = &ctrl_c_handler;
	control_c.sa_flags = SA_RESTART;
	control_z.sa_handler = &ctrl_z_handler;
	control_z.sa_flags = SA_RESTART;
	if(sigaction(SIGINT, &control_c, NULL) == INVALID) {
		sys_err(SIGACT);
	}
	if(sigaction(SIGTSTP, &control_z, NULL) == INVALID) {
		sys_err(SIGACT);
	}

	smash_pid = getpid();
	if(smash_pid == -1)
	{
		sys_err(GETPID);
		return FAILED;
	}
	current_pid = smash_pid;
	if (isTest) cout << "Smash PID is: " << current_pid << endl;
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	//signal(SIGINT, ctrl_c_handler);
    //signal(SIGTSTP, ctrl_z_handler);

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
		//if(isTest) cout << "got input" << endl;
		strcpy(cmdString, lineSize);  
		//if(isTest) cout << "copied input" << endl;  	
		cmdString[strlen(lineSize)-1]='\0';
					// background command	
	 	if(!BgCmd(lineSize)) continue; 
					// built in commands
		ExeCmd(lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

