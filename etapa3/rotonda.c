#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <string.h>
#define SEGSIZE 100

union semun 
{
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

void mostrarError (char palabra[]);
void down (int idSem,int nroSem);
void up (int idSem,int nroSem);

int main (int argc, char *argv[])
{
	int nroIte = atoi(argv[1]);
	key_t keysem;
	int semid;

	if ((keysem = ftok(".", 'A')) == -1) mostrarError("ftok");
	if ((semid = semget(keysem, 5, IPC_CREAT |  0666 )) == -1) mostrarError("semget");

	union semun arg;
	arg.val = 1;
	if (semctl(semid, 0, SETVAL, arg) == -1) mostrarError("inicializandosem0");
	if (semctl(semid, 1, SETVAL, arg) == -1) mostrarError("inicializandosem1");º
	if (semctl(semid, 2, SETVAL, arg) == -1) mostrarError("inicializandosem2");
	if (semctl(semid, 3, SETVAL, arg) == -1) mostrarError("inicializandosem3");
	arg.val = 8;
	if (semctl(semid, 4, SETVAL, arg) == -1) mostrarError("inicializandosem4");


/*
//crear o conectarse a segmento de memoria compartida:
	key_t key;
	int shmid;
	char *segptr;
	char *endptr;

	if ((key = ftok(".", 'S')) == -1) mostrarError("ftok");

	if ((shmid = shmget(key, SEGSIZE, IPC_CREAT | 0777)) == -1) mostrarError("shmget");

	if (*(segptr = shmat(shmid, 0, 0)) == -1) mostrarError("shmat");


//concatenar Y al segmento

	strcat(segptr, "Z");
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
		struct sembuf sb3 = {0, 1, 0};

		if (semop(semid, &sb3, 1) == -1) mostrarError("error al hacer up en el sem1");
	}
*/

	return 0;
}


void vehiculo (int semId, int origen, int destino, int tipoVehiculo, int nroVehiculo)
{
	if (tipoVehiculo==0)
	{
		tipoVehiculoS="CAMION";
	}
	else
	{
		tipoVehiculoS="COLECTIVO";
	}
	
	printf("Origen: %d, Destino: %d\n", origen, destino);
	printf("%s: %d (%d) con destino desde: %d a %d", tipoVehiculoS, nroVehiculo, getpid(), origen, destino);

	down(semId,4);

	down(semId, origen);
	sleep(1);
	printf("%s: %d (%d) ingresando a la rotonda por: %d", tipoVehiculoS, nroVehiculo, getpid(),origen);
	up(semId, origen);

	printf("Circulando %s (%d) %d", tipoVehiculoS, getpid(), nroVehiculo);
	sleep(2);
	
	//QUE HAY QUE HACER CON ESTO
	for(int pos=origen; pos<destino-1; pos++)
	{
		down(semId, pos);
		printf("%s (%d): %d cruzando por: %d", tipoVehiculoS, getpid(), nroVehiculo, semActual);
		up(semId, pos);
	}


	down(semId, destino);
	sleep(1);
	printf("%s: %d (%d) saliendo de la rotonda por: %d", tipoVehiculoS, nroVehiculo, getpid(),origen);
	up(semId, destino);

	up(semId,4);

	return 0;
}

void mostrarError(char palabra[])
{
	printf("Error número: %d\n", errno);
	perror(palabra);
	exit(1);
}

void down (int idSem, int nroSem)
{
	struct sembuf registro;
	registro.sem_num = nroSem;
	registro.sem_op = -1;
	registro.sem_flg = 0;
	if (semop(idSem, &registro, 1) == -1){
		perror("semop");
	}
	return;
}

void up (int idSem,int nroSem)
{
	struct sembuf registro;
	registro.sem_num = nroSem;
	registro.sem_op = 1;
	registro.sem_flg = 0;
	if (semop(idSem, &registro, 1) == -1){
		perror("semop");
	}
	return;
}