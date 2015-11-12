#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct pub {
    unsigned int mugs;
    unsigned int clients;
};

struct pub my_pub;

pthread_mutex_t mug_mutex;
pthread_mutex_t bar_mutex;
//watek albo czeka na napelnienie, albo pije



void *visit_pub(unsigned int *arg) {
    unsigned int th_num = *arg;
    int success = 0;
    int k;
    printf("Moje ID %d; Wchodze do pubu\n", th_num);
    for (k = 1; k < 3; k++) {
    	printf("Moje ID %d; Chce pobrac kufel %d \n", th_num, k);
		for (;;) {
			pthread_mutex_lock(&mug_mutex);
			if (my_pub.mugs > 0) {
				success = 1;
				my_pub.mugs--;
			}
			pthread_mutex_unlock(&mug_mutex);
			if (success == 1) {
				success = 0;
				printf("Moje ID %d; Mam kufel, czekam na kran\n", th_num);
				pthread_mutex_lock(&bar_mutex);
				printf("Moje ID %d; Mam kran, nalewam piwo\n", th_num);
					sleep(3);
				pthread_mutex_unlock(&bar_mutex);
				// picie piwa
				printf("Moje ID %d; Pije piwo\n", th_num);
				sleep(10);
				printf("Moje ID %d; Wypilem. oddaje kufel\n", th_num);
				pthread_mutex_lock(&mug_mutex);
				my_pub.mugs++;
				pthread_mutex_unlock(&mug_mutex);
				break;
			} else {

			}	
		}
	}
	printf("Moje ID %d; Upilem sie. Wychodze z baru.\n", th_num);
    return NULL;
}

int main() {
	int * tab_clients;
    unsigned int i;
    printf("\nLiczba klientow: "); scanf("%d", &my_pub.clients);
    printf("\nLiczba kufli: "); scanf("%d", &my_pub.mugs);
    pthread_t tid_vector[my_pub.clients];
    pthread_mutex_init(&mug_mutex, NULL);
    tab_clients = (int *) malloc(my_pub.clients*sizeof(int));
    for (i = 0; i < my_pub.clients; i++) {
    	tab_clients[i] = i;
    }
    for (i = 0; i < my_pub.clients; i++) {
        pthread_create(&tid_vector[i], NULL, visit_pub, &tab_clients[i]);
    }
    for (i = 0; i < my_pub.clients; i++) {
        pthread_join(tid_vector[i], NULL);
    }
    pthread_mutex_destroy(&mug_mutex);
    return 0;
}
