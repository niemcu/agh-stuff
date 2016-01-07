#include <stdlib.h>
#include<stdio.h>
#include<math.h>

#include "mpi.h"

int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, tag, i; 
  MPI_Status status;
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  
  tag = 1;
  
  if (rank == 0) {
  	dest = 1;
  	printf("Jestem pierwszy, wysylam rank: %d \n", rank);
    MPI_Send(&rank, 1, MPI_INT, dest, tag, MPI_COMM_WORLD );
  } else {
  	MPI_Recv(&ranksent, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  	printf("Dane od procesu o randze %d: %d (%d) \n", rank, ranksent, status.MPI_SOURCE);
  	dest = rank + 1;
    if (rank != size - 1) {
      printf("Wysylam dalej do procesu dest= %d \n", dest);
   	  MPI_Send( &rank, 1, MPI_INT, dest, tag, MPI_COMM_WORLD );
   	}
  }
  MPI_Finalize(); 
  
  return(0);

}

