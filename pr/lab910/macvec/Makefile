CC = gcc
LD = gcc


OPT = -O3 -fopenmp
LIB = -lm

openmp_mat_vec: openmp_mat_vec.o mat_vec.o pomiar_czasu.o
	$(LD) $(OPT) openmp_mat_vec.o mat_vec.o  pomiar_czasu.o -o openmp_mat_vec $(LIB)


openmp_mat_vec.o: openmp_mat_vec.c pomiar_czasu.h
	$(CC) -c $(OPT) openmp_mat_vec.c

mat_vec.o: mat_vec.c
	$(CC) -c  $(OPT) mat_vec.c

pomiar_czasu.o: pomiar_czasu.c pomiar_czasu.h
	$(CC) -c $(OPT) pomiar_czasu.c


clean:
	rm -f *.o
