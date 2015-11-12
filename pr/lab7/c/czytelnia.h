#ifndef _czytelnia_
#define _czytelnia_

/*** Definicje typow zmiennych ***/
typedef struct {
  // <- zasoby czytelni
  int readers_waiting,
      readers_inside,
      writers_waiting,
      writers_inside;
  pthread_cond_t readers_cond, writers_cond;
  pthread_mutex_t mutex;
} reading_room_t;

/*** Deklaracje procedur interfejsu ***/
void rr_init(reading_room_t* rr);
void rr_read(reading_room_t* rr);
void rr_write(reading_room_t* rr);

int my_read_lock_lock(reading_room_t* rr);
int my_read_lock_unlock(reading_room_t* rr);
int my_write_lock_lock(reading_room_t* rr);
int my_write_lock_unlock(reading_room_t* rr);

#endif
