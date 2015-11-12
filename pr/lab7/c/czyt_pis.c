#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"

void *funkcja_czytelnika( void *);
void *funkcja_pisarza( void *);


int main(){
  
  int i;
  pthread_t pisarze[5], czytelnicy[10];
  int indeksy[10] = {0,1,2,3,4,5,6,7,8,9}; 
  czytelnia_t czytelnia;
  
  inicjuj(&czytelnia);
    
  for(i=0; i<5; i++){
    pthread_create( &pisarze[i], NULL, funkcja_pisarza, (void *)&czytelnia );
  }
  for(i=0; i<10; i++){
    pthread_create( &czytelnicy[i], NULL, funkcja_czytelnika, (void *)&czytelnia );
  }
  for(i=0; i<5; i++){
    pthread_join( pisarze[i], NULL); 
  }
  for(i=0; i<10; i++){
    pthread_join( czytelnicy[i], NULL ); 
  }
  
}

void *funkcja_czytelnika( void * arg){
  
  czytelnia_t* czytelnia_p = (czytelnia_t *)arg;
  
  for(;;){
    
    usleep(rand()%10000000);
    printf("czytelnik %d - przed zamkiem\n", pthread_self());
    
    my_read_lock_lock(czytelnia_p);
    
    // korzystanie z zasobow czytelni
    printf("czytelnik %d - wchodze\n", pthread_self());
    
    czytam(czytelnia_p);
    
    
    printf("czytelnik %d - wychodze\n", pthread_self());
    
    my_read_lock_unlock(czytelnia_p);
    
    printf("czytelnik %d - po zamku\n", pthread_self());
    
  }
  
}

void *funkcja_pisarza( void * arg){
  
  czytelnia_t* czytelnia_p = (czytelnia_t *)arg;
  
  for(;;){
    
    usleep(rand()%11000000);
    printf("pisarz %d - przed zamkiem\n", pthread_self());
    
    my_write_lock_lock(czytelnia_p);
    
    // korzystanie z zasobow czytelni
    printf("pisarz %d - wchodze\n", pthread_self());
    
    pisze(czytelnia_p);
    
    printf("pisarz %d - wychodze\n", pthread_self());
    
    my_write_lock_unlock(czytelnia_p);
    
    printf("pisarz %d - po zamku\n", pthread_self());
  }
  
}


