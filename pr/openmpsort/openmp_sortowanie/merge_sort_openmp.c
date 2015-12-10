#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

#include "sortowanie_seq.h"

void merge_sort_openmp(
  double* A,   
  int p,      
  int r,
  int poziom      
){

/*++++++++++++++++ executable statements ++++++++++++++++*/

  if(p<r){
    
    //printf("watek %d, poziom %d\n", omp_get_thread_num(),poziom);

#define  max_poziom 2

    poziom++;

    int q1=(p+r)/2;
    
#pragma omp task final( poziom>max_poziom ) default(none) firstprivate(A,p,r,q1,poziom) 
    {
      merge_sort_openmp(A,p,q1,poziom);
    }

#pragma omp task final( poziom>max_poziom ) default(none) firstprivate(A,p,r,q1,poziom) 
    {
      merge_sort_openmp(A,q1+1,r,poziom);
    }

#pragma omp taskwait  

    scal(A,p,q1,r);
    
  }
  
  return;
}

void merge_sort_openmp_2(
  double* A,   
  int p,      
  int r,
  int poziom      
){

/*++++++++++++++++ executable statements ++++++++++++++++*/

  if(p<r){
    
    printf("watek %d, poziom %d\n", omp_get_thread_num(),poziom);

    poziom++;

    int q1=(p+r)/2;
    
#define poziom_max 3

#pragma omp task final( poziom > poziom_max ) default(none) firstprivate(A,p,r,q1,poziom) 
    {
      if(omp_in_final()) sortowanie_szybkie(A,p,q1);
      else merge_sort_openmp_2(A,p,q1,poziom);
    }

#pragma omp task final( poziom > poziom_max ) default(none) firstprivate(A,p,r,q1,poziom) 
    {
      if(omp_in_final()) sortowanie_szybkie(A,q1+1,r);
      else merge_sort_openmp_2(A,q1+1,r,poziom);
    }

#pragma omp taskwait  

    scal(A,p,q1,r);
    
  }
  
  return;
}


void merge_sort_openmp_4(
  double* A,   
  int p,      
  int r      
){

/*++++++++++++++++ executable statements ++++++++++++++++*/

  if(p<r){

    int q1=(p+r)/2;

    omp_set_nested(1);
    
#pragma omp parallel sections default(none) firstprivate(A,p,r,q1)
    {
      
#pragma omp section
      {
	
	int q2 = (p+q1)/2;
	
#pragma omp parallel sections default(none) firstprivate(A,p,r,q1,q2)
	{
	  
#pragma omp section
	  {
	    sortowanie_szybkie(A,p,q2);
	  }
	  
#pragma omp section
	  {
	    sortowanie_szybkie(A,q2+1,q1);
	  }
	  
	}
	
	scal(A,p,q2,q1);
      }

#pragma omp section
      {
	
	int q2 = (q1+1+r)/2;
	
#pragma omp parallel sections default(none) firstprivate(A,p,r,q1,q2)
	{
	  
#pragma omp section
	  {
	    sortowanie_szybkie(A,q1+1,q2);
	  }
	  
#pragma omp section
	  {
	    sortowanie_szybkie(A,q2+1,r);
	  }
	  
	}
	
	scal(A,q1+1,q2,r);
      }
      
    }
  
    scal(A,p,q1,r);
    
  }
  
  return;
}


