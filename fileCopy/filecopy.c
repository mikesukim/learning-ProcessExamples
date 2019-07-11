#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

#define ARRAY_SIZE 100

int main(int argc, char* argv [])
{
    /*Read Input file*/
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
	
	// Init Array for passing pipe
    char inputStr[ARRAY_SIZE];
	char outputStr[ARRAY_SIZE];
    printf("\n");
    
	/* create pipe */
	int fd[2];
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
    
	/* now fork a child process */
	pid_t pid;
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0) {  /* parent process */
		/* close the unused end of the pipe */
		close(fd[READ_END]);

		/* write to the pipe */
		char c;
		int i = 0;
		while(1){
			c = fgetc(fp);
			if (c == EOF){
				write(fd[WRITE_END], inputStr, strlen(inputStr));
				printf("%.*s",ARRAY_SIZE,inputStr); 
				break;
				}
			if ( i == ARRAY_SIZE - 1){
				inputStr[i] = c;
				
				write(fd[WRITE_END], inputStr, ARRAY_SIZE);
				printf("%.*s",ARRAY_SIZE,inputStr); 
				memset(inputStr, 0, sizeof inputStr);
				i = 0;
			}
			else{
				inputStr[i] = c;
				i++;
			}
		}

		/* close the write end of the first pipe */
		close(fd[WRITE_END]);
		fclose(fp);
		wait(NULL);
	}
	else { /* child process */
		/* close the unused end of the first pipe */
		close(fd[WRITE_END]);

		FILE *f = fopen(argv[2], "w");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}

		/* read from the first pipe */
		int  n = 0;
		while ((n = read(fd[0], outputStr, ARRAY_SIZE)) > 0) { // Read until it returns 0 (EOF) or -1 (error)
			if(n != ARRAY_SIZE)
			{
				int i = 0;
				while (i < n-1)
				{
					// printf("%c", outputStr[i]);
					fprintf(f,"%c",outputStr[i]);
					i++;
				}
			}
			else{
				// printf("%s",outputStr);
				fprintf(f,"%.*s",ARRAY_SIZE,outputStr);
			}
		}

		/* close the write end of the first pipe */
		close(fd[READ_END]);
		fclose(f);
	}
    
	return 0;
}
