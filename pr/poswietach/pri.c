#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>
 
#include "mpi.h"
 
int main( int argc, char** argv ){
 
  int a,b,rank, ranksent, size, source, dest, tag, i;
  MPI_Status status;
 
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
 
  int liczba;
  if( rank == 0 )
     scanf("%d",&liczba);
 
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Bcast(&liczba,1,MPI_INT,0,MPI_COMM_WORLD);
 
  int koniec,poczatek;
  poczatek=(liczba/size)*rank;
 
  if(rank==size-1)
      koniec=liczba;
  else
     koniec=poczatek + (liczba/size);
  float wynik=0;
 
  for(i = poczatek; i < koniec-1; i++)
  {
     if(i%2 == 0)
         wynik += 1./(2*i + 1);
     else
         wynik -= 1./(2*i + 1);
  }
  printf("moja ranga %d wynik: %f poczatek %d koniec %d size %d\n",rank,wynik,poczatek,koniec,size);
  float wynik_g=0;  
  MPI_Allreduce(&wynik,&wynik_g,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
  if(rank == 0)
     printf("wynik koncowy: %f\n",wynik_g);
  MPI_Finalize();
  return(0);
 

    }

