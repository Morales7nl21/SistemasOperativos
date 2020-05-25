#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <signal.h>
int main(){


	signal(SIGINT,SIG_IGN );
	while(1){
		printf("\n :3 ...aggg\n");
		sleep(1);


	}

return 0;
}
