#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *retirar(void *);
void *ingresar(void *);
//61676463
int saldo;
//Asignar atributos por default
pthread_mutex_t s=PTHREAD_MUTEX_INITIALIZER;//Esto puede ser sustituido con un semph 
int main(){
	pthread_t h1;
	pthread_t h2;
	int deposito=4000;
	int retiro=250;
	saldo=300;
	printf("\nSaldo %d\n", saldo);
	pthread_create(&h1,NULL,retirar,&retiro);
	pthread_create(&h2,NULL,ingresar,&deposito);
	pthread_join(h1,NULL);
	pthread_join(h2,NULL);
	printf("\nSaldo %d\n", saldo);
}

void *retirar(void *arg){
	int var;
	int dinero = *(int *)arg;
	//Equivalente al down
	pthread_mutex_lock(&s); //down
	printf("\n Retiro: %d", dinero);
	var=saldo;
	var-=dinero;
	saldo=var;
	pthread_mutex_unlock(&s);//up
	pthread_exit(0);
}
void *ingresar(void *arg){
	int var;
	int dinero = *(int *)arg;
	//Equivalente al down
	pthread_mutex_lock(&s); //down
	printf("\n Ingreso: %d", dinero);
	var=saldo;
	var+=dinero;
	saldo=var;
	pthread_mutex_unlock(&s);//up
	pthread_exit(0);
}

