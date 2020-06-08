/*
	Autor: Eugenio Ayona Milton & Morales Blas David Israel
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

#define MAX_ARGS 20
#define MAX_CHAR 1000

/*

	Nota 1: por si lo estas viendo en github, se puede optimizar solo sacando el izquierdo de cada
	char*[] que contiene los argumentos que se buscan a ejecutar, para así solo mandar a ejecutar
	los dos comandos de la izquierda, recortar el char * (comando entero) y con ello hacerlo recursivo

	Nota 2: las funciones "extrae_argumentos_leftPipe", "extrae_argumentos_rightPipe",
	"extrae_argumentos_M", "extrae_argumentos_d"  se pueden sustitur por funcion "extrae_argumentos_general"
	Existen cambios donde se estaban trasladando esas funciones, mas abajo se explica como se pasarian.

	Nota 3: la funcion resolveOl se puede juntar con la funcion verifyCad para ahorrar lineas de código.

	Nota 4: las funciones onePiperedirect... y twoPiperedirect y twoPipeconcat... se pueden unir haciendo de esas 4
	1 sola funcion en realidad ocupan lo mismo solo se tiene que mandar la funcion a sustituir.


	Nota 5: al poner "exit" cuando se esta ejecutando el programa no se cierra :0 no sabemos aun porque aiudaa


*/




//*********************extrae general(EN PROCESO)******************************/
// El primero es la cadena, el segundo el delimitador
// el tercero guardara las las palabras extaidas de la cadena
//el cuarto un tope de maximos argumentos, el quinto la flag estado actual
// y el sexto el modo, el septimo guarda una cadena de ser que solo divida cadena.
// ya sea por izquierda, derecha, general o de extraer nueva cadena.
// general = 1, izquierda = 2, derecha = 3, nueva cad = 4;

int extrae_argumentos_general(char* , char* , char*[] ,int ,int* ,int,char * );
/******************************************************************************/

int extrae_argumentos_d(char *, char *, char *[], int, char*);
int extrae_argumentos_leftPipe(char *, char *, char *[], int, int*);
int extrae_argumentos_rightPipe(char *, char *, char *[], int, int*);
int extrae_argumentos_M(char *, char *, char *);

int resolveOl(char *, char*,char*[],int);

char* nameUser();

void limpiaCad(char *[], int);
char* leerLinea(char*);
int verifyCad(char**, int);
void printARGS(char **,int);
void removeChar(char *,char);


void oneARG(char *);
void onePipe(char *, int*);
void twoPipes(char *, int*);
void redirectOutputToFile(char *,int *);
void redirectInputOfFile(char *,int *);
void concatredirectOutputToFile(char *, int*);
void concatredirectInputOfFile(char *, int *);
void onePipeRedirectOutputToFile(char *, int*);
void onePipeconcatredirectOutputToFile(char *, int*);
void twoPipesRedirectOutputToFile(char *, int *);
void twoPipesConcatredirectOutputToFile(char *, int *);


