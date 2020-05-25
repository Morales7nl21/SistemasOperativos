#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]){
        int i,n1,n2,pid,status,op;
	float n1f,n2f,n3f;

	op=0;
        n1=atoi(argv[1]);
        n2=atoi(argv[2]);
	n1f=n1;
	n2f=n2;
	n3f=0;
	/*printf("N = %i", n);  */
        for(i=0; i<4; i++){
                pid=fork();

                if(pid==-1){
                perror("Error en la llamada a Fork");
                exit(-1);

                }
                else if(pid==0){
                 printf("\n\n Hijo\t Mi PID es: %i\n El PID de mi padre es: %i", getpid(), getppid());
			if(op==0){
			printf("\nID = %i\nOperacion Suma %i + %i = %i\n Terminacion: 0 ",op+1, n1,n2, n1+n2);
			}
			if(op==1){
                        printf("\nID = %i\nOperacion Resta %i - %i = %i Terminacion: 0 ",op+1, n1,n2, n1-n2);
                        }
			if(op==2){
                        printf("\nID = %i\nOperacion Multriplicacion %i * %i = %i Terminacion: 0 ",op+1, n1,n2, n1*n2);
                        }
			if(op==3){
				if(n2==0){

				printf("\nBad, Terminación 1");
				exit(1);
				}else{

					printf("\nID = %i\nOperacion Division %i / %i = %.3f Terminacion:  0",op+1, n1,n2, n1f/n2f);

				}
                        }

			op++;
                 exit(0);
                 }
                else{
                pid=wait(&status);
                if(WIFEXITED(status)){
                        //printf("\n Padre\t Mi pid es: %i\n El PID de mi hijo es: %i, termino con estad$


                } 
	

	
	op++;
        }

}
}


