#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){

	//char *argumentos[]={"./sumE", "4.5", "6.5", NULL};
	//printf("Programa que invoca a  ./sumE con exec");
	//execv(argumentos[0],argumentos);
	int pid;
	pid=fork();
	if(pid == -1 ){
		perror("\n Error al llamar a Fork\n");
		exit(-1);
	}else if(pid == 0){

	printf("Programa Hijo que invoca a ls con exec\n");
	execl("/bin/ls","ls","-l", "-a", NULL);	
	perror("\n\nError al llamar a exec\n");
	printf("\nDasdasdw");


	}else{
		wait(NULL); //Espera a que termine su hijo es equivalente a lo que recibe exit, no nos interesa lo que recibe el hijo
		printf("\nSoy el padre Mi hijo termino\n");
	}



return 0;
}
