#include <fcntl.h> 
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <linux/fs.h>
#include <linux/ext2_fs.h>
// si linux/ext2_fs.h no existe, instalar e2fslibs-dev 
// y utilizar la libreria siguiente
//#include <ext2fs/ext2_fs.h> 
#include <errno.h>
#include "buddyFS.h"

void mostrarError(char texto[]) {
    printf("Error numero: %d\n", errno);
    perror(texto);
    exit(1);
}

void mostrarUso(){
	 printf("Modo de uso\nbuddyFS [-s][-l][-b tamanioNube]\nEl archivo se leera siempre en la ruta /dev/fd0\n");
     exit(0);
	}

int main(int argc, char * argv[]) {
	
    int fd1, posTablaInodos,c,tamBloque,entradaDirectorio, cantEntradasDirectorios;
    __le32  cantInodos, idBloqueDeEntradasRaiz;
	
    struct ext2_super_block * sb = malloc(sizeof(struct ext2_super_block));
	struct ext2_group_desc * gd = malloc(1024);
    
    if (argc < 2) 
		mostrarUso();

    if ((fd1 = open("extra/imagen2.flp", O_RDONLY) ) == -1) mostrarError("open");

	//superbloque
	cargarSuperbloque(fd1,sb);
	
	//groupDescriptor
	cargarGroupDescriptor(fd1,gd);
	
	//tabla de inodos
    cantInodos = (sb->s_inodes_count) - (sb->s_free_inodes_count);
    posTablaInodos = (gd->bg_inode_table * 1024);
	struct ext2_inode *ti = (struct ext2_inode *)malloc(cantInodos * sizeof(struct ext2_inode));
	cargarTablaInodos(fd1,ti,posTablaInodos,cantInodos);

	//tablaEntradasDirectorios
    tamBloque = sb->s_log_block_size + 1024;
    idBloqueDeEntradasRaiz = ti[1].i_block[0];
    entradaDirectorio = idBloqueDeEntradasRaiz * tamBloque;
    
    struct ext2_dir_entry_2 *tde = (struct ext2_dir_entry_2  *)malloc(cantInodos * sizeof(struct ext2_dir_entry_2 ));
    cantEntradasDirectorios = cargarTablaEntradasDirectorios(fd1,tde, entradaDirectorio,tamBloque);



    if ((c = getopt (argc, argv, "slb")) < 0) mostrarError("Debe ingresar un argumento");

    switch (c) {
    case 's':
        ejercicio1(*sb);
        break;
    case 'l':
        ejercicio2(fd1, *sb, *gd, ti, tde, cantEntradasDirectorios);
        break;
    case 'b':
        if(argc != 3){
            printf("Debe ingresarse el tamanio de la nube, y solo uno.\n");
        } else {     
    int tamNube = atoi(argv[2]);
            ejercicio3(ti, tde, cantEntradasDirectorios, tamNube);
        }
        break;
    default:
		mostrarUso();
    }

    return 0;

}

