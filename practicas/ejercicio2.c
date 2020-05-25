#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_ARGS 10
int extrae_argumentos_d(char *, char *, char *[], int);

int main(int argc,  char *argv[]){

	int pid,i,flag,exitt,temp=0;
	char comando[25];
	char *args[temp], *comand[temp];
	int nargs;
	int tuberia[2];
	if(pipe(tuberia)== -1){
                perror("\n Error en la tuberia \n");
                exit(-1);
        }



	pid=fork();

	if(pid==-1){
		perror("\nError al crear hijo\n");
	}
	else if(pid==0){
		//Procesi hijo
		exitt=1;
		while(exitt){
			read(tuberia[0],comando, sizeof(comando));
			nargs = extrae_argumentos_d(comando, " ", args, MAX_ARGS);
			printf("\nNumero de argumentos mostrados: %i", nargs);
			for (i = 0; i<nargs; i++){
				printf("\nHijo el comando ingresado es : %s",args[i]);
			}
			args[nargs]=(char *)0;
			flag = execvp(args[0], args);
			if(!flag){
				printf("\n No se ha encontrado el comando o no existe\n");
			}

			exitt=strcmp(args[0], "exit\n");
			fflush(stdin);
		}
		exit(0);
	}else{
		//Proceso Padre
		exitt=1;
		while(exitt){
			printf("\nPADRE -> Ingresa un comando para que mi hijo lo ejecute:  ");
			fflush(stdin);
			gets(comando);
			//fgets(comando, sizeof(comando),stdin);
			/* Verifica que por lo menos se le mande un valor */
			if(strlen(comando) > 1){
                		temp=(strlen(comando));
       			 }else{
		                printf("\nIntroduzca al menos un comando, puedes pasarle parametros, vuelva a ejecutar \n");
                		exit(0);
        		}
			/* Manda a la funcion para modificar a args obteniendo el arreglo de cadenas metiso en comando */
			//nargs = extrae_argumentos_d(comando, " ", args, MAX_ARGS);
			write(tuberia[1], comando, strlen(comando)+1);
			/* Prrueba de la funcion
			for (i = 0; i<nargs; i++){
                        printf("\n%s",args[i]);
                        }
			*/


			exitt=strcmp(comando,"exit\n");
			sleep(10);
			fflush(stdin);
		}
		wait(NULL);
	}


	return 0;
}
int extrae_argumentos_d(char *orig, char *delim, char *args[], int max_args)
{

  char *tmp;
  int num=0;
  /* Reservamos memoria para copiar la candena ... pero la memoria justa */
  char *str = malloc(strlen(orig)+1);
  strcpy(str, orig);

  /* Extraemos la primera palabra */
  args[0]= "./comando.out";
  tmp=strtok(str, delim);
  do
    {
      if (num==max_args)
    return max_args+1;  /* Si hemos extraído más cadenas que palabras devolvemos */
                /* El número de palabras máximo y salimos */

      args[num]=tmp;            /* Copiamos la dirección de memoria tmp en args[num] */
      num++;

      /* Extraemos la siguiente palabra */
      tmp=strtok(NULL, delim);
    } while (tmp!=NULL);

  return num;
}
