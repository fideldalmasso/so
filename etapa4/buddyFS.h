#include <linux/fs.h>
//#include <linux/ext2_fs.h>
// si linux/ext2_fs.h no existe, instalar e2fslibs-dev 
// y utilizar la libreria siguiente
#include <ext2fs/ext2_fs.h> 


void mostrarError(char texto[]);
void ejercicio1(struct ext2_super_block sb);
void ejercicio2(int fd1, struct ext2_super_block sb, struct ext2_group_desc gd, struct ext2_inode ti[], struct ext2_dir_entry_2 tde[], int cantEntradasDirectorios);
void ejercicio3 (struct ext2_inode ti [], struct ext2_dir_entry_2 tde [], int cantEntradasDirectorios, int tamNube);

void cargarGroupDescriptor(int fd, struct ext2_group_desc * gd);
void cargarSuperbloque(int fd,  struct ext2_super_block * sb);
void cargarTablaInodos(int fd1,struct ext2_inode *ti, int posTablaInodos,int cantInodos);
int cargarTablaEntradasDirectorios(int fd1,struct ext2_dir_entry_2  *tde,int entradaDirectorio,int tamBloque);


