#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//---------
#include <ctype.h>
#include <string.h>
#include <sys/sem.h>

#define SEGSIZE 100

// void writeshm(int shmid, char *segptr, char *text) {
// 	strcpy(segptr, text);
// 	printf("Hecho...\n");
// }
// void readshm(int shmid, char *segptr) {
// 	printf("valor de segptr: %s\n", segptr);
// }
// void removeshm(int shmid) {
// 	shmctl(shmid, IPC_RMID, 0);
// 	printf("Segmento marcado para borrado\n");
// }
// void changemode(int shmid, char *mode) {
// 	struct shmid_ds myshmds;
// 	/* Obtener valor actual de la estructura de datos interna */
// 	shmctl(shmid, IPC_STAT, &myshmds);
// 	/* Mostrar antiguos permisos */
// 	printf("Antiguos permisos: %o\n", myshmds.shm_perm.mode);
// 	/* Convertir y cargar el modo */
// 	sscanf(mode, "%o", &myshmds.shm_perm.mode);
// 	/* Actualizar el modo */
// 	shmctl(shmid, IPC_SET, &myshmds);
// 	printf("Nuevos permisos : %o\n", myshmds.shm_perm.mode);
// }
void mostrarError(char palabra[]) {
	printf("Error número: %d\n", errno);
	perror(palabra);
	exit(1);
}

//-------------------------
int main(int argc, char *argv[]) {

//crear o conectarse a segmento de memoria compartida:
	key_t key;
	int shmid;
	char *segptr;
	char *endptr;

	//if (argc == 1) mostrarError("No Argument");

	if ((key = ftok(".", 'S')) == -1) mostrarError("ftok");

	if ((shmid = shmget(key, SEGSIZE, IPC_CREAT | 0777)) == -1) mostrarError("shmget");

	if (*(segptr = shmat(shmid, 0, 0)) == -1) mostrarError("shmat");

	//long opcion = strtol(argv[1], &endptr, 10);
	// if(opcion==0) verificar error

//conectar con semaforos:

	key_t keysem;
	int semid;

	if ((keysem = ftok(".", 'A')) == -1) mostrarError("ftok");
	if ((semid = semget(keysem, 1, 0777 | IPC_CREAT)) == -1) mostrarError("semget");

//inicializar los semaforos en 0
	union semun arg;
	arg.val = 0;
	if (semctl(semid, 0, SETVAL, arg) == -1) mostrarError("semctl1");
	if (semctl(semid, 1, SETVAL, arg) == -1) mostrarError("semctl1");
	if (semctl(semid, 2, SETVAL, arg) == -1) mostrarError("semctl1");


//hacer 
int x = 2;
struct sembuf sb ={x, 1, 0}
if (semop(semid, &sb, 1) == -1) mostrarError("semop%dup", x);

sb.sem_op = -1;
if (semop(semid, &sb, 0) == -1) mostrarError("semop0down", x);

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

/* links

http://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html
https://www.geeksforgeeks.org/ipc-shared-memory/
https://stackoverflow.com/questions/2797813/how-to-convert-a-command-line-argument-to-int
https://stackoverflow.com/questions/9748393/how-can-i-get-argv-as-int/38669018


*/
