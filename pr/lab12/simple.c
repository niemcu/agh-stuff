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
  
  if( rank != 0 ){ dest=0; tag=0; 
    
    MPI_Send( &rank, 1, MPI_INT, dest, tag, MPI_COMM_WORLD );
    
  } else {
    
    for( i=1; i<size; i++ ) { 
      
      MPI_Recv( &ranksent, 1, MPI_INT, MPI_ANY_SOURCE, 
		MPI_ANY_TAG, MPI_COMM_WORLD, &status );
      printf("Dane od procesu o randze: %d (%d)\n", 
	     ranksent, status.MPI_SOURCE );}
    
  }

  
  MPI_Finalize(); 
  
  return(0);

}

