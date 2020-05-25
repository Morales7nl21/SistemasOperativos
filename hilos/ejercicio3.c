#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct
{
    int id;
    int to_add;
    int times;
} contenedorHilo;

int get_random_number(int number);

int number=0;

int main(int argc,char **argv){

	int incremento, decremento;
	if(argc<2){
		printf("No ingresaste valores para aumentar o disminuir en cada hilo\n");
		printf("Valores por defecto  +10 y -10 \n");
	}
        if (argc>=2){
       		incremento = atoi(argv[1]);
        	decremento = atoi(argv[2]);
        		if (a < 0){
            			a*=-1;
			}
        		if (b > 0)
           			 b*=-1;
    	}
	return 0;
}

void manage_threads(int a, int b)
{
    pthread_t threads[2];
    	int status = 0;
	contenedorHilo *hiloM = malloc(sizeof(*hiloM));
	hiloM->id = 0;
	hiloM->to_add = a;
	status = pthread_create(&threads[0], NULL, create_thread, mensaje);

	hiloM = malloc(sizeof(*hiloM));
    	msjs->id = 1;
    	msjs->to_add = b;
	status = pthread_create(&threads[0], NULL, create_thread, msjs);
}

void *create_thread(void *args)
{
    contenedorHilo *p = args;
    int i;
    for (i = 0; i < p->times; i++)
    {
        number += p->to_add;
    }
    pthread_exit(args);
}
