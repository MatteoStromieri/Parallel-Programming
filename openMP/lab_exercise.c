
/* ────────────────────────────────────────────────────────────────────────── *
 *  │                                                                            │
 *   │ This file is part of the exercises for the Lectures on                     │
 *    │   "Foundations of High Performance Computing"                              │
 *     │ given at                                                                   │
 *      │   Master in HPC and                                                        │
 *       │   Master in Data Science and Scientific Computing                          │
 *        │ @ SISSA, ICTP and University of Trieste                                    │
 *         │                                                                            │
 *          │ contact: luca.tornatore@inaf.it                                            │
 *           │                                                                            │
 *            │     This is free software; you can redistribute it and/or modify           │
 *             │     it under the terms of the GNU General Public License as published by   │
 *              │     the Free Software Foundation; either version 3 of the License, or      │
 *               │     (at your option) any later version.                                    │
 *                │     This code is distributed in the hope that it will be useful,           │
 *                 │     but WITHOUT ANY WARRANTY; without even the implied warranty of         │
 *                  │     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          │
 *                   │     GNU General Public License for more details.                           │
 *                    │                                                                            │
 *                     │     You should have received a copy of the GNU General Public License      │
 *                      │     along with this program.  If not, see <http://www.gnu.org/licenses/>   │
 *                       │                                                                            │
 *                        * ────────────────────────────────────────────────────────────────────────── */


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>


#define N_DFLT 100
#define NTHREADS_DFLT 15

/*
 * I expect to receive two inputs
 * 1) the length of the array 
 * 2) the number of threads
 * */
int main ( int argc, char **argv )
{
	
	omp_set_dynamic(0);

	// length of the array
	int N = ( (argc > 1) ? atoi(*(argv+1)) : N_DFLT);

	// number of threads to spawn
	int nthreads = ( (argc > 2) ? atoi(*(argv+2)) :  NTHREADS_DFLT);

	int *array = (int*)malloc( sizeof(int) * N );
	      
	for(int i = 0; i < N; i++){
		array[i]=-1;
		printf("%d ", array[i]);
					  }

	omp_set_num_threads(nthreads);
	#pragma omp parallel firstprivate(array)
	{
		
		int myid = omp_get_thread_num();
		int myrange = (N/nthreads) + (myid < N%nthreads);
				        
		array += myrange*myid;
		if(myid >= N%nthreads)
			array += N%nthreads;

		for ( int i = 0; i < myrange; i++ )
			array[i] = myid;
	}

	printf("\n");

	for(int i = 0; i < N; i++){
		printf("%d ", array[i]);
		}

	printf("\n");

	return 0;
}


