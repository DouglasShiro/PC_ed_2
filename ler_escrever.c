#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#define leitores 20
#define escritores 10
#define TRUE 1

pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int rc = 0;

void * ler(void * arg){
    int i = *((int*) arg);

    while(TRUE){
        pthread_mutex_lock(&turno);
        pthread_mutex_lock(&mutex);
        rc = rc + 1;
        if(rc == 1){
            pthread_mutex_lock(&bd);
        }
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&turno);

        printf("leitor %d lendo dados bd %d\n", i, rc);
        sleep(2);

        pthread_mutex_lock(&mutex);
        rc = rc - 1;
        if(rc == 0){
            pthread_mutex_unlock(&bd);
        }
        pthread_mutex_unlock(&mutex);

        printf("dados lidos sendo usados\n");
        sleep(2);
    }

    pthread_exit(0);
}

void * escrever(void * arg){
    int i = *((int *) arg);
    while(TRUE){
        printf("escritor %d precisa escrever no bd\n", i);
        sleep(2);
        pthread_mutex_lock(&turno);
        pthread_mutex_lock(&bd);
        printf("escritor %d escrevendo no bd\n", i);
        sleep(2);
        pthread_mutex_unlock(&bd);
        pthread_mutex_unlock(&turno);
    }
    pthread_exit(0);
}

int main() {
   pthread_t l[leitores];
   pthread_t e[escritores];

   int i;
   int *id;
   for (i = 0; i < leitores ; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&l[i], NULL, ler, (void *) (id));
    }

   for (i = 0; i < escritores ; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&e[i], NULL, escrever, (void *) (id));
    }

    pthread_join(l[0],NULL);

    return 0;
}
