#include <errno.h> //errno
#include <fcntl.h> //open 
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h> //strcpy
#include <unistd.h> //pipe dup execve close fork


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
    int tablaInodos; //buffer de int
    int primerPunteroDeInodo, bloquePrimerPunteroDeInodo, entradaDirectorio;
    int inodo, inodoNum;
    int fd1 = open("extra/lab_fs", O_RDONLY);
    if (fd1 == -1) mostrarError("error abriendo");

    //grup_desc empieza en el bloque 3, osea, en el byte 2048
    //2048 + 8 = 2056
    leerEImprimir2(fd1,2056,4,&tablaInodos,"Bloque de la tabla de i-nodos");
    tablaInodos*=1024;
    int inodoRaiz = tablaInodos + 128; //para llegar al segundo inodo
    int iblockRaiz = inodoRaiz + 40; //para llegar al iblock del inodo raiz
    
    leerEImprimir2(fd1,iblockRaiz,4,&bloquePrimerPunteroDeInodo,"Bloque del primer puntero del inodo raiz");
    entradaDirectorio=bloquePrimerPunteroDeInodo*1024;



    int recLen=0;
    
    do{
    leerEImprimir2(fd1,entradaDirectorio,4,&inodoNum,"Numero Inodo:");
    leerEImprimir2(fd1,entradaDirectorio+4,2,&recLen,"RecLen:");
    inodo = 10240 + ((inodoNum-1) * 128);
    int modo, links, usr, grupo, tamanio, fecha, archivos;
    leerEImprimir2(fd1,inodo,2,&modo,"Modo:");
    leerEImprimir2(fd1,inodo+26,2,&links,"CantLinks:");
    leerEImprimir2(fd1,inodo+2,2,&usr,"Usuario:");
    leerEImprimir2(fd1,inodo+24,2,&grupo,"Grupo:");
    leerEImprimir2(fd1,inodo+4,4,&tamanio,"Tamanio:");
    leerEImprimir2(fd1,inodo+16,4,&fecha,"Fecha de ultima modific:");

    int nameLen;
    char nombre[255];
    leerEImprimir2(fd1,entradaDirectorio+6,1,&nameLen,"NameLen:");
    leerEImprimir1(fd1,entradaDirectorio+8,nameLen,nombre,"Nombre:");

    entradaDirectorio += recLen;
    //leer2(fd1,entradaDirectorio,4,&inodoNum);
    printf("-------------------------------------------------\n");
}while(inodoNum!=0);

    return 0;

}