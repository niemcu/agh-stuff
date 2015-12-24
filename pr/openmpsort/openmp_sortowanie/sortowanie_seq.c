/************************************************************************
Plik sortowanie.c, zawartosc:

  procedury sortowania:
    sortowanie_przez_wstawianie
    sortowanie_przez_scalanie
    sortowanie_szybkie

*************************************************************************/

#include<stdlib.h>
#include<stdio.h>

#include"sortowanie_seq.h"

/*** Deklaracje pomocniczych procedur wewnetrznych ***/

/*---------------------------------------------------------
  scal - procedura scala dwa posortowane fragmenty tablicy
         w posortowana calosc
----------------------------------------------------------*/
void scal(
  double* A,    /* in: tablica, dwa fragmenty ktorej maja zostac scalone */
  int p,        /* in: poczatkowy indeks pierwszego fragmentu do scalenia */
  int q,        /* in: koncowy indeks pierwszego fragmentu do scalenia */
  int r         /* in: koncowy indeks drugiego fragmentu do scalenia */
  );

/*---------------------------------------------------------
  podziel - procedura dzieli tablice A[p..k] na dwie podtablice 
            A[p..q] i A[q+1..k] tak, zeby kazdy element lewej podtablicy
            byl mniejszy lub rowny dowolnemu elementowi prawej podtablicy
---------------------------------------------------------*/
int podziel(    /* out: procedura zwraca indeks q dzielacy tablice */
  double* A,    /* in: tablica fragment ktorej ma zostac podzielony */
  int p,        /* in: poczatkowy indeks fragmentu do podzielenia */
  int k         /* in: koncowy indeks fragmentu do podzielenia */
  );


/*** Definicje procedur sortowania i pomocniczych ***/

/*---------------------------------------------------------
  sortowanie_przez_wstawianie - sortowanie tablicy n-elementowej,
    o wyrazach od A[1] do A[n] (wymaga alokacji tablicy rozmiaru n+1)
    - wersja z wartownikiem, uzywa elementu A[0] i zalozenia A[i]>0 
----------------------------------------------------------*/
void sortowanie_przez_wstawianie(
  double* A,    /* in: tablica do posortowania */
  int p,        /* in: poczatkowy indeks tablicy */
  int k         /* in: koncowy indeks tablicy */
  )
{

  int i,j,n;
  double t;

/*++++++++++++++++ executable statements ++++++++++++++++*/

  /* rozmiar tablicy */
  n=(p-k+1);

  /* dla kazdego i-tego wyrazu tablicy (wyrazy p..i-1 sa juz posortowane */
  for(i=p+1;i<=k;i++){

    /* rozwazamy i-ty wyraz ... */
    t=A[i];
    j=i-1;
    /* ... oraz wszystkie na lewo od niego, poczawszy od najblizszego ... */
    /* ... az do wyrazu mniejszego od i-tego */
    while(j>=p && A[j]>t){
      /* przesun o 1 w prawo wyraz mniejszy od i-tego */
      A[j+1]=A[j];
      j--;
    }

    /* wstaw i-ty wyraz przed ostatni wyraz wiekszy od niego */
    A[j+1]=t;

  }

  return;
}

/*---------------------------------------------------------
  sortowanie_przez_scalanie(A,p,r) - procedura sortuje czesc tablicy A
                                     od indeksu p do indeksu q 
----------------------------------------------------------*/
void sortowanie_przez_scalanie(
  double* A,    /* in: tablica fragment ktorej ma zostac posortowany */
  int p,        /* in: poczatkowy indeks fragmentu do posortowania */
  int r         /* in: koncowy indeks fragmentu do posortowania */
  )
{

  int q;

/*++++++++++++++++ executable statements ++++++++++++++++*/

  /* jezeli jest jeszcze cos do posortowania */
  if(p<r){

    //q=(int)(p+r)/2;
    /* znajdz srodkowy indeks A */
    q=(p+r)/2;
    /* posortuj oddzielnie lewa i prawa polowke A */
    sortowanie_przez_scalanie(A,p,q);
    sortowanie_przez_scalanie(A,q+1,r);
    /* scal posortowane polowki w posortowana calosc */
    scal(A,p,q,r);

  }

  return;
}

