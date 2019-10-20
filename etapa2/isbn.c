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
            isbn = isbn + 1;
        }
    }
    return 0;
}
