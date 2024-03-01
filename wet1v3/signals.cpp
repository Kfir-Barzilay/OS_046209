// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.hpp"
#include <iostream>


using namespace std;
extern bool isTest;
extern pid_t current_pid;
extern pid_t smash_pid;

void ctrl_c_handler(int sig)
{
    sigset_t signal_set;
    sigset_t old_signal_set;
    string error[] = {"smash: ", "caught ctrl-C", " process ", " was killed"};
    int kill_signal;

    /* initializes mask_set with all possible signals */
    int fail_p = sigfillset(&signal_set);
    if(fail_p == -1)
    {
        sys_err(SIGFILLSET);
        return;
    }

    /* changes the signal mask of the call process. It blocks all signals specified in mask_set,
    and the previous signal mask is stored in old_set.SIG_SETMASK is a flag indicating that the 
    set of signals specified by mask_set should be the new set of blocked signals for the process. */
    fail_p = sigprocmask(SIG_SETMASK, &signal_set, &old_signal_set);
    cout << error[0] << error[1] << endl;
    if(fail_p == -1)
    {
        sys_err(SIGPROCMASK);
        return;
    }
    
    if(current_pid == -1)
    {
        sys_err(GETPID);
        return;
    }
    
    if(current_pid != smash_pid)/*kill the child process that run in foreground*/
    {
        kill_signal = kill(current_pid, SIGKILL);
        if( kill_signal == -1)
        {
            sys_err(KILL);//the sigkill failed
            return;
        }
        cout << error[0] << error[2] << current_pid << error[3] << endl;
    }

    fail_p = sigprocmask(SIG_SETMASK, &signal_set, &old_signal_set);
    if(fail_p == -1)
    {
        sys_err(SIGPROCMASK);
        return;
    }
}

void ctrl_z_handler(int sig)
{   
    if (isTest) cout << "handeling Z" <<endl;
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigset_t old_signal_set;
    string error[] = {"smash: ", "caught ctrl-Z", " process ", " was stopped"};
    int kill_signal;

    /* initializes mask_set with all possible signals */
    int fail_p = sigfillset(&signal_set);
    if(fail_p == -1)
    {
        sys_err(SIGFILLSET);
        return;
    }

    /* changes the signal mask of the call process. It blocks all signals specified in mask_set,
    and the previous signal mask is stored in old_set.SIG_SETMASK is a flag indicating that the 
    set of signals specified by mask_set should be the new set of blocked signals for the process. */
    fail_p = sigprocmask(SIG_SETMASK, &signal_set, &old_signal_set);
    cout << error[0] << error[1] << endl;
    if(fail_p == -1)
    {
        sys_err(SIGPROCMASK);
        return;
    }
    
    if(current_pid == -1)
    {
        sys_err(GETPID);
        return;
    }
    
    if(current_pid != smash_pid)/*kill the child process that run in foreground*/
    {
        if (isTest) cout << "stopping process: " << current_pid << endl;
        kill_signal = kill(current_pid, SIGSTOP);
        if( kill_signal == -1)
        {
            sys_err(KILL);//the sigkill failed
            return;
        }
        cout << error[0] << error[2] << current_pid << error[3] << endl;
    }

    fail_p = sigprocmask(SIG_SETMASK, &signal_set, &old_signal_set);
    if(fail_p == -1)
    {
        sys_err(SIGPROCMASK);
        return;
    }
    if (isTest) cout << "done with z handler" << endl;
}
