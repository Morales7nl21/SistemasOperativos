#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,  char *argv[]){

	int i,flag, temp=0,temp2=0;
	char *comand[temp];

	if(argc>1){
		temp=argc-1;
		temp2=temp;
	}else{
		printf("\nIntroduzca al menos un comando, puedes pasarle parametros, vuelva a ejecutar\n");
		exit(0);
	}
	for (i=1;i<argc;i++){
		comand[i-1] = argv[i];
	}
	//Puede usarse (char *)0 para NULL
	comand[temp2] = NULL;
	/*
	execvp(arg1,arg2)
	Recibe  como primer argumento una matriz de cadenas a ejecutar, el segundo una matriz de cadenas terminadas en nulo
	*/
	flag = execvp(comand[0], comand);

	if (!flag){
		printf("\nNo se ha encontrado el comando, o no existe\n");
	}
	return flag;
}
