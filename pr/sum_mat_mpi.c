#include <stdio.h>
#include "mpi.h"

#define DIM 800

int main (int argc, char** argv) {
	double m[DIM][DIM];
	int i, j;
	int rank, my_rank, size, tag, source, dest;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	tag = 1;
	int start, end;
	double partial_sum = 0.0, sum_seq = 0.0, parallel_sum = 0.0;
  // fill mat
	for (i=0;i<DIM;i++) for(j=0;j<DIM;j++) m[i][j] = 1.02*i+1.04*j;

	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			sum_seq += m[i][j];
		}
	}

	if (rank == 0) {  
		printf("Jestem pierwszy. Watkow: %d Suma sekwencyjnie: %lf\n", size, sum_seq);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	start = rank * (DIM / size);
	if (rank != size -1) {
		end = (rank +1) * (DIM / size);
	} else {
		end = DIM;
	}
	printf("watek %d | start: %d | end: %d\n", rank, start, end);
  for (j = start; j < end; j++) {
		for (i = 0; i < DIM; i++) {
			//printf("watek %d : i=%d / j=%d\n", rank, i, j);
			partial_sum += m[i][j];
		}
	}

	MPI_Reduce(&partial_sum, &parallel_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
	  printf("parallelsum: %lf\n", parallel_sum);
	}

	MPI_Finalize();
	return 0;
}

