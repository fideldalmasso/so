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
  int area=0;
  int perimetro=0;
  int lado=-1;
  int base=-1;


  while ((c = getopt (argc, argv, "apl:b:")) != -1)
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
      /*case '?':
        if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
        */
      default:
        abort ();
      }

 if (lado <= 0 ) {
       printf("-l es obligatorio!\n");
       exit(1);
    }
 if (base <= 0 ) {
       printf("-b es obligatorio!\n");
       exit(1);
    }

  printf ("mostrarArea = %d, mostrarPerimetro = %d, lado = %d, base = %d\n", mostrarArea, mostrarPerimetro, lado, base);

  return 0;
}
