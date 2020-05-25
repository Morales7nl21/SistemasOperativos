#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NHILOS 10

void funcion_hilos(void){
	printf("\nHilo %lu \n", pthread_self());
	pthread_exit(0);
}
void main(){
	int i;
	pthread_attr_t atributos;
	pthread_t hilos[NHILOS];

	pthread_attr_init(&atributos);
	pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);
	for(i=0; i<NHILOS; i++){
		pthread_create(&hilos[i], &atributos, (void *) funcion_hilos,NULL);

	}

	sleep(4);

}
