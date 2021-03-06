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

#define MAX_ARGS 20
#define MAX_CHAR 1000

//extrae general, el primero es la cadena, el segundo el delimitador
// el tercero guardara las las palabras extaidas de la cadena
//el cuarto un tope de maximos argumentos, el quinto la flag estado actual
// y el sexto el modo, el septimo guarda una cadena de ser que solo divida cadena.
// ya sea por izquierda, derecha, general o de extraer nueva cadena.
// general = 1, izquierda = 2, derecha = 3, nueva cad = 4;

int extrae_argumentos_general(char* , char* , char*[] ,int ,int* ,int,char * );
int extrae_argumentos_d(char *, char *, char *[], int);
int extrae_argumentos_leftPipe(char *, char *, char *[], int, int*);
int extrae_argumentos_rightPipe(char *, char *, char *[], int, int*);
int extrae_argumentos_M(char *, char *, char *);


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

int main(int argc,  char *argv[]){

	struct utsname unameD;
	int pid,exitt,temp=0, nargs, tuberia[2],nargsLP,priority=0, flagIO;
	char *args[temp], comando[MAX_CHAR], nombre[20], dir[1024], *argsLeft[temp];
	
	
	
	uname(&unameD);
	getcwd(dir,sizeof(dir));
	strcpy(nombre,nameUser());
	do{
		if(pipe(tuberia)== -1){
                	perror("\n [-] ERROR PIPE \n");
               		exit(-1);
        	}
		pid_t pid=fork();
		if( pid == -1){
			perror("\n [-] ERROR FORK\n");
		}
		else if(pid==0){
			close(tuberia[1]);
			read(tuberia[0],comando, sizeof(comando));
			nargs=0;
			nargs = extrae_argumentos_d(comando, " ", args, MAX_ARGS);
			//nargs=extrae_argumentos_general(comando, " ", args, MAX_ARGS, )
			priority=verifyCad(&args[0], nargs);
			//printf("\nPriority: %i\n", priority); //See priotity to check if works fine
				switch (priority)
				{
				case 0:
					oneARG(comando);
					break;
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
						fflush(stdout);
						fflush(stdin);
						nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
						//printf("comando %s", comando);
						twoPipes(comando,&flagIO);
					break;
				default:
					break;
				}
		}else{
			close(tuberia[0]);
			printf("\n%s@%s:~%s$ ", nombre,unameD.nodename, dir);
			leerLinea(&comando[0]);
			write(tuberia[1], comando, strlen(comando)+1);
			wait(NULL);
		}
	}while( strcmp(comando,"exit") != 0);
	return 0;
}
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

int extrae_argumentos_d(char *orig, char *delim, char *args[], int max_args){

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
	}while (tmp!=NULL);
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
	fflush(stdin);
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

	int nargs=0, priority=0, temp=0;
	char *args[0];
	int flag;
	nargs = extrae_argumentos_d(comando, " ", args, MAX_ARGS);
	priority=verifyCad(&args[0], nargs);
	sleep(1);
	args[nargs]=(char *)0;
	flag = execvp(args[0], args);
	if(flag == -1){
		printf("\n [-] There is not found that command or does not exist\n");
		exit(0);
	}
	else{limpiaCad(&args[0],nargs);	

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
	int p1,p2;
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
	wait(&status1);
	wait(&status1);

}
void redirectOutputToFile(char *comando,int *IOFlag){

	int flagIO=*IOFlag, temp=0,fd,pid3,nargsLP=0,nargsDP=0;
	char comando2[MAX_CHAR],comando3[MAX_CHAR], comando4[MAX_CHAR],*argsLeft[temp];

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
		wait(NULL);
	}
	strcpy(comando3," ");strcpy(comando2," ");strcpy(comando4," ");

}
void redirectInputOfFile(char *comando,int *IOFlag){
	int flagIO=*IOFlag, temp=0,fd,pid3;
	char comando2[MAX_CHAR],comando3[MAX_CHAR], comando4[MAX_CHAR],*argsLeft[temp];
	int nargsLP=0,nargsDP=0;
	strcpy(comando2,comando);
	strcpy(comando3,comando2);		
	printf("comando : %s ", comando2 );
	nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
	argsLeft[nargsLP]= (char *)0;
	nargsDP= extrae_argumentos_M(comando2,"<",comando3);
	removeChar(comando3,' ');		
	pid3=fork();
	printf("Archivo a re escribir:%s, flag: %i", comando3, flagIO);	
	if (pid3 == 0){       
		int fd = open(comando3,O_RDWR , S_IRUSR);
		close(0);
		dup2(fd, 0);
		close(fd);
		execvp(argsLeft[0], argsLeft);				
		exit(0);
	}
	else{
		wait(NULL);
	}
	strcpy(comando3," ");
	strcpy(comando2," ");
	strcpy(comando4," ");
}
void concatredirectOutputToFile(char *comando,int *IOFlag){
	
	int flagIO=*IOFlag, temp=0,fd,pid3;
	char comando2[MAX_CHAR],comando3[MAX_CHAR], comando4[MAX_CHAR],*argsLeft[temp];
	int nargsLP=0,nargsDP=0;
	strcpy(comando2,comando);
	strcpy(comando3,comando2);
	nargsLP= extrae_argumentos_leftPipe(comando, " ", argsLeft, MAX_ARGS, &flagIO);
	argsLeft[nargsLP]= (char *)0;
	nargsDP= extrae_argumentos_M(comando2,">",comando3);				
	removeChar(comando3,' ');
	removeChar(comando3,'>');
	pid3=fork();
	//printf("\nArchivo a re escribir:%s, flag: %i\n", comando3, flagIO);	
	if (pid3 == 0){       
		int fd = open(comando3,O_RDWR| O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IWOTH);
		dup2(fd, 1);  
		execvp(argsLeft[0], argsLeft);
		close(fd);
		exit(0);
	}
	else{
		wait(NULL);
	}
	strcpy(comando," ");
	strcpy(comando3," ");
	strcpy(comando2," ");
	strcpy(comando4," ");
}
void concatredirectInputOfFile(char *comando,int *IOFlag){
	int flagIO=*IOFlag, temp=0,fd,pid3;
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
	pid3=fork();
	printf("Archivo a re escribir:%s, flag: %i", comando3, flagIO);	
	if (pid3 == 0){       
		int fd = open(comando3,O_RDWR , S_IRUSR);
		close(0);
		dup2(fd, 0);
		close(fd);
		execvp(argsLeft[0], argsLeft);				
		exit(0);
	}
	else{
		wait(NULL);
	}
	strcpy(comando3," ");
	strcpy(comando2," ");
	strcpy(comando4," ");
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
	wait(&status2);
	wait(&status2);
	wait(&status2);	
}/*
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
				if (num==max_args)
				return max_args+1;
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
		break;
	default:
		break;
	}





}
*/

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
