#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define EVEN 0
#define ODD 1
#define ERROR -1

//todo: better name for this function
int fileHasEvenAmountOfCharacters (char* filename) {
    //version 1: use 'stat' to find the length.
    //the length is the amount of characters
    struct stat sb;
    if (stat(filename, &sb) != 0) {
        //failed to read file
        return ERROR;
    } else if (!S_ISREG(sb.st_mode)) {
        //not a file (directory?)
        return ERROR;
    } else {
        //is a file
        //if even, modulus 2 will give 0
        //if odd, will give 1
        int even = sb.st_size % 2 == 0;
        //yes this can all be done in one line,
        //but making it explicit is better?
        //depends on what skill your markers expect.
        if (even) return EVEN;
        else return ODD;
    }
}

int main (int argc, char *argv[])
{
    int processesForked = 0;

    //arguments start at argv[1] to argv[argc-1]
    for (int i = 1; i < argc; i++) {
        char* filename = argv[i];
        int forkResult = fork();
        if (forkResult == -1) {
            //fork failed
            return ERROR;
        } else if (forkResult == 0) {
            //running in child process:
            //the return result is the exit code
            return fileHasEvenAmountOfCharacters(filename);
        } else {
            //running in parent process:
            processesForked++;
        }
    }

    int currentState = -1;

    for (int i = 0; i < processesForked; i++) {
        //wait for any of the child process to exit,
        //copying it's exit code into status
        int status;
        if (wait(&status) == -1 || !WIFEXITED(status)) {
            //waiting failed
            return ERROR;
        }

        int8_t exitCode = (int8_t)WEXITSTATUS(status);

        if (currentState == -1) {
            //for the first child returned,
            //simply set the state
            currentState = exitCode;
        } else if (exitCode != ERROR) {
            //0 + 1 % 2 == ODD
            //1 + 1 % 2 == EVEN
            currentState = (currentState + exitCode) % 2;
        }
    }

    if (currentState == EVEN) {
        printf("Even\n");
    } else if (currentState == ODD) {
        printf("Odd\n");
    }

    return currentState;
}