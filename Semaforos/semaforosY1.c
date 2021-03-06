#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#define NR_LOOP 100

static void *routinehilo1(void *);
static void *routinehilo2(void *);
static int counter = 0;


sem_t sem1;

int main(void){

	pthread_t hilo1;
	pthread_t hilo2;
	sem_init(&sem1,0,1); //Argumento 1 referencia al semaforo, el segundo si es compartido entre procesos
				//El tercero valor de iniciacion, en este caso 1 sólo un hilo aaccedera al elemento

	pthread_create(&hilo1, NULL,* routinehilo1,NULL);
	pthread_create(&hilo2, NULL,* routinehilo2,NULL);


	pthread_join(hilo1, NULL); // El segundo es la variable que retorna
	sleep(1);
	pthread_join(hilo2, NULL); // El segundo es la variable que retorna

	printf("Valor de contador = %i ", counter);


	return 0;
}
static void *routinehilo1(void *args){
	int i=0;
		sem_wait(&sem1);
	for(i = 0; i < NR_LOOP; i++){
		counter +=1;
		//sem_post(&sem1);
	}
		sem_post(&sem1);


}
static void *routinehilo2(void *args){
	int i=0;
		sem_wait(&sem1);
	for(i = 0; i<NR_LOOP; i--){
		counter -=1;
	}
		sem_post(&sem1);

}

