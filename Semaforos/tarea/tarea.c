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

typedef struct data{
	char *cadena;
	int id;
}parametro;

sem_t semaforo;
sem_t barrera;


int main(void){

	int i, id[NHILOS],status, *salida;
	char *nombres[]= {"David", "Luis", "Gerardo", "Juan", "Rodrigo", "Pedro", "Ana", "Yisel", "Karla", "Sofia"};
	pthread_t hilos[NHILOS];
	sem_init(&semaforo,0,1);
        sem_init(&barrera,0,0);
	parametro p[NHILOS];

	for(i=0; i<NHILOS; i++){
		p[i].id = i;
		p[i].cadena = nombres[i];
	}
	for(i=0; i<NHILOS; i++){
		id[i]=i;
		status=pthread_create(&hilos[i], NULL, (void *) rutinahilo,&p[i]);
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
return 0;
}

void rutinahilo(void *args){
	parametro *par = (parametro *) args;
	//printf("Ya llegué al metro  soy el hilo : %s con id: %i\n", *(int *)args);
	printf("Ya llegué al metro  soy el hilo : %s con id: %i\n", par->cadena, par->id);
	sleep(1);
	sincronizarProceso();
	printf("\nYa vamos para biblioteca\n");
	pthread_exit(0);
}
void sincronizarProceso(){
	int i;
	sem_wait(&semaforo);
	counter+=1;
	if(counter<NP){
		sem_post(&semaforo);
		sem_wait(&barrera);

	}else{
		for(i=0; i<NP-1; i++ )
			sem_post(&barrera);
		counter=0;
		sem_post(&semaforo);
	}
}
