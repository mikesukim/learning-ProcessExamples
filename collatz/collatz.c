#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    //Getting Input Number
    //Error checking should be included greater than 0.
    int inputNumber;
    if (argc >= 2){
        inputNumber = strtol(argv[1], NULL, 10);
    }
    else{
        printf("argument list is empty.\n");
        exit(0);
    }

    pid_t pid;
    /* fork a child process */
    pid = fork();
    
    if (pid < 0) {
        /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) { 

        /* child process */
        printf("%d ", inputNumber);
        while(inputNumber != 1){
            if(inputNumber % 2 == 0){
                inputNumber /= 2;
                printf("%d ", inputNumber);
            }
            else{
                inputNumber = 3 * inputNumber + 1;
                printf("%d ", inputNumber);
            }
        }
    }
    else { 
        /* parent process */  
        /* parent will wait for the child to complete */
        wait(NULL);
    }


    return 0;
}