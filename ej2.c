#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pipefd[10][2];  // Array para la tubería (pipe)
    __pid_t pid;
    for (int i = 0; i < 10; i++)
    {
    if (pipe(pipefd[i][0])==-1){
        perror("Error tuberias");
        exit(1);
    }
    }
    
    pid=fork();
    if (pid==-1)
    {
        perror("Error al crear hijo");
        exit(1);
    }
  
    




    
    return 0;
}
