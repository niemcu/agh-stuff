# prosty Makefile dla prostych programow

# nazwa pliku z kodem wykonywalnym
NAZWA = prod_cons

# kompilator C
CC = gcc
# konsolidator (linker, loader) C
LD = gcc
# archiwizator (program do tworzenia bibliotek)
AR = ar -ru

# opcje kompilacji
CFL = -pthread
LDFL = -pthread
# katalogi z bibliotekami i dolaczane biblioteki
LIB = -L. -lm
# katalogi z plikami naglowkowymi i wklejane pliki naglowkowe 
INC = -I.

prod_cons: prod_cons.o kolejka.o
	$(LD) $(LDFL) -o $(NAZWA) prod_cons.o kolejka.o $(LIB)

prod_cons.o: prod_cons.c kolejka.h
	$(CC) $(CFL) -c prod_cons.c $(INC)

kolejka.o: kolejka.c kolejka.h
	$(CC) $(CFL) -c kolejka.c $(INC)

clean:
	rm *.o 
