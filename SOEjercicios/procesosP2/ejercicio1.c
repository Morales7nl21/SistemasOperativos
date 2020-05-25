#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]){
	int i,n,pid,status;
	n=atoi(argv[1]);
	/*printf("N = %i", n);	*/
	for(i=0; i<n; i++){
		pid=fork();
		if(pid==-1){
		perror("Error en la llamada a Fork");
		exit(-1);

		}
		else if(pid==0){
       		 printf("\n Hijo\t Mi PID es: %i\n El PID de mi padre es: %i", getpid(), getppid());
		 exit(0);
       		 }
		else{
		pid=wait(&status);
		if(WIFEXITED(status)){
			printf("\n Padre\t Mi pid es: %i\n , termino con estado %i ", getpid(), WEXITSTATUS(status));


		}

		}

	}
}
