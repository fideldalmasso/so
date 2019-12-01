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

    //1024 + 120 = 1144
    if (lseek(fd1, 1144, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer1, 16) == -1) mostrarError("read");
    imprimir1("Nombre del volumen:",buffer1);

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


    if (lseek(fd1, 1112, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer2, 2) == -1) mostrarError("read");
    imprimir2("Tamanio estructura de un i-nodo:",buffer2);

    if (lseek(fd1, 1044, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer2, 4) == -1) mostrarError("read");
    imprimir2("Primer bloque de datos:",buffer2);

    if (lseek(fd1, 1028, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer2, 4) == -1) mostrarError("read");
    imprimir2("Cantidad de bloques:",buffer2);

    if (lseek(fd1, 1036, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer2, 4) == -1) mostrarError("read");
    imprimir2("Cantidad de bloques libres:",buffer2);

    int bufferNum3, bufferNum4;
    int * buffer3= &bufferNum3;
    int * buffer4= &bufferNum4;

    if (lseek(fd1, 1048, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer3, 4) == -1) mostrarError("read");
    if (lseek(fd1, 1028, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, buffer4, 4) == -1) mostrarError("read");

    int aux = (bufferNum3 + 1024) * bufferNum4;
    imprimir2("Tamanio total en disco:",&aux);
    return 0;

}


