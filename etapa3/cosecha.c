#include <sys/types.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct msgbuf {
	long mtype;
	char mtext[10];
} message_buf;


void mostrarError(char palabra[]) {
	printf("Error número: %d\n", errno);
	perror(palabra);
	exit(1);
}


int main() {

	//cola
	key_t keyCola;
	int msqid;
	struct msqid_ds qb = {0};

	keyCola = ftok(".", 'C');

	if ((msqid = msgget(keyCola, IPC_CREAT | 0666)) < 0)
		mostrarError("Fallo la creacion");


	//mensaje
	size_t tamanioMensaje = 10;
	message_buf mensajeVacio;
	message_buf mensaje;
	mensajeVacio.mtype = 3;

	int i = 0;
	//Envia 3 mensajes vacios antes de comenzar
	for (i = 0; i < 3; i++) {
		strcpy(mensajeVacio.mtext, "");
		tamanioMensaje = strlen(mensajeVacio.mtext);
		if (msgsnd(msqid, &mensajeVacio, tamanioMensaje, 0) < 0)
			mostrarError("ERROR ENVIANDO MSJ");
		printf("Aviso: parcela lista para sembrar\n");
	}

	while (1) {

		if (msgrcv(msqid, &mensaje, tamanioMensaje, 1, 0) < 0)
			mostrarError("ERROR RECIBIENDO MSJ");

		strcpy(mensajeVacio.mtext, "");
		tamanioMensaje = strlen(mensajeVacio.mtext);
		if (msgsnd(msqid, &mensajeVacio, tamanioMensaje, 0) < 0)
			mostrarError("ERROR ENVIANDO MSJ");

		printf("Nueva cosecha prioritaria (%s)\n", mensaje.mtext);
		printf("Aviso, porcion de parcela libre para nuevo elemento a sembrar....\n");
	}
	return 0;
}

