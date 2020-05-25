#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#define NR_LOOP 10000
sem_t semaforo;
void hola(void);
void mundo(void);


int main(){
	//inicializando los semaforos
	pthread_t h1,h2;
	//param el tercer valor indica el valor de inicializacion
	sem_init(&semaforo,0,0);
	pthread_create(&h1,NULL,(void *)hola,NULL);
	//dormir un seg
	pthread_create(&h2,NULL,(void *)mundo,NULL);
	pthread_join(h1,NULL);
	sleep(1);
	pthread_join(h2,NULL);



	return 0;
}
void hola(void){
	printf("hola");
	sem_post(&semaforo);
	pthread_exit(0);
}

void mundo(void){
	sem_wait(&semaforo);
	printf("mundo");
	pthread_exit(0);
}

