#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "smash.hpp"


#define MAX_LINE_SIZE 80
#define MAX_ARG 20

int ExeCmd(smash_t* p_smash, 
            char* lineSize, 
            char* cmdString);

int BgCmd(char* lineSize, void* jobs);

void ExeExternal(char* args[MAX_ARG], 
                char* cmdString, 
                smash_t* psmash);
#endif

