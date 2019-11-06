#include <sys/sem.h>
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

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

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

	if (argc == 1) mostrarError("No Argument");

	if ((key = ftok(".", 'S')) == -1) mostrarError("ftok");

	if ((shmid = shmget(key, SEGSIZE, /*IPC_EXCL |*/  IPC_CREAT | 0777)) == -1) mostrarError("shmget");

	if (*(segptr = shmat(shmid, 0, 0)) == -1) mostrarError("shmat");

//vaciar contenido de memoria compartida
	strcpy(segptr, "");

	long opcion = strtol(argv[1], &endptr, 10);
	// if(opcion==0) verificar error
	//int opcion = atoi(argv[1]);
	opcion = opcion - 1;

//conectar con semaforos:

	key_t keysem;
	int semid;

	if ((keysem = ftok(".", 'A')) == -1) mostrarError("ftok");
	if ((semid = semget(keysem, 3, IPC_CREAT |  0666 )) == -1) mostrarError("semget");

//inicializar los semaforos en 0
	union semun arg;
	arg.val = 0;
	if (semctl(semid, 0, SETVAL, arg) == -1) mostrarError("inicializandosem1");
	if (semctl(semid, 1, SETVAL, arg) == -1) mostrarError("inicializandosem2");
	if (semctl(semid, 2, SETVAL, arg) == -1) mostrarError("inicializandosem3");

	printf("numero ingresado: %ld\n", opcion );
// hacer up en el semaforo
	struct sembuf sb1 = {opcion, 1, 0};

	if (semop(semid, &sb1, 1) == -1) mostrarError("error al hacer up en el sem%d");

// hacer down en el semaforo 1
	struct sembuf sb2 = {0, -1, 0};
	if (semop(semid, &sb2, 1) == -1) mostrarError("error haciendo down en sem1");

//concatenar X al segmento
	strcat(segptr, "X");
	printf("SELLO: %s\n", segptr);



//eliminar semaforo

	if (strlen(segptr) > 3) {
		union semun arg2;
		if (shmctl(shmid, IPC_RMID, 0) == -1) mostrarError("borrandomemoriacompartida");
		if (semctl(semid, 0, IPC_RMID, arg2) < 0) mostrarError("borrandoSEM");
		//el argumento 2: semnum es ignorado con IPC_RMID
	}
	else {
		// hacer up en el semaforo siguiente
		struct sembuf sb3 = {1, 1, 0};

		if (semop(semid, &sb3, 1) == -1) mostrarError("error al hacer up en el sem2");
	}

	return 0;
}


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