int main(int argc,  char *argv[]){

	struct utsname unameD;
	int exitt,temp=0, nargs, tuberia[2],nargsLP,priority=0, flagIO;
	char *args[temp], comando[MAX_CHAR], nombre[20], dir[1024], *argsLeft[temp];
	pid_t pid;
	char comando2[MAX_CHAR];
	int verify=0;
	char *args2[temp];
	uname(&unameD);
	getcwd(dir,sizeof(dir));
	strcpy(nombre,nameUser());


	do{
		if(pipe(tuberia)== -1){
                	perror("\n [-] ERROR PIPE \n");
               		exit(-1);
        	}
		//pid_t
		pid=fork();
		if( pid == -1){
			perror("\n [-] ERROR FORK\n");
		}
		else if(pid==0){
			close(tuberia[1]);
			read(tuberia[0],comando, sizeof(comando));
			nargs=0;
			strcpy(comando2,comando);
			verify=resolveOl(comando2," ",args2,MAX_ARGS);
			nargs=extrae_argumentos_general(comando, " ", args, MAX_ARGS,&flagIO ,1,NULL);
			priority=verifyCad(&args[0], nargs);
			//printf("\nPriority: %i\n", priority); //See priotity to check if works fine
				switch (priority)
				{
				case 0:
					oneARG(comando);
				case 1:
						nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
						if(flagIO==1){
							//Caso para 2 comandos establecidos por una pipe ejem: ls | wc
							fflush(stdout);
							onePipe(comando,&flagIO);

						}else if (flagIO == 2){
							// Caso para 1 comandos con > ejem: ls -la > 1.txt
							fflush(stdout);
							redirectOutputToFile(comando,&flagIO);

						}else if(flagIO == 3){
							//Caso para comando con < ejemmplo wc < 1.txt
							fflush(stdout);
							fflush(stdin);
							redirectInputOfFile(comando,&flagIO);

						}else if (flagIO == 4){
							// Caso para 1 comando con >> ejem: ls -la >> 1.txt
							fflush(stdout);
							concatredirectOutputToFile(comando,&flagIO);
						}
						else if(flagIO == 5)
						{
							fflush(stdout);
							fflush(stdin);
							concatredirectInputOfFile(comando,&flagIO);

						}
					break;
				case 2:

						if (((strcmp(args2[0],"|") == 0) && (strcmp(args2[1],"|") == 0)) != 0)
						{
							fflush(stdout);fflush(stdin);
							nargsLP=extrae_argumentos_general(comando, " ", argsLeft, MAX_ARGS,&flagIO ,1,NULL);
							twoPipes(comando,&flagIO);
						}
						if (((strcmp(args2[0],"|") == 0) && (strcmp(args2[1],">") == 0)) != 0)
						{
							fflush(stdout);fflush(stdin);
							onePipeRedirectOutputToFile(comando,&flagIO);


						}
						if (((strcmp(args2[0],"|") == 0) && (strcmp(args2[1],">>") == 0)) != 0)
						{
							fflush(stdout);fflush(stdin);
							onePipeconcatredirectOutputToFile(comando,&flagIO);

						}
				case 3:
						if (((strcmp(args2[0],"|") == 0) && (strcmp(args2[1],"|") == 0) && (strcmp(args2[2],">") == 0)) != 0)
						{
							fflush(stdout);fflush(stdin);
							twoPipesRedirectOutputToFile(comando,&flagIO);

						}
						if (((strcmp(args2[0],"|") == 0) && (strcmp(args2[1],"|") == 0) && (strcmp(args2[2],">>") == 0)) != 0)
						{
							fflush(stdout);
							fflush(stdin);
							twoPipesConcatredirectOutputToFile(comando,&flagIO);
						}

					break;
				default:
					break;
				}

		}else{
			close(tuberia[0]);
			printf("\n%s@%s:~%s$ ", nombre,unameD.nodename, dir);
			leerLinea(&comando[0]);
			write(tuberia[1], comando, strlen(comando)+1);
			wait(&pid);
			close(tuberia[1]);
		}
		if(strcmp(comando,"exit")==0){
			kill(pid, SIGTERM);
		}

	}while( strcmp(comando,"exit") != 0);

	return 0;
}


/***************************************************************************************END MAIN*************************************************************************************************************/
// FUNCTION INFO NAME
char *nameUser(){
	char *login;
	struct passwd *pentry;
	//log user verify
	if((login = getlogin()) == NULL){
		perror("getlogin");
		exit(-1);
	}
	//Pass user veryfy
	if((pentry = getpwnam(login)) == NULL){
		perror("getpwnam");
		exit(-1);
	}
	return pentry->pw_name;
}

int extrae_argumentos_d(char *orig, char *delim, char *args[], int max_args, char *delim2){

	char *tmp;
	int num=0;
	char *str = malloc(strlen(orig)+1);
	strcpy(str, orig);
	args[0]= "./comando.out";
	tmp=strtok(str, delim);
	do{
		if (num==max_args)
		return max_args+1;
		args[num]=tmp;
		num++;
		tmp=strtok(NULL, delim);
	}while (tmp!=delim2);
	return num;
}

int extrae_argumentos_M(char *orig, char *delim, char *args){

	char *tmp;
	int num=0;
	char *str = malloc(strlen(orig)+1);

	strcpy(str, orig);
	tmp=strtok(str,delim);
	tmp=strtok(NULL,"\0");
	strcpy(args,tmp);
	return strlen(args)+1;
}

void limpiaCad(char *cad[], int lim){
	int i;
	for(i=0; i<lim; i++){
		cad[i]="";
	}

}

