#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>

#define HILOS 10

int procesos_ligeros=0;
sem_t s;
sem_t muralla;

void* rutina_hilo(void *arg){
	int i;
	sem_wait(&s);   //down(s)
	
     procesos_ligeros+=1;
	
	printf("\nYa llege al metro, pero aun no llegan todos\n");
	
	if(procesos_ligeros<HILOS){
		
		sem_post(&s);   //up(s)	
		sem_wait(&muralla);   //down(muralla)
		
	}else{
		
		for(i=0;i<HILOS;i++)
		
			sem_post(&muralla);  //up(murralla)
			
		procesos_ligeros=0;
		sem_post(&s);  //up(s)
		
	}
	pthread_exit(0);
	

}

void main(){
	
	int i, valor, id[HILOS];
	
	pthread_t hilos[HILOS];
	
	sem_init(&s,0,1);
	
	sem_init(&muralla,0,0);
	
	for(i=0;i<HILOS;i++){
		
		id[i]=i;
		
		valor= pthread_create(&hilos[i],NULL,rutina_hilo,NULL);
		
		if(valor){
			
			printf("\nNO SE HA PODIDO CREAR PTHREAD_CREATE\n");
			exit(-1);
			
		}
	}


	for(i=0;i<HILOS;i++){
		pthread_join(hilos[i],NULL);
	}

	printf("\nYa estamos listos, vamos en camino, todos en fila XD\n");
	return 0;
}
