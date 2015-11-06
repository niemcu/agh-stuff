#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct pub {
    unsigned int mugs;
    unsigned int clients;
};

pthread_mutex_t mutex;

//watek albo czeka na napelnienie, albo pije

void *visit_pub(unsigned int *arg) {
    unsigned int th_num = *arg;
    printf("Moje ID %d; Wchodze do pubu\n", th_num);
    pthread_mutex_lock(&mutex);
    printf("Moje ID %d; Nalewajo mi\n", th_num);
    sleep(3);
    pthread_mutex_unlock(&mutex);
    printf("Moje ID %d; Mam nalane. Pije\n", th_num);
    sleep(8);
    printf("Moje ID %d; Wypilem", th_num);
    return NULL;
}

int main() {
    struct pub my_pub = { 5, 7 };
    unsigned int i;
    pthread_t tid_vector[my_pub.clients];
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < my_pub.clients; i++) {
        pthread_create(&tid_vector[i], NULL, visit_pub, &i);
    }
    for (i = 0; i < my_pub.clients; i++) {
        pthread_join(tid_vector[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
