#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void gotoxy(int,int);
void *recorrer(void *);
void *recorrerInverso(void *);

int arreglo[]={1,2,3,4,5,6};
pthread_mutex_t llave;



int main(int argc, char const *argv[]){
	//inicializamos mutex
	system("clear");
	pthread_t hilo1, hilo2;
	pthread_mutex_init(&llave,NULL);
	pthread_create(&hilo1,NULL,recorrer,NULL);
	pthread_create(&hilo2,NULL,recorrerInverso,NULL);
	pthread_join(hilo1,NULL);
	pthread_join(hilo2,NULL);
	return 0;
}
void *recorrer(void *args){
	int i, x=1, y=3;
	printf("Esperando para bloquear llave \n");
	pthread_mutex_lock(&llave); //Con esto le indicamos al compilador que bloque la llave

	printf("Llave bloqueada \n");

	for(i=0; i<(sizeof(arreglo)/sizeof(int)); i++){
		fflush(stdout);
		gotoxy(x,y);
		y=y+2;
		printf("%d", arreglo[i]);
		usleep(1000000);
	}

	printf("\n");
	pthread_mutex_unlock(&llave);

	return NULL; 
}
void gotoxy(int x, int y){
	printf("\033[%d;%df",y,x);

}

void *recorrerInverso(void *args){ 
        int i, x=1, y=20;
        printf("Esperando para bloquear llave \n");
        pthread_mutex_lock(&llave); //Con esto le indicamos al compilador que bloque la llave

        printf("Llave bloqueada \n");

        for(i=(sizeof(arreglo)/sizeof(int))-1;i>=0; i--){
                fflush(stdout);
		y+=2;
                gotoxy(x,y);
                printf("%d", arreglo[i]);
                usleep(1000000);
        }
	printf("\n");
	pthread_mutex_unlock(&llave);
	printf("Llave desbloqueada \n");
	return NULL; 
}
/*
void *saludo(void *args){
	//parametro *par = (parametro *) args;
	char *cadena= (char *) args;
	int i;
	for(i=0; i<strlen(par->cadena); i++){
		fflush(stdout);
		gotoxy(par->x,par->y);
		par->x++;
		printf("%c",par-> cadena[i]);

		sleep(1);
	}
	printf("\n");
}

*/
