#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>

typedef struct msgbuf{
	long mtype;
	char mtext[10];
};

void enviar_mensaje(char prod);

int main(){
	//int cSoja=0, cOtro=0; 
	key_t keyCola;
	int msqid;
	int msgflg = IPC_CREAT | 0666;
	char productos [4] = {'S','M','G','L'};
	char prod;
	srand(time(NULL));
	msgbuf sbuf;
	size_t buf_lenght;

	keyCola=260899	

	if((msqid = msgget(keyCola, msgflg) < 0){
		perror("msgget");
		exit(1);
	}

	/*
	int i=0;
	while(i<15){
		prod=productos[rand()%4];
		if(prod=='S' || (cOtro==2 && cSoja<3)){
			for(int i=0; i<3; i++){
				enviar_mensaje(prod);
			}
			cSoja++;
			i+=3;
		}
		else{
			enviar_mensaje(prod);
			cOtro++;
			i++;
		}
	 	
	}
	*/

	return 0;
}


void enviar_mensaje(char prod){			
	sbuf.mtype = 1;
	strcpy(s.buf.mtext, /*Falta mensaje*/);
	buf_lenght = strlen(sbuf.mtext) + 1;
	printf("Se enviara a sembrar: %c\n", prod);
}
