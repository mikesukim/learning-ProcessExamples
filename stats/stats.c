#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS     3

int *intArray;
int arraySize;
double average;
int minimum;
int maximum;

void *CalculateAverage(void *intArrayIn)
{
   int *intArray;
   intArray = (int *)intArrayIn;

   double totalSum = 0;
   for(int i = 0; i < arraySize; i++){
      totalSum += intArray[i];
   }
   average = totalSum/arraySize;
   pthread_exit(NULL);
}
void *CalculateMinimum(void *intArrayIn)
{
   int *intArray;
   intArray = (int *)intArrayIn;
   for (int i = 0; i < arraySize; i++)
   {
      if (i == 0)
      {
         minimum = intArray[i];
      }
      else
      {
         if (minimum > intArray[i])
         {
            minimum = intArray[i];
         }
      }
   }

   pthread_exit(NULL);
}
void *CalculateMaximum(void *intArrayIn)
{
   int *intArray;
   intArray = (int *)intArrayIn;
   for (int i = 0; i < arraySize; i++)
   {
      if (i == 0)
      {
         maximum = intArray[i];
      }
      else
      {
         if (maximum < intArray[i])
         {
            maximum = intArray[i];
         }
      }
   }
   
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   if (argc < 2) {
	fprintf(stderr,"usage: stats <int> <int> <int> ...\n");
	/*exit(1);*/
	return -1;
   }

   intArray = malloc((argc-1) * sizeof(int));
   arraySize = 0;
   for(int i = 1; argv[i] != NULL; i++){
      int num = atoi(argv[i]);
      intArray[i-1] = num;
      arraySize++;
   }

   pthread_t threads[NUM_THREADS];
   int rc;

   for(int t=0; t<NUM_THREADS; t++){
      switch (t){
         case 0 : 
            rc = pthread_create(&threads[t], NULL, CalculateAverage, (void *)intArray); 
            break;
         case 1 :
            rc = pthread_create(&threads[t], NULL, CalculateMaximum, (void *)intArray);
            break;
         case 2 :
            rc = pthread_create(&threads[t], NULL, CalculateMinimum, (void *)intArray);
            break;
      }
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   for(int t=0; t<NUM_THREADS; t++){
       pthread_join(threads[t], NULL);
   }

   printf("The average value is %.2lf\n",average);
   printf("The minimum value is %d\n",minimum);
   printf("The maximum value is %d\n",maximum);
   
}