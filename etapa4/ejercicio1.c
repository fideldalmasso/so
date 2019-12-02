#include <errno.h> //errno
#include <fcntl.h> //open 
#include <getopt.h>
//#include <stdbool.h> //bool
#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h> //strcpy
//#include <sys/stat.h> //chmod
#include <unistd.h> //pipe dup execve close fork
#include <sys/vfs.h> //statfs

void mostrarError(char texto[]) {
    printf("Error número: %d\n", errno);
    perror(texto);
    exit(1);
}
void imprimir1(char texto1[], char texto2[]) { //para imprimir arreglos de char formateados
    printf("%-40s", texto1);
    printf("%10s\n", texto2);
}
void imprimir2(char texto1[], int * numero) { //para imprimir int formateados
    printf("%-40s", texto1);
    printf("%12d\n", * numero);
}
void leer1(int fd, int offset, int bytes, char buffer[]) {
    if (lseek(fd, offset, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd, buffer, bytes) == -1) mostrarError("read");
}
void leer2(int fd, int offset, int bytes, int *buffer) {
    if (lseek(fd, offset, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd, buffer, bytes) == -1) mostrarError("read");
}
void leerEImprimir1(int fd, int offset, int bytes, char buffer[], char texto[]) {
    leer1(fd, offset, bytes, buffer);
    imprimir1(texto, buffer);
}
void leerEImprimir2(int fd, int offset, int bytes, int *buffer, char texto[]) {
    leer2(fd, offset, bytes, buffer);
    imprimir2(texto, buffer);
}

int main(int argc, char * argv[]) {

    char buffer1[16]; //buffer de texto
    int fd1, buffer2, buffer3, buffer4;
    struct statfs bufStat;

    if ((fd1 = open("extra/imagen1.flp", O_RDONLY) ) == -1) mostrarError("open");


    if (fstatfs(fd1, &bufStat) < 0 ) mostrarError("fstatfs");
    if (bufStat.f_type != 0xef53) {
        printf("%s\n", "No hay sistema de archivos tipo EXT2");
        exit(1);
    }
    
    imprimir1("Sistema de archivos tipo:", "EXT2");
    leerEImprimir1(fd1, 1144, 16, buffer1, "Nombre del volumen:");
    leerEImprimir2(fd1, 1024, 4, &buffer2, "Cantidad de i-nodos:");
    leerEImprimir2(fd1, 1040, 4, &buffer2, "Cantidad de i-nodos libres:");
    leerEImprimir2(fd1, 1108, 4, &buffer2, "Primer i-nodo no reservado:");
    leerEImprimir2(fd1, 1112, 2, &buffer2, "Tamanio estructura de un i-nodo:");
    leer2(fd1, 1048, 4, &buffer4);
    buffer4 += 1024;
    imprimir2("Tamanio de bloque:", &buffer4);
    leerEImprimir2(fd1, 1044, 4, &buffer2, "Primer bloque de datos:");
    leerEImprimir2(fd1, 1028, 4, &buffer3, "Cantidad de bloques:");
    leerEImprimir2(fd1, 1036, 4, &buffer2, "Cantidad de bloques libres:");

    int aux = buffer4 * buffer3;
    imprimir2("Tamanio total en disco:", &aux);
    return 0;

}


