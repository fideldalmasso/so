//~ #include <sys/types.h>
#include <stdlib.h> //exit
#include <unistd.h> //getpid execve
#include <stdio.h>
//~ #include <string.h>

int main(int argc, char *argv[], char *envp[]) {

	if (argc != 2) {
		printf("El programa acepta solo un parámetro\n");
		exit(1);
	}

	printf("Procesamiento de tejido: %d\n", getpid());

	execve("celulas", argv, envp);

	return 0;
}