/*---------------------------------------------------------
  scal - procedura scala dwa posortowane fragmenty tablicy
         w posortowana calosc;
	 uwaga!!! musi zachodzic A[i]<1 dla kazdego i
----------------------------------------------------------*/
void scal(
  double* A,    /* in: tablica, dwa fragmenty ktorej maja zostac scalone */
  int p,        /* in: poczatkowy indeks pierwszego fragmentu do scalenia */
  int q,        /* in: koncowy indeks pierwszego fragmentu do scalenia */
  int r         /* in: koncowy indeks drugiego fragmentu do scalenia */
  )
{

  int i,i1,i2;
  double *b1, *b2;

/*++++++++++++++++ executable statements ++++++++++++++++*/

  /* przydziel miejsce dla tablic pomocniczych */
  b1 = (double *)malloc((q-p+2)*sizeof(double));
  b2 = (double *)malloc((r-q+1)*sizeof(double));

  /* przepisz podtablice A[p..q] i A[q+1..r], odpowiednio do b1 i b2 */
  for(i=0;i<=q-p;i++) b1[i]=A[p+i];
  for(i=0;i<=r-q-1;i++) b2[i]=A[q+1+i];

  /* wstaw wartownikow: uwaga!!! musi zachodzic A[i]<1 dla kazdego i */
  b1[q-p+1]=1.0;
  b2[r-q]=1.0;

  i1=0;
  i2=0;

  /* dla kazdego wolnego miejsca we fragmencie A[p..r] */
  for(i=0;i<=r-p;i++){

    /* jesli biezacy wyraz b1 jest mniejszy niz biezacy wyraz b2 */
    if(b1[i1]<b2[i2]){
      /* podstaw wyraz z b1 w odpowiednie miejsce A i przesun indeks w b1 */
      A[p+i]=b1[i1];
      i1++;
    }
    else{
      /* analogicznie do b1 jesli to wyraz b2 jest mniejszy */
      A[p+i]=b2[i2];
      i2++;
    }

  }

  /* zwolnij pamiec */
  free(b1);
  free(b2);

  return;
}


/*---------------------------------------------------------
  sortowanie_szybkie - procedura sortuje czesc tablicy A
                       od indeksu p do indeksu q
---------------------------------------------------------*/
void sortowanie_szybkie(
  double* A,    /* tablica fragment ktorej ma zostac posortowany */
  int p,        /* poczatkowy indeks fragmentu do posortowania */
  int r         /* koncowy indeks fragmentu do posortowania */
  )
{

  int q;

/*++++++++++++++++ executable statements ++++++++++++++++*/

  /* jezeli zostalo jeszcze cos do posortowania */
  if(p<r){ 

    /* podziel tablice tak, zeby kazdy element lewej podtablicy */
    /* byl mniejszy lub rowny dowolnemu elementowi prawej podtablicy */
    q=podziel(A,p,r);

    //printf("podzial: %d - %d - %d\n",p,q,r);

    /* posortuj podtablice */
    sortowanie_szybkie(A,p,q);
    sortowanie_szybkie(A,q+1,r);

  }

}


/*---------------------------------------------------------
  podziel - procedura dzieli tablice A[p..k] na dwie podtablice 
            A[p..q] i A[q+1..k] tak, zeby kazdy element lewej podtablicy
            byl mniejszy lub rowny dowolnemu elementowi prawej podtablicy
---------------------------------------------------------*/
int podziel(    /* out: procedura zwraca indeks q dzielacy tablice */
  double* A,    /* in: tablica fragment ktorej ma zostac podzielony */
  int p,        /* in: poczatkowy indeks fragmentu do podzielenia */
  int k         /* in: koncowy indeks fragmentu do podzielenia */
  )
{

  int i, j, q;
  double t, s;

/*++++++++++++++++ executable statements ++++++++++++++++*/

  /* wybierz element do porownan */
  t=A[p];

  /* zainicjuj indeksy: i do przeszukiwania od lewej, j od prawej */
  i=p-1;
  j=k+1;

  /* w nieskonczonej petli */
  for(;;){

    /* przeszukuj tablice w prawo az do elementu wiekszego lub rownego t */
    do j--; while(A[j]>t);
    /* przeszukuj tablice w lewo az do elementu mniejszego lub rownego t */
    do i++; while(A[i]<t);

    /* jesli jeszcze nie dokonal sie podzial - indeksy sa jeszcze daleko */
    if(i<j){

      /* zamien miejscami maly i duzy element */
      s=A[j];
      A[j]=A[i];
      A[i]=s;

    }
    /* jesli wszystkie elementy zostaly przegladniete - indeksy sie spotkaly */
    else{
      return(j);
    }

  }
  
}
