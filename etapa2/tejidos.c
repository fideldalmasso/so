#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]){

printf("Procesamiento de tejido: %d\n",getpid());


execve("celulas", argv, envp);

return 0;
}