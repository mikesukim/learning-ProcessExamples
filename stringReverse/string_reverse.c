#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 30
#define READ_END	0
#define WRITE_END	1

int main(void)
{
    printf("Please type in a message:\n");
    char inputStr[BUFFER_SIZE];
    char outputStr[BUFFER_SIZE];
    scanf("%[^\n]%*c", inputStr);
    printf("%s\n", inputStr);

	pid_t pid;
	int fd[2];
    int fd1[2];

	/* create first pipe */
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
    /* create second pipe */
	if (pipe(fd1) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
   
	/* now fork a child process */
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0) {  /* parent process */
		/* close the unused end of the pipe */
		close(fd[READ_END]);

		/* write to the first pipe */
		write(fd[WRITE_END], inputStr, strlen(inputStr)+1); 
        printf("parent write >%s<\n",inputStr);    
		/* close the write end of the first pipe */
		close(fd[WRITE_END]);

        

        /* close the unused end of the second pipe */
		close(fd1[WRITE_END]);
		/* read from the second pipe */
		read(fd1[READ_END], outputStr, BUFFER_SIZE);
		printf("parent read >%s<\n",outputStr);
		/* close the write end of the second pipe */
		close(fd1[READ_END]);
	}
	else { /* child process */
		/* close the unused end of the first pipe */
		close(fd[WRITE_END]);
		/* read from the first pipe */
		read(fd[READ_END], inputStr, BUFFER_SIZE);
		printf("child read >%s<\n",inputStr);
		/* close the write end of the first pipe */
		close(fd[READ_END]);


        /*inputString operation*/
        for (int i = 0; inputStr[i] != 0; i++){

            if(isupper(inputStr[i])){
                outputStr[i] = tolower(inputStr[i]);
            }
            else if(islower(inputStr[i])){
                outputStr[i] = toupper(inputStr[i]);
            }
			else{
				outputStr[i] = inputStr[i];
			}
        }

        /* close the unused end of the second pipe */
		close(fd1[READ_END]);
        /* write to the second pipe */
		write(fd1[WRITE_END], outputStr, strlen(outputStr)+1); 
        printf("child write >%s<\n",outputStr);
        /* close the write end of the first pipe */
		close(fd1[WRITE_END]);
	}

	return 0;
}
