#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

int
main (int argc, char **argv)
{
  int perimetro=false;
  int area=false;
  int c;


  while ((c = getopt (argc, argv, "ap")) != -1)
    switch (c)
      {
      case 'a':
        area = true;
        break;
      case 'p':
        perimetro = true;
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


  printf ("area = %d, perimetro = %d\n", area,perimetro);

  return 0;
}
