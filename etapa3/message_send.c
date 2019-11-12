#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>


#define MSGSZ     128

/*
 * Estructura del mensaje.
 */

typedef struct msgbuf 
{
	long    mtype;
	char    mtext[MSGSZ];
} message_buf;

int main()
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;

   
    /*
     * Obtenemos el id de la cola de mensajes de “nombre”
     * 1234, la cual es creada por el sistema.
     */

    key = ftok(".", 'P');

    if ((msqid = msgget(key, msgflg)) < 0)
    {
        perror("msgget");
        exit(1);
    }
    sbuf.mtype = 1;
        
    strcpy(sbuf.mtext, "¿Te llega este mensaje?");
        
    buf_length = strlen(sbuf.mtext) + 1 ;
    
    /*
     * Enviamos el mensaje
     */

    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) 
    { 
        perror("msgsnd");
        exit(1);
    }

    printf("Mensaje: \"%s\" enviado!\n", sbuf.mtext);
      
    return 0;
}
