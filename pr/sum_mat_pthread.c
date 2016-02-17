#include <stdio.h>
#include <pthread.h>

#define DIM 2000

pthread_mutex_t mutex;
double m[DIM][DIM];
double parallel_sum;

void* sum_matrix(void* column) {
	int i, j;
	double sum_parallel;
	j = *( (int*) column );
	sum_parallel = 0.0;
	
	for (i = 0; i < DIM; i++) 
		sum_parallel += m[i][j];
	pthread_mutex_lock(&mutex);	
	parallel_sum += sum_parallel;
	pthread_mutex_unlock(&mutex);
}

int main () {
	int i, j;
	// fill mat
	for (i=0;i<DIM;i++) for(j=0;j<DIM;j++) m[i][j] = 1.02*i+1.04*j;

	double sum_seq = 0.0;
	parallel_sum = 0.0;
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			sum_seq += m[i][j];
		}
	}

	printf("Suma sekwencyjnie: %lf\n", sum_seq);

	pthread_t tid_vector[DIM];
	int col_vector[DIM];
	
	for (j = 0; j < DIM; j++)
		col_vector[j] = j;

	for (j = 0; j < DIM; j++)
		pthread_create(&tid_vector[j], NULL, sum_matrix, &col_vector[j]);

	for (j = 0; j < DIM; j++)
		pthread_join(tid_vector[j], NULL);

	printf("parallelsum: %lf\n", parallel_sum);
	return 0;
}

