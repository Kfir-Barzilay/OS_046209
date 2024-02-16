#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void ctrl_c_handler(int sig, int smash_pid, int process_pid);
void ctrl_z_handler(int sig, int smash_pid, int process_pid);

#endif

