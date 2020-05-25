#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>



void limpiarCadena(char *);


int main(){
        int pid;
        int tuberiaMensaje[2];
        int tuberiaReceptor[2];
        char mensaje[250];
        char respuesta[250];
        int flagSalida;
        //CREACION DE TUBERIA
        if(pipe(tuberiaMensaje)== -1 || pipe(tuberiaReceptor)== -1)
        {
                perror("\n Error TUBERIA \n");
                exit(-1);
        }


        pid=fork();
        if(pid==-1)
        {
                perror("\nError  en fork\n");
                exit(-1);
        }
        else if(pid==0) {
                flagSalida=1;
                while(flagSalida){
                        read(tuberiaMensaje[0], mensaje,sizeof(mensaje));
                        printf("\n\tHIJO: Valor leido de mi padre: %s\n", mensaje);
                        strcpy(respuesta,"HIJO: mensjae:   ");
                        limpiarCadena(respuesta);
                        write(tuberiaReceptor[1],respuesta,strlen(respuesta)+1);
                        limpiarCadena(respuesta);
                        flagSalida=strcmp(mensaje, "EXIT\n");

                }
                close(tuberiaMensaje[0]);
		close(tuberiaMensaje[1]);
                close(tuberiaReceptor[0]);
		close(tuberiaReceptor[1]);
                exit(0);

        }else{
                flagSalida=1;
                while(flagSalida){
                        printf("\n\tPADRE: Tecle un valor, si mandas EXIT saldra:  " );
                        fgets(mensaje, sizeof(mensaje),stdin);
                        write(tuberiaMensaje[1], mensaje,strlen(mensaje)+1);
                        read(tuberiaReceptor[0],respuesta,sizeof(respuesta));
                        limpiarCadena(respuesta);
                        printf("\n\tPadre: mensaje del hijo:  %s\n", respuesta);
                        flagSalida=strcmp(mensaje,"EXIT\n");
                sleep(1);
                }
                wait(NULL);
                close(tuberiaMensaje[0]);
		close(tuberiaMensaje[1]);
                close(tuberiaReceptor[0]);
		close(tuberiaReceptor[1]);
           
        }
return 0;
}
void limpiarCadena(char *cad) {
        char *let = strchr(cad, '\n');
        if (let) {
                *let = '\0';
        }
}


