#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void  funcionHilos(int,int);
void *incrOrDecrVal(void *);


typedef struct data{
	int id; //No es necesario pero por si uno me fallaba lo metí.
	int valor;
}contenedorHilo;

int numerG=0;

int main(int argc,char *argv[]){

	pthread_t hilo1;
	pthread_t hilo2;
	int incremento, decremento,status;

	if(argc<2){
		printf("No ingresaste valores para aumentar o disminuir en cada hilo\n");
		printf("Valores por defecto  +20 y -10 \n");
		incremento = 20;
		decremento = -10;
	}
        if (argc>=2){
       		incremento = atoi(argv[1]);
        	decremento = atoi(argv[2]);
        		if (incremento < 0){
            			incremento*=-1;
			}
        		if (decremento > 0)
           			 decremento*=-1;
    	}

        contenedorHilo p1;
        p1.id = 0;
        p1.valor = incremento;
        status= pthread_create(&hilo1, NULL, incrOrDecrVal, (void *)&p1);

        contenedorHilo p2; 
        p2.id = 1;
        p2.valor = decremento;

	status=pthread_create(&hilo2, NULL, incrOrDecrVal, (void *)&p2);

	pthread_join(hilo1,NULL);
	pthread_join(hilo2,NULL);
	printf("Valor de la variable global: %d", numerG);
	return 0;
}

void *incrOrDecrVal(void *args){
	contenedorHilo *p = (contenedorHilo *) args;
	numerG += p->valor;
	usleep(10000);
}
