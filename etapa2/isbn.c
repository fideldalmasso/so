#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[]) {

  char buffer[1];
  int isbn = 3025;

  while ((read(0, buffer, 1)) != 0) {

    if (buffer[0] != '\n')
      printf("%c", buffer[0]);

    else {
      printf(" %d\n", isbn);
      /*
			intento de recrear printf con write
	   		char arreglo[6];
	   		char aux[4];
	   		arreglo[0] = ' ';
	   		sprintf(aux, "%d", isbn);
	   		strcat(arreglo,aux);
	   		strcat(arreglo,"\n");
		   	write(1,arreglo,6);*/
      isbn = isbn + 1;
    }
  }
}