char *leerLinea(char *comando){
	int i;
	gets(comando);
	return comando;

}
int verifyCad(char **comand, int tam){
	int priority=0;

	for(int i=0; i<tam; i++){
		if(strcmp(comand[i],"|") == 0 || (strcmp(comand[i],">") ==0)  || (strcmp(comand[i],"<") ==0) || strcmp(comand[i],">>") == 0 || strcmp(comand[i],"<<") == 0){
			priority++;
		}
	}
	return priority;
}
int resolveOl(char *orig, char*delim,char*args[],int max_args){
	char *tmp,f=1,*tmp2;
	int num=0,band[5];
	char *str=malloc(strlen(orig)+1);

	strcpy(str, orig);
	args[0]= "./comando.out";
	tmp=strtok(str, delim);
	do{

		band[0]=(strcmp(tmp,"|"));
		band[1]=(strcmp(tmp,">"));
		band[2]=(strcmp(tmp,"<"));

		band[3]=(strcmp(tmp,">>"));
		band[4]=(strcmp(tmp,"<<"));
		if(band[0]==0){
			args[num]=tmp;
			num++;
		}
		if(band[1]==0){
			args[num]=tmp;
			num++;
		}
		if(band[2]==0){
			args[num]=tmp;
			num++;
		}
		if(band[3]==0){
			args[num]=tmp;
			num++;
		}
		if(band[4]==0){
			args[num]=tmp;
			num++;
		}
		//tmp2=strtok(NULL, delim);
		tmp=strtok(NULL, delim);

	}while (tmp!=NULL);
	return num;

}

