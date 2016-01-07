#include <stdlib.h>
#include<stdio.h>
#include<math.h>

#include "mpi.h"

int main( int argc, char** argv ){ 
  
  int rank, num, size, i, j; 
  double sum, result;
  double starttime, endtime;
  MPI_Status status;
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  
  sum = 0.0;
  result = 0.0;
  
  if (rank == 0) {
    printf("Podaj liczbe skladnikow szeregu: \n");
  	scanf("%d", &num);	
    starttime = MPI_Wtime();
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Bcast(&num, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  int start = rank * (num/size);
  int end;
  
  if (rank != size - 1) {
  	end = (rank + 1) * (num/size);
  } else {
  	end = num;
  }
  //printf ("proces nr %d : start: %d koniec: %d \n", rank, start, end);
  for (i = start; i < end; i = i+2) {
    sum += (1. / (2.0*i + 1.));
  }
  for (j = start + 1; j < end; j = j+2) {
  	sum -= (1. / (2.0*j + 1.));
  }
  
  //printf ("proces nr %d : suma czesciowa: %f \n", rank, sum);
  
  
  MPI_Reduce(&sum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
  if (rank == 0) {
    endtime   = MPI_Wtime();
    printf("Czas wykonania: %f sekund \n", endtime - starttime);
  	printf ("Wynik moj: %.20lf \nliczba PI: %.20lf \n", result * 4, M_PI);
  }
  
  MPI_Finalize(); 
  
  return(0);

}

