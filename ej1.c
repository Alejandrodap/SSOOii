#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pipefd[2];  // Array para la tubería (pipe)
    __pid_t pid;
    char men[]="Pablo goat el mas putero\n";

    if (pipe(pipefd)==-1){
        perror("Error tuberias");
        exit(1);
    }

    pid=fork();
    if (pid==-1)
    {
        perror("Error al crear hijo");
        exit(1);
    }
    if(pid==0){
        close(pipefd[1]);
        char buffer[100];
        read(pipefd[0],buffer,sizeof(buffer));
        printf("Hijo: %s",buffer);
        close(pipefd[0]);
        exit(0);
    }else{
        close(pipefd[0]);
        write(pipefd[1],men,sizeof(men));
        printf("Padre mando mensaje");
        close(pipefd[1]);
        wait(NULL);
    }
    




    
    return 0;
}
