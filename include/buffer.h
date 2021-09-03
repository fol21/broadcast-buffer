/**
 * 
 * Usando a técnica de passagem de bastão e a modelagem proposta por Andrews, 
 * implemente em C e pthreads uma estrutura de dados que é um buffer limitado com N posições,
 * usado para broadcast entre P produtores e C consumidores. Cada produtor deve depositar I ítens e depois terminar execução,
 * e cada consumidor deve consumir P*I itens e terminar sua execução.
 * 
 * (Os valores de N, P, C e I devem ser parâmetros de linha de comando para o programa de teste desenvolvido, nesta ordem.).
 * 
 * Para simplificar, supor que os itens de dados enviados são inteiros.
 * 
 * O buffer oferece operações iniciabuffer, deposita, consome e finalizabuffer, com buffer.h contendo as declarações a seguir.
 * 
 * Ao chamar deposita, o produtor deve ficar bloqueado até conseguir inserir o novo item,
 * e ao chamar consome o consumidor deve ficar bloqueado até conseguir um item para consumir.
 * 
 * Uma posição só pode ser reutilizada quando todos os C consumidores tiverem lido o dado.
 * 
 * Cada consumidor deve receber as mensagens (dados) na ordem em que foram depositadas.
 * 
 * Utilizar o arquivo disponibilizado buffer.h para a interface das funções implementadas.
 * (A função iniciabuffer inicializa as estruturas necessárias para controle de um buffer manipulado pelas demais funções.)
 * 
 * 
 *  
 **/

#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue.h>


#define BUFFER_STD_SIZE 32
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

typedef struct sbuffer
{
    int numpos;
    int numprod;
    int numcons;

    int free_slots;
    Queue* nxt_free;
    Queue** nxtdata;
    int* to_read;
    
    int* data;
    sem_t mutex;

} tbuffer;

int get_buffer_std_size();
tbuffer* iniciabuffer (int numpos, int numprod, int numcons);
tbuffer* iniciabuffer_pre (int numpos, int numprod, int numcons, int numinsertions, int data[]);
void deposita (tbuffer* buffer, int item);
int consome (tbuffer* buffer, int meuid);
void finalizabuffer (tbuffer* buffer);

#endif // !BUFFER_H