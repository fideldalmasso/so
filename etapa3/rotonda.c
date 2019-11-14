#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <sys/unistd.h>
#include <unistd.h>

union semun
{
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

void vehiculo (int semId, int origen, int destino, int tipoVehiculo, int nroVehiculo);
void mostrarError (char palabra[]);
void down (int idSem, int nroSem);
void up (int idSem, int nroSem);

int main (int argc, char *argv[])
{

	if (argc != 2)
	{
		printf("Se debe ingresar como parametro un numero positivo que indique la cantidad de procesos a generar\n");
		exit(1);
	}

	int pid = 3, nroIte = atoi(argv[1]), tipoVehiculo, origen, destino, semid;
	key_t keysem;
	union semun arg;

	if ((keysem = ftok(".", 'B')) == -1) mostrarError("ftok");
	if ((semid = semget(keysem, 5, IPC_CREAT |  0666 )) == -1) mostrarError("semget");

	arg.val = 1;
	if (semctl(semid, 0, SETVAL, arg) == -1) mostrarError("inicializandosem0");
	if (semctl(semid, 1, SETVAL, arg) == -1) mostrarError("inicializandosem1");
	if (semctl(semid, 2, SETVAL, arg) == -1) mostrarError("inicializandosem2");
	if (semctl(semid, 3, SETVAL, arg) == -1) mostrarError("inicializandosem3");
	arg.val = 8;
	if (semctl(semid, 4, SETVAL, arg) == -1) mostrarError("inicializandosem4");

	while (pid != 0 && nroIte != 0)
	{
		pid = fork();

		srand(time(NULL) + getpid());

		tipoVehiculo = rand() % 2;
		origen = rand() % 4;
		destino = rand() % 4;

		while (destino == origen) {
			destino = rand() % 4;
		}

		if (tipoVehiculo == 0)
		{
			vehiculo(semid, origen, destino, tipoVehiculo, nroIte);
		}
		else
		{
			vehiculo(semid, origen, destino, tipoVehiculo, nroIte);
		}
		nroIte--;
	}

	if (semctl(semid, 0, IPC_RMID) == -1) mostrarError("borrandosemaforos");

	return 0;
}

void vehiculo (int semId, int origen, int destino, int tipoVehiculo, int nroVehiculo)
{
	char tipoVehiculoS[10];
	int pos;

	if (tipoVehiculo == 0)
	{
		strcpy(tipoVehiculoS, "CAMION\0");
	}
	else
	{
		strcpy(tipoVehiculoS, "COLECTIVO\0");
	}

	printf("Origen: %d, Destino: %d\n", origen, destino);
	printf("%s: %d (%d) con destino desde: %d a %d\n", tipoVehiculoS, nroVehiculo, getpid(), origen, destino);

	down(semId, 4);

	down(semId, origen);
	//sleep(1);
	printf("%s: %d (%d) ingresando a la rotonda por: %d\n", tipoVehiculoS, nroVehiculo, getpid(), origen);
	up(semId, origen);

	printf("Circulando %s (%d) %d\n", tipoVehiculoS, getpid(), nroVehiculo);
	//sleep(2);


	if (origen == 3)
	{
		pos = 0;
	}
	else
	{
		pos = origen + 1;
	}
	while (pos != destino)
	{
		down(semId, pos);
		printf("%s (%d): %d cruzando por: %d\n", tipoVehiculoS, getpid(), nroVehiculo, pos);
		up(semId, pos);

		if (pos == 3) {
			pos = 0;
		}
		else
		{
			pos++;
		}
	}

	down(semId, destino);
	//sleep(1);
	printf("%s: %d (%d) saliendo de la rotonda por: %d\n", tipoVehiculoS, nroVehiculo, getpid(), destino);
	up(semId, destino);

	up(semId, 4);
}

void mostrarError(char palabra[])
{
	printf("Error número: %d\n", errno);
	perror(palabra);
	exit(1);
}

void down (int idSem, int nroSem)
{
	struct sembuf conjSem;
	conjSem.sem_num = nroSem;
	conjSem.sem_op = -1;
	conjSem.sem_flg = 0;
	if (semop(idSem, &conjSem, 1) == -1) {
		perror("semopdown");
	}
	return;
}

void up (int idSem, int nroSem)
{
	struct sembuf conjSem;
	conjSem.sem_num = nroSem;
	conjSem.sem_op = 1;
	conjSem.sem_flg = 0;
	if (semop(idSem, &conjSem, 1) == -1) {
		perror("semopup");
	}
	return;
}