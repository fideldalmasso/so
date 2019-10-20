#include <stdio.h>
#include <stdlib.h> //atoi
#include <unistd.h> //getpid

void binario (int n);

int main (int argc, char *argv[]) {

	int n = atoi(argv[1]);

	binario(n);
	printf(" Tejido %d regenerado\n", getpid());

	return 0;
}

void binario (int n) {
	int mod = n % 2;
	if (n / 2 > 0) {
		binario(n / 2);
		printf("%d", mod);
	} else {
		printf("%d", mod);
	}
}
