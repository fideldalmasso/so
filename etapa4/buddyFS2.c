#include <errno.h> //errno
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

#define EXT2_S_IFMT     0xF000  /* format mask  */
#define EXT2_S_IFLNK    0xA000  /* symbolic link */
#define EXT2_S_IFREG    0x8000  /* regular file */
#define EXT2_S_IFDIR    0x4000  /* directory */


void fechaF(int segundos, char fechaS []);
void grupoF(int gid, char grupoS []);
void imprimir1(char texto1[], char texto2[]);
void imprimir2(char texto1[], int * numero);
void imprimirBits (int num);
void leer1(int fd, int offset, int bytes, char buffer[]);
void leer2(int fd, int offset, int bytes, int *buffer);
//void leer3(int fd, int offset, int bytes, struct *buffer);
void leerEImprimir1(int fd, int offset, int bytes, char buffer[], char texto[]);
void leerEImprimir2(int fd, int offset, int bytes, int *buffer, char texto[]);
void mostrarError(char texto[]);
void obtenerModo(int modo, char salida[11]);
void usuarioF(int uid, char usuarioS []);



void mostrarError(char texto[]) {
    printf("Error número: %d\n", errno);
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
/*void leer3(int fd, int offset, int bytes, struct * buffer){
    if (lseek(fd, offset, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd, buffer, bytes) == -1) mostrarError("read");
}*/
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


void mostrarOpciones(){

    printf("Modo de uso\nbuddyFS -s\nbuddyFS -l\nbuddyFS -b tamanioNube\n");
    exit(0);
}


//ejercicio1------------------------------------------------------------------------

void ejercicio1(struct ext2_super_block sb) {

    if (sb.s_magic != EXT2_SUPER_MAGIC) {
        printf("%s\n", "No hay sistema de archivos tipo EXT2");
        exit(1);
    }
    printf("%-40s%12s\n", "Sistema de archivos tipo:","EXT2");
    printf("%-40s%12s\n", "Nombre del volumen:",sb.s_volume_name);
    printf("%-40s%12d\n", "Cantidad de i-nodos:",sb.s_inodes_count);
    printf("%-40s%12d\n", "Cantidad de i-nodos libres:",sb.s_free_inodes_count);
    printf("%-40s%12d\n", "Primer i-nodo no reservado:",sb.s_first_ino);
    printf("%-40s%12d\n", "Tamanio estructura de un i-nodo:",sb.s_inode_size);
    printf("%-40s%12d\n", "Tamanio de bloque:",sb.s_log_block_size+1024);
    printf("%-40s%12d\n", "Primer bloque de datos:",sb.s_first_data_block);
    printf("%-40s%12d\n", "Cantidad de bloques:",sb.s_blocks_count);
    printf("%-40s%12d\n", "Cantidad de bloques libres:",sb.s_free_blocks_count);
    printf("%-40s%12d\n", "Tamanio total en disco:",sb.s_blocks_count * (sb.s_log_block_size+1024));
    

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
/*
Archivo cargarArchivo(int entradaDirectorio, FS s) {

    Archivo i;
    i.entradaDirectorio = entradaDirectorio;

    i.inodo = i.recLen = i.modo = i.cantLinks = 0;
    i.usuario = i.grupo =  i.tamanio = i.fecha = i.nameLen = 0;
    char cadenaVacia[255] = {' '}; 
    strncpy(i.nombreTexto,cadenaVacia,255);

    leer2(s.fd, i.entradaDirectorio, 4, &i.idInodo);

    leer2(s.fd, i.entradaDirectorio + 4, 2, &i.recLen);
    i.inodo = s.tablaInodos + ((i.idInodo - 1) * s.tamanioInodoEnBytes);
    leer2(s.fd, i.inodo, 2, &i.modo);
    obtenerModo(i.modo, i.modoTexto);

    leer2(s.fd, i.inodo + 26, 2, &i.cantLinks);
    leer2(s.fd, i.inodo + 4, 4, &i.tamanio);

    leer2(s.fd, i.inodo + 16, 4, &i.fecha);
    fechaF(i.fecha, i.fechaTexto);

    leer2(s.fd, i.inodo + 24, 2, &i.grupo);
    grupoF(i.grupo, i.grupoTexto);

    leer2(s.fd, i.inodo + 2, 2, &i.usuario);
    usuarioF(i.usuario, i.usuarioTexto);

    leer2(s.fd, i.entradaDirectorio + 6, 1, &i.nameLen);
    leer1(s.fd, i.entradaDirectorio + 8, i.nameLen, i.nombreTexto);
    return i;

};
*/
void ejercicio2(int fd1, struct ext2_super_block sb, struct ext2_group_desc gd, struct ext2_inode i) {

	int tamInodo= sb.s_inode_size;
	int tamBloque= sb.s_log_block_size + 1024; 
	
	
	
	int directorioRaiz = (gd.bg_inode_table * tamBloque)+ tamInodo;
	
	//struct ext2_inode i;
	//struct ext2_inode * i=malloc(sizeof(struct ext2_inode));

	//printf("dirraiz %d\n",directorioRaiz);
	//printf("bginodetalbe %d\n",gd.bg_inode_table);
	//printf("tambloque %d\n",tamBloque);
	if (lseek(fd1, directorioRaiz, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, &i, tamInodo) == -1) mostrarError("read");
    
     //__le32 bloqueEntradasRaiz;
     __le32 idBloqueDeEntradasRaiz = i.i_block[0];
    // printf("idber:%d\n", idBloqueDeEntradasRaiz);
     int entradaDirectorio = idBloqueDeEntradasRaiz * tamBloque;
    // printf("entradadir%d\n",entradaDirectorio);
    
    //int primerBloqueEntradaRaiz; 
    //if (lseek(fd1, directorioRaiz, SEEK_SET) == -1) mostrarError("lseek");
    //if (read(fd1, i, tamInodo) == -1) mostrarError("read");
    
    //printf("%d",i->i_block);
    
	
    //int entradaDirectorio = s.primerEntradaDirectorio;
    int aux=0, recLen=0, nameLen=0, dirInodo=0;
    printf("%-8s %-16s %-6s %-8s %-8s %-8s %-16s %-16s\n", "Inodo", "Modo", "Links", "Usr", "Grp", "Tamanio", "Fecha", "Archivos");
    while (1) {
		
		if (lseek(fd1, entradaDirectorio + 4, SEEK_SET) == -1) mostrarError("lseek");
		if (read(fd1, &recLen, 2) == -1) mostrarError("read");
		
		if (lseek(fd1, entradaDirectorio + 6, SEEK_SET) == -1) mostrarError("lseek");
		if (read(fd1, &nameLen, 1) == -1) mostrarError("read");
		
       // Archivo i = cargarArchivo(entradaDirectorio, s);
        if ((entradaDirectorio + recLen) % tamBloque == 0) break;
        
        struct ext2_dir_entry * di = malloc(8+nameLen);
        
        if (lseek(fd1, entradaDirectorio, SEEK_SET) == -1) mostrarError("lseek");
		if (read(fd1, &di, 8+nameLen) == -1) mostrarError("read");
        
        
        //dirInodo = (di->inode -1) ;
        if (lseek(fd1, di->inode, SEEK_SET) == -1) mostrarError("lseek");
		if (read(fd1, &i, tamInodo) == -1) mostrarError("read");
        
        
//printf("%-8d %-16s %-6d %-8s %-8s %-8d %-16s %-16s\n", i.idInodo, i.modoTexto, i.cantLinks, i.usuarioTexto, i.grupoTexto, i.tamanio, i.fechaTexto, i.nombreTexto);
       // entradaDirectorio += i.recLen;
        aux++;
    }

}

//------------------------------------------------------------------------


int main(int argc, char * argv[]) {

    int fd1;
    if ((fd1 = open("extra/imagen1.flp", O_RDONLY) ) == -1) mostrarError("open");

    struct ext2_super_block * sb =malloc(sizeof(struct ext2_super_block));
    struct ext2_group_desc * gd =malloc(sizeof(struct ext2_group_desc));

    //if (sb == NULL) {printf("errorEnMalloc\n"); exit(1);}


	if (lseek(fd1, 1024, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, sb, 1024) == -1) mostrarError("read");
    
    if (lseek(fd1, 1024 * 2, SEEK_SET) == -1) mostrarError("lseek");
    if (read(fd1, gd, 1024) == -1) mostrarError("read");

	int cantInodos = sb->s_inodes_count;
	int tamInodo = sb->s_inode_size;
	struct ext2_inode * ti=calloc(cantInodos,tamInodo);


    int c;

    if(argc==1) mostrarOpciones();

    if ((c = getopt (argc, argv, "sl")) < 0) mostrarError("getopt");

    switch (c) {
    case 's':
       ejercicio1(*sb);
        break;
    case 'l':
		ejercicio2(fd1,*sb,*gd, *ti);
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
*/
