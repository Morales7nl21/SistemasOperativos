#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#define NHILOS 10
#define NP 10
int counter = 0;
void rutinahilo(void *);
void sincronizarProceso();
sem_t semaforo;
sem_t barrera;


void main(){

	int i, id[NHILOS],status, *salida;


	pthread_t hilos[NHILOS];
	sem_init(&semaforo,0,1);
        sem_init(&barrera,0,0);



	for(i=0; i<NHILOS; i++){
		id[i]=i;
		status=pthread_create(&hilos[i], NULL, (void *) rutinahilo,&id[i]);
		sleep(1);
		if(status){
			printf("Error en el hilo %i \n", status);
			exit(-1);
		}

	}
	for(i = 0; i< NHILOS; i++){
		pthread_join(hilos[i],(void *) &salida);
		printf("\n Hilo %i termino con código %i \n", i, *salida);

	}


	sleep(1);
	pthread_exit(0);


}

void rutinahilo(void *args){
	printf("Ya llegué al metro  soy el hilo con id: %i\n", *(int *)args);
	sincronizarProceso();
	printf("Ya vamos para biblioteca");
}
void sincronizarProceso(){
	int i;
	sem_wait(&semaforo);

	counter = counter+1;
		printf("\nContador = % i\n", counter);

	if(counter<NP){

		sem_post(&semaforo);
		sem_wait(&barrera);

	}else if (counter == NP){
		for(i=0; i< NP; i++ ){
			sem_post(&barrera);	
		}
		counter=0;

		sem_post(&semaforo);
	}



}
