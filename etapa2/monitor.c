#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int sigT=0;
int sig1=0;
int sig2=0;

void manejador1 (int sig);
void manejador2 (int sig);

int main(){
	
	signal(SIGUSR1, &manejador1);
	signal(SIGUSR2, &manejador2);
	while(sigT != 15) pause();
	
	
	return 0;
}

void manejador1 (int sig){
	sigT++;
	sig1++;
	printf("Acumulado de archivos regulares menores a 1 K: %d\n", sig1);
}
void manejador2 (int sig){
	sigT++;
	sig2++;
	printf("Acumulado de archivos regulares mayores a 1 K: %d\n", sig2);
}
