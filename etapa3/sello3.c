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

//conectar con semaforos:

	key_t keysem;
	int semid;

	if ((keysem = ftok(".", 'A')) == -1) mostrarError("ftok");
	if ((semid = semget(keysem, 3, IPC_CREAT |  0666 )) == -1) mostrarError("semget");


// hacer down en el semaforo 3
	struct sembuf sb2 = {2, -1, 0};
	if (semop(semid, &sb2, 1) == -1) mostrarError("error haciendo down en sem3");


//crear o conectarse a segmento de memoria compartida:
	key_t key;
	int shmid;
	char *segptr;

	if ((key = ftok(".", 'S')) == -1) mostrarError("ftok");

	if ((shmid = shmget(key, SEGSIZE, IPC_CREAT | 0777)) == -1) mostrarError("shmget");

	if (*(segptr = shmat(shmid, 0, 0)) == -1) mostrarError("shmat");


//tras desbloquearse, concatenar Y al segmento

	strcat(segptr, "Z");
	printf("SELLO: %s\n", segptr);


	if (strlen(segptr) > 3) {
//eliminar semaforo
		union semun arg2;
		if (shmctl(shmid, IPC_RMID, 0) == -1) mostrarError("borrandomemoriacompartida");
		if (semctl(semid, 0, IPC_RMID, arg2) < 0) mostrarError("borrandoSEM");

	}
	else {
// hacer up en el semaforo siguiente
		struct sembuf sb3 = {0, 1, 0};

		if (semop(semid, &sb3, 1) == -1) mostrarError("error al hacer up en el sem1");
	}

	return 0;
}

