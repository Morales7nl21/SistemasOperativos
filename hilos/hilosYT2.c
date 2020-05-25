#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
void *procesoSeparado(void *);
int main(void){
	//Se declaran 
	pthread_t proceso1;
	pthread_t proceso2;
	int iret1, iret2;
	//Se crean los  hilos
	/*
		@Param &proceso1 Es la direccion de memoria del hilo declarado a usar
		@param NULL Necesario para crearlo "no sé bien porque null xd"
		@Param &procesoSeparado Es la direccion de la función a ejecutar
		@Param "hola" Parametro a mandar

	*/
	iret1=pthread_create(&proceso1, NULL, &procesoSeparado, "hola");
	iret2=pthread_create(&proceso2, NULL, &procesoSeparado, "Adios");
	/*
	Lo anterior genera los datos necesarios a las instancias que son de tipo
	int sin signo, pero son identificadores de procesos en la memoria RAM
	Asi le decimos que almacene memoria para los procesos
	*/

	//EJECUCION

	pthread_join(proceso1,NULL);
	pthread_join(proceso2,NULL);

	printf("Thread 1 returns: %d\n", iret1);
	printf("Thread 2 returns: %d\n", iret2);
return 0;
}
void *procesoSeparado(void *data){
        char *texto = (char *) data;
	//Variable de timespec el primero representa los segunods y el segundo los nano segundos.
	struct timespec tiempo = {1,0};
        while(1){
                printf("%s\n", texto);
		//DORMIR EL HILO
		/*
			@Param de la direccion del struct de la variable tiempo
		*/
//		nanosleep(2);
        }

}


