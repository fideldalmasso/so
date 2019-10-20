#include <dirent.h> //opendir readdir
#include <signal.h> //kill SIGUSR1
#include <stdio.h>
#include <stdlib.h> //atoi
#include <sys/stat.h> //stat DIR
#include <unistd.h> //sleep close


int main(int argc, char* argv[]) {
	int pid = atoi(argv[1]);
	DIR *dir;
	dir = opendir(argv[2]);
	struct dirent *ptr;
	struct stat st;

	if (dir != NULL) {
		while ((ptr = readdir(dir))) {
			stat((ptr->d_name), &st);
			if (S_ISREG(st.st_mode)) {
				printf(ptr->d_name);
				int tam = st.st_size;
				if (tam < 1024) {
					kill(pid, SIGUSR1);
					printf(" - Regular\n");
				}
				if (tam > 1024) {
					kill(pid, SIGUSR2);
					printf(" - Regular size > 1 KB %d\n", tam);
				}
			}
			sleep(1);
		}
	} else {
		printf("El directorio ingresado no existe.\n");
	}

	closedir(dir);

	return 0;
}
