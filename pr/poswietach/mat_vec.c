#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void mat_vec(double* a, double* x, double* y, int nn, int nt)
{
  

//   printf("nr threads = %d\n", omp_get_num_threads());

#pragma omp parallel num_threads(nt) 
 {
   register int k=0;
   register int n=nn;
   register int i;
   register int j;

//   printf("nr threads = %d\n", omp_get_num_threads());

#pragma omp for
  for(i=0;i<n;i+=2){
    register double ty1 = 0;
    register double ty2 = 0;
   for(j=0;j<n;j+=2){
      register double t0=x[j];
      register double t1=x[j+1];
      k= i*n+j;
      ty1  +=a[k]*t0    +a[k+1]*t1 ;
      ty2  +=a[k+n]*t0  +a[k+1+n]*t1;
    }
      y[i]  = ty1;
      y[i+1]+=ty2;
  }
 }  
}

