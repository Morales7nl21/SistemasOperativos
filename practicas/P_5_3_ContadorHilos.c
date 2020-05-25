
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
/* Prototipo de la función que va a ejecutar el thread hijo */
void *funcionDecrementa(void *parametro);
void *funcionIncrementa(void *parametro);

/* Contador, global para que sea visible desde el main y desde funcionThread */
int contador = 0;

/*
 * Principal
 * Lanza un thread para la función funcionThread.
 * Después de comprobar el posible error, se mete en un bucle infinito
 * incrementando y mostrando el contador.
 */
int main(int argc, char* argv[])
{
	/* Identificador del thread hijo */
	pthread_t idHilo1;
    pthread_t idHilo2;
        
  int valores[2]; //usado para almacenar los datos que recibimos desde consola 

int cad,contcad,reinicio;
char numeros[3][10];//el primer arreglo indica los numeros que vamos a recibir y el segundo son los digitos de cada numero

for(cad=0,contcad=0;argv[1][contcad] != 0;) {
	for(reinicio=0;argv[1][contcad]!='*';++reinicio)
	{         
   		numeros[cad][reinicio]= argv[1][contcad]; 
                   
                 ++contcad;          
	}
       
        ++contcad;
       ++cad;
   
    // Use c as you need
}

int num;
for(num=0;num<2;++num){
  valores[num]=atoi(numeros[num]);
}

printf("Incremento: %d Decrecemento: %d \n",valores[0],valores[1]);


	/* error devuelto por la función de creación del thread */
	int error1,error2;

	/* Este Hilo se encargara de incrementar*/
		error2 = pthread_create (&idHilo2, NULL, funcionDecrementa,&valores);

        /* Este Hilo se encargara de incrementar*/

	error1 = pthread_create (&idHilo1, NULL, funcionIncrementa, &valores);
	/* Comprobamos el error al arrancar los threads */
	if (error1 != 0 && error2 != 0)
	{
		perror ("No puedo crear thread");
		exit (-1);
	}


/* Bucle infinito para incrementar el contador y mostrarlo en pantalla */
	
pthread_join(idHilo2, NULL);  
pthread_join(idHilo1, NULL);   

  

  
printf("El contador resultante es: %d\n",contador);
return 0;
}

int* ncont= &contador;
/* Funcion que se ejecuta en el thread hijo.*/
void *funcionIncrementa(void *parametro)
{	
int* p = (int*)parametro;
int incremento=*((int*) (p+0));	

*(int*)ncont=incremento;
	
}



void *funcionDecrementa(void *parametro)
{
int* p = (int*)parametro;
int Decremento=*((int*) (p+1));	
usleep(10000);
 //wait(pthread_self(),NULL);
*(int*)ncont=(contador-Decremento);

}
