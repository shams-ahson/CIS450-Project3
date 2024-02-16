/*
 * boundedBuffer.c
 *
 * A complete example of simple producer/consumer program. The Producer
 * and Consumer functions are executed as independent threads.  They
 * share access to a single buffer.  The producer deposits a sequence
 * of integers from 1 to numIters into the buffer.  The Consumer fetches
 * these values and adds them.  Two semaphores,sem_empty and sem_full are used to
 * ensure that the producer and consumer alternate access to the buffer.
 *
 * This version uses thread coordination to ensure that the
 * consumer only reads valid data and the producer doesn't overwrite unread
 * data.
 */
#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>

using namespace std;

const int MAX = 100; // the maximum possible value of the internal counter for semaphores

int buffer; // shared buffer, size = 1
int numIters;

// deposit 1, ..., numIters into the data buffer
void Producer(counting_semaphore<MAX> &sem_empty, counting_semaphore<MAX> &sem_full)
{
    for (int produced = 0; produced < numIters; produced++)
    {
        sem_empty.acquire();
        buffer = produced;
        sem_full.release();
    }
}

// fetch numIters items from the buffer and sum them
void Consumer(counting_semaphore<MAX> &sem_empty, counting_semaphore<MAX> &sem_full)
{
    int total = 0;

    for (int consumed = 0; consumed < numIters; consumed++)
    {
        sem_full.acquire();
        total = total + buffer;
        sem_empty.release();
    }

    printf("the total is %d\n", total);
}

// main() -- read command line and create threads
int main(int argc, char *argv[])
{
    thread producer, consumer;

    if (argc < 2)
    {
        printf("Usage: boundedBuffer <Number of Iterations>\n");
        exit(0);
    }
    numIters = atoi(argv[1]);

    counting_semaphore<MAX> sem_empty(1);
    counting_semaphore<MAX> sem_full(0);

    producer = thread(Producer, ref(sem_empty), ref(sem_full));
    consumer = thread(Consumer, ref(sem_empty), ref(sem_full));

    producer.join();
    consumer.join();
    exit(0);
}
