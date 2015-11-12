#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include"kolejka.h"


/*** Definicje procedur ***/

/*---------------------------------------------------------
  kolejka_inicjuj - procedura inicjuje kolejke i zwraca uchwyt do kolejki
----------------------------------------------------------*/
kolejka* kolejka_inicjuj(
  int rozmiar
  )
{
  kolejka* K;
  K = (kolejka *)malloc(sizeof(kolejka));
  K->indeksmax = rozmiar;
  K->A = (int *)malloc((rozmiar+1)*sizeof(int));
  K->glowa = 0;
  K->ogon = 0;
  return(K);
}

/*---------------------------------------------------------
  kolejka_skasuj - procedura usuwa kolejke i zwalnia pamiec
----------------------------------------------------------*/
void kolejka_skasuj(
  kolejka* K // uchwyt do kolejki
)
{
  free(K->A);
  free(K);   
}

/*---------------------------------------------------------
  kolejka_pelna, kolejka_pusta - informacje o kolejce
----------------------------------------------------------*/
int kolejka_pelna(
  kolejka* K // uchwyt do kolejki
  )
{
  if((K->ogon==K->indeksmax && K->glowa==0) || K->glowa-1 == K->ogon) return(1);
  else return(0);
}

int kolejka_pusta(
  kolejka* K // uchwyt do kolejki
  )
{
  if(K->glowa == K->ogon) return(1);
  else return(0);
}

/*---------------------------------------------------------
  kolejka_wstaw, kolejka_pobierz - obsluga kolejki, wstawianie i usuwanie elementow
----------------------------------------------------------*/
void kolejka_wstaw(
  kolejka* K, // uchwyt do kolejki
  int v       /* wartosc wstawiana do kolejki */
  )
{

/*++++++++++++++++ executable statements ++++++++++++++++*/
  
#ifdef DEBUG
  /* powrot w przypadku przepelnienia */
  if((K->ogon==K->indeksmax && K->glowa==0) || K->glowa-1 == K->ogon){
    printf("KOLEJKA: BLAD PRZEPELNIENIA!!!\n"); 
    exit(-1); 
  }
#endif

  /* podstawienie na koniec */
  K->A[K->ogon]=v;
  /* przesuniecie konca kolejki */
  if(K->ogon==K->indeksmax) K->ogon=0;
  else K->ogon++;

}

int kolejka_pobierz(
  kolejka* K // uchwyt do kolejki
  )
{
  int v;  /* zmienna pomocnicza */

/*++++++++++++++++ executable statements ++++++++++++++++*/

#ifdef DEBUG
  /* powrot dla pustej listy */
  if(K->glowa==K->ogon) {
    printf("KOLEJKA: BLAD NIEDOMIARU!!!\n"); 
    exit(-1); 
  }
#endif

  /* podstawienie wartosci z poczatku listy */
  v=K->A[K->glowa];

  /* przesuniecie poczatku kolejki */
  if(K->glowa==K->indeksmax) K->glowa=0;
  else K->glowa++;

  return(v);
}

/*---------------------------------------------------------
  kolejka_drukuj - wypisz zawartosc kolejki 
----------------------------------------------------------*/
void kolejka_drukuj( 
  kolejka* K // uchwyt do kolejki
  )
{
  int i;

  printf("glowa %d,   ogon %d\n", K->glowa, K->ogon);
  i=K->glowa;
  while(i!=K->ogon){
    printf("%d ",K->A[i]);
    i++;
    if(i>K->indeksmax) i=0;
  }
  printf("\n");
}
