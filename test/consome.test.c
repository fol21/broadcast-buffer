#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include <buffer.h>
#include <queue.h>
  
typedef struct consome_args { tbuffer* buffer; int id;} consome_args;

void* consome_thread(void* arg)
{
    pid_t tid = syscall(SYS_gettid);

    consome_args* args= (consome_args*) arg;
    
    int count = 5;
    int data[100];
    int nxt = 0;
    for (int i = 0; i < 100; i++){ data[i] = -1;}
    
    while (count-- > 0)
    {
        sleep(rand() % 2);
        
        //wait
        sem_wait(&(args->buffer->mutex));
        data[nxt] = consome(args->buffer, args->id);
        printf("Consumer %d data: [ ", args->id);
        int it = 0;
        while(data[it] != -1)
        {
            printf("%d ", data[it++]);
        }
        printf(
            "] ( free slots: %d next data: %d )\n",
            args->buffer->free_slots,
            isEmpty(args->buffer->nxtdata[args->id]) ? -1 : front(args->buffer->nxtdata[args->id])
        );

        printf("%d::%d <- Buffer[ ", tid, data[nxt++]);
        for(int i = 0; i < args->buffer->numpos; i++)
        {
            char str[10];
            sprintf(str, "%d", args->buffer->data[i]);
            char* symbol = args->buffer->data[i] >= 0 ? str : "*";
            printf("%s[%d] ", symbol, args->buffer->to_read[i]);
        }
        printf(" ] ( free slots: %d next free: %d )\n", args->buffer->free_slots, front(args->buffer->nxt_free));
        //signal
        sem_post(&(args->buffer->mutex));
    } 
    return NULL;
}
  
  
int main(int argc, char *argv[])
{
    // Argument Input
    int N = (int) strtol(argv[1], NULL, 10);
    int P = (int) strtol(argv[2], NULL, 10);
    int C = (int) strtol(argv[3], NULL, 10);
    int I = (int) strtol(argv[4], NULL, 10);
    printf("Parametros N, P, C, I = %d %d %d %d\n", N, P, C, I);
    

    pthread_t* cons_thds = (pthread_t*)malloc(sizeof(pthread_t) * C); 
    int start[] = {100, 200, 300 , 400 , 500, 600, 700};
    tbuffer* buffer = iniciabuffer_pre(N, P, C, NELEMS(start), start);
    
    consome_args* c_arg = (consome_args *) malloc(sizeof(consome_args) * C);
    for (int i = 0; i < C; i++)
    {
        //Consome
        c_arg[i].buffer = buffer;
        c_arg[i].id = i;
        pthread_create(&(cons_thds[i]), NULL, consome_thread, &c_arg[i]);
    }

    for (int i = 0; i < C; i++) {pthread_join(cons_thds[i], NULL);}
    finalizabuffer(buffer);
    return 0;
}