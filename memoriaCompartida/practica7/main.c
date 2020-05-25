#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"TADPilaEst.h"

int main(int argc, char *argv[])
{
	int i,tam_cad;
	pila mi_pila;
	elemento mi_elemento;	
	char expresion[51];
	if(argc!=2)
		exit(1);
	
	strcpy(argv[1],expresion);
	
	Initialize(&mi_pila);
	
	tam_cad=strlen(expresion);
	for(i=0;i<=tam_cad;i++)
	{
		if(expresion[i]=='(')
		{
			mi_elemento.c='(';
			Push(&mi_pila,mi_elemento);
		}
		else if(expresion[i]==')')
		{
			if(Empty(&mi_pila)==TRUE)
			{
				printf("La expresion es incorrecta, intenta cerrar parentesis que nunca abrieron");
				exit(1);
			}
			else
			{
				mi_elemento=Pop(&mi_pila);
			}
		}
	}
	
	if(Empty(&mi_pila))
		printf("OK");
	else
		printf("La expresion es incorrecta, hay parentesis que nunca cerraron");
}