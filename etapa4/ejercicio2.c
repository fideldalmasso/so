#include "buddyFS.h"
#include <fcntl.h> //open 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <linux/fs.h>
#include <linux/ext2_fs.h>
// si linux/ext2_fs.h no existe, instalar e2fslibs-dev 
// y utilizar la libreria siguiente
//#include <ext2fs/ext2_fs.h> 


#define EXT2_S_IFMT     0xF000  /* format mask  */
#define EXT2_S_IFLNK    0xA000  /* symbolic link */
#define EXT2_S_IFREG    0x8000  /* regular file */
#define EXT2_S_IFDIR    0x4000  /* directory */


void fechaF(int segundos, char fechaS []);
void grupoF(int gid, char grupoS []);
void obtenerModo(int modo, char salida[11]);
void usuarioF(int uid, char usuarioS []);

void cargarGroupDescriptor(int fd, struct ext2_group_desc * gd){
	if (lseek(fd, 1024 * 2, SEEK_SET) == -1) mostrarError("lseek");
	if (read(fd, gd, 1024) == -1) mostrarError("read");
	
}


void cargarTablaInodos(int fd1,struct ext2_inode *ti, int posTablaInodos,int cantInodos){
	
	struct ext2_inode * i = (struct ext2_inode*)malloc(sizeof(struct ext2_inode));
	int p;
	for (p = 0; p < cantInodos; p++) {
		if (lseek(fd1, posTablaInodos + (p * 128), SEEK_SET) == -1) mostrarError("lseek");
		if (read(fd1, i, 128) == -1) mostrarError("read");
		ti[p] = *i;
	}
}

int cargarTablaEntradasDirectorios(int fd1,struct ext2_dir_entry_2  *tde,int entradaDirectorio,int tamBloque){
	
	struct ext2_dir_entry_2 * de = malloc(sizeof(struct ext2_dir_entry_2));
	
	int n=0;
	while (1) {
		if (lseek(fd1, entradaDirectorio, SEEK_SET) == -1) mostrarError("lseek");
		if (read(fd1, de, sizeof(struct ext2_dir_entry_2)) == -1) mostrarError("read");
		
		tde[n]=*de;
		
		tde[n].name[tde[n].name_len] = '\0';
		
		entradaDirectorio += de->rec_len;
		n++;
		if ((entradaDirectorio % tamBloque) == 0) break;
		
	}
	return n;
	
}




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
	for (i = 0; i < 9; i++) { 
		//los permisos estan ubicados en los primeros 9 bits
		if ((modo >> i) & 1)
			salida2[9 - i] = salida3[9 - i];
	}
	
	salida2[10] = '\0';
	strcpy(salida, salida2);
	
}

void ejercicio2(int fd1, struct ext2_super_block sb, struct ext2_group_desc gd, struct ext2_inode ti[], struct ext2_dir_entry_2 tde[], int cantEntradasDirectorios) {
	
	
	printf("%-8s %-16s %-6s %-8s %-8s %-8s %-16s %-16s\n", "Inodo", "Modo", "Links", "Usr", "Grp", "Tamanio", "Fecha", "Archivos");
	int i = 0;
	for(i=0;i<cantEntradasDirectorios;i++) {
		char usuarioTexto[16];
		char grupoTexto[16];
		char fechaTexto[24];
		char modoTexto[11];
		
		obtenerModo(ti[tde[i].inode - 1].i_mode, modoTexto);
		fechaF(ti[tde[i].inode - 1].i_mtime, fechaTexto);
		grupoF(ti[tde[i].inode - 1].i_gid, grupoTexto);
		usuarioF(ti[tde[i].inode - 1].i_uid, usuarioTexto);
		
		int cantLinks = ti[tde[i].inode - 1].i_links_count;
		printf("%-8d %-16s %-6d %-8s %-8s %-8d %-16s %-16s\n", tde[i].inode, modoTexto, cantLinks, usuarioTexto, grupoTexto, ti[tde[i].inode - 1].i_size, fechaTexto, tde[i].name);
		
	}
	
}
