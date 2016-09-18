#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"

#define TRUE 1

#define NE 10 //numero de escritores
#define NL 20 //numero de leitores


pthread_mutex_t turno;
pthread_mutex_t mutex;                    /* controla o acesso a 'rc' */
pthread_mutex_t db;                       /* controla o acesso a base de dados */
int rc = 0;                     /* nÃºmero de processos lendo ou querendo ler */

void* reader(void *arg);
void* writer(void *arg);
void read_data_base();
void use_data_read();
void think_up_data();
void write_data_base();

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&db, NULL);
	pthread_mutex_init(&turno, NULL);

	pthread_t r[NL], w[NE];

	int i;
        int *id;
        /* criando leitores */
    	for (i = 0; i < NL ; i++) {
	   id = (int *) malloc(sizeof(int));
           *id = i;
		 pthread_create(&r[i], NULL, reader, (void *) (id));
	}

	 /* criando escritores */
	for (i = 0; i< NE; i++) {
	   id = (int *) malloc(sizeof(int));
           *id = i;
		 pthread_create(&w[i], NULL, writer, (void *) (id));
	}

	pthread_join(r[0],NULL);
	return 0;
}

void* reader(void *arg) {
	int i = *((int *) arg);
	while(TRUE) {               /* repere para sempre */
		 pthread_mutex_lock(&turno);
		 pthread_mutex_lock(&mutex);       /* obtÃ©m acesso exclusivo Ã  'rc' */
		 rc = rc + 1;            /* um leitor a mais agora */

		 if (rc == 1) {          /* se este for o primeiro leitor... */
		     pthread_mutex_lock(&db);
		 }
		 pthread_mutex_unlock(&mutex);       /* libera o acesso exclusivo a 'rc' */
	         pthread_mutex_unlock(&turno);
		 read_data_base(i);       /* acesso aos dados */
		 pthread_mutex_lock(&mutex);       /* obtÃ©m acesso exclusivo a 'rc' */
		 rc = rc - 1;            /* um leitor a menos agora */
		 if (rc == 0) {          /* se este for o Ãºltimo leitor */
		     pthread_mutex_unlock(&db);
		 }
		 pthread_mutex_unlock(&mutex);       /* libera o acesso exclusivo a 'rc' */
		 use_data_read(i);        /* regiÃ£o nÃ£o crÃ­tica */
	}
        pthread_exit(0);
}

void* writer(void *arg) {
	int i = *((int *) arg);

	while(TRUE) {               /* repete para sempre */
		 think_up_data(i);        /* regiÃ£o nÃ£o crÃ­tica */
		 pthread_mutex_lock(&turno);
		 pthread_mutex_lock(&db);          /* obtÃ©m acesso exclusivo */
		 write_data_base(i);      /* atualiza os dados */
		 pthread_mutex_unlock(&db);          /* libera o acesso exclusivo */
		 pthread_mutex_unlock(&turno);
	}
        pthread_exit(0);
}

void read_data_base(int i) {
	printf("Leitor %d estÃ¡ lendo os dados! NÃºmero de leitores = %d \n", i,rc);
	sleep( rand() % 5);
}

void use_data_read(int i) {
	printf("Leitor %d estÃ¡ usando os dados lidos!\n", i);
	sleep(rand() % 5);
}

void think_up_data(int i) {
	printf("Escritor %d estÃ¡ pensando no que escrever!\n", i);
	sleep(rand() % 5);
}

void write_data_base(int i) {
	printf("Escritor %d estÃ¡ escrevendo os dados!\n", i);
	sleep( rand() % 5);
}
