#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

unsigned long long int uint_pow(unsigned int base, unsigned int exp){
        unsigned long long int result = (unsigned long long int)base;
        for(int i=1; i<exp; i++){
                result = result * base;
        }
        return result;
}

int main(int argc, char *argv[]){

        unsigned long long int accuracy = (argc == 2)? atoi(argv[1]) : 5;
        unsigned long long int power = uint_pow(10,2*accuracy+3);
        unsigned long long int iterations = 3*power;

        printf("power: %llu \n",power);
        printf("accuracy: %llu \n",accuracy);
        printf("iterations: %llu \n",iterations);

        double x,y;
        unsigned long long int k=0;

        int my_seed;
        #pragma omp parallel for reduction(+:k) private(my_seed)
        for(int i = 0; i < iterations; i++){
                my_seed=i*omp_get_thread_num();
                x = (((double)rand_r(&my_seed)) / RAND_MAX)*2 -1;
                my_seed++;
                y = (((double)rand_r(&my_seed)) / RAND_MAX)*2 -1;
                if(x*x + y*y < 1)
                        k++;
        }

        printf("The sum of Ks is %llu \n",k);
        long double pi = 4.0 * (long double)k/iterations;
        printf("Computed PI = %Lf",pi);

        return 1;
}
