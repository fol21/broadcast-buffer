#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

#include <buffer.h>
      
int main(int argc, char *argv[])
{
    // Argument Input
    int N = (int) strtol(argv[1], NULL, 10);
    int P = (int) strtol(argv[2], NULL, 10);
    int C = (int) strtol(argv[3], NULL, 10);
    int I = (int) strtol(argv[4], NULL, 10);
    
    printf("Parametros N, P, C, I = %d %d %d %d\n", N, P, C, I);
    tbuffer* buffer = iniciabuffer(N, P, C);

    printf("Buffer [ ");
    for(int i = 0; i < buffer->numpos; i++)
    {
        char str[10];
        sprintf(str, "%d", buffer->data[i]);
        char* symbol = buffer->data[i] >= 0 ? str : "*";
        printf("%s ", symbol);
    }
    printf(" ] ( free slots: %d next free: %d )\n", buffer->free_slots, front(buffer->nxt_free));

    finalizabuffer(buffer);
    return 0;
}