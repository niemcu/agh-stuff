#include<stdlib.h>
//#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"

/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(reading_room_t* rr) {
    if (&rr->writers_inside > 0 || &rr->writers_waiting > 0) {
        // poczekajmy
        pthread_cond_wait(&rr->readers_cond, &rr->mutex);
    }

    rr->readers_inside++;
    pthread_cond_signal(&rr->readers_cond);
}

int my_read_lock_unlock(reading_room_t* rr) {
    rr->readers_inside--;
    if (&rr->readers_inside == 0) {
        pthread_cond_signal(&rr->writers_cond);
    } 
}

int my_write_lock_lock(reading_room_t* rr) {
    //pthread_mutex_lock(&rr->mutex);
    while (rr->readers_inside + rr->writers_inside > 0) {
        pthread_cond_wait(&rr->writers_cond, &rr->mutex);
    }
    rr->writers_inside++;
}

int my_write_lock_unlock(reading_room_t* rr) {
    rr->writers_inside--;
    //pthread_mutex_unlock(&rr->mutex);
    if (&rr->readers_waiting) {
        pthread_cond_signal(&rr->readers_cond);
    } else {
        pthread_cond_signal(&rr->writers_cond);
    }
}

void rr_init(reading_room_t* rr) {
    rr->readers_inside = 0;
    rr->readers_waiting = 0;
    rr->writers_inside = 0;
    rr->writers_waiting = 0;
    pthread_mutex_init(&rr->mutex, NULL);
    pthread_cond_init(&rr->readers_cond, NULL);
    pthread_cond_init(&rr->writers_cond, NULL);
}

void rr_read(reading_room_t* rr) {
    usleep(rand()%1000000);
}

void rr_write(reading_room_t* rr) {
    usleep(rand()%1000000);
}

