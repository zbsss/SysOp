#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>
#define HAIRCUT_TIME 10
#define MAX_WAIT 6
#define NEW_CLIENT 4


int K, N;

pthread_mutex_t barber_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barber_cv =  PTHREAD_COND_INITIALIZER;
int barber_sleeping = 0; //1 true, 0 false

pthread_mutex_t waitingroom_mutex = PTHREAD_MUTEX_INITIALIZER;
int* waitingroom;


int free_seats(){
    int free = 0;
    pthread_mutex_lock(&waitingroom_mutex);

    for(int i=0; i<K; i++)
        if(waitingroom[i] == -1)
            free++;
    return free;
    pthread_mutex_unlock(&waitingroom_mutex);
}

void client(int* id){
    pthread_mutex_lock(&waitingroom_mutex);

    int free = 0;

    for(int i=0; i<K; i++)
        if(waitingroom[i] == -1)
            free++;

    if(free == 0){
        printf("Zajete; %d\n", *id);

        for(int j=0; j<K; j++)
            printf("%d", waitingroom[j]);
        printf("\n");

        pthread_mutex_unlock(&waitingroom_mutex);

        sleep(rand() % MAX_WAIT + 1);
        client(id);
        return;
    }
    

    for(int i=0; i<K; i++){
        if(waitingroom[i] == -1){
            waitingroom[i] = *id;
            break;
        }
    }

    printf("Poczekalnia, wolne miejsca: %d; %d\n",free, *id);
    
    //Czy trzeba blokowac mutex na fladze?
    pthread_mutex_lock(&barber_mutex);
    if(barber_sleeping){
        printf("Budze golibrode; %d\n", *id);

        pthread_cond_signal(&barber_cv);
    }
    pthread_mutex_unlock(&barber_mutex);

    pthread_mutex_unlock(&waitingroom_mutex);
}

void barber(){
    int finished = 0;
    while(finished < N){
        pthread_mutex_lock(&waitingroom_mutex);

        int free = 0;

        for(int i=0; i<K; i++)
            if(waitingroom[i] == -1)
                free++;

        if(free == K){
            printf("Golibroda: ide spac\n");

            pthread_mutex_lock(&barber_mutex);
            barber_sleeping = 1;
            pthread_mutex_unlock(&barber_mutex);

            pthread_cond_wait(&barber_cv, &waitingroom_mutex); //czeka az klient go obudzi
        }

        pthread_mutex_lock(&barber_mutex);
        barber_sleeping = 0;
        pthread_mutex_unlock(&barber_mutex);

        int client;
        for(int i=0; i<K; i++){
            if(waitingroom[i] != -1){
                client = i;
                break;
            }
        }

        printf("Golibroda: czeka %d klientow, gole klienta %d\n", K - free, waitingroom[client]);
        waitingroom[client] = -1;
        finished++;
        pthread_mutex_unlock(&waitingroom_mutex);

        sleep(rand() % HAIRCUT_TIME + 1);
    }
    printf("Golibroda: ogolilem %d, fajrant\n", finished);
}

int main(int argc, char** argv){
    if(argc != 3){
        fprintf(stderr, "Wrong number of arguments\n");
        exit(1);
    }

    K = atoi(argv[1]);
    N = atoi(argv[2]);

    waitingroom = calloc(K, sizeof(int));
    for(int i=0; i<K; i++){
        waitingroom[i] = -1;
    }

    pthread_t barber_thread;
    pthread_create(&barber_thread, NULL, (void* (*)(void*))barber, NULL);

    pthread_t* clients = calloc(N, sizeof(pthread_t));

    for(int i=0; i<N; i++){
        sleep(rand() % NEW_CLIENT + 1);
        int* id = &i;
        pthread_create(&clients[i], NULL, (void* (*)(void*))client, id);
    }

    for(int i=0; i<N; i++)
        pthread_join(clients[i], NULL);
    pthread_join(barber_thread, NULL);

    free(clients);
    free(waitingroom);

    return 0;
}