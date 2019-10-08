#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

// invocar este proceso de la siguiente forma:
// ./imprenta -l libros.txt -i imprimir.txt

void procesarEntrada(char libros[20], char imprimir[20], int argc, char * argv[]) {

  int c;
  bool l, i = 0;

  while ((c = getopt(argc, argv, ":l:i:")) != -1) {
    switch (c) {

    case 'l':
      l = 1;
      strcpy(libros, optarg);
      break;

    case 'i':
      i = 1;
      strcpy(imprimir, optarg);
      break;

    case ':':
      printf("La opción necesita un valor\n");
      exit(1);
      break;

    case '?':
      printf("Opción inválida: %c\n", optopt);
      exit(1);
      break;
    }
  }

  if (!l) {
    printf("-l es obligatorio!\n");
    exit(1);
  }
  if (!i) {
    printf("-i es obligatorio!\n");
    exit(1);
  }

}

void procesoISBN(char libros[], int tuberia[], char * argv[]) {

  int fd1 = open(libros, O_RDONLY);
  if (fd1 == -1) {
    printf("Error número: %d\n", errno);
    perror(libros);
    exit(1);
  }
  close(0);
  dup(fd1);

  close(1);
  dup(tuberia[1]);

  close(tuberia[0]);
  execve("isbn", argv, 0);

}

void procesoEditorial(char imprimir[], int tuberia[], char * argv[]) {

  int fd2 = open(imprimir, O_CREAT + O_WRONLY);
  if (fd2 == -1) {
    printf("Error número: %d\n", errno);
    perror(imprimir);
    exit(1);
  }
  close(0);
  dup(tuberia[0]);

  close(1);
  dup(fd2);

  close(tuberia[1]);
  chmod(imprimir, 0666);
  execve("editorial", argv, 0);

}

int main(int argc, char * argv[]) {

  char libros[20];
  char imprimir[20];

  procesarEntrada(libros, imprimir, argc, argv);

  pid_t pid;
  int tuberia[2];
  pipe(tuberia);
  int * y = NULL;

  pid = fork();

  if (!pid) {
    procesoISBN(libros, tuberia, argv);
  }
  wait(y);

  pid = fork();

  if (!pid) {
    procesoEditorial(imprimir, tuberia, argv);
  }
  return 0;
}