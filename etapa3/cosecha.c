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

	//cola 
	key_t keyCola;
	int msqid;
	struct msqid_ds qb={0};
	
	keyCola= ftok(".", '8');
	
	if((msqid = msgget(keyCola, IPC_CREAT | 0666)) < 0) 
		mostrarError("Fallo la creacion");
	
	
	//mensaje
	size_t tamanioMensaje = 10;
	message_buf mensaje;

	int i;
	while(true){
		char msj[10];
		tamanioMensaje = strlen(mensaje.mtext);
		printf("%d\n", tamanioMensaje);
		if(msgrcv(msqid,&mensaje,tamanioMensaje,1,0)<0)
			mostrarError("ERROR RECIBIENDO MSJ");
		
		printf("Nueva cosecha prioritaria (%s)\n", mensaje.mtext);
		printf("Aviso, porcion de parcela libre para nuevo elemento a sembrar....\n", mensaje.mtext);

	}
	return 0;
}

