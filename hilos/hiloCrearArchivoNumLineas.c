#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
void *thread_routine(void *);

int main(int argc, char *argv[]){

	pthread_t thread1;
	int value =atoi( argv[1]);

	if(0!= pthread_create(&thread1, NULL,thread_routine,&value)){
		return -1;
	}
	pthread_join(thread1,NULL);



	return 0;
}
void *thread_routine(void *arg){
	int nr_lines= *((int*)arg);
	int fd;
	char buf[] = "Nueva linea \n";
	printf("El hilo comienza ejecución");
	int i;
	for(i=0; i<nr_lines; i++){
		fd=open("/home/pg721oz/Documents/SistemasOpartivos/hilos/CarpetaPrueba/file.txt", O_WRONLY | O_APPEND);
		write(fd, buf,sizeof(buf)-1); // Ya que el buf es un fichero terminado en 0  no nos interesa y quitamos un 1
		close(fd);
	}


}
