# prosty Makefile dla prostych programow

NAZWA = test_sortowanie

CC = gcc
LD = gcc
AR = ar -ru

CFL = -O -fopenmp
LDFL = -O -fopenmp
LIB = -L. -lsortowanie_seq
INC = -I.

#sortowanie: sterowanie.o sortowanie.o 
#	$(LD) $(LDFL) -o $(NAZWA) sterowanie.o sortowanie.o 

test_sortowanie: test_sortowanie.o  merge_sort_openmp.o libsortowanie_seq.a
	$(LD) $(LDFL) -o $(NAZWA) test_sortowanie.o merge_sort_openmp.o $(LIB)

test_sortowanie.o: test_sortowanie.c sortowanie_seq.h
	$(CC) $(CFL) -c test_sortowanie.c $(INC)

libsortowanie_seq.a: sortowanie_seq.o
	$(AR) libsortowanie_seq.a sortowanie_seq.o

sortowanie_seq.o: sortowanie_seq.c sortowanie_seq.h
	$(CC) $(CFL) -c sortowanie_seq.c $(INC)

merge_sort_openmp.o: merge_sort_openmp.c 
	$(CC) $(CFL) -c merge_sort_openmp.c $(INC)

clean:
	rm *.o $(NAZWA)
