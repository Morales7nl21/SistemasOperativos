#include <stdio.h>
#include <stdlib.h>
#include"TADPilaEst.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

void seleccionMenu();
int main(int argc, char *argv[])
{

    char* data;
    char* cadena;//error: sucede debido a que como la variable, no era parte del segmento, no tenia acceso
    
   key_t key;
   int shmid;
   pila* mi_pila;
   elemento mi_elemento,mi_elemento2,mi_elementoaux;	
   int opcion;
char c,vaux,top;

   

    /* Crea la llave para compartir la memoria*/
    if ((key = ftok("hello1.txt", 40)) == -1) /*Here the file must exist */ 
{
        perror("ftok");
        exit(1);
    }

    /* Creacion de la memoria compartida */
    if ((shmid = shmget(key, sizeof(pila),  IPC_CREAT|0644)) == -1) {
        perror("shmget");
        exit(1);
    }

 //se crea acceso a las variables dentro del segmento

    mi_pila=(struct pila*) shmat(shmid, NULL, 0);
    Initialize(mi_pila);

    if (mi_pila==NULL ) {
        perror("shmat");
        exit(1);
    }

while(1){  
seleccionMenu();
scanf("%d",&opcion);
switch(opcion)
{
    case 1:
           system("clear");
           printf("1. Ingresa elemento a la pila\n");
           fflush(stdin);
           scanf("%c",&c);         
          //strncpy(data,  cadena, SHM_SIZE);
           mi_elemento.c=c;
           Push(mi_pila,mi_elemento);
       
    break;

     case 2:
                  system("clear");
                  mi_elementoaux=Top(mi_pila);
                  vaux=mi_elementoaux.c;
                  Pop(mi_pila);
                  printf("Se retiro el elemento %c" ,vaux);
    break;
   
    case 3:  
                   system("clear");
		   mi_elemento2=Top(mi_pila);
                   top= mi_elemento2.c;
                   printf("3. Visualiza elemento superior de la pila %c\n",top);
    break;

    case 4:
                                system("clear");
				printf("Visualizar Elementos en la Pila\n");
				Shows(mi_pila);
				break;
    break;

    case 5:
                Destroy(mi_pila); 
               shmdt((char *)mi_pila);
                shmctl(shmid,IPC_RMID,(struct shmid_ds *) NULL);
                             
 printf("Finalizo\n");
    break;

default:
system("clear");
 printf("Ingrese la opcion correcta\n");


}	

	}
    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}

void seleccionMenu(){
	printf("\n MEMORIA COMPARTIDA\n");
	for(int i=0;i<21;i++)
		printf("-");
	printf("\n1 Ingresar Elemento\n2 Retirar Elemento\n3 Visualizar Elemento");
	printf("\n4 Visualizar Pila\n5 Finalizar\n");
	for(int i=0;i<21;i++)
		printf("-");
	printf("\nIngrese su Opcion: ");
}
