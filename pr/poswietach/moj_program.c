#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"
#include "pomiar_czasu.h"

#define WYMIAR 19200
#define ROZMIAR 368640000

void mat_vec(double* a, double* x, double* y, int n, int nt);

int
main ( int argc, char** argv )
{
  static double x[WYMIAR],y[WYMIAR],z[WYMIAR];
  double *a;
  int n,nt,i,j;
  const int ione=1;
  const double done=1.0;
  const double dzero=0.0;

  double t_begin, t_end;

  int rank, size, source, dest, tag=0; 
  int n_wier, n_wier_last;
  MPI_Status status;
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  n=WYMIAR;
  int width = n;
  int height = n;

  if(rank==0){  

  a = (double *) malloc(ROZMIAR*sizeof(double));

  for(i=0;i<ROZMIAR;i++) a[i]=1.0*i;
  for(i=0;i<WYMIAR;i++) x[i]=1.0*(WYMIAR-i);

  //for(i=0;i<WYMIAR;i++) printf("1 rank %d, x[%2d] = %lf\n",rank,i,x[i]);


  printf("Podaj liczbe watkow: \n"); scanf("%d",&nt);
  //nt = 2;

  printf("poczatek\n");

  t_begin = czas_zegara();
  mat_vec(a,x,y,n,nt);
  t_end = czas_zegara();

  printf("EXECUTION TIME: executing standard loop: %lf\n", t_end-t_begin);
  printf("\tNumber of operations %d, PERFORMANCE %lf GFlops\n",
	 2*width*height, 2*width*height / (t_end-t_begin) * 1e-9);
  printf("\tGBytes transferred to processor %lf, speed %lf GB/s\n",
	 2*width*height*sizeof(float)*1e-9, 
	 2*width*height*sizeof(float)/(t_end-t_begin)*1e-9);


  // test
  printf("TEST\n");

  t_begin = czas_zegara();

  for(i=0;i<n;i++){
    double t=0.0;
    int ni = n*i;
#pragma omp parallel for num_threads(nt) firstprivate(ni) reduction(+:t)
    for(j=0;j<n;j++){
      t+=a[ni+j]*x[j];
    }
    z[i]=t;
  }
  t_end = czas_zegara();

  printf("EXECUTION TIME: executing standard loop: %lf\n", t_end-t_begin);
  printf("\tNumber of operations %d, PERFORMANCE %lf GFlops\n",
	 2*width*height, 2*width*height / (t_end-t_begin) * 1e-9);
  printf("\tGBytes transferred to processor %lf, speed %lf GB/s\n",
	 2*width*height*sizeof(float)*1e-9, 
	 2*width*height*sizeof(float)/(t_end-t_begin)*1e-9);

  for(i=0;i<WYMIAR;i++){
    if(fabs(y[i]-z[i])>1.e-9*z[i]) printf("Blad!\n");
  }

  // test 2
  printf("TEST 2\n");

  t_begin = czas_zegara();

#pragma omp parallel for num_threads(nt) firstprivate(n) private(j) 
  for(i=0;i<n;i++){
    double t=0.0;
    int ni = n*i;
    for(j=0;j<n;j++){
      t+=a[ni+j]*x[j];
    }
    z[i]=t;
  }
  t_end = czas_zegara();

  printf("EXECUTION TIME: executing standard loop: %lf\n", t_end-t_begin);
  printf("\tNumber of operations %d, PERFORMANCE %lf GFlops\n",
	 2*width*height, 2*width*height / (t_end-t_begin) * 1e-9);
  printf("\tGBytes transferred to processor %lf, speed %lf GB/s\n",
	 2*width*height*sizeof(float)*1e-9, 
	 2*width*height*sizeof(float)/(t_end-t_begin)*1e-9);

  for(i=0;i<WYMIAR;i++){
    if(fabs(y[i]-z[i])>1.e-9*z[i]) printf("Blad!\n");
  }

  //for(i=0;i<WYMIAR;i++) printf("5 rank %d, x[%2d] = %lf\n",rank,i,x[i]);

  // test
  //printf("MKL\n");

  //  t_begin = czas_zegara();

  //dgemv_("T", &n, &n, &done, a, &n, x, &ione, &dzero, z, &ione);
  //drukuj_czas();

  //for(i=0;i<WYMIAR;i++){
  //  if(fabs(y[i]-z[i])>1.e-9*z[i]) printf("Blad!\n");
  //}
  
  }



/************** || *******************/

  if(size>1){

  for(i=0;i<WYMIAR;i++) z[i]=0.0;
  MPI_Bcast(&nt, 1, MPI_INT, 0, MPI_COMM_WORLD );

// podzial wierszowy
  n_wier = ceil(WYMIAR / size);
  n_wier_last = WYMIAR - n_wier*(size-1);

  if(n_wier!=n_wier_last){

    printf("This version does not work with WYMIAR not a multiple of size!\n");
    MPI_Finalize(); 
    exit(0);

  }
  
  if(rank==0){

    for(i=1;i<size-1;i++){
      MPI_Send( &a[i*WYMIAR*n_wier], n_wier*WYMIAR, MPI_DOUBLE, i, tag, MPI_COMM_WORLD );
      MPI_Send( &x[i*n_wier], n_wier, MPI_DOUBLE, i, tag, MPI_COMM_WORLD );
    }

    MPI_Send( &a[(size-1)*WYMIAR*n_wier], n_wier_last*WYMIAR, MPI_DOUBLE, size-1, tag, MPI_COMM_WORLD );
    MPI_Send( &x[(size-1)*n_wier], n_wier_last, MPI_DOUBLE, size-1, tag, MPI_COMM_WORLD );
	
  } else {
	
      //printf("beginning: rank %d size %d\n",rank, size);
	
    for(i=0;i<WYMIAR;i++) x[i]=0.0;
	
    source = 0;
    if(rank<size-1){
	    
	//printf("before aloc, n_wier %d, rank %d size %d\n",n_wier,rank, size);
	    
      a = (double *) malloc(WYMIAR*n_wier*sizeof(double));
	    
      //printf("after aloc, n_wier %d, rank %d size %d\n",n_wier,rank, size);
	    
      MPI_Recv( a, n_wier*WYMIAR, MPI_DOUBLE, source,
		MPI_ANY_TAG, MPI_COMM_WORLD, &status );
      MPI_Recv( &x[rank*n_wier], n_wier, MPI_DOUBLE, source,
		MPI_ANY_TAG, MPI_COMM_WORLD, &status );
	    
    } else {

	//printf("before aloc, n_wier %d, rank %d size %d\n",n_wier_last,rank, size);

      a = (double *) malloc(WYMIAR*n_wier*sizeof(double));

      //printf("after aloc, n_wier %d, rank %d size %d\n",n_wier_last,rank, size);

      MPI_Recv( a, n_wier_last*WYMIAR, MPI_DOUBLE, source,
		MPI_ANY_TAG, MPI_COMM_WORLD, &status );
      MPI_Recv( &x[(size-1)*n_wier], n_wier_last, MPI_DOUBLE, source,
		MPI_ANY_TAG, MPI_COMM_WORLD, &status );


    }
  
    //for(i=0;i<WYMIAR;i++) printf("rank %d, received x[%2d] = %lf\n",rank,i,x[i]);

    for(j=0;j<n_wier;j++){
      for(i=0;i<WYMIAR;i++) {
	  //printf("rank %d, received a[%4d] = %lf\n",rank,
	  //     rank*n_wier*WYMIAR+j*WYMIAR+i,a[j*WYMIAR+i]);
      }
    }

  }

  if(rank==0)   t_begin = czas_zegara();


  MPI_Allgather(&x[rank*n_wier], n_wier, MPI_DOUBLE, x, n_wier, MPI_DOUBLE, MPI_COMM_WORLD );

  //for(i=0;i<WYMIAR;i++) printf("after Allgather: rank %d, x[%2d] = %lf\n",rank,i,x[i]);

#pragma omp parallel for num_threads(nt) firstprivate(n, n_wier) private(j) 
  for(i=0;i<n_wier;i++){
    double t=0.0;
    int ni = n*i;
    for(j=0;j<n;j++){
      t+=a[ni+j]*x[j];
    }
    z[i]=t;
  }
   
  //for(i=0;i<WYMIAR;i++) printf("after mult: rank %d, z[%2d] = %lf\n",rank,i,z[i]);

  MPI_Barrier(MPI_COMM_WORLD);

  if(rank==0) {
  t_end = czas_zegara();
      printf("Werja rownolegla MPI\n");

  printf("EXECUTION TIME: executing standard loop: %lf\n", t_end-t_begin);
  printf("\tNumber of operations %d, PERFORMANCE %lf GFlops\n",
	 2*width*height, 2*width*height / (t_end-t_begin) * 1e-9);
  printf("\tGBytes transferred to processor %lf, speed %lf GB/s\n",
	 2*width*height*sizeof(float)*1e-9, 
	 2*width*height*sizeof(float)/(t_end-t_begin)*1e-9);

  }

  if(rank>0){

       MPI_Send( z, n_wier, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD );
     
  } else {

    for(i=1;i<size;i++){
      MPI_Recv( &z[i*n_wier], n_wier, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status  );
    }

    for(i=0;i<WYMIAR;i++){
	if(fabs(y[i]-z[i])>1.e-9*z[i]) {
	    printf("Blad! i=%d, y[i]=%lf, z[i]=%lf\n",i, y[i], z[i]);
	}
    }
  }
  }

  MPI_Finalize(); 
  
  return(0);


}
