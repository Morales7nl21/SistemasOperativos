#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
void limpiarCad(char *);
int main(){
	int pid,exitt;
	int tuberia[2],tuberia2[2];
	char mensaje[250],respuesta[250];

	//Creando la tuberia
	if(pipe(tuberia)== -1 && pipe(tuberia2)== -1){
		perror("\n Error en la tuberia \n");
		exit(-1);
	}
	pid=fork();
	if(pid==-1){
		perror("\nError al crear al hijo en fork\n");
		exit(-1);
	}else if(pid==0) {
		//HIJO
		exitt=1;
		while(exitt){
			read(tuberia[0], mensaje,sizeof(mensaje));
			printf("\nHIJO --> El valor leido es: %s\n", mensaje);
			strcpy(respuesta,"HIJO --> respuesta:  ");
			limpiarCad(respuesta);

			write(tuberia2[1],respuesta,strlen(respuesta)+1);
			limpiarCad(respuesta);
			exitt=strcmp(mensaje, "EXIT\n");

		}
		close(tuberia[0]);
		close(tuberia[1]);
		close(tuberia[0]);
		close(tuberia[1]);
		exit(0);

	}else{
		//PADRE
		exitt=1;
		while(exitt){
			printf("\nPADRE --> Da un valor:  " );
			fgets(mensaje, sizeof(mensaje),stdin);
			write(tuberia[1], mensaje,strlen(mensaje)+1);
			read(tuberia2[0],respuesta,sizeof(respuesta));
			limpiarCad(respuesta);
			printf("\nPadre --> Respuesta del hijo:  %s\n", respuesta);
			exitt=strcmp(mensaje,"EXIT\n");
		sleep(1);
		}
		wait(NULL);
		close (tuberia[0]);
                close (tuberia[1]);
                close (tuberia2[0]);
                close (tuberia2[1]);
	}
return 0;
}
void limpiarCad(char *cadena) {
	char *p = strchr(cadena, '\n');
	if (p) {
		*p = '\0';
	}
}
