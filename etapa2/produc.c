#include <stdio.h>
#include <stdlib.h> //atoi rand srand
#include <time.h> //time
//~ #include <sys/types.h>
#include <sys/wait.h> //wait
#include <sys/unistd.h> //fork getpid getppid sleep


int main (int argc, char *argv[]) {

	int parametro = atoi(argv[1]);
	int tiempo, i;
	pid_t pid;
	srand (time (NULL));



	if ((pid = fork()) == 0) {
		printf("INICIO MAQUINA A: %d\n", getppid());
		printf("MAQUINA B %d\n", getpid());


		for (i = 0; i < 7; i++) {
			tiempo = rand() % 6;
			sleep(tiempo);
			printf("Trabajo realizado %d por MAQ B - time %d\n", i, tiempo);
		}
	}
	else {

		if (parametro != 1) {
			for (i = 0; i < 7; i++) {
				tiempo = rand() % 6;
				sleep(tiempo);
				printf("Trabajo realizado %d por MAQ A - time %d\n", i, tiempo);

			}
		}
		else {
			printf("MAQUINA A aguardando termine MAQUINA B.\n");
			wait(NULL);
			printf("MAQUINA A recibe aviso MAQUINA B ha concluido - Inicia a operar MAQUINA A\n");
			for (i = 0; i < 7; i++) {
				tiempo = rand() % 6;
				sleep(tiempo);
				printf("Trabajo realizado %d por MAQ A - time %d\n", i, tiempo);
			}
		}
	}

	return 0;
}
