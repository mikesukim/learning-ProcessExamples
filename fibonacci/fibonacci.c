
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *FibSequenceArray;
int arraySize;
int number;
int indexOfArray = 0;

pthread_mutex_t Lock;          
pthread_cond_t Cond;
pthread_cond_t Cond1;

void *CalculateFibonacci()
{

   for(int i = 0; i < arraySize; i++){
      if(i == 0){
         pthread_mutex_lock(&Lock);
         FibSequenceArray[i] = 0;
         number = FibSequenceArray[i];
         indexOfArray++;
         pthread_cond_signal(&Cond);
         pthread_cond_wait(&Cond1,&Lock);
         pthread_mutex_unlock(&Lock);
      }
      else if(i == 1){
         pthread_mutex_lock(&Lock);
         FibSequenceArray[i] = 1;
         number = FibSequenceArray[i];
         indexOfArray++;
         pthread_cond_signal(&Cond);
         pthread_cond_wait(&Cond1,&Lock);
         pthread_mutex_unlock(&Lock);
      }
      else{
         pthread_mutex_lock(&Lock);
         FibSequenceArray[i] = FibSequenceArray[i-1] + FibSequenceArray[i-2];
         number = FibSequenceArray[i];
         indexOfArray++;
         pthread_cond_signal(&Cond);
         pthread_cond_wait(&Cond1,&Lock);
         pthread_mutex_unlock(&Lock);
      }
   }

   pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
   pthread_mutex_init(&Lock, NULL);
   pthread_cond_init(&Cond, NULL);
   pthread_cond_init(&Cond1, NULL);
   

   if (argc < 2) {
	fprintf(stderr,"usage: fibonacci <int>\n");
	/*exit(1);*/
	return -1;
   }
   arraySize = atoi(argv[1]);
   FibSequenceArray = malloc((arraySize) * sizeof(int));
   
   pthread_t thread;
   int rc;

   pthread_mutex_lock(&Lock);

   rc = pthread_create(&thread, NULL, CalculateFibonacci,NULL); 
   if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
   }

   
   while(indexOfArray < arraySize){
      pthread_cond_wait(&Cond,&Lock);
      printf("%d\n",number);
      pthread_cond_signal(&Cond1);
   }

   pthread_mutex_unlock(&Lock);
   pthread_join(thread, NULL);
}