#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

int main (int argc, char **argv)
{
bool mostrarArea=false;
bool mostrarPerimetro=false;
int c;
  int lado=-1;
  int base=-1;


	while ((c = getopt (argc, argv, ":apl:b:")) != -1)
		switch (c)
      {
      case 'a':
        mostrarArea = true;
        break;
      case 'p':
        mostrarPerimetro = true;
        break;
      case 'l':
        lado=atoi(optarg);
        break;
      case 'b':
        base=atoi(optarg);
        break;
	  case '?':
		printf("Opción inválida: %c\n", optopt);
		exit(1);
      break;
      }

 if (lado <= 0 && base <=0) {
       printf("-l y -b son obligatorios y necesitan argumentos positivos!\n");
       exit(1);
    }
 if (lado <= 0 ) {
       printf("-l es obligatorio y necesita un argumento positivo!\n");
       exit(1);
    }
 if (base <= 0 ) {
       printf("-b es obligatorio y necesita un argumento positivo!\n");
       exit(1);
    }
	if (mostrarArea) printf("Area: %d\n", lado*base);
	if (mostrarPerimetro) printf("Perimetro: %d\n", lado*2+base*2);
  
  return 0;
}
