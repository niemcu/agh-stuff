CC = gcc
LD = gcc


OPT = -O3 -fopenmp
LIB = -lm

test_search_max_openmp: test_search_max_openmp.o search_max_openmp.o 
	$(LD) $(OPT) test_search_max_openmp.o search_max_openmp.o -o test_search_max_openmp $(LIB)


test_search_max_openmp.o: test_search_max_openmp.c search_max_openmp.h
	$(CC) -c $(OPT) test_search_max_openmp.c

search_max_openmp.o: search_max_openmp.c search_max_openmp.h
	$(CC) -c $(OPT) search_max_openmp.c



clean:
	rm -f *.o
