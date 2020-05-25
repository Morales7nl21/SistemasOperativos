#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main(){
	int p;

	p=getpriority(PRIO_PROCESS,getpid());

	printf("Prioridad %d ", p);

	setpriority(PRIO_PROCESS,getpid(),5);

	p=getpriority(PRIO_PROCESS,getpid());
	printf("Prioridad %d ", p);

	while(1){
	printf("Hola mundo \n");
	sleep(1);
	}

return 0;
}
