#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
//Esperar a una señal del padre o del hijo y ver que se obtiene, tiene que esperar a que el hijo 
//Mandar la señal el hijo al padre al terminar el padre continua su ejecución
void control (int sig){

     printf("Hijo recibe señal... %d \n", sig);
}
int main(){
	int pid=fork();
	if(pid==-1){

	}else if(pid == 0){
                //while(1){
                kill(getppid(),SIGUSR1);
                printf("\n :3 ----\n");
                sleep(1);
                //}

        }
	else{
                //kill(pid,SIGTERM);
                sleep(3);
                signal(SIGUSR1,control);
                sleep(1);
                  }


return 0;
}


