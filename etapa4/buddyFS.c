#include <fcntl.h> //open 
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h> //strcpy
#include <unistd.h> //pipe dup execve close fork
#include <sys/vfs.h> //statfs
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <linux/magic.h>
#include <linux/fs.h>
#include <linux/ext2_fs.h>
// si linux/ext2_fs.h no existe, instalar e2fslibs-dev 
// y utilizar la libreria siguiente
//#include <ext2fs/ext2_fs.h> 
#include <errno.h>

#define EXT2_S_IFMT     0xF000  /* format mask  */
#define EXT2_S_IFLNK    0xA000  /* symbolic link */
#define EXT2_S_IFREG    0x8000  /* regular file */
#define EXT2_S_IFDIR    0x4000  /* directory */


#define printf1(...) printf("%-40s%12s\n", __VA_ARGS__)
#define printf2(...) printf("%-40s%12d\n", __VA_ARGS__)
#define printf4(x) printf("%s\n",x)
#define printf5(x) printf("%d\n",x)


void fechaF(int segundos, char fechaS []);
void grupoF(int gid, char grupoS []);
void imprimir1(char texto1[], char texto2[]);
void imprimir2(char texto1[], int * numero);
void imprimirBits (int num);
void leer1(int fd, int offset, int bytes, char buffer[]);
void leer2(int fd, int offset, int bytes, int *buffer);
void leerEImprimir1(int fd, int offset, int bytes, char buffer[], char texto[]);
void leerEImprimir2(int fd, int offset, int bytes, int *buffer, char texto[]);
void mostrarError(char texto[]);
void obtenerModo(int modo, char salida[11]);
void usuarioF(int uid, char usuarioS []);



void mostrarError(char texto[]) {
    printf("Error numero: %d\n", errno);
    perror(texto);
    exit(1);
}
void imprimir1(char texto1[], char texto2[]) {
    printf("%-40s", texto1);
    printf("%12s\n", texto2);
}
void imprimir2(char texto1[], int * numero) {
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


//ejercicio1------------------------------------------------------------------------

void ejercicio1(struct ext2_super_block sb) {


    printf("-----------------INFORMACION DEL SB-----------------\n");

    printf("%-40s%12s\n", "Sistema de archivos tipo:", "EXT2");
    printf("%-40s%12s\n", "Nombre del volumen:", sb.s_volume_name);
    printf("%-40s%12d\n", "Cantidad de i-nodos:", sb.s_inodes_count);
    printf("%-40s%12d\n", "Cantidad de i-nodos libres:", sb.s_free_inodes_count);
    printf("%-40s%12d\n", "Primer i-nodo no reservado:", sb.s_first_ino);
    printf("%-40s%12d\n", "Tamanio estructura de un i-nodo:", sb.s_inode_size);
    printf("%-40s%12d\n", "Tamanio de bloque:", sb.s_log_block_size + 1024);
    printf("%-40s%12d\n", "Primer bloque de datos:", sb.s_first_data_block);
    printf("%-40s%12d\n", "Cantidad de bloques:", sb.s_blocks_count);
    printf("%-40s%12d\n", "Cantidad de bloques libres:", sb.s_free_blocks_count);
    printf("%-40s%12d\n", "Tamanio total en disco:", sb.s_blocks_count * (sb.s_log_block_size + 1024));
    printf("----------------------------------------------------\n");

}

//ejercicio2------------------------------------------------------------------------
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

    int aux = EXT2_S_IFMT & modo; //aplicar mask

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
    for (i = 0; i < 9; i++) { //los permisos estan ubicados en los primeros 9 bits
        if ((modo >> i) & 1)
            salida2[9 - i] = salida3[9 - i];
    }

    //salida2[10] = '\0';
    strcpy(salida, salida2);

}

