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
#include "job.hpp"


#define MAX_LINE_SIZE 80

int ExeCmd( char* lineSize, 
            char* cmdString);

int BgCmd(char* lineSize);

void ExeExternal(char* args[MAXARGS], 
                char* cmdString);
#endif

