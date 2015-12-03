#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10
#define NUMTHREADS 10
main ()
{
    double a[WYMIAR][WYMIAR];
    int n,i,j,k, numthr;

    for(i=0;i<WYMIAR;i++) for(j=0;j<WYMIAR;j++) a[i][j]=1.02*i+1.01*j;

    n=WYMIAR;

    double suma=0.0;
    for(i=0;i<WYMIAR;i++) {
        for(j=0;j<WYMIAR;j++) {
            suma += a[i][j];
        }
    }

    printf("Suma wyrazów tablicy: %lf\n", suma);
 
    omp_set_num_threads(NUMTHREADS);

    double suma_parallel=0.0;
    // -----------------------------------------------------
    //podzial wierszowy
    // -----------------------------------------------------    
    //#pragma omp parallel for default(none) shared(suma_parallel, a) private(i, j)
    //for(i=0;i<WYMIAR;i++) {
    //  for(j=0;j<WYMIAR;j++) {
    //    suma_parallel += a[i][j];
    //    printf("thread %d, i %d, j %d\n", omp_get_thread_num(), i, j);
    //  }
    //}
    
    // -----------------------------------------------------
    //podzial kolumnowy
    // -----------------------------------------------------
    //for(i=0;i<WYMIAR;i++) {
    //	#pragma omp parallel for default(none) shared(a, i) private(j) reduction(+:suma_parallel)
    //  for(j=0;j<WYMIAR;j++) {
    //      suma_parallel += a[i][j];
    //      printf("thread %d, i %d, j %d\n", omp_get_thread_num(), i, j);
    //  }
    //}
    
    //double* private_sum = malloc(NUMTHREADS * sizeof(double));
    //for (k = 0; k < numthr; k++) {
    //	private_sum[k] = 0.0;
    //}
    
    
    // -----------------------------------------------------
    //podzial kolumnowy z sekcja krytyczna
    // -----------------------------------------------------
    #pragma omp parallel default(none) shared(suma_parallel, a) private(i, j)
    for(j=0;j<WYMIAR;j++) {
    	{
    		int tnum = omp_get_thread_num();
    		double private_sum = 0.0;
			#pragma omp for
		    for(i=0;i<WYMIAR;i++) {
		    	private_sum += a[i][j];
		        printf("thread %d, i %d, j %d\n", tnum, i, j);
		    }
		    #pragma omp critical
			{
				suma_parallel += private_sum;
			}
		}
    }
        
    //free(private_sum);
    
    // -----------------------------------------------------
    // podzial nested (2D)
    // -----------------------------------------------------
    //omp_set_nested(1);
    //#pragma omp parallel for default(none) shared(a) private(i, j) reduction(+:suma_parallel)
    //for(i=0;i<WYMIAR;i++) {
    //  #pragma omp parallel for default(none) shared(a, i) private(j) reduction(+:suma_parallel)
    //  for(j=0;j<WYMIAR;j++) {
    //      suma_parallel += a[i][j];
    //      printf("thread %d, i %d, j %d\n", omp_get_thread_num(), i, j);
    //  }
    //}
    
    printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);
}
