#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){
	double n1,n2,suma;
	if(argc!=3){
	printf("Ingresa 2 numeros a sumar\n");
	exit(-1);

	}

	n1=atof(argv[1]);
	n2=atof(argv[2]);

	suma=n1+n2;
	printf("---------------------------\n");
	printf("\nSoy el proceso %s, me incovó exec\n", argv[0]);
	printf("\nLa suma de %f y de %f es %f\n\n", n1,n2,suma);


return 0;
}
