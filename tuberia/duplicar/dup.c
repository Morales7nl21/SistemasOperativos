#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
	printf("\nDuplicando salida estandar %i \n",dup(1));
	write(3,"Duplicado\n",11);
	close(0);
	printf("\nDuplicando salida estandar %i \n",dup(1));

	return 0;
}
