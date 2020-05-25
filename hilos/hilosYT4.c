#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void gotoxy(int,int);

void *saludo(void *);

typedef struct data{
	char *cadena;
	int x,y;
}parametro;


int main(int argc, char *argv[]){



	//char *cadena = "hola mundo";
	pthread_t hilo1;
	pthread_t hilo2;

	parametro p1;
	p1.cadena=argv[1];
	p1.x=5;
	p1.y=2;

	parametro p2;
	p2.cadena=argv[2];
	p2.x=15;
	p2.y=2;


	pthread_create(&hilo1,NULL,saludo,(void *)&p1);
	pthread_create(&hilo2,NULL,saludo,(void *)&p2);
	pthread_join(hilo1,NULL);
	pthread_join(hilo2,NULL);
	printf("\nFIN\n");


	return 0;
}
void gotoxy(int x, int y){
	printf("\033[%d;%df",y,x);

}

void *saludo(void *args){
	system("clear");
	parametro *par = (parametro *) args;
	//char *cadena= (char *) args;
	int i;
	for(i=0; i<strlen(par->cadena); i++){
		fflush(stdout);
		gotoxy(par->x,par->y);
		par->x++;
		printf("%c",par-> cadena[i]);

		sleep(1);
	}
	printf("\n");
	pthread_exit(NULL);
}
