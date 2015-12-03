#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"

void *reader_thread(void*);
void *writer_thread(void*);

int main(void) {
    int readers_num = 10;
    int writers_num = 5;
    int i;
    pthread_t writers[writers_num], readers[readers_num];
    reading_room_t czytelnia;

    rr_init(&czytelnia);

    for(i = 0; i < writers_num; i++) {
        pthread_create(&writers[i], NULL, writer_thread, (void *)&czytelnia );
    }

    for(i = 0; i < readers_num; i++) {
        pthread_create(&readers[i], NULL, reader_thread, (void *)&czytelnia );
    }

    for(i = 0; i < writers_num; i++) {
        pthread_join(writers[i], NULL); 
    }

    for(i = 0; i<readers_num; i++) {
        pthread_join(readers[i], NULL); 
    }
}

void *reader_thread(void *arg) {
    reading_room_t* rr = (reading_room_t*)arg;

    for(;;) {
        usleep(rand()%10000000);

        printf("l.pis(%d) l.czyt(%d) | czytelnik %d - przed zamkiem\n", 
                rr->writers_inside,
                rr->readers_inside,
                (int)pthread_self());

        my_read_lock_lock(rr);

        printf("l.pis(%d) l.czyt(%d) | czytelnik %d - wchodze\n", 
                rr->writers_inside,
                rr->readers_inside,
                (int)pthread_self());   

        rr_read(rr);

        printf("l.pis(%d) l.czyt(%d) | czytelnik %d - wychodze\n", 
                rr->writers_inside,
                rr->readers_inside,
                (int)pthread_self());  

        my_read_lock_unlock(rr);

        printf("l.pis(%d) l.czyt(%d) | czytelnik %d - po zamku\n", 
                rr->writers_inside,
                rr->readers_inside,
                (int)pthread_self());  
    }
}

void *writer_thread(void *arg) {
    reading_room_t* rr = (reading_room_t*)arg;

    for(;;) {

        usleep(rand()%11000000);

        printf("l.pis(%d) l.czyt(%d) | pisarz %d - przed zamkiem\n", 
                rr->writers_inside,
                rr->readers_inside,
                (int)pthread_self());

        my_write_lock_lock(rr);

        printf("l.pis(%d) l.czyt(%d) | pisarz %d - wchodze\n", 
                rr->writers_inside,
                rr->readers_inside,
                (int)pthread_self());   

        rr_write(rr);

        printf("l.pis(%d) l.czyt(%d) | pisarz %d - wychodze\n", 
                rr->writers_inside,
                rr->readers_inside,
                (int)pthread_self());  

        my_write_lock_unlock(rr);

        printf("l.pis(%d) l.czyt(%d) | pisarz %d - po zamku\n", 
                rr->writers_inside,
                rr->readers_inside,
                (int)pthread_self());  
    }
}

