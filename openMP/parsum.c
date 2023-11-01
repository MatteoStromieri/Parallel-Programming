#include <stdlib.h>
#include <stdio.h>

/*
 *We want to write a funnction that receives an array as input and sums a value to its entries in a parallel fashion
 * */

/*
 *	Inputs:
 *	- int* is the array
 * 	- int is the length 
 * 	- int is the value that we want to add to every entry
 * */
void parallelSum(int*,int,int); 

void parallelSum(int* array, int n, int x){
	
	#pragma omp parallel for
	for(int i = 0; i < n; i++)
		array[i] += x;
}

int main(){
	return 1;
}
