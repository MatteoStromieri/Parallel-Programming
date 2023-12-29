#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

unsigned long long int uint_pow(unsigned int base, unsigned int exp){

        unsigned long long int result = (unsigned long long int)base;

        for(int i=1; i<exp; i++){
                result = result * base;
        }
        return result;
}

int main(int argc,char* argv[]){

        MPI_Init(&argc,&argv);

        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        unsigned long long int accuracy = (argc == 2)? atoi(argv[1]) : 5;
        unsigned long long int power = uint_pow(10,2*accuracy+3);
        unsigned long long int iterations = 3*power;
        int remainder = (iterations%world_size==0) ? 0 : (world_rank < iterations%world_size);
        int my_iterations = iterations/world_size + remainder;

        double x,y;
        int current_time = time(NULL);
        unsigned long int k = 0;

        for(int i = 0; i <  my_iterations; i++){
            srand(current_time+i+world_rank);
            x = (((double)rand()) / RAND_MAX)*2 -1;
            y = (((double)rand()) / RAND_MAX)*2 -1;
            if(x*x + y*y < 1)
                k++;
        }
	
	unsigned long int * rcvbuf;
	if(world_rank==0){
		 rcvbuf = (unsigned long int*)malloc((world_size)*sizeof(unsigned long int));
	}
	int return_value=MPI_Gather((void*)&k,1,MPI_UNSIGNED_LONG,(void*)rcvbuf,1,MPI_UNSIGNED_LONG,0,MPI_COMM_WORLD);
	printf("process %d Gather return value = %d \n ",world_rank,return_value);
	
		
	if(world_rank==0){
		printf("Let's print the gathered array: ");
		for(int i = 0; i<world_size;i++)
			printf("%lu ",rcvbuf[i]);
		printf("\n");

		unsigned long long int sum=0;
		
		for(int i=0;i<world_size-1;i++){
			sum+=(unsigned long long int)rcvbuf[i];
		}
		free(rcvbuf);
		long double pi = (long double)4.0 * (long double)sum/iterations;
		printf("Computed PI = %Lf \n",pi);
	}

	MPI_Finalize();
	
        return EXIT_SUCCESS;
}
