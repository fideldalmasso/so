#include <errno.h> //errno
#include <fcntl.h> //open 
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h> //strcpy
#include <unistd.h> //close 
#include <linux/fs.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include "buddyFS.h"

#define EXT2_S_IFMT     0xF000  /* format mask  */
#define EXT2_S_IFSOCK   0xC000  /* socket */
#define EXT2_S_IFLNK    0xA000  /* symbolic link */
#define EXT2_S_IFREG    0x8000  /* regular file */
#define EXT2_S_IFBLK    0x6000  /* block device */
#define EXT2_S_IFDIR    0x4000  /* directory */
#define EXT2_S_IFCHR    0x2000  /* character device */
#define EXT2_S_IFIFO    0x1000  /* fifo */
//----------------------------------------------------

void fechaF (int segundos, char fechaS []) {
    time_t fechaAux = segundos;
    struct tm *fechaAux2 = localtime(&fechaAux);
    strftime(fechaS, 80, "%b %d %Y", fechaAux2);
}

void grupoF (int gid, char grupoS []) {
    struct group* grupoAux = getgrgid(gid);
    strcpy(grupoS, grupoAux->gr_name);
}

void usuarioF(int uid, char usuarioS []) {
    struct passwd* usuarioAux = getpwuid(uid);
    strcpy(usuarioS, usuarioAux->pw_name);
}

void obtenerModo(int modo, char salida[11]) {

    char salida2[11] = "---------- ";
    char salida3[11] = "-rwxrwxrwx ";

    int aux = EXT2_S_IFMT & modo; //aplicamos el mask

    switch (aux) {
    case EXT2_S_IFDIR:
        salida2[0] = 'd';
        break;
    case EXT2_S_IFREG:
        salida2[0] = '-';
        break;
    case EXT2_S_IFLNK:
        salida2[0] = 'l';
        break;
    }

    int i;
    for (i = 0; i < 9; i++) {
        if ((modo >> i) & 1)
            salida2[9 - i] = salida3[9 - i];
    }

    salida2[10] = '\0';
    strcpy(salida, salida2);

}


//----------------------------------------------------


void mostrarError(char texto[]) {
    printf("Error número: %d\n", errno);
    perror(texto);
    exit(1);
}
void imprimir1(char texto1[], char texto2[]) { //para imprimir arreglos de char formateados
    printf("%-40s", texto1);
    printf("%12s\n", texto2);
}
void imprimir2(char texto1[], int * numero) { //para imprimir int formateados
    printf("%-40s", texto1);
    printf("%12d\n", * numero);
}
void leer1(int fd, int offset, int bytes, char buffer[]) {
    if (lseek(fd, offset, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd, buffer, bytes) == -1) mostrarError("read");
    buffer[bytes] = '\0';
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
void imprimirBits (int num) {
    int i, bit;
    for (i = 0; i < 32; i++) {
        printf("%-3d", i);
    }
    printf("\n");
    for (i = 0; i < 32; i++) {
        bit = (num >> i) & 1;
        printf("%-3d", bit);
    }
    printf("\n");
}


int main(int argc, char * argv[]) {

    int tablaInodos; //buffer de int
    int bloquePrimerPunteroDeInodo = 0, entradaDirectorio = 0;
    int inodoNum = 0;
    int fd1 = open("extra/lab_fs", O_RDONLY);
    if (fd1 == -1) mostrarError("error abriendo");

    //grup_desc empieza en el bloque 3, osea, en el byte 2048
    //2048 + 8 = 2056
    leerEImprimir2(fd1, 2056, 4, &tablaInodos, "Bloque de la tabla de i-nodos");
    tablaInodos *= 1024;
    int inodoRaiz = tablaInodos + 128; //para llegar al segundo inodo
    int iblockRaiz = inodoRaiz + 40; //para llegar al iblock del inodo raiz

    leerEImprimir2(fd1, iblockRaiz, 4, &bloquePrimerPunteroDeInodo, "Bloque del primer puntero del inodo raiz");
    entradaDirectorio = bloquePrimerPunteroDeInodo * 1024;

    leer2(fd1, entradaDirectorio, 4, &inodoNum);
    printf("%d-----------------------\n", entradaDirectorio);
    int contador = 0;
    printf("%-8s %-16s %-6s %-8s %-8s %-8s %-16s %-16s\n", "Inodo", "Modo", "Links", "Usr", "Grp", "Tamanio", "Fecha", "Archivos");

    while (contador != 2) {
        int inodo = 0;
        int recLen = 0;
        int modo = 0, links = 0, usr = 0, grupo = 0, tamanio = 0, fecha = 0;
        int nameLen = 0;
        char nombre[255] = {' '};

        leer2(fd1, entradaDirectorio + 4, 2, &recLen);
        inodo = 10240 + ((inodoNum - 1) * 128);
        leer2(fd1, inodo, 2, &modo);
        char modoTexto[11] = "           ";
        obtenerModo(modo, modoTexto);

        leer2(fd1, inodo + 26, 2, &links);
        leer2(fd1, inodo + 4, 4, &tamanio);
        char fs[32];
        char gid[32];
        char nid[32];
        leer2(fd1, inodo + 16, 4, &fecha);
        fechaF(fecha, fs);

        leer2(fd1, inodo + 24, 2, &grupo);
        grupoF(grupo, gid);

        leer2(fd1, inodo + 2, 2, &usr);
        usuarioF(usr, nid);

        leer2(fd1, entradaDirectorio + 6, 1, &nameLen);
        leer1(fd1, entradaDirectorio + 8, nameLen, nombre);

        printf("%-8d %-16s %-6d %-8s %-8s %-8d %-16s %-16s\n", inodoNum, modoTexto, links, nid, gid, tamanio, fs, nombre);

        entradaDirectorio += recLen;
        leer2(fd1, entradaDirectorio, 4, &inodoNum);
        if (inodoNum == 11) contador++;

    }
    close(fd1);

    return 0;

}

/*
Links
https://www.win.tue.nl/~aeb/linux/fs/ext2/ext2.html#I-UID
http://ext2read.sourceforge.net/old/ext2fs.htm
https://github.com/exscape/exscapeOS/blob/master/src/include/kernel/ext2.h
https://stackoverflow.com/questions/10493411/what-is-bit-masking
http://homepage.smc.edu/morgan_david/cs40/analyze-ext2.htm
https://codeforwin.org/2016/01/c-program-to-get-value-of-nth-bit-of-number.html
http://blog.olkie.com/2013/11/05/online-c-function-prototype-header-generator-tool/
*/