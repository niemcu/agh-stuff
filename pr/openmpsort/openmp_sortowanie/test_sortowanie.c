/************************************************************************
Plik sort.c, zawartosc:

  main       - program glowny do testowania algorytmow sortowania 
  generuj    - procedura generowania ciagu liczbowego o losowych wartosciach
  sprawdz    - procedura sprawdzania czy posortowany ciag jest rosnacy
 *************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include"sortowanie_seq.h"

/*** Deklaracje pomocniczych procedur wewnetrznych ***/

/*---------------------------------------------------------
  generuj    - procedura generowania ciagu liczbowego o losowych wartosciach
----------------------------------------------------------*/
void generuj(
  double* A,    /* wskaznik do generowanej tablicy */
  int n         /* rozmiar tablicy */
  );

/*---------------------------------------------------------
  sprawdz    - procedura sprawdzania czy posortowany ciag jest rosnacy
----------------------------------------------------------*/
int sprawdz(
  double* A,    /* tablica do sprawdzenia */
  int n         /* rozmiar tablicy */
  );

/*---------------------------------------------------------
  porownaj - porownanie dwoch elementow tablicy
----------------------------------------------------------*/
int porownaj( /* procedura zwraca: -1 - x<y */
	      /*                    0 - x=y */
	      /*                    1 - x>y */
  double* x,  /* element do porownania */
  double* y   /* element do porownania */
  );


/*---------------------------------------------------------
  getdaytime - czas zegarowy
---------------------------------------------------------*/
double getdaytime();

/*---------------------------------------------------------
  getcputime - czas procesora 
---------------------------------------------------------*/
double getcputime();

void merge_sort_openmp(
  double* A,   
  int p,      
  int r,
  int poziom      
);

/*** Definicje procedur ***/

/*---------------------------------------------------------
  main - program glowny do testowania algorytmow sortowania 
----------------------------------------------------------*/
int main(int argc, char **argv)
{

/* zmienne lokalne */
  double *A;     /* tablica do posortowania */
  int n;         /* rozmiar tablicy */
  int i, info;   /* zmienne pomocnicze */
  double czas;   /* zmienna do pomiaru czasu wykonania sortowania */

/*++++++++++++++++ executable statements ++++++++++++++++*/

  printf("podaj rozmiar tablicy: \n");
  scanf("%d",&n);
  A = (double *)malloc((n+1)*sizeof(double));

  //generuj(A,n);

  //czas = getdaytime();

  //sortowanie_przez_wstawianie(A,1,n);

  //printf("czas sortowania przez wstawianie: %lf\n",getdaytime()-czas);

  //info=sprawdz(A,n);

  //if(info>=0) printf("wynik OK\n");
  //else printf("wynik bledny !!!\n");

  generuj(A,n);

  //for(i=1;i<=n;i++){
  //  printf("%10.5lf",A[i]);
  //}

  czas = getdaytime();

  //qsort( &A[1], n, sizeof(double), porownaj);
  qsort((void *) &A[1], (size_t) n, (size_t) sizeof(double), 
        (int (*)(const void*,const void*))porownaj);

  printf("czas sortowania systemowego: %lf\n",getdaytime()-czas);

  info=sprawdz(A,n);

  if(info>=0) printf("wynik OK\n");
  else printf("wynik bledny !!!\n");

  generuj(A,n);

  czas = getdaytime();

  sortowanie_przez_scalanie(A,1,n);

  printf("czas sortowania przez scalanie: %lf\n",getdaytime()-czas);

  info=sprawdz(A,n);

  if(info>=0) printf("wynik OK\n");
  else printf("wynik bledny !!!\n");

  generuj(A,n);

  czas = getdaytime();

  sortowanie_szybkie(A,1,n);

  printf("czas sortowania szybkiego: %lf\n",getdaytime()-czas);

  info=sprawdz(A,n);

  if(info>=0) printf("wynik OK\n");
  else printf("wynik bledny !!!\n");

  generuj(A,n);

  czas = getdaytime();

#pragma omp parallel
  {
#pragma omp single
    { 
      merge_sort_openmp(A,1,n,0);
    }
  }

  printf("czas rownoleglego sortowania przez scalanie: %lf\n",
	 getdaytime()-czas);

  info=sprawdz(A,n);

  if(info>=0) printf("wynik OK\n");
  else printf("wynik bledny !!!\n");

  generuj(A,n);

  czas = getdaytime();

#pragma omp parallel
  {
#pragma omp single
    { 
      merge_sort_openmp_2(A,1,n,0);
    }
  }

  printf("czas rownoleglego sortowania przez scalanie (final): %lf\n",
	 getdaytime()-czas);

  info=sprawdz(A,n);

  if(info>=0) printf("wynik OK\n");
  else printf("wynik bledny !!!\n");

  generuj(A,n);

  czas = getdaytime();

  merge_sort_openmp_4(A,1,n);

  printf("czas rownoleglego sortowania przez scalanie (nested): %lf\n",
  	 getdaytime()-czas);

  info=sprawdz(A,n);

  if(info>=0) printf("wynik OK\n");
  else printf("wynik bledny !!!\n");

  free(A);

  return(0);
}


