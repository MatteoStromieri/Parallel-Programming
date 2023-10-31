#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#define SEED 35791246

// the input is the amount of numbers that we want to generate 
int main (int argc , char *argv[])
{
  // number of points inside the circle 
  double pi=0;
  int nthreads;
  int niterations; 

  if (argc <=1 ) {
    printf("Please insert the number of iterations");
    return -1;
  }

  // get the number of iterations
  niterations = atoi(*(argv+1));
  // get the value of the environment variable RAND_MAX

  // start the threads
  #pragma omp parallel reduction(+ : pi)
  {
 	unsigned int myid = omp_get_thread_num();
  	unsigned int myseed = myid * SEED;
	nthreads = omp_get_num_threads();
  	// printf("nthreads: %d \n", nthreads);
	int my_iterations = niterations/nthreads + ((myid < niterations%nthreads) ? 1:0);
  
  	double x,y;

  	for(int i = 0; i < my_iterations; i++){
  		// printf("rand max %d \n", RAND_MAX);
		x = ((double)rand_r(&myseed)) / ((double)RAND_MAX);
		y = ((double)rand_r(&myseed)) / ((double)RAND_MAX);
  		
		if(x*x + y*y <= 1)
			pi += 1;
  	}
  }

  pi = pi / ((double)niterations) * 4.0;
  printf("Computed value: %f \n", pi);

  return 1;
  
}
