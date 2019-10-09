#include <stdio.h>
#include <time.h>
#include <sys/types.h>

main (int argc, char *argv[]){

int parametro = atoi(argv[1]);
int tiempo, i;
srand (time (NULL));

printf("INICIO MAQUINA A: %d\n", getpid());
printf("MAQUINA B %d\n", getpid());

if (fork()==0){
	for(i=0; i<7; i++){
	tiempo= rand()%6;
	sleep(tiempo);
	printf("Trabajo realizado %d por MAQ B - time %d\n",i,tiempo);
	}
}
else {
	if(parametro != 1){	
		for(i=0; i<7; i++){
		tiempo= rand()%6;
		sleep(tiempo);
		printf("Trabajo realizado %d por MAQ A - time %d\n",i, tiempo);

		}
	}
	else {
		printf("MAQUINA A aguardando termine MAQUINA B.\n");		
		wait(NULL);
		printf("MAQUINA A recibe aviso MAQUINA B ha concluido - Inicia a operar MAQUINA A\n");
		for(i=0; i<7; i++){
		tiempo= rand()%6;
		sleep(tiempo);
		printf("Trabajo realizado %d por MAQ A - time %d\n",i,tiempo);
		}
	}
}

	return 0;
}
