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

		// now we can compute the partial sum 
		for(int i = 1; i < myLength; i++)
			myStart[i] += myStart[i-1]; 
	}

	// print the array
	for(int i=0; i < length; i++)
		printf("%d ", array[i]);
	printf("\n");


	return 0;
}
