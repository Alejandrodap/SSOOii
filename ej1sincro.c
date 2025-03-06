#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define NUM_SEMAFOROS 2

// Función para hacer una operación semáforica (P o wait)
void sem_wait_op(int semid, int semnum) {
    struct sembuf op;
    op.sem_num = semnum;
    op.sem_op = -1;  // Operación de decremento (esperar)
    op.sem_flg = 0;
    if (semop(semid, &op, 1) == -1) {
        perror("Error en sem_wait");
        exit(1);
    }
}

// Función para hacer una operación semáforica (V o signal)
void sem_post_op(int semid, int semnum) {
    struct sembuf op;
    op.sem_num = semnum;
    op.sem_op = 1;  // Operación de incremento (señal)
    op.sem_flg = 0;
    if (semop(semid, &op, 1) == -1) {
        perror("Error en sem_post");
        exit(1);
    }
}

int main() {
    // Crear un conjunto de semáforos (2 semáforos en este caso)
    key_t key = ftok(".", 'A');  // Generar una clave única para el conjunto de semáforos
    int semid = semget(key, NUM_SEMAFOROS, IPC_CREAT | 0666);  // Crear el conjunto de semáforos

    if (semid == -1) {
        perror("Error al crear el conjunto de semáforos");
        exit(1);
    }

    // Inicializar los semáforos: sem[0] para el padre y sem[1] para el hijo
    if (semctl(semid, 0, SETVAL, 1) == -1) {  // El semáforo 0 (para el padre) comienza en 1
        perror("Error al inicializar semáforo 0");
        exit(1);
    }

    if (semctl(semid, 1, SETVAL, 0) == -1) {  // El semáforo 1 (para el hijo) comienza en 0
        perror("Error al inicializar semáforo 1");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == 0) { // Proceso hijo
        for (int i = 0; i < 100; i++) {
            sem_wait_op(semid, 1);  // Esperar al semáforo 1 (el hijo espera)
            printf("0");
            fflush(stdout);
            sem_post_op(semid, 0);  // Liberar el semáforo 0 (permitir al padre imprimir)
        }

    } else { // Proceso padre
        for (int i = 0; i < 100; i++) {
            sem_wait_op(semid, 0);  // Esperar al semáforo 0 (el padre espera)
            printf("1");
            fflush(stdout);
            sem_post_op(semid, 1);  // Liberar el semáforo 1 (permitir al hijo imprimir)
        }

        // Esperar a que termine el proceso hijo
        wait(NULL);
       if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("Error al eliminar el conjunto de semáforos");
        exit(1);
    }
    }

    // Eliminar el conjunto de semáforos al finalizar
 

    return 0;
}
