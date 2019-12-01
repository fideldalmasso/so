#include <errno.h> //errno
#include <fcntl.h> //open 
#include <getopt.h>
//#include <stdbool.h> //bool
#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h> //strcpy
//#include <sys/stat.h> //chmod
#include <unistd.h> //pipe dup execve close fork


void imprimir1(char texto1[], char texto2[]) { //para imprimir texto
    printf("%-40s", texto1);
    printf("%10s\n", texto2);
}
void imprimir2(char texto1[], int texto2[]) { //para imprimir int
    printf("%-40s", texto1);
    printf("%10d\n", texto2[0]);
}

void mostrarError(char texto[]) {
    printf("Error número: %d\n", errno);
    perror(texto);
    exit(1);
}
int main(int argc, char * argv[]) {

    char buffer1[2]; //buffer de texto
    int bufferNum;
    int *buffer2 = &bufferNum; //buffer de int

    int fd1 = open("extra/lab_fs", O_RDONLY);
    if (fd1 == -1) mostrarError("error abriendo");

    //1024 + 120 = 1144
    if (lseek(fd1, 1144, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer1, 2) == -1) mostrarError("read");
    imprimir1("Nombre Del Volumen:",buffer1);

    if (lseek(fd1, 1024, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer2, 4) == -1) mostrarError("read");
    imprimir2("Cantidad de i-nodos:",buffer2);

    //1024 + 16 = 1040
    if (lseek(fd1, 1040, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer2, 4) == -1) mostrarError("read");
    imprimir2("Cantidad de i-nodos libres:",buffer2);

    if (lseek(fd1, 1108, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer2, 4) == -1) mostrarError("read");
    imprimir2("Primer i-nodo no reservado:",buffer2);



    return 0;

}


