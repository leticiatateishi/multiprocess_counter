#include <stdio.h>
#include <unistd.h>     // fork
#include <sys/wait.h>   // waitpid
#include <sys/mman.h>   // mmap

#define NUM_PROCESSOS 4

int primo(int numero){
    if (numero < 2)
        return 0;
    for (int i = 2; i <= (numero/2); i++){
        if (numero%i == 0)
            return 0;
    }
    return 1;
}

int main() {

    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANON;

    int *mem;
    mem = (int*) mmap(NULL, sizeof(int)*1000, protection, visibility, 0, 0);

    int *posicao = &(mem[0]);
    (*posicao) = 2;
    int *resultado = &(mem[1]);

    while(scanf(" %d ", &(mem[*posicao])) != EOF) {
        (*posicao)++;
    }

    int pid_pai = getpid();

    int pid[NUM_PROCESSOS];
    for (int i = 0; i < NUM_PROCESSOS; i++) {
        pid[i] = fork();
        if (pid[i] == 0) {

            break;
        }
    }

    while (1){
        int pos = (*posicao)--;
        if (pos < 2)
            break;
        if (primo(mem[pos]) == 1)
            (*resultado)++;
    }

    if (pid_pai == getpid()) {
        for (int i = 0; i < NUM_PROCESSOS; i++) {
            waitpid(pid[i], NULL, 0);
        }
        printf("%d\n", (*resultado));
    }

    return 0;
}
