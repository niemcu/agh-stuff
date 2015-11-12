#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t* czytelnia_p){
  
}


int my_read_lock_unlock(czytelnia_t* czytelnia_p){
   
}


int my_write_lock_lock(czytelnia_t* czytelnia_p){
  
}


int my_write_lock_unlock(czytelnia_t* czytelnia_p){
    
}

void inicjuj(czytelnia_t* czytelnia_p){

}

void czytam(czytelnia_t* czytelnia_p){
    usleep(rand()%1000000);
}

void pisze(czytelnia_t* czytelnia_p){
    usleep(rand()%1000000);
}


