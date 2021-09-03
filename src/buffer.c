#include <buffer.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))


int get_buffer_std_size()
{
    return BUFFER_STD_SIZE;
}

tbuffer* iniciabuffer (int numpos, int numprod, int numcons)
{
    tbuffer* b = (tbuffer*) malloc(sizeof(tbuffer));
    b->numpos = numpos;
    b->numprod = numprod;
    b->numcons = numcons;
    b->free_slots = numpos;
    b->nxt_free = createQueue(numpos);
    b->to_read = (int*) malloc(sizeof(int) * numpos);
    b->data = (int*) malloc(sizeof(int) * numpos);

    for (int i = 0; i < b->numpos; i++) {
        b->data[i] = -1;
        b->to_read[i] = -1;
    }

    //Queues
    for (int i = 0; i < b->numpos; i++) {enqueue(b->nxt_free, i);}
    b->nxtdata = (Queue **)malloc(sizeof(Queue*) * b->numcons);
    for (int i = 0; i < b->numcons; i++) {b->nxtdata[i] = createQueue(numpos);}
    sem_init(&(b->mutex), 0, 1);

    return b;
}

tbuffer* iniciabuffer_pre (int numpos, int numprod, int numcons, int numinsertions, int data[])
{
    tbuffer* b = iniciabuffer(numpos, numprod, numcons);
    for (int i = 0; i < numinsertions; i++) {deposita(b, data[i]);}
    return b;
}

/**
 *
 * process Producer
 * {
 *      int data;
 *      while (true)
 *      {
 *          <await free_slots > 0;>
 *          buf[nxt_free] = data;
 *          nxt_free = (nxt_free + 1) % SIZE;
 *          free_slots--;>
 *      }
 * } 
 * 
 * 
 */
void deposita (tbuffer* buffer, int item)
{   
    if(buffer->free_slots > 0)
    {
        
        int idx = dequeue(buffer->nxt_free);
        for (int i = 0; i < buffer->numcons; i++) {enqueue(buffer->nxtdata[i], idx);}
        buffer->data[idx] = item;
        buffer->free_slots--;
        buffer->to_read[idx] = buffer->numcons;
    }
}

int consome (tbuffer* buffer, int meuid)
{
    int data = -1;

    if(buffer->free_slots < buffer->numpos && !isEmpty(buffer->nxtdata[meuid]))
    {
        int idx = dequeue(buffer->nxtdata[meuid]);
        data = buffer->data[idx];
        (buffer->to_read[idx])--;
        if(buffer->to_read[idx] == 0)
        {
            buffer->free_slots++;
            buffer->data[idx] = -1;
            buffer->to_read[idx] = -1;
            enqueue(buffer->nxt_free, idx);
        }
    }
    return data;
}


void finalizabuffer (tbuffer* buffer)
{
    sem_destroy(&(buffer->mutex));
    free(buffer);
}