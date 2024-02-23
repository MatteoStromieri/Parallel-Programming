#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include <time.h>

#define ALIGNMENT 64
#define CHUNK_SIZE 5

void mandelbrotSet(unsigned char, double complex, double complex, int, int);
void printMatrix(unsigned char*, int, int);
unsigned char mandelbrotSeries(double complex, unsigned char max_iterations);

void mandelbrotSet(unsigned char max_iterations, double complex c_left, double complex c_right, int n_x, int n_y){
    
    unsigned char* mandelbrot_matrix;//(unsigned char*) malloc(n_x*n_y*sizeof(unsigned char));
    if(posix_memalign((void**)&mandelbrot_matrix,ALIGNMENT,n_x*n_y*sizeof(unsigned char))){
        fprintf(stderr, "[mandelbrotSet function] Errror in memory allocation.\n");
        exit(1);
    }
    double interval_width = creal(c_right) - creal(c_left);
    double interval_height = cimag(c_right) - cimag(c_left);
    double delta_x = interval_width/n_y;
    double delta_y = interval_height/n_x;
    double complex delta = c_left;

    printf("delta x = %lf \ndelta y = %lf \n", delta_x, delta_y);

    #pragma omp for schedule(dynamic,CHUNK_SIZE)
    for(int i = 0; i < n_x; i++){
        for(int j = 0; j < n_y; j++){
            delta = delta_x*i + (delta_y*j)*I;
            mandelbrot_matrix[i*n_y+j] = mandelbrotSeries(c_left + delta,max_iterations);
        }
    }
    // printMatrix(mandelbrot_matrix,n_x,n_y);
    writePGM("mandelbrot_image.pgm",mandelbrot_matrix,n_x,n_y);
}


int main(int argc, char *argv[]){

    clock_t start, end;
    start = clock();

    unsigned char max_iterations = 100;
    double complex c_left = -2.0 -2.0*I;
    double complex c_right = 2.0 + 2.0*I;
    int n_x = (argc==3)? atoi(argv[1]) : 10;
    int n_y = (argc==3)? atoi(argv[2]) : 10;

    mandelbrotSet(max_iterations,c_left,c_right,n_x,n_y);

    end = clock();
    printf("CPU time for the optimized version: %f \n", (double) end-start);
    return 0;
}

unsigned char mandelbrotSeries(double complex c, unsigned char max_iterations){
    unsigned char tmp = 1;
    double complex z = c;

    while(cabs(z)<2.0 && tmp < max_iterations){
        z = z*z + c;
        tmp++;
    }

    if(tmp == max_iterations)
        return 0;
    return tmp;
}

void printMatrix(unsigned char* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", matrix[i*cols + j]);  // Assuming integer matrix; adjust format specifiers as needed
        }
        printf("\n");  // Move to the next line after printing each row
    }
}