/*---------------------------------------------------------
  generuj    - procedura generowania ciagu liczbowego o losowych wartosciach
----------------------------------------------------------*/
void generuj(
  double* A,    /* wskaznik do generowanej tablicy */
  int n         /* rozmiar tablicy */
  )
{

  int i;
  double suma;

/*++++++++++++++++ executable statements ++++++++++++++++*/

  suma=0.0;
  for(i=1;i<=n;i++){

    A[i]=drand48();
    suma += A[i];
    //printf("%10.5lf",A[i]);

  }

  //printf("srednia %lf\n", suma/n);

  return;
}


/*---------------------------------------------------------
  sprawdz    - procedura sprawdzania czy posortowany ciag jest rosnacy
----------------------------------------------------------*/
int sprawdz(    /* procedura zwraca:  0 w przypadku sukcesu */
	        /*                   -1 w przypadku bledu   */
  double* A,    /* tablica do sprawdzenia */
  int n         /* rozmiar tablicy */
  )
{

  int i;

/*++++++++++++++++ executable statements ++++++++++++++++*/

  for(i=2;i<=n;i++){
    if(A[i]<A[i-1]) return(-1);
  }

  return(0);
}

/*---------------------------------------------------------
  porownaj - porownanie dwoch elementow tablicy
----------------------------------------------------------*/
int porownaj( /* procedura zwraca: -1 - x<y */
	      /*                    0 - x=y */
	      /*                    1 - x>y */
  double* x,  /* element do porownania */
  double* y   /* element do porownania */
  )
{

  if(*x<*y) return(-1);
  else if(*x>*y) return(1);
  else return(0);

}



/* static variables for time measurements */
static struct rusage rp,rk;
static struct timeval tp,tk;
static struct timezone tzp;

/*---------------------------------------------------------
getdaytime - to return number of wall clock seconds from 
	time measurement initialization
---------------------------------------------------------*/
double getdaytime()
{ 
  
  double daytime;
  
  gettimeofday(&tk, &tzp);
  
  daytime=(tk.tv_usec-tp.tv_usec)/1e6+tk.tv_sec-tp.tv_sec;
  
  return(daytime);
}

/*---------------------------------------------------------
getcputime - to return number of cpu seconds from 
	time measurement initialization
---------------------------------------------------------*/
double getcputime()
{ 
  
  double cputime;
  
  getrusage(RUSAGE_SELF, &rk);
  
  cputime = (rk.ru_utime.tv_usec-rp.ru_utime.tv_usec)/1e6;
  cputime += rk.ru_utime.tv_sec-rp.ru_utime.tv_sec;
  
  return(cputime);
}
