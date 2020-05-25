#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
void limpiarCad(char *);
//Que hariamos si el hijo ahora va  a mandar texto es decir escirbir igual, hacer el código
int main(){
	int pid,exitt;
	int tuberia[2],tuberia2[2];
	char mensaje[250],respuesta[250];

	//Creando la tuberia
	if(pipe(tuberia)== -1 && pipe(tuberia2)== -1){
		perror("\n Error en la tuberia \n");
		exit(-1);
	}
	//printf("\n %i, %i \n", tuberia[0], tuberia[1]);
	//SE crea al hijo
	pid=fork();
	if(pid==-1){
		perror("\nError al crear al hijo en fork\n");
		exit(-1);
	}else if(pid==0) { //HIJO
		exitt=1;
		while(exitt){
			read(tuberia[0], mensaje,sizeof(mensaje));
			printf("\nHijo el valor leido es: %s\n", mensaje);
			//close(tuberia[0]);
			printf("\nHijo - Mi respuesta será: \n");
			printf("\nHijo el valor leido es: %s\n", mensaje);
			close(tuberia);
			fgets(respuesta,sizeof(respuesta),stdin);
			close(tuberia2[0]);
			write(tuberia2[1],respuesta,strlen(respuesta)+1);
			limpiarCad(respuesta);
			close(tuberia2[1]);
			exitt=strcmp(mensaje, "EXIT\n");

		}
		sleep(1);
		exit(0);
	}else{ //PAdre
		do{
		exitt=1;
		while(exitt){
			printf("\nPadre: Da un valor:  " );
			//close(tuberia[0]);
			fgets(mensaje, sizeof(mensaje),stdin); //Se puede usar scan, primer es a donde va  almacenar, segundo cantidad y el tercero de donde va a leer
			//write(tuberia[1], mensaje,strlen(mensaje)+1); //PAra manejo de arhcivos en este caso la tuberia, el primer parametro es a donde va a escribir, lo que va a escribir,
			//y 3 el tamaño real del mensaje se le suma uno para le terminador de linea
			//close(tuberia[1]);
			
			read(tuberia2[0],respuesta,sizeof(respuesta));
			printf("\nPadre- Respuesta del hijo: \n %s", respuesta);
			//close(tuberia[0]); //Val agregado
			write(tuberia[1], mensaje,strlen(mensaje)+1); //PAra manejo de arhcivos en este $
                        //y 3 el tamaño real del mensaje se le suma uno para le terminador de linea
                        //close(tuberia[1]);
			//close(tuberia[1]); //val agregado


			//limpiarCad(mensaje);
			exitt=strcmp(mensaje,"EXIT\n"); //ponemos el salto de linea ya que lo toma
		sleep(1);
		}
		wait(NULL);
			read(tuberia2[0],respuesta,sizeof(respuesta));
		}while(!(strcmp(respuesta,"Aceptado")));
	}



return 0;
}
void limpiarCad(char *cadena){
	char *p =strchr(cadena,'\n');
	if (p){ *p = '\0';}
}
