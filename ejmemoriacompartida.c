#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>


int main() {
    // Crear un conjunto de semáforos (2 semáforos en este caso)
    int shmid =shmet(IPC_PRIVATE,1024,IPC_CREAT|0666)
    // Eliminar el conjunto de semáforos al finalizar
 

    return 0;
}
