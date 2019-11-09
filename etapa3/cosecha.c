#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct msgbuf {
  long mtype;
  char mtext[10];
};


int main(){
  int msqid;
  key_t keyCola;
  msgbuf rbuf;
  key=260899;

  if((msqid = msgget(keyCola, 0666)) < 0){
    perror("msgget");
    exit(1);
  }
  
  for(int i=0; i<3; i++){
    printf("Aviso: parcela lista para sembrar...\n");
  }
  
  for(int i=0; i<15, i++){
    
    if(msgrcv(msqid, &rbuf, 10, 1, 0 ) < 0){
      perror("msgrcv");
      exit(1);
    }
    printf("Nueva cosecha prioritaria %s\n", rbuf.mtext);
    printf("Aviso, porcion de parcela libre para nuevo elemento a       sembrar")
  }


return 0;
}
