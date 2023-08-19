/*
    Dining Savages Problem - projekat AROS 2023
    Aleksa Rodic 2021/0467
    v1.0
*/

#include <stdio.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <semaphore.h> 

#define POT_CAPACITY_MAX 5
#define TRIBE_POPULATION 10

int servings = POT_CAPACITY_MAX;

pthread_mutex_t mutex;

sem_t fullPot, emptyPot;

void putServingsInPot(int M){
    sleep(3);
    printf("Pot refilled!\n");
    servings = M;
}

void getServingFromPot(){
    servings--;
}

void *savage(){
    while(1){ 
        pthread_mutex_lock(&mutex);
        printf("Savage checks pot...\n");
        if(servings == 0){
            printf("The pot is empty!\n");
            sem_post(&emptyPot);
            sem_wait(&fullPot);
        }
        sleep(2);
        printf("There is food in the pot!\n");
        sleep(1);
        printf("Savage enjoys a meal...\n\n");
        getServingFromPot();
        sleep(3);
        pthread_mutex_unlock(&mutex);
    }
}

void *cook(){
    while(1){
        sem_wait(&emptyPot);
        putServingsInPot(POT_CAPACITY_MAX);
        sem_post(&fullPot);
    }
}

int main(void){
    pthread_mutex_init(&mutex, NULL);
    pthread_t t[TRIBE_POPULATION];
    pthread_t t_cook;
    pthread_mutex_init(&mutex,NULL);
    sem_init(&emptyPot,0,0);
    sem_init(&fullPot,0,0);
    
    pthread_create(&t_cook, NULL, cook, NULL);
    for(int i = 0; i < TRIBE_POPULATION; i++){
        pthread_create(&t[i], NULL, savage, NULL);
    }
    pthread_join(t_cook,NULL);
    for(int i = 0; i < TRIBE_POPULATION; i++){
        pthread_join(t[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
// test