void ejercicio2(int fd1, struct ext2_super_block sb, struct ext2_group_desc gd, struct ext2_inode ti[], struct ext2_dir_entry tde[], int cantEntradasDirectorios) {

    int tamBloque = sb.s_log_block_size + 1024;

    

    __le32 idBloqueDeEntradasRaiz = ti[1].i_block[0];
    int entradaDirectorio = idBloqueDeEntradasRaiz * tamBloque;

    printf("%-8s %-16s %-6s %-8s %-8s %-8s %-16s %-16s\n", "Inodo", "Modo", "Links", "Usr", "Grp", "Tamanio", "Fecha", "Archivos");
    int i = 0;
    for(i=0;i<cantEntradasDirectorios;i++) {
        char usuarioTexto[16];
        char grupoTexto[16];
        char fechaTexto[24];
        char modoTexto[11];
        memset(usuarioTexto,' ',16);
        memset(grupoTexto,' ',16);
        memset(fechaTexto,' ',24);
        memset(modoTexto,' ',11);
        
        obtenerModo(ti[tde[i].inode - 1].i_mode, modoTexto);
        fechaF(ti[tde[i].inode - 1].i_mtime, fechaTexto);
        grupoF(ti[tde[i].inode - 1].i_gid, grupoTexto);
        usuarioF(ti[tde[i].inode - 1].i_uid, usuarioTexto);

        printf("%-8d %-16s %-6d %-8s %-8s %-8d %-16s %-16s\n", tde[i].inode, modoTexto, ti[tde[i].inode - 1].i_links_count, usuarioTexto, grupoTexto, ti[tde[i].inode - 1].i_size, fechaTexto, tde[i].name);

    }

}

//------------------------------------------------------------------------


int main(int argc, char * argv[]) {
    if (argc < 3) {
        printf("Modo de uso\nbuddyFS [-s][-l][-b tamanioNube] /rutaimagen\n");
        exit(0);
    };

    int fd1;
    if ((fd1 = open(argv[2], O_RDONLY) ) == -1) mostrarError("open");

    struct ext2_super_block * sb = malloc(1024);

    if (lseek(fd1, 1024, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, sb, 1024) == -1) mostrarError("read");

    if (sb->s_magic != EXT2_SUPER_MAGIC) {
        printf("%s\n", "No hay sistema de archivos tipo EXT2");
        exit(1);
    }
    struct ext2_group_desc * gd = malloc(1024);
    struct ext2_inode * i = (struct ext2_inode*)malloc(128);

    if (lseek(fd1, 1024 * 2, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, gd, 1024) == -1) mostrarError("read");

    __le32  cantInodos = (sb->s_inodes_count) - (sb->s_free_inodes_count);

    struct ext2_inode ti[184];             //tabla de inodos

    int posTablaInodos = (gd->bg_inode_table * 1024);
    int p;
    for (p = 0; p < cantInodos; p++) {
        if (lseek(fd1, posTablaInodos + (p * 128), SEEK_SET) == -1) mostrarError("lseek");
        if (read(fd1, i, 128) == -1) mostrarError("read");
        ti[p] = *i;
    }

    int c;

    int tamBloque = sb->s_log_block_size + 1024;
    __le32 idBloqueDeEntradasRaiz = ti[1].i_block[0];
    int entradaDirectorio = idBloqueDeEntradasRaiz * tamBloque;
    struct ext2_dir_entry * de = malloc(sizeof(struct ext2_dir_entry));
    struct ext2_dir_entry tde[200];
    int n =0;
	while (1) {
    	if (lseek(fd1, entradaDirectorio, SEEK_SET) == -1) mostrarError("lseek");
        if (read(fd1, de, sizeof(struct ext2_dir_entry)) == -1) mostrarError("read");
        tde[n]=*de;

        entradaDirectorio += de->rec_len;
        if ((entradaDirectorio % tamBloque) == 0) break;
        n++;
    }
    int cantEntradasDirectorios = n++;

    if ((c = getopt (argc, argv, "slb")) < 0) mostrarError("getopt");

    switch (c) {
    case 's':
        ejercicio1(*sb);
        break;
    case 'l':
        ejercicio2(fd1, *sb, *gd, ti, tde, cantEntradasDirectorios);
        break;
    case 'b':
    	//ejercicio3(ti, tde, cantEntradasDirectorios);
        break;
    }

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
https://stackoverflow.com/questions/228684/how-to-declare-a-structure-in-a-header-that-is-to-be-used-by-multiple-files-in-c
http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/2-C-adv-data/dyn-array.html
https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/
*/
