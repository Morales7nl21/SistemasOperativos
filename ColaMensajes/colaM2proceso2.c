#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>

struct mensaje{
	long tipo;
	char cadena[50];
};
int main(){

	key_t llave;
	struct mensaje msg;
	int msgsize=sizeof(msg)-sizeof(msg.tipo);
	int msgid,snd,rcv;

	//PAso 1 obtener llave
	llave = ftok("/bin/pwd",2);
	if(llave==-1){perror("Error en el ftok");}
	//PAso 2 crear la cola de mensajes
	msgid=msgget(llave,IPC_CREAT|0777);
	if(msgid==-1){
		perror("Error en msget");
		exit(-1);
	}
	//Paso 3 manipular la col de mensajes
	msg.tipo=2;
	printf("\n[+] Introduce mensaje \t>");
	fgets(msg.cadena,sizeof(msg.cadena),stdin);
	printf("\n [+] Enviando mensaje . . .\n");
	snd=msgsnd(msgid,&msg,msgsize,0);
	if(snd==-1){
		perror("error en msgsnd");
		exit(-1);
	}
	//Leer mensaje
	printf("\n [+] Esperando mensaje . . .\n");
	rcv=msgrcv(msgid,&msg,msgsize,1,0);
	if(rcv==-1){
		perror("[-] Error en msgrcv");
	}
	printf("\n [+] Mensaje recibido \n\tTipo > %li\n\tMensaje\t> %s\n\n",msg.tipo,msg.cadena);
	//PAso 4 liberar cola de mensajes
	msgctl(msgid,IPC_RMID,0);


}
