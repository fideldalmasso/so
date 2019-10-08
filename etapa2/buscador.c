#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	int pid=atoi(argv[1]);
	DIR *dir;
	dir = opendir(argv[2]);
	struct dirent *ptr;
	struct stat st;
	
	printf(argv[2]);
	
	if (dir != NULL){
		while(ptr=readdir(dir)){
			stat((ptr->d_name), &st);
			if(S_ISREG(st.st_mode)){
				printf(ptr->d_name);
				int tam = st.st_size;
				if(tam < 1024){
					kill(pid, SIGUSR1);
					printf(" - Regular\n");
				}
				if(tam > 1024){
					kill(pid, SIGUSR2);
					printf(" - Regular size > 1 KB %d\n",tam);
				}
			}
		}
	}
	
	close(dir);
	
	return 0;
}
