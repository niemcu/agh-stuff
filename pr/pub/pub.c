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

void *visit_pub(void *args) {
    printf("Wchodze do pubu\n");
    pthread_mutex_lock(&mutex);
    printf("Nalewajo mi\n");
    sleep(3);
    printf("Mam nalane\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    struct pub my_pub = { 5, 7 };
    unsigned int i;
    pthread_t tid_vector[my_pub.clients];
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < my_pub.clients; i++) {
        pthread_create(&tid_vector[i], NULL, visit_pub, NULL);
    }
    for (i = 0; i < my_pub.clients; i++) {
        pthread_join(tid_vector[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
