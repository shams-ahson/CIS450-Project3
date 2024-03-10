/*
 * boundedBuffer.c
 *
 * A complete example of simple producer/consumer program. The Producer
 * and Consumer functions are executed as independent threads.  They
 * share access to a single buffer, data.  The producer deposits a sequence
 * of integers from 1 to numIters into the buffer.  The Consumer fetches
 * these values and adds them.  Two semaphores,empty and full are used to
 * ensure that the producer and consumer alternate access to the buffer.
 *
 * SOURCE: adapted from example code in "Multithreaded, Parallel, and
 *         Distributed Programming" by Gregory R. Andrews.
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define SHARED 1
#define NUM 3            // number of producers and consumers

void *Producer (void *); // the two threads
void *Consumer (void *);

sem_t empty, full;       // global semaphores
pthread_mutex_t mutex;   // global mutex
int* buffer;             // shared buffer
int buffSize;
int numIters;
int front = 0;
int rear = 0;

// main() -- read command line and create threads
int main(int argc, char *argv[]) {
    pthread_t pid[NUM], cid[NUM];

    if (argc < 3) {
	    printf("Usage: boundedBuffer <Number of Iterations> <Buffer Size>\n");
	    exit(0);
    }
    numIters = atoi(argv[1]);
    buffSize = atoi(argv[2]);
    buffer = (int*)malloc(buffSize * sizeof(int)); // dynamically allocate memory

    sem_init(&empty, SHARED, buffSize);    // sem empty = 1
    sem_init(&full, SHARED, 0);            // sem full = 0
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < NUM; i++) {
        pthread_create(&pid[i], NULL, Producer, NULL);
        pthread_create(&cid[i], NULL, Consumer, NULL);
    }
    
    for(int i = 0; i < NUM; i++) {
        pthread_join(pid[i], NULL);
        pthread_join(cid[i], NULL);
    }
    
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    pthread_exit(0);
}

// deposit 1, ..., numIters into the data buffer
void *Producer(void *arg) {
    int produced;

    for (produced = 0; produced < numIters; produced++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[rear] = produced;
        rear = (rear + 1) % buffSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

//fetch numIters items from the buffer and sum them
void *Consumer(void *arg) {
    int total = 0;
    int consumed;

    for (consumed = 0; consumed < numIters; consumed++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        total += buffer[front];
        front = (front + 1) % buffSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }

    printf("the total is %d\n", total);
}
