#include <errno.h> //errno
#include <fcntl.h> //open 
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h> //strcpy
#include <unistd.h> //pipe dup execve close fork


void imprimir1(char texto1[], char texto2[]) { //para imprimir texto
    printf("%-40s", texto1);
    printf("%10s\n", texto2);
}
void imprimir2(char texto1[], int texto2[]) { //para imprimir int
    printf("%-40s", texto1);
    printf("%12d\n", texto2[0]);
}

void mostrarError(char texto[]) {
    printf("Error número: %d\n", errno);
    perror(texto);
    exit(1);
}
int main(int argc, char * argv[]) {

    char buffer1[16]; //buffer de texto
    int bufferNum;
    int *buffer2 = &bufferNum; //buffer de int

    int fd1 = open("extra/imagen1.flp", O_RDONLY);
    if (fd1 == -1) mostrarError("error abriendo");

    //grup_desc empieza en el bloque 3, osea, en el byte 2048
    //2048 + 8 = 2056
    if (lseek(fd1, 2056, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer2, 4) == -1) mostrarError("read");
    imprimir2("Direccion de la tabla de inodos: ",buffer2);

    if (lseek(fd1, 2056, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer2, 4) == -1) mostrarError("read");
    imprimir2("Direccion de la tabla de inodos: ",buffer2);

    return 0;

}


