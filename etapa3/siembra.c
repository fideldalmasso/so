#include <sys/types.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

typedef struct msgbuf{
	long mtype;
	char mtext[10];
} message_buf;


void mostrarError(char palabra[]) {
	printf("Error número: %d\n", errno);
	perror(palabra);
	exit(1);
}


int main(){
	
	char productos [4] = {'S','M','G','L'};
	int prod;
	srand(time(NULL));
	//cola 
	key_t keyCola;
	int msqid;
	struct msqid_ds qb={0};
	
	keyCola= ftok(".", '8');
	

	
	if((msqid = msgget(keyCola, IPC_CREAT | 0666)) < 0) 
		mostrarError("FALLO LA CREACION");
		
	if(msgctl(msqid,IPC_STAT,&qb) < 0)
		mostrarError("FALLO LA COPIA");
	qb.msg_qbytes=33;
	msgctl(msqid,IPC_SET,&qb);
	
	//mensaje
	size_t tamanioMensaje = 10;
	message_buf mensaje;

	int i;
	for(i=0; i<15; i++){
		prod = rand()%4;
		char msj[10]= 
		strcpy(mensaje.mtext,msj);
		tamanioMensaje = strlen(mensaje.mtext);
		
		//SWITCH
		
		if(prod==0){
		mensaje.mtype = 1;
		for(i=0; i<3; i++){
			if(msgsnd(msqid,&mensaje,tamanioMensaje,0)<0)
			mostrarError("ERROR ENVIANDO MSJ");
		}
		else mensaje.mtype=2;
		char msj[10]={'s','s','s','s','s','s','s','s','s','s'};
		//strcpy(mensaje.mtext,msj);
		//tamanioMensaje = strlen(mensaje.mtext);
		
		if(msgsnd(msqid,&mensaje,tamanioMensaje,0)<0)
			mostrarError("ERROR ENVIANDO MSJ");
		
		printf("Se enviara a sembrar: %c\n", productos[prod]);

	}
	msgctl(msqid, IPC_RMID, msqid_ds )
	return 0;
}

