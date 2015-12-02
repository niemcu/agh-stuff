#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10

main ()
{
  double a[WYMIAR][WYMIAR];
  int n,i,j;

  for(i=0;i<WYMIAR;i++) for(j=0;j<WYMIAR;j++) a[i][j]=1.02*i+1.01*j;

  n=WYMIAR;

  double suma=0.0;
  for(i=0;i<WYMIAR;i++) {
    for(j=0;j<WYMIAR;j++) {
      suma += a[i][j];
    }
  }
  
  printf("Suma wyrazów tablicy: %lf\n", suma);

  omp_set_nested(1);
  omp_set_num_threads(5);
  double suma_parallel=0.0;
  #pragma omp parallel for private(i, j)
  for(i=0;i<WYMIAR;i++) {
    for(j=0;j<WYMIAR;j++) {
      suma_parallel += a[i][j];
      printf("thread %d, i %d, j %d\n", omp_get_thread_num(), i, j);
    }
  }

  printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);

}
