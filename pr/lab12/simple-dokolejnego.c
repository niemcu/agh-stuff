#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>

#include "mpi.h"

int main( int argc, char** argv ){ 
  
  int rank, size, source, dest, tag, i; 
  MPI_Status status;
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  char hostname[1024];
  gethostname(hostname,1024);
  hostname[1023]='\0';
 
  if(rank != 0){
      dest=0;
      MPI_Send( hostname, 1024, MPI_CHAR, dest , tag, MPI_COMM_WORLD );
  }else{
    char *hname = malloc(1024*sizeof(char));
    for(i=1;i<size;i++){
        MPI_Recv( hname, 1024, MPI_CHAR, MPI_ANY_SOURCE, 
                  MPI_ANY_TAG, MPI_COMM_WORLD, &status );
        printf("Dane od procesu o hoscie: %s (dane od rangi : %d moj rank %d)\n", 
                 hname, status.MPI_SOURCE, rank );
    } 
  }
  
  MPI_Finalize(); 
  
  return(0);

}

