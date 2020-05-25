#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include<fcntl.h>
int main(){
	int pid,tuberia[2];
	if(pipe(tuberia) == -1){perror("\nError en tuberia\n");exit(-1);}
	pid = fork();
	if(pid==-1){perror("Error en fork");exit(-1);}
	else if(pid == 0){ //Ejeecutara sort -r > archivo.txt
		close(0);//Cierra su entrada estandar
		dup(tuberia[0]);
		close(1);
		open("archivo.txt", O_CREAT|O_RDWR,S_IRWXU);
		close(tuberia[0]);
		close(tuberia[1]);
		execlp("sort","sort","-r",NULL);
		perror("\nError en el execlp\n");

	}else{
		close(1);//Cerrar sañida estandar
		dup(tuberia[1]); //Duplica WR de tuberia
		close(tuberia[0]);
		close(tuberia[1]);
		execlp("ls","ls", NULL);
		perror("\nError en execlp\n");

	}
}


