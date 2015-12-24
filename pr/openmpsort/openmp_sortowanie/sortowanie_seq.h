/*************************************************************************
Plik sort.h, naglowki procedur sortowania z pliku sortowanie.c
*************************************************************************/
#ifndef _sort_
#define _sort_


/*---------------------------------------------------------
  sortowanie_przez_wstawianie - sortowanie tablicy n-elementowej,
    o wyrazach od A[1] do A[n] (wymaga alokacji tablicy rozmiaru n+1)
----------------------------------------------------------*/
extern void sortowanie_przez_wstawianie(
  double* A,    /* tablica do posortowania */
  int p,        /* in: poczatkowy indeks tablicy */
  int k         /* in: koncowy indeks tablicy */
  );

/*---------------------------------------------------------
  sortowanie_przez_scalanie(A,p,r) - procedura sortuje czesc tablicy A
                                     od indeksu p do indeksu q 
----------------------------------------------------------*/
extern void sortowanie_przez_scalanie(
  double* A,    /* tablica fragment ktorej ma zostac posortowany */
  int p,        /* poczatkowy indeks fragmentu do posortowania */
  int r         /* koncowy indeks fragmentu do posortowania */
  );

/*---------------------------------------------------------
  sortowanie_szybkie - procedura sortuje czesc tablicy A
                       od indeksu p do indeksu q
---------------------------------------------------------*/
extern void sortowanie_szybkie(
  double* A,    /* tablica fragment ktorej ma zostac posortowany */
  int p,        /* poczatkowy indeks fragmentu do posortowania */
  int r         /* koncowy indeks fragmentu do posortowania */
  );

#endif

/*************************************************************************
koniec sort.h
*************************************************************************/
