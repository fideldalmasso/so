#include "buddyFS.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <linux/magic.h>
#include <linux/fs.h>
#include <linux/ext2_fs.h>
// si linux/ext2_fs.h no existe, instalar e2fslibs-dev 
// y utilizar la libreria siguiente
//#include <ext2fs/ext2_fs.h> 
#include <errno.h>
#include "buddyFS.h"

#define EXT2_S_IFMT     0xF000  /* format mask  */
#define EXT2_S_IFLNK    0xA000  /* symbolic link */
#define EXT2_S_IFREG    0x8000  /* regular file */
#define EXT2_S_IFDIR    0x4000  /* directory */


void cargarSuperbloque(int fd,  struct ext2_super_block * sb){
	
	if (lseek(fd, 1024, SEEK_SET) == -1) mostrarError("lseek");
	if (read(fd, sb, 1024) == -1) mostrarError("read");
	
	if (sb->s_magic != EXT2_SUPER_MAGIC) {
		printf("%s\n", "No hay sistema de archivos tipo EXT2");
		exit(1);
	}
}

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
