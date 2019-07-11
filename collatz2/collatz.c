#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "protocol.h"
#include <fcntl.h>
#include <sys/mman.h>

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

    //Calculation
    int results[NUM];
    results[0] = inputNumber;
    for(int i = 1; inputNumber != 1; i++){
         if(inputNumber % 2 == 0){
            inputNumber /= 2;
            results[i] = inputNumber;
        }
        else{
            inputNumber = 3 * inputNumber + 1;
            results[i] = inputNumber;
        }
    }
    
    /* fork a child process */
    pid_t pid;
    pid = fork();
    
    if (pid < 0) {
        /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0){
        /* child process */

        //Create share Memory
        int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO); //0777);
        if (fd < 0)
        {
            perror("shm_open()");
            return EXIT_FAILURE;
        }
        ftruncate(fd, SIZE);
        int *data = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        printf("sender mapped address: %p\n", data);

        //Write the sequence to the shared memory object
        for (int i = 0; i < NUM; ++i)
        {
            data[i] = results[i];
        }

        munmap(data, SIZE);
        close(fd);
        ////
    }
    else
    {
        /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);

        int fd = shm_open(NAME, O_RDONLY, 0666);
        if (fd < 0)
        {
            perror("shm_open()");
            return EXIT_FAILURE;
        }

        int *data =
            (int *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
        printf("receiver mapped address: %p\n", data);

        //output the contents of shared memeory
        for (int i = 0; i< NUM; ++i)
        {
            printf("%d ", data[i]);
	    if(data[i] == 1){break;}
        }
        printf("\n");

        //Remove the shared-memory object
        munmap(data, SIZE);
        close(fd);
        shm_unlink(NAME);
    }

    return 0;
}
