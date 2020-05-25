#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
//#include <dispatch/dispatch.h>

#define NFILOSOFOS 5
#define PENSANDO 0
#define COMIENDO 1
#define HAMBRIENTO  2

sem_t  sfilosofos[NFILOSOFOS];
sem_t  sem;//cambiarlo por un mutex

int estado_filosofo[NFILOSOFOS];

void *vida_filosofo(void *);
void pensar();
void comer();
void tomar_tenedores(int);
void dejar_tenedores(int);

void tenedores_disponibles(int);
int filDerecho(int);
int filIzquierdo(int);


int main(){
	pthread_t filosofos[NFILOSOFOS];
	int idFil[NFILOSOFOS],i;
	for(i=0;i<NFILOSOFOS;i++){
		sem_init(&sfilosofos[i],0,0);
		sem_init(&sem,0,1);
	}
	for(i=0; i<NFILOSOFOS; i++){
	idFil[i]=i;
	pthread_create(&filosofos[i],NULL,vida_filosofo,&idFil[i]);
	}
	for(i=0; i<NFILOSOFOS; i++){
	idFil[i]=i;
	pthread_join(filosofos[i],NULL);
	}



return 0;
}
void *vida_filosofo(void *arg){

	int fil= *(int *)arg;
	while(1){
		pensar(fil);
		tomar_tenedores(fil);
		comer(fil);
		dejar_tenedores(fil);
	}

}

void pensar(int fil){


	printf("\n Soy el filosofo %i. Voy a pensar\n",fil);
	sleep(2);
}

void comer(int fil){


	printf("\n Soy el filosofo %i. Voy a comer\n",fil);
	sleep(2);
}
void tenedores_disponibles(int fil){
	if(estado_filosofo[fil] == HAMBRIENTO && estado_filosofo[filDerecho(fil)] != COMIENDO && estado_filosofo[filIzquierdo(fil)] != COMIENDO){
		estado_filosofo[fil]=COMIENDO;
		sem_post(&sfilosofos[fil]); //UP
		printf("\n Soy el filosofo %i. Estoy tomando mis tenedores\n",fil);


	}else{
		printf("\n Soy el filosofo %i. Voy a esperar  los tenedores\n",fil);


	}



}
void tomar_tenedores(int fil){

	sem_wait(&sem);
	estado_filosofo[fil] = HAMBRIENTO;
	printf("\n Soy el filosofo %i. Estoy hambriento, intentaré tomar un tenedor\n",fil);
	tenedores_disponibles(fil);
	sem_post(&sem);  //DOWN
	sem_wait(&sfilosofos[fil]);  //DOWN



}
int filDerecho(int fil){

	return (fil+1)%NFILOSOFOS;

}
int filIzquierdo(int fil){

	return (fil-1)%NFILOSOFOS;

}
void dejar_tenedores(int fil){
	//pthread_mutex_lock(&sem);
	sem_wait(&sem);
	estado_filosofo[fil]=PENSANDO;
	printf("Soy el filosofo %i, Estoy satisfecho, dejare mis tenedores\n",fil);
	tenedores_disponibles(filDerecho(fil));
	tenedores_disponibles(filIzquierdo(fil));
	sem_post(&sem);
	//pthread_mutex_unlock(&sem);
}
