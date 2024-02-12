// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include <iostream>
#include <Windows.h>
using namespace std;


int main() {
    bool ctrlZPressed = false;
    bool ctrlCPressed = false;

    while (!ctrlCPressed) {
        // Check for Ctrl+Z
        if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('Z') & 0x8000) {
            if (!ctrlZPressed) {
                cout << "Ctrl+Z Pressed" << endl;
                /*
                
                handeling ctrl+z for the running process in smash
                
                
                */
                ctrlZPressed = true;
            }
        } else {
            ctrlZPressed = false;
        }

        // Check for Ctrl+C
        if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('C') & 0x8000) {
            if (!ctrlCPressed) {
                cout << "Ctrl+C Pressed" << endl;
                /*
                
               handling ctrl+c for running process in smash


                */
                ctrlCPressed = true;
            }
        } else {
            ctrlCPressed = false;
        }

        // Your main program logic goes here

        // Sleep to avoid high CPU usage in the loop
        Sleep(100);
    }

    return 0;
}
