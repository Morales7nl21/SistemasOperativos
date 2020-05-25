#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <signal.h>
void funcion_sig(int sig){
	printf("Señal recibida %d\n",sig);
	printf("\nNo moriré!\n");
}

int main(){
	//kill(-1,SIGKILL);
	signal(SIGTERM,funcion_sig);
	signal(SIGINT,funcion_sig);
	while(1){
		pause();
		printf("\n :3 ...aggg\n");
		sleep(1);


	}

return 0;
}
