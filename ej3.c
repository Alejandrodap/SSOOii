#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MSG_SIZE 256

typedef struct {
    int tipo;
    union {
        char texto[MSG_SIZE];
        int numero;
    };
} Mensaje;

int main() {
    int pipefd[2];
    pid_t pid1, pid2;
    Mensaje msg1, msg2;
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    if ((pid1 = fork()) == 0) {
        close(pipefd[1]);  
        read(pipefd[0], &msg1, sizeof(Mensaje));
        
        if (msg1.tipo == 1) {
            printf("Hijo 1 recibió mensaje de texto: %s\n", msg1.texto);
        }
        close(pipefd[0]);
        exit(0);
    }
    if ((pid2 = fork()) == 0) {
        close(pipefd[1]);
        read(pipefd[0], &msg2, sizeof(Mensaje));
        if (msg2.tipo == 2) {
            printf("Hijo 2 recibió número: %d\n", msg2.numero);
        }
        close(pipefd[0]);
        exit(0);
    }
    close(pipefd[0]);  
    msg1.tipo = 1;
    strncpy(msg1.texto, "Hola, soy el primer hijo!", MSG_SIZE);
    write(pipefd[1], &msg1, sizeof(Mensaje));
    msg2.tipo = 2;
    msg2.numero = 42;
    write(pipefd[1], &msg2, sizeof(Mensaje));
    sleep(10);
    wait(NULL);
    wait(NULL);
    close(pipefd[1]);
    return 0;
}
