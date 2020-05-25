#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
/*
	Son unidireccionales se necesitarn dos tuberias por cada entidad
	Simular comunicacion por radio, cuando se diga cambio se cambia la
	persona en hablar .


*/
//int mkfifo(char *path,mode_t modo);
//void crearTuberias(char**, mode_t);
int main(){

	int fifo_emisor,fifo_receptor,cambio,cambioFuera;
	//char *fE="fifo_emisor", *rE="fifo_receptor";
	char mensaje[150];
	//Creando tuberias
	//crearTuberias(fE,S_IRWXU);
	//crearTuberias(rE,S_IRWXU);

	//if(mkfifo("fifo_emisor", S_IRWXU) == -1){perror("Error en la tuberia FIFO EM"); exit(-1);}
	//if(mkfifo("fifo_receptor", S_IRWXU) == -1){perror("Error en la tuberia FIFO RE"); exit(-1);}

	//Accediendo a las tuberias
	/*
		Primer parametro es el archivo a abrir
		Segudno parametro el modo
	*/
	fifo_emisor=open("fifo_emisor",O_WRONLY);
	 /*
		Abre la tuberia, busca en la tabla de descriptor de archivos
		 la primer entrada disponible y devuelve el identificador
		 donde coloco a la tuberia en la tabla
	*/
	fifo_receptor=open("fifo_receptor", O_RDONLY);

	do{
		printf("\nEMISOR <._.> \n");
		fgets(mensaje,sizeof(mensaje),stdin);
		write(fifo_emisor,mensaje,strlen(mensaje)+1);
		cambio=strcmp(mensaje,"CAMBIO\n");
		cambioFuera=strcmp(mensaje,"CAMBIO Y FUERA\n");
		if(cambio==0){
			do{
				read(fifo_receptor,mensaje,sizeof(mensaje));
				printf("\nRECEPTOR (*>*) %s",mensaje);
				cambio=strcmp(mensaje,"CAMBIO\n");
				cambioFuera=strcmp(mensaje,"CAMBIO Y FUERA\n");

			}while(cambio!=0 && cambioFuera!=0); //Mientras cambio o cambio y fuera no sea eso
			//Continue ese ciclo hasta que les mande cambio  o cambio y fuera
		}
	}while(cambioFuera!=0);

	close(fifo_emisor);
	close(fifo_receptor);

return 0;
}
/*void crearTuberias(char *nombre,mode_t modo){
	if(mkfifo(nombre,modo) == -1){
		perror("Error en la tuberia FIFO");
		exit(-1);
	}
}
*/
