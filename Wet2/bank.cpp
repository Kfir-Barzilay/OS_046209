#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bank.hpp"
#define FAIL 1

using namespace std;

int main(int argc, char *argv[])
{
    /* ------------ init bank -----------*/


    /*------- take care of the input ----*/
    int n = argc;// number of ATMs

    for(int i = 0; i < n; n++)
    {
        std::ifstream file(argv[i]);
        // Check if the file was opened successfully
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << argv[i] << '\n';
            continue;  // Move on to the next file
        }
        while (std::getline(file, line)) {
            // Create a stringstream from the line
            std::stringstream ss(line);
            std::string word;
            std::vector<std::string> words;  // Vector to store separated words

            // Split the line into words based on spaces
            while (ss >> word) {
                words.push_back(word);
            }

            string operation = *(words.front());
            words.erase(0);
            int account = atoi(words.front());
            words.erase(0);
            int password = atoi(words.front());
            words.erase(0);
            int amount = atoi(words.front());
            words.erase(0);

            switch operation
            {
                case 'O':
                    /*------ open acount -------*/
                case 'D':
                    /*----- make a deposite ----*/
                case 'W':
                    /*----- make a withdraw ----*/
                case 'B':
                    /*------ balance check -----*/
                case 'Q':
                    /*----- closing account ----*/
                case 'T':
                    /*---- make a transaction---*/ 
                case default:
                    cerr << "got: " << operation << "we don't support" << endl;
                    return FAIL;
            }

        }

    }
}