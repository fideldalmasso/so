#include<stdio.h>
#include<unistd.h>

int main (int argc, char *argv[]) {
	int i;
	printf("voy a gritar!!!!: %d\n", getpid());
	for(i=1; i<4; i++){
		fork();
		printf("holaaaaa....)))))))) %d reflejo: %d sonido: %d \n", i, getpid(), getppid());	
	}
	
	return 0;
}
