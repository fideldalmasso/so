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
	struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

void mostrarError(char palabra[]) {
	printf("Error número: %d\n", errno);
	perror(palabra);
	exit(1);
}

void mostrarYSalir(char palabra[]) {
	printf("Error: %s\n", palabra);
	exit(1);
}


//-------------------------
int main(int argc, char *argv[]) {

//validar entrada
	if (argc == 1) mostrarYSalir("Debe pasar un argumento");

	char *endptr = NULL;
	long opcion = strtol(argv[1], &endptr, 10) - 1;

	if (errno == ERANGE) mostrarError("strtol");
	if (endptr == argv[1]) mostrarYSalir("No se encontró un número en la entrada");
	if (opcion < 0 || opcion > 2) mostrarYSalir("Debe ingresar el número 1, 2 o 3");

//conectar con semaforos:
	key_t keysem;
	int semid;

	if ((keysem = ftok(".", 'A')) == -1) mostrarError("ftok");
	if ((semid = semget(keysem, 3, IPC_CREAT |  0666 )) == -1) mostrarError("semget");

//crear o conectarse a segmento de memoria compartida:
	key_t key;
	int shmid;
	char *segptr;

	if ((key = ftok(".", 'S')) == -1) mostrarError("ftok");

	if ((shmid = shmget(key, SEGSIZE, /*IPC_EXCL |*/  IPC_CREAT | 0777)) == -1) mostrarError("shmget");

	if (*(segptr = shmat(shmid, 0, 0)) == -1) mostrarError("shmat");

//vaciar contenido de memoria compartida
	strcpy(segptr, "");


//inicializar los semaforos en 0
	union semun arg;
	arg.val = 0;
	if (semctl(semid, 0, SETVAL, arg) == -1) mostrarError("inicializandosem1");
	if (semctl(semid, 1, SETVAL, arg) == -1) mostrarError("inicializandosem2");
	if (semctl(semid, 2, SETVAL, arg) == -1) mostrarError("inicializandosem3");

// hacer up en el semaforo indicado
	struct sembuf sb1 = {opcion, 1, 0};

	if (semop(semid, &sb1, 1) == -1) mostrarError("error al hacer up en el sem%d");

// hacer down en el semaforo 1
	struct sembuf sb2 = {0, -1, 0};
	if (semop(semid, &sb2, 1) == -1) mostrarError("error haciendo down en sem1");

// tras desbloquearse, concatenar X al segmento
	strcat(segptr, "X");
	printf("SELLO: %s\n", segptr);

	if (strlen(segptr) > 3) {

//eliminar semaforos
		union semun arg2;
		if (shmctl(shmid, IPC_RMID, 0) == -1) mostrarError("borrandoMemoriaCompartida");
		if (semctl(semid, 0, IPC_RMID, arg2) < 0) mostrarError("borrandoSEM");
	}
	else {

// hacer up en el semaforo siguiente
		struct sembuf sb3 = {1, 1, 0};

		if (semop(semid, &sb3, 1) == -1) mostrarError("error al hacer up en el sem2");
	}

	return 0;
}