int extrae_argumentos_leftPipe(char *orig, char *delim, char *args[], int max_args, int* IOFlag){

	char *tmp;
	int band[5], f=1;
	int num=0;
	char *str = malloc(strlen(orig)+1);
	strcpy(str, orig);
	args[0]= "./comando.out";
	tmp=strtok(str, delim);
	do{
		if (num==max_args)
		return max_args+1;
		args[num]=tmp;
		num++;
		tmp=strtok(NULL, delim);
		band[0]=(strcmp(tmp,"|"));
		band[1]=(strcmp(tmp,">"));
		band[2]=(strcmp(tmp,"<"));
		band[3]=(strcmp(tmp,">>"));
		band[4]=(strcmp(tmp,"<<"));
		if(band[0]==0){
			f=0;
			*IOFlag=1;
		}
		if(band[1]==0){
			f=0;
			*IOFlag=2;
		}
		if(band[2]==0){
			f=0;
			*IOFlag=3;
		}
		if(band[3]==0){
			f=0;
			*IOFlag=4;
		}
		if(band[4]==0){
			f=0;
			*IOFlag=5;
		}
	}while (f);
	return num;
}
int extrae_argumentos_rightPipe(char *orig, char *delim, char *args[], int max_args, int *IOFlag){

	char *tmp;
	int band[5];
	char *str = malloc(strlen(orig)+1);
	int flag=0;
	int num=0;
	strcpy(str, orig);
	args[0]= "./comando.out";
	tmp=strtok(str, delim);
	do{
		band[0]=strcmp(tmp,"|");
		band[1]=strcmp(tmp,">");
		band[2]=strcmp(tmp,"<");
		band[3]=(strcmp(tmp,">>"));
		band[4]=(strcmp(tmp,"<<"));
		if (band[0]==0){
			flag=1;
			*IOFlag=1;
			tmp=strtok(NULL, delim);
		}
		if (band[1]==0){
			flag=1;
			*IOFlag=2;
			tmp=strtok(NULL, delim);
		}
		if (band[2]==0){
			flag=1;
			*IOFlag=3;
			tmp=strtok(NULL, delim);
		}
		if(band[3]==0){
			flag=1;
			*IOFlag=4;
		}
		if(band[4]==0){
			flag=1;
			*IOFlag=5;
		}
		if (flag==1){
			args[num]=tmp;
			num++;
		}

		tmp=strtok(NULL, delim);
	}while  (tmp!=NULL);
	return num;
}
void removeChar(char *str, char value){
	size_t strl_len = strlen(str);
	size_t i = 0, p=0;
	char result[strl_len];
	for ( i = 0; i < strl_len; i++)
	{
		if (str[i] != value)
		{
			result[p] = str[i];
			p++;
		}
	}
	if(p<strl_len)
		str[p] = '\0';
	for ( i = 0; i < p; i++)
	{
		str[i]=result[i];
	}

}
//Functions WF
void printARGS(char **arr,int size){
	for(int i=0; i<size; i++){
		printf("\n Value :%s", arr[i]);
	}

}
//FUNCIONTS TO MANIPULATE PIPES,STANDAR INPUT AND STANDAR OUTPUT  -> |,>,<,>>,<<.
void oneARG(char *comando){

	int nargs=0 ,temp=0;
	char *args[0];
	int flag;

	nargs = extrae_argumentos_general(comando, " ", args, MAX_ARGS,NULL,1,NULL);
	//priority=verifyCad(&args[0], nargs);
	sleep(1);
	if(strcmp(comando,"exit")!=0){
	args[nargs]=(char *)0;
	flag = execvp(args[0], args);
	if(flag == -1){
		printf("\n [-] There is not found that command or does not exist\n");
	}
	else{limpiaCad(&args[0],nargs);	
	}
	}

}
void onePipe(char *comando, int *IOFlag){

	int flagIO=*IOFlag, temp=0,fd[2],pid3,nargsLP=0,nargsDP=0,nargsRP=0;
	char comando2[MAX_CHAR],*argsLeft[temp], *argsRight[temp];

	strcpy(comando2,comando);
	nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
	nargsRP= extrae_argumentos_rightPipe(comando2, " ", argsRight, MAX_ARGS, &flagIO);
	//printf("\n Flag %i", flagIO);
	int status1;
	pipe(fd);
	pid_t p1,p2;
	p1=fork();
	if(p1==0){
		//printf("\r");
		argsRight[nargsRP]= (char *)0;
		close(STDIN_FILENO);
		dup(fd[0]);
		close(fd[1]);
		execvp(argsRight[0], argsRight);
		limpiaCad(&argsLeft[0],nargsRP);
	}if(p1!=0){
		p2=fork();
		if(p2==0){
			argsLeft[nargsLP]= (char *)0;
			close(STDOUT_FILENO);
			dup(fd[1]);
			close(fd[0]);
			execvp(argsLeft[0], argsLeft);
			limpiaCad(&argsLeft[0],nargsLP);
		}
	}
	close(fd[0]);
	close(fd[1]);
	sleep(1);
	fflush(stdin);
	fflush(stdout);
	wait(&p1);
	wait(&p2);
	//kill(p1, SIGKILL);
	//kill(p2, SIGKILL);
	exit(0);


}
void redirectOutputToFile(char *comando,int *IOFlag){

	int flagIO=*IOFlag, temp=0,fd,nargsLP=0,nargsDP=0;
	char comando2[MAX_CHAR],comando3[MAX_CHAR], comando4[MAX_CHAR],*argsLeft[temp];
	pid_t pid3;
	strcpy(comando2,comando);
	strcpy(comando3,comando2);
	//printf("comando : %s ", comando2 );
	nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
	argsLeft[nargsLP]= (char *)0;
	nargsDP= extrae_argumentos_M(comando2,">",comando3);
	removeChar(comando3,' ');
	pid3=fork();
	//printf("Archivo a re escribir:%s, flag: %i", comando3, flagIO);
	if (pid3 == 0){
		int fd = open(comando3, O_RDONLY | O_WRONLY | O_TRUNC);
		dup2(fd, 1);
		execvp(argsLeft[0], argsLeft);
		close(fd);
		exit(0);
	}
	else{
		wait(&pid3);
	}
	strcpy(comando3," ");strcpy(comando2," ");strcpy(comando4," ");

	//kill(pid3, SIGKILL);
	exit(0);
}
void redirectInputOfFile(char *comando,int *IOFlag){
	int flagIO=*IOFlag, temp=0,fd;
	pid_t pidRIF;
	char comando2[MAX_CHAR],comando3[MAX_CHAR], comando4[MAX_CHAR],*argsLeft[temp];
	int nargsLP=0,nargsDP=0;
	strcpy(comando2,comando);
	strcpy(comando3,comando2);
	//printf("comando : %s ", comando2 );
	nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
	argsLeft[nargsLP]= (char *)0;
	nargsDP= extrae_argumentos_M(comando2,"<",comando3);
	removeChar(comando3,' ');
	pidRIF=fork();
	//printf("Archivo a re escribir:%s, flag: %i", comando3, flagIO);
	if (pidRIF == 0){
		int fd = open(comando3,O_RDWR , S_IRUSR);
		close(0);
		dup2(fd, 0);
		close(fd);
		execvp(argsLeft[0], argsLeft);
		exit(0);
	}
	else{
		wait(&pidRIF);
	}
	strcpy(comando3," ");
	strcpy(comando2," ");
	strcpy(comando4," ");
	//kill(pidRIF, SIGKILL);
	exit(0);

}
void concatredirectOutputToFile(char *comando,int *IOFlag){

	int flagIO=*IOFlag, temp=0,fd;
	pid_t pidCROF;
	char comando2[MAX_CHAR],comando3[MAX_CHAR], comando4[MAX_CHAR],*argsLeft[temp];
	int nargsLP=0,nargsDP=0;
	strcpy(comando2,comando);
	strcpy(comando3,comando2);
	nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
	argsLeft[nargsLP]= (char *)0;
	nargsDP= extrae_argumentos_M(comando2,">",comando3);
	removeChar(comando3,' ');
	removeChar(comando3,'>');
	pidCROF=fork();
	//printf("\nArchivo a re escribir:%s, flag: %i\n", comando3, flagIO);
	if (pidCROF == 0){
		int fd = open(comando3,O_RDWR| O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IWOTH);
		dup2(fd, 1);
		execvp(argsLeft[0], argsLeft);
		close(fd);
		exit(0);
	}
	else{
		wait(&pidCROF);
	}
	strcpy(comando," ");
	strcpy(comando3," ");
	strcpy(comando2," ");
	strcpy(comando4," ");
	//kill(pidCROF, SIGKILL);
	exit(0);
}
void concatredirectInputOfFile(char *comando,int *IOFlag){
	int flagIO=*IOFlag, temp=0,fd;
	pid_t pidCRIF;
	char comando2[MAX_CHAR],comando3[MAX_CHAR], comando4[MAX_CHAR],*argsLeft[temp];
	int nargsLP=0,nargsDP=0;
	strcpy(comando2,comando);
	strcpy(comando3,comando2);
	//printf("comando : %s ", comando2 );
	nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
	argsLeft[nargsLP]= (char *)0;
	nargsDP= extrae_argumentos_M(comando2,"<",comando3);
	removeChar(comando3,' ');
	removeChar(comando3,'<');
	pidCRIF=fork();
	//printf("Archivo a re escribir:%s, flag: %i", comando3, flagIO);
	if (pidCRIF == 0){
		int fd = open(comando3,O_RDWR , S_IRUSR);
		close(0);
		dup2(fd, 0);
		close(fd);
		execvp(argsLeft[0], argsLeft);
		exit(0);
	}
	else{
		wait(&pidCRIF);
	}
	strcpy(comando3," ");
	strcpy(comando2," ");
	strcpy(comando4," ");
	//kill(pidCRIF, SIGKILL);
	exit(0);

}
void twoPipes(char *comando, int *IOFlag){
	//READ_END 0//WRITE END 1
	//CASO PARA DOS PIPES | | EJ LS -LA | GREP U | WC

	int flagIO=*IOFlag, temp=0,fd,fd1[2], fd2[2],status2,nargsLP=0,nargsDP=0, nargsMP=0, nargsRP=0;
	char comando2[MAX_CHAR],comando3[MAX_CHAR], comando4[MAX_CHAR],*argsRight[temp],*argsLeft[temp],*argsCenter[temp];
	pid_t pidC2,pid3;

	strcpy(comando2,comando);
	nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
	nargsDP= extrae_argumentos_M(comando2,"|", comando3);
	strcpy(comando4,comando3);
	nargsMP=extrae_argumentos_leftPipe(comando3," ", argsCenter, MAX_ARGS, &flagIO);
	nargsRP=extrae_argumentos_rightPipe(comando4," ", argsRight, MAX_ARGS, &flagIO);
	pipe(fd1);
	pidC2 = fork();
	if(pidC2==0){
		close(fd1[0]);
		dup2(fd1[1], STDOUT_FILENO);
		close(fd1[1]);
		argsLeft[nargsLP]= (char *)0;
		execvp(argsLeft[0],argsLeft);
	}else{
		close(fd1[1]);
		pipe(fd2);
		pidC2=fork();
		if (pidC2 == 0){
			close(fd2[0]);
			dup2(fd1[0],STDIN_FILENO);
			close(fd1[0]);
			dup2(fd2[1], STDOUT_FILENO);
			close(fd2[1]);
			argsCenter[nargsMP] = (char *)0;
			execvp(argsCenter[0], argsCenter);
		}else{
			close(fd1[0]);
			close(fd2[1]);
			pid3 = fork();
			if(pid3==0){
				dup2(fd2[0], STDIN_FILENO);
				close(fd2[0]);
				argsRight[nargsRP]=(char *)0;
				execvp(argsRight[0], argsRight);
			}
		}
	}
	wait(&pidC2);
	wait(&pid3);
	wait(&status2);
	//kill(pidC2, SIGKILL);
	//kill(pid3, SIGKILL);
	exit(0);
}

