#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//---------
#include <ctype.h>
#include <string.h>

#define SEGSIZE 100

void writeshm(int shmid, char *segptr, char *text){
	strcpy(segptr, text);
	printf("Hecho...\n");
}
void readshm(int shmid, char *segptr){
	printf("valor de segptr: %s\n", segptr);	
}
void removeshm(int shmid){
	shmctl(shmid, IPC_RMID, 0);
	printf("Segmento marcado para borrado\n");
}
void changemode(int shmid, char *mode){
	struct shmid_ds myshmds;
	/* Obtener valor actual de la estructura de datos interna */
	shmctl(shmid, IPC_STAT, &myshmds);
	/* Mostrar antiguos permisos */
	printf("Antiguos permisos: %o\n", myshmds.shm_perm.mode);
	/* Convertir y cargar el modo */
	sscanf(mode, "%o", &myshmds.shm_perm.mode);
	/* Actualizar el modo */
	shmctl(shmid, IPC_SET, &myshmds);
	printf("Nuevos permisos : %o\n", myshmds.shm_perm.mode);
}

//-------------------------
int main(int argc, char *argv[]){

//crear o conectarse a segmento de memoria compartida:
	key_t key = ftok(".", 'S');
	int shmid;
	char *segptr;
	char *endptr;
	
	
	if(argc == 1){
		printf("Error: este proceso necesita un argumento");
		exit(1);
		}

	if((shmid = shmget(key, SEGSIZE, IPC_CREAT)) == -1){
		perror("shmget");
		exit(1);
	}

	if(*(segptr = shmat(shmid,0, 0)) == -1){
	//if((segptr = shmat(shmid, 0, 0)) == -1){   OPCION ORIGINAL
		perror("shmat");
		exit(1);
	}
	
	long opcion = strtol(argv[1],&endptr,10);
	// if(opcion==0) verificar error	

//conectar con semaforos:



//comenzar ejecucion:
	switch(opcion){
		case 1:
		//up en sem1
		break;
		case 2:
		//up en sem2
		break;
		case 3:
		//up en sem3
		break;
	}
	
//down en sem1
//leer segmento
//concatenar X al segmento
//escribir segmento

	
	
	
	return 0;
}

/*		case 'e': writeshm(shmid, segptr, argv[2]);
		break;
		case 'l': readshm(shmid, segptr);
		break;
		case 'b': removeshm(shmid);
		break;
		case 'm': changemode(shmid, argv[2]);
		*/ 
