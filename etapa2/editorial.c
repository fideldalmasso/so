#include <stdio.h>
#include <unistd.h> //read

int main(int argc, char * argv[]) {

	char buffer[1];


	while ((read(0, buffer, 1)) != 0) {

		if (buffer[0] != '\n')
			printf("%c", buffer[0]);

		else
			printf(" Tulibro SA\n");
	}

	return 0;
}
