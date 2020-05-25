#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<math.h>

struct datos{
	int suma; //almacena la suma total de cada hilo
	int ciclo;//Veces en que E1 se sumara
	int E1;//Numero de E1 para que el valor que se sumara n ciclos
};
 int  hilo=0;
void* hilos_muestra(void* para)
{

struct datos* p = (struct datos*)para;
printf("E1= %d  se sumara: %d veces en este hilo\n ",p->E1, p->ciclo);

int i;
for(i=1;i<=(p->ciclo); ++i){
p->suma+= p->E1;	
}

++hilo;

printf("HILO:%d Suma=  %d\n\n",hilo,(p->E1*p->ciclo));  	
  	
}
 
int main(int argc,char *argv[])
{
	struct datos parametros;
	int N[3]; //usado para almacenar los datos que recibimos desde consola 

int cad,contcad,reinicio;
int num;
for(num=0;num<3;++num){
  N[num]=atoi(argv[num+1]);
}

printf("E1: %d E2: %d N: %d\n",N[0],N[1],N[2]);

    
 	while(N[2]>N[1])
	 {
 		 printf("No se puede tener una cantidad de hilos mayor E2\n");
 		printf("Ingrese Numero hilos <E2\n");
               scanf("%d",&N[2]);
	 }
 	
 /*Creacion de hilos*/ 
    pthread_t thread[N[2]];
 

 /*  Creamos un hilo  y  esta funcion genera un valor que usamos para identificar al hilo*/
 
 int i=0;
 double partent;
 parametros.ciclo=N[1]/N[2];
 parametros.E1=N[0];
 parametros.suma=0;

 while(i<N[2])
 {
			   	if((N[1]%N[2])==0)
				   { //Si el modulo E1 y el numero de ciclos es par, esto significa que a cada hilo, le tocara un ciclo de trabajo igual
			   		parametros.ciclo=N[1]/N[2];//Ciclo de trabajo par
 	  	 		    pthread_create(&thread[i], NULL, &hilos_muestra,&parametros);
 	  	 		    usleep (10000) ;
				   }else
			   		 {//Si el modulo no fuera par, el ciclo de trabajo del hilo seria igual hasta i-1, en i su ciclo de trabajo se vuelve diferente
					  if(i<N[2]-1)//Este if evalua a todo los N-1, siendo el ciclo de trabaja igual para cada hilo
						{
				      	 parametros.ciclo=N[1]/N[2];
 	  	 		      	 pthread_create(&thread[i], NULL, &hilos_muestra,&parametros);
 	  	 		      	 usleep (10000) ;//Es importante el tiempo para que alcancehacertoda la tarea  
 	  	 		      	 
					 	}else//Cuando se rompa la condicion anterior osea que estemos en el ultimo ciclo, este se tiene un ciclo de trabajo diferente
							{
                        	   parametros.ciclo=  parametros.ciclo + (N[2]*modf((double)N[1]/N[2], &partent)) + 1e-9; //el 1e-9 es un redondeo ya que la funcion fmod trabaja  con double y al sumar esto hacemos que el tipo int se redondee;
 	  	 		         	   pthread_create(&thread[i], NULL, &hilos_muestra,&parametros);
							
					        }				   			   	
				}
	++i;
}

i=0;
while(i<N[2]){
  pthread_join(thread[i], NULL);  	
  ++i;
}

printf("\n\n La suma total de los %d hilos es %d",N[2],parametros.suma);
exit(0); 
    
}
    
