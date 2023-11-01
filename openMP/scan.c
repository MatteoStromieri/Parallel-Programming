#include <stdio.h>
#include <stdlib.h>
#include <omp.h>



int main(int argc, char** argv){
	
	// check that you receive in input an array of integer
	if (argc == 1){
		printf("Please insert some input values \n");
		return -1;
	}
	
	int length = argc -1;
	int* array = (int*) malloc((length) * sizeof(int));
	printf("Array at location %p \n", array);
	// we want to store both the starting point of each partition and the highest value of each partition
	int** startingPoints;
	int* highestValue;
	int* lastInt = array + length - 1;
	// int* remainingLength;

	for(int i = 0; i < length; i++){
		array[i] = atoi(*(argv+i+1));	
		printf("%d: %d \n", i, array[i]);
	}

	// first of all we want to compute the partial partial sums 
	// we have to divide the array
	
	# pragma omp parallel 
	{
		int myId = omp_get_thread_num();
		int nThreads = omp_get_num_threads();
		int myLength = length / nThreads + ((myId < length % nThreads) ? 1:0);
		int* myStart = array + myLength*myId + ((myId >= length % nThreads) ? length % nThreads : 0);
		

		#pragma omp single
		{
			startingPoints = (int**) malloc(nThreads * sizeof(int*));
			highestValue = (int*) malloc(nThreads * sizeof(int));
			// remainingLength = (int*) malloc(nThreads * sizeof(int));
		}

		// store the starting point of the interval
		startingPoints[myId] = myStart;
		
		// store the remainder length
		// remainingLength[myId] = &(array[length-1]) - (myStart + myLength - 1);

		// now we can compute the partial sum 
		for(int i = 1; i < myLength; i++)
			myStart[i] += myStart[i-1];
		
		// store the highest value in the interval 
		highestValue[myId] = myStart[myLength-1];

		// wait for all the threads to write the array
		#pragma omp barrier
		
		// now we can compute the partial sums
		#pragma omp for ordered
		for(int i = 1; i < nThreads; i++){
			#pragma omp parallel for 
			for(int* j = startingPoints[i]; j <= lastInt; j++){
				*j = *j + highestValue[i-1];
			}	
		}
	} // implicit barrier synchronization

	// print the array
	for(int i=0; i < length; i++)
		printf("%d ", array[i]);
	printf("\n");

	// now, we have to combine the partial sum

	return 0;
}
