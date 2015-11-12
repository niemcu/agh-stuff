#ifndef _kolejka_
#define _kolejka_

/*** Definicje typow zmiennych ***/
typedef struct{
  int* A;
  int indeksmax;
  int glowa;
  int ogon;
} kolejka;

/*** Deklaracje procedur interfejsu stosu ***/

/*---------------------------------------------------------
  kolejka_inicjuj - procedura inicjuje kolejke i zwraca uchwyt do kolejki
----------------------------------------------------------*/
kolejka* kolejka_inicjuj();

/*---------------------------------------------------------
  kolejka_skasuj - procedura usuwa kolejke i zwalnia pamiec
----------------------------------------------------------*/
void kolejka_skasuj(
  kolejka* K // uchwyt do kolejki
);

/*---------------------------------------------------------
  kolejka_pelna, kolejka_pusta - informacje o kolejce
----------------------------------------------------------*/
int kolejka_pelna(
  kolejka* K // uchwyt do kolejki
  );
int kolejka_pusta(
  kolejka* K // uchwyt do kolejki
  );


/*---------------------------------------------------------
  kolejka_wstaw, kolejka_pobierz - obsluga kolejki, wstawianie i usuwanie elementow
----------------------------------------------------------*/
void kolejka_wstaw(
  kolejka* K, // uchwyt do kolejki
  int v       /* wartosc wstawiana do kolejki */
  );
int kolejka_pobierz(
  kolejka* K // uchwyt do kolejki
  );

/*---------------------------------------------------------
  kolejka_drukuj - wypisz zawartosc kolejki 
----------------------------------------------------------*/
void kolejka_drukuj( 
  kolejka* K // uchwyt do kolejki
  );

#endif
