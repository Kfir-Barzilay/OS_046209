// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include <iostream>

using namespace std;

void ctrl_c_handler(int sig, int smash_pid, int process_pid)
{
    sigset_t signal_set;
    sigset_t old_signal_set;
    char* error = {"smash: ", "caught ctrl-C", "process ", "was killed"};
    int kill_signal;

    /* initializes mask_set with all possible signals */
    sigfillset(&signal_set);

    /* changes the signal mask of the call process. It blocks all signals specified in mask_set,
    and the previous signal mask is stored in old_set.SIG_SETMASK is a flag indicating that the 
    set of signals specified by mask_set should be the new set of blocked signals for the process. */
    sigprocmask(SIG_SETMASK, &signal_set, &old_signal_set);
    cout << error[0] << error[1] << endl;

    if(smash_pid != process_pid)/*kill the child process that run in foreground*/
    {
        kill_signal = kill(process_pid, SIGKILL);
        if( kill_signal == -1)
        {
            perror("smash error: kill failed");//the sigkill failed
            return;
        }
        cout << error[0] << error[2] << process_pid << error[3] << endl;
    }

    sigprocmask(SIG_SETMASK, &signal_set, &old_signal_set);
}

void ctrl_z_handler(int sig, int smash_pid, int process_pid)
{
    sigset_t signal_set;
    sigset_t old_signal_set;
    char* error = {"smash: ", "caught ctrl-Z", "process ", "was stopped"};
    int kill_signal;

    sigfillset(&signal_set);
    sigprocmask(SIG_SETMASK, &signal_set, &old_signal_set);
    cout << error[0] << error[1] << endl;
    if(smash_pid != process_pid)/*kill the child process that run in foreground*/
    {
        kill_signal = kill(process_pid, SIGSTOP);
        if( kill_signal == -1)
        {
            perror("smash error: kill failed");//the sigstop failed
            return;
        }
        cout << error[0] << error[2] << process_pid << error[3] << endl;
    }

    sigprocmask(SIG_SETMASK, &signal_set, &old_signal_set);
}