int extrae_argumentos_general(char *orig, char *delim, char *args[], int max_args, int* IOFlag, int modo,char *cad){

	char *tmp;
	int num=0;
	int band[5], f;
	char *str = malloc(strlen(orig)+1);
	//char *cad = malloc(strlen(orig)+1);
	int flag=0;

	switch (modo)
	{
	case 1:
			//char *str = malloc(strlen(orig)+1);
			strcpy(str, orig);
			args[0]= "./comando.out";
			tmp=strtok(str, delim);
			do{
				if (num==max_args){
				return max_args+1;
				}
				args[num]=tmp;
				num++;
				tmp=strtok(NULL, delim);
			}while (tmp!=NULL);
			return num;
			break;
	case 2:
			strcpy(str, orig);
			args[0]= "./comando.out";
			tmp=strtok(str, delim);
			do{
				if (num==max_args)
				return max_args+1;
				args[num]=tmp;
				num++;
				tmp=strtok(NULL, delim);
				band[0]=(strcmp(tmp,"|"));
				band[1]=(strcmp(tmp,">"));
				band[2]=(strcmp(tmp,"<"));
				band[3]=(strcmp(tmp,">>"));
				band[4]=(strcmp(tmp,"<<"));
				if(band[0]==0){
					f=0;
					*IOFlag=1;
				}
				if(band[1]==0){
					f=0;
					*IOFlag=2;
				}
				if(band[2]==0){
					f=0;
					*IOFlag=3;
				}
				if(band[3]==0){
					f=0;
					*IOFlag=4;
				}
				if(band[4]==0){
					f=0;
					*IOFlag=5;
				}
			}while (f);
			return num;
		break;
	case 3:
			strcpy(str, orig);
			args[0]= "./comando.out";
			tmp=strtok(str, delim);
			do{
				band[0]=strcmp(tmp,"|");
				band[1]=strcmp(tmp,">");
				band[2]=strcmp(tmp,"<");
				band[3]=(strcmp(tmp,">>"));
				band[4]=(strcmp(tmp,"<<"));
				if (band[0]==0){
					flag=1;
					*IOFlag=1;
					tmp=strtok(NULL, delim);
				}
				if (band[1]==0){
					flag=1;
					*IOFlag=2;
					tmp=strtok(NULL, delim);
				}
				if (band[2]==0){
					flag=1;
					*IOFlag=3;
					tmp=strtok(NULL, delim);
				}
				if(band[3]==0){
					flag=1;
					*IOFlag=4;
				}
				if(band[4]==0){
					flag=1;
					*IOFlag=5;
				}
				if (flag==1){
					args[num]=tmp;
					num++;
				}

				tmp=strtok(NULL, delim);
			}while  (tmp!=NULL);
			return num;

		break;
	case 4:
			strcpy(str,orig);
			tmp=strtok(str,delim);
			tmp=strtok(NULL,"\0");
			strcpy(cad,tmp);
			return strlen(cad)+1;
	default:
			break;
	}

}
void twoPipesRedirectOutputToFile(char *comando, int *IOFlag){
	int nargsRP,temp=0, flagIO=*IOFlag;
	char *argsRight[temp];
	char *str = malloc(strlen(comando)+1);
	char *tmp;
	char comando2[MAX_CHAR];
	pid_t pidCASE3;
	strcpy(comando2,comando);
	strcpy(str,comando);
	tmp=strtok(str,">");
	nargsRP=extrae_argumentos_M(comando,">",comando2);
	removeChar(comando2,' ');
	pidCASE3=fork();
	if (pidCASE3 == 0){
		int fd = open(comando2, O_RDONLY | O_WRONLY | O_TRUNC);
		dup2(fd, 1);
		twoPipes(str,&flagIO);
		close(fd);
		exit(0);
	}
	else{
		wait(&pidCASE3);
	}
	//kill(pidCASE3, SIGKILL);
	exit(0);

}
void twoPipesConcatredirectOutputToFile(char *comando, int *IOFlag){

	int nargsRP,temp=0, flagIO=*IOFlag;
	char *argsRight[temp];
	char *str = malloc(strlen(comando)+1);
	char *tmp;
	char comando2[MAX_CHAR];
	pid_t pidCASE3;
	strcpy(comando2,comando);
	strcpy(str,comando);
	tmp=strtok(str,">");
	nargsRP=extrae_argumentos_M(comando,">",comando2);
	removeChar(comando2,' ');
	removeChar(comando2,'>');
	pidCASE3=fork();
	if (pidCASE3 == 0){
		int fd = open(comando2,O_RDWR| O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IWOTH);
		dup2(fd, 1);
		twoPipes(str,&flagIO);
		close(fd);
		exit(0);
	}
	else{
		wait(&pidCASE3);
	}
	//kill(pidCASE3, SIGKILL);
	exit(0);

}
void onePipeconcatredirectOutputToFile(char *comando, int *IOFlag){

	int nargsRP,temp=0, flagIO=*IOFlag;
	char *argsRight[temp];
	char *str = malloc(strlen(comando)+1);
	char *tmp;
	char comando2[MAX_CHAR];
	pid_t pidCASE3;
	strcpy(comando2,comando);
	strcpy(str,comando);
	tmp=strtok(str,">");
	nargsRP=extrae_argumentos_M(comando,">",comando2);
	removeChar(comando2,' ');
	removeChar(comando2,'>');
	pidCASE3=fork();
	if (pidCASE3 == 0){
		int fd = open(comando2,O_RDWR| O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IWOTH);
		dup2(fd, 1);
		onePipe(str,&flagIO);
		close(fd);
		exit(0);
	}
	else{
		wait(&pidCASE3);
	}
	//kill(pidCASE3, SIGKILL);
	exit(0);

}
void onePipeRedirectOutputToFile(char *comando, int *IOFlag){

	int nargsRP,temp=0, flagIO=*IOFlag;
	char *argsRight[temp];
	char *str = malloc(strlen(comando)+1);
	char *tmp;
	char comando2[MAX_CHAR];
	pid_t pidCASE3;
	strcpy(comando2,comando);
	strcpy(str,comando);
	tmp=strtok(str,">");
	nargsRP=extrae_argumentos_M(comando,">",comando2);
	removeChar(comando2,' ');
	pidCASE3=fork();
	if (pidCASE3 == 0){
		int fd = open(comando2, O_RDONLY | O_WRONLY | O_TRUNC);
		dup2(fd, 1);
		onePipe(str,&flagIO);
		close(fd);
		exit(0);
	}
	else{
		wait(&pidCASE3);
	}
	//kill(pidCASE3, SIGKILL);
	exit(0);
}
/*
void nComand(char *comando, int *IOFlag, int count, char *tok[]){

	char *argsLeft[0];
	int flagIO=*IOFlag;
	char *str = malloc(strlen(comando)+1);
	int nMArgs;
	int p[2];
	pipe(p);
	for ( int i = 0; i < count; i++)
	{
		int (*p)[2];
		pipe(p[i]);
		extrae_argumentos_leftPipe(comando,tok[i],argsLeft, MAX_ARGS, flagIO);
		nMArgs=extrae_argumentos_M(comando,tok[i],comando);
		
			close(p[0]);
			dup2(p[1],STDOUT_FILENO);
			execvp(&argsLeft[0],argsLeft);

		if(tok[i]=="|"){
			

		}else if (tok[i]==">")
		{
			
		}else if (tok[i]=="<")
		{
			
		}else if (tok[i]==">>")
		{
			
		}else if (tok[i]=="<<")
		{
			
		}
		
		
		
		

	}
	
	

}
*/