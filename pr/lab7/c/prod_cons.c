#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include<kolejka.h>

void *produkuj( void *);
void *konsumuj( void *);

pthread_mutex_t  muteks= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t nie_pelna, nie_pusta; 	

int main(){
  
  pthread_t producent, konsument; 
  kolejka *fifo;
  fifo = kolejka_inicjuj(10); 
  pthread_create( &producent, NULL, produkuj, (void *)fifo );
  pthread_create( &konsument, NULL, konsumuj, (void *)fifo );
  pthread_join( producent, NULL); 
  pthread_join( konsument, NULL ); 
}

void *produkuj( void *q){
  kolejka *fifo; int i;
  fifo = (kolejka *)q; 	// kolejka zawiera bufor do zapisu i odczytu
  for(;;){
    int l = rand()%100;

    pthread_mutex_lock (&muteks);
    while( kolejka_pelna(fifo) ) pthread_cond_wait(&nie_pelna, &muteks );
    kolejka_wstaw(fifo, l);
    printf("Producent: wstawiam %d\n",l);
    kolejka_drukuj(fifo);
    pthread_mutex_unlock( &muteks );
    pthread_cond_signal( &nie_pusta );
    usleep(rand()%1000000);
  }	
}



void *konsumuj( void *q){
  kolejka *fifo; int i, d;
  fifo = (kolejka *)q;	 // kolejka zawiera bufor do zapisu i odczytu
  for(;;){
    pthread_mutex_lock (&muteks);
    while( kolejka_pusta(fifo) ) pthread_cond_wait(&nie_pusta, &muteks);
    d = kolejka_pobierz(fifo);
    printf("Konsument: pobralem %d\n",d); 
    kolejka_drukuj(fifo);
    pthread_mutex_unlock( &muteks );
    pthread_cond_signal( &nie_pelna );
    usleep(rand()%1000000);
  }	
}

