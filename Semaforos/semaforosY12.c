#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

static void *routinehilo1(void *);
static void *routinehilo2(void *);
static void *routinehilo3(void *);



sem_t sem1, sem2;

int main(void){

	pthread_t hilo1;
	pthread_t hilo2;
	pthread_t hilo3;

	sem_init(&sem1,0,0);
	sem_init(&sem2,0,0);

	pthread_create(&hilo3, NULL,* routinehilo3,NULL);
	pthread_create(&hilo1, NULL,* routinehilo1,NULL);
	pthread_create(&hilo2, NULL,* routinehilo2,NULL);
	pthread_join(hilo3, NULL);
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);

	return 0;
}
static void *routinehilo1(void *args){
	sem_wait(&sem1);
	printf(" Sistemas");;
	sem_post(&sem2);
}
static void *routinehilo2(void *args){
	printf("Examen de");
	sem_post(&sem1);
}
static void *routinehilo3(void *args){
	sem_wait(&sem2);
	printf(" Operativos ");
	sem_post(&sem2);//opcional
}

