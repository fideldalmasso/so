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

void mostrarError(char palabra[]);
void procesarEntrada(char libros[20], char imprimir[20], int argc, char * argv[]);


int main(int argc, char * argv[]) {

    char libros[20];
    char imprimir[20];

    procesarEntrada(libros, imprimir, argc, argv);

    int fd1, fd2;
    pid_t pid;
    int tuberia[2];
    pipe(tuberia);


    switch (pid = fork()) {
    case -1:
        mostrarError("ProcesoISBN");
        break;

    case 0:
        fd1 = open(libros, O_RDONLY);
        if (fd1 == -1) mostrarError(libros);
        close(0);
        dup(fd1);

        close(1);
        dup(tuberia[1]);

        close(tuberia[0]);
        execve("isbn", argv, 0);
    }

//------------------------

    switch (pid = fork()) {
    case -1:
        mostrarError("ProcesoEditorial");
        break;

    case 0:
        fd2 = open(imprimir, O_CREAT + O_WRONLY);
        if (fd2 == -1) mostrarError(imprimir);
        close(0);
        dup(tuberia[0]);

        close(1);
        dup(fd2);

        close(tuberia[1]);
        chmod(imprimir, 0666);
        execve("editorial", argv, 0);
    }

    return 0;
}

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


void mostrarError(char palabra[]) {
    printf("Error número: %d\n", errno);
    perror(palabra);
    exit(1);
}