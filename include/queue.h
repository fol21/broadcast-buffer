#ifndef QUEUE_H
#define QUEUE_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>  

  
// A structure to represent a FIFO queue
// enqueue ---> |rear|*|*|*|*|front| -----> dequeue
typedef struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;
  
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
Queue* createQueue(unsigned capacity);
  
// Queue is full when size becomes
// equal to the capacity
bool isFull(struct Queue* queue);
  
// Queue is empty when size is 0
bool isEmpty(struct Queue* queue);
  
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item);

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue);

// Function to get front of queue
int front(struct Queue* queue);

// Function to get rear of queue
int rear(struct Queue* queue);

#endif // !QUEUE_H

// int main()
// {
//     struct Queue* queue = createQueue(1000);
  
//     enqueue(queue, 10);
//     enqueue(queue, 20);
//     enqueue(queue, 30);
//     enqueue(queue, 40);
  
//     printf("%d dequeued from queue\n\n",
//            dequeue(queue));
  
//     printf("Front item is %d\n", front(queue));
//     printf("Rear item is %d\n", rear(queue));
  
//     return 0;
// }