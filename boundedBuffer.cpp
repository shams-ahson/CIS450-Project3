#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

const int MAX = 100; // the maximum possible value of the internal counter for semaphores
int* buffer; // shared buffer, size = n
int numIters;
int buffSize; 
int front = 0;
int rear = 0;

// deposit 1, ..., numIters into the data buffer
void Producer(counting_semaphore<MAX> &sem_empty, counting_semaphore<MAX>&sem_full, int id)
{
    for (int produced = 0; produced < numIters; produced++)
    {
    sem_empty.acquire();
    buffer[rear] = produced;
    rear = (rear + 1) % buffSize;
    sem_full.release();
    }
}
// fetch numIters items from the buffer and sum them
void Consumer(counting_semaphore<MAX> &sem_empty, counting_semaphore<MAX>&sem_full, int id)
{
    int total = 0;
    for (int consumed = 0; consumed < numIters; consumed++)
    {
    sem_full.acquire();
    total += buffer[front];
    front = (front + 1) % buffSize;
    sem_empty.release();
    }
    printf("the total is %d\n", total);
}
// main() -- read command line and create threads
int main(int argc, char *argv[])
{
    thread producer, consumer;
    if (argc < 3)
    {
    printf("Usage: boundedBuffer <Number of Iterations>\n");
    exit(0);
    }
    numIters = atoi(argv[1]);
    buffSize = atoi(argv[2]);
    buffer = new int[buffSize]; // dynamically allocate memory

    counting_semaphore<MAX> sem_empty(1);
    counting_semaphore<MAX> sem_full(0);
    vector<thread> producers;
    vector<thread> consumers;

    // Create 3 producers and 3 consumers
    for (int i = 0; i < 3; i++)
    {
        producers.emplace_back(Producer, ref(sem_empty), ref(sem_full), i);
        consumers.emplace_back(Consumer, ref(sem_empty), ref(sem_full), i);
    }

    // Join producer and consumer threads
    for (auto &producer : producers)
    {
        producer.join();
    }

    for (auto &consumer : consumers)
    {
        consumer.join();
    }

    delete[] buffer;
    exit(0);
}
