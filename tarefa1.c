#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include <buffer.h>
#include <queue.h>
  
typedef struct deposita_args { tbuffer* buffer; int item; int insertions;} deposita_args;
typedef struct consome_args { tbuffer* buffer; int id; int consome} consome_args;

void* deposita_thread(void* arg)
{
    pid_t tid = syscall(SYS_gettid);

    deposita_args* args= (deposita_args*) arg;
    int count = args->insertions;
    while (count > 0)
    {
        sleep(rand() % 5);
        int data = args->item * (rand() % 100);
        
        //wait
        sem_wait(&(args->buffer->mutex));
        deposita(args->buffer, data);
        count--;

        printf("%d::%d -> Buffer[ ", tid, data);
        for(int i = 0; i < args->buffer->numpos; i++)
        {
            char str[10];
            sprintf(str, "%d", args->buffer->data[i]);
            char* symbol = args->buffer->data[i] >= 0 ? str : "*";
            printf("%s ", symbol);
        }
        printf(" ] ( free slots: %d next free: %d )\n", args->buffer->free_slots, front(args->buffer->nxt_free));
        //signal
        sem_post(&(args->buffer->mutex));
    }
    return NULL;
}

void* consome_thread(void* arg)
{
    pid_t tid = syscall(SYS_gettid);

    consome_args* args= (consome_args*) arg;
    
    int count = args->consome;
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
    

    pthread_t* prod_thds = (pthread_t*)malloc(sizeof(pthread_t) * P); 
    pthread_t* cons_thds = (pthread_t*)malloc(sizeof(pthread_t) * C); 
    tbuffer* buffer = iniciabuffer(N, P, C);
    
    //Deposita
    deposita_args* d_arg = (deposita_args *) malloc(sizeof(deposita_args) * P);
    for (int i = 0; i < P; i++)
    {
        //Produz
        d_arg[i].buffer = buffer;
        d_arg[i].item = rand() % 100;
        d_arg[i].insertions = I;
        pthread_create(&(prod_thds[i]), NULL, deposita_thread, &d_arg[i]);
    }

    sleep(5);
    consome_args* c_arg = (consome_args *) malloc(sizeof(consome_args) * C);
    for (int i = 0; i < C; i++)
    {
        //Consome
        c_arg[i].buffer = buffer;
        c_arg[i].id = i;
        c_arg[i].consome = P * I;
        pthread_create(&(cons_thds[i]), NULL, consome_thread, &c_arg[i]);
    }

    for (int i = 0; i < P; i++) {pthread_join(prod_thds[i], NULL);}
    for (int i = 0; i < C; i++) {pthread_join(cons_thds[i], NULL);}

    finalizabuffer(buffer);
    return 0;
}