#include <errno.h> //errno
#include <fcntl.h> //open 
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h> //strcpy
#include <unistd.h> //pipe dup execve close fork
#include <linux/fs.h> //pipe dup execve close fork

//----------------------------------------------------


#define EXT2_S_IFMT 0xF000  /* format mask  */
#define EXT2_S_IFSOCK   0xC000  /* socket */
#define EXT2_S_IFLNK    0xA000  /* symbolic link */
#define EXT2_S_IFREG    0x8000  /* regular file */
#define EXT2_S_IFBLK    0x6000  /* block device */
#define EXT2_S_IFDIR    0x4000  /* directory */
#define EXT2_S_IFCHR    0x2000  /* character device */
#define EXT2_S_IFIFO    0x1000  /* fifo */

// Note that these bits are not mutually exclusive, so simply testing
// e.g. if (i_mode & EXT2_S_IFLNK) doesn't work. That will return true on
// regular files, since EXT2_S_IFLNK == EXT2_S_IFREG | EXT2_S_IFCHR!


/* inode i_mode flags: permission bits */
#define EXT2_S_ISUID    0x0800  /* SUID */
#define EXT2_S_ISGID    0x0400  /* SGID */
#define EXT2_S_ISVTX    0x0200  /* sticky bit */
#define EXT2_S_IRWXU    0x01C0  /* user access rights mask */
#define EXT2_S_IRUSR    0x0100  /* read */
#define EXT2_S_IWUSR    0x0080  /* write */
#define EXT2_S_IXUSR    0x0040  /* execute */
#define EXT2_S_IRWXG    0x0038  /* group access rights mask */
#define EXT2_S_IRGRP    0x0020  /* read */
#define EXT2_S_IWGRP    0x0010  /* write */
#define EXT2_S_IXGRP    0x0008  /* execute */
#define EXT2_S_IRWXO    0x0007  /* others access rights mask */
#define EXT2_S_IROTH    0x0004  /* read */
#define EXT2_S_IWOTH    0x0002  /* write */
#define EXT2_S_IXOTH    0x0001  /* execute */



void obtenerFileType(int i_mode, char *c) {

    int aux = EXT2_S_IFMT & i_mode;

    switch (aux) {
    case EXT2_S_IFDIR:
        *c = 'd';
        break;
    case EXT2_S_IFREG:
        *c = '-';
        break;
    case EXT2_S_IFLNK:
        *c = 'l';
        break;
    }

}

void obtenerPermisosUsuario(int i_mode, char salida[9]) {

    char salida2[9] = {'-'};
    int aux = EXT2_S_IRWXU & i_mode;
    //if((aux >> )

    switch (aux) {
    case EXT2_S_IRUSR:
        salida[0] = 'r';
        break;
    case EXT2_S_IWUSR:
        salida[1] = 'w';
        break;
    case EXT2_S_IXUSR:
        salida[2] = 'x';
        break;
    }


    strcpy(salida,salida2);

}








//----------------------------------------------------




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
void leer3(int fd, int offset, int bytes, u_int16_t *buffer) {
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
    int primerPunteroDeInodo = 0, bloquePrimerPunteroDeInodo = 0, entradaDirectorio = 0;
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
    while (contador != 2) {
        int inodo = 0;
        int recLen = 0;
        int modo = 0, links = 0, usr = 0, grupo = 0, tamanio = 0, fecha = 0, archivos = 0;
        int nameLen = 0;
        char nombre[255] = "";

        imprimir2("Numero inodo", &inodoNum);
        leerEImprimir2(fd1, entradaDirectorio + 4, 2, &recLen, "RecLen:");
        inodo = 10240 + ((inodoNum - 1) * 128);
        leerEImprimir2(fd1, inodo, 2, &modo, "Modo:");
        //--------------tipoArchivo
        char c[1];
        obtenerFileType(modo, &c[0]);
        imprimir1("TipoDeArchivo:", c);
        //--------------permisosUsuario
        int aux = EXT2_S_IRWXU & modo;
        printf("PermisosUsrHex: %x\n", aux);
        printf("PermisosUsrDec: %d\n", aux);


        int aux2 = EXT2_S_IRWXG & modo;
        printf("PermisosGrupoHex: %x\n", aux2);
        printf("PermisosGrupoDec: %d\n", aux2);


        leerEImprimir2(fd1, inodo + 26, 2, &links, "CantLinks:");
        leerEImprimir2(fd1, inodo + 2, 2, &usr, "Usuario:");
        leerEImprimir2(fd1, inodo + 24, 2, &grupo, "Grupo:");
        leerEImprimir2(fd1, inodo + 4, 4, &tamanio, "Tamanio:");
        leerEImprimir2(fd1, inodo + 16, 4, &fecha, "Fecha de ultima modific:");

        leerEImprimir2(fd1, entradaDirectorio + 6, 1, &nameLen, "NameLen:");
        leerEImprimir1(fd1, entradaDirectorio + 8, nameLen, nombre, "Nombre:");

        entradaDirectorio += recLen;
        leer2(fd1, entradaDirectorio, 4, &inodoNum);
        if (inodoNum == 11) contador++;
        printf("-------------------------------------------------\n");

    }
    close(fd1);

    return 0;

}