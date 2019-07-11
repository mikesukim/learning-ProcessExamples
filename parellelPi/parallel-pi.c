#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define NUM_THREADS     4
#define NUM_POINTS 100000000

double totalHitCount = 0; 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

void *GeneragatePoints()
{

   srand(time(NULL));
   int hitCount = 0;
   for(int i = 0; i < (NUM_POINTS/NUM_THREADS); i++){

      int m = rand();
      double x = ((m / (RAND_MAX + 1.0) - 0.5) * 2);
      int n = rand();
      double y = ((n / (RAND_MAX + 1.0) - 0.5) * 2);
      
      if((pow(x,2) + pow(y,2)) < 1){
         hitCount++;
      }
   }
   pthread_mutex_lock(&lock);
   totalHitCount += hitCount;
   pthread_mutex_unlock(&lock);

   pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
   
   pthread_t threads[NUM_THREADS];
   int rc;

   if (pthread_mutex_init(&lock, NULL) != 0) 
   { 
        printf("\n mutex init has failed\n"); 
        return 1; 
   }


  for(int t=0; t<NUM_THREADS; t++){
      rc = pthread_create(&threads[t], NULL, GeneragatePoints, NULL);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }
 
   for(int t=0; t<NUM_THREADS; t++){
       pthread_join(threads[t], NULL);
   }
   
   double pi = 4 * totalHitCount / NUM_POINTS;
   printf("pi = %lf\n",pi);
   return 0;
}