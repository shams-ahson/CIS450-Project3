# CIS450-Project3

This repository contains the implementation and documentation for Project 3 in CIS 450.

## Question 1

### Implementation

```cpp
// Declare constants
const MAX = 100

// Declare global variables
int[] buffer
int numIters
int buffSize
int front = 0
int rear = 0

// Producer function
function Producer(sem_empty, sem_full, id)
    for produced in 0 to numIters - 1
        sem_empty.acquire()
        buffer[rear] = produced
        rear = (rear + 1) % buffSize
        sem_full.release()

// Consumer function
function Consumer(sem_empty, sem_full, id)
    total = 0
    for consumed in 0 to numIters - 1
        sem_full.acquire()
        total += buffer[front]
        front = (front + 1) % buffSize
        sem_empty.release()
    print("the total is", total)

// Main function
function main(argc, argv)
    // Check command line arguments
    if argc < 3
        print("Usage: boundedBuffer <Number of Iterations>")
        exit(0)

    // Parse command line arguments
    numIters = atoi(argv[1])
    buffSize = atoi(argv[2])

    // Dynamically allocate memory for the buffer
    buffer = new int[buffSize]

    // Initialize semaphores
    sem_empty = counting_semaphore<MAX>(1)
    sem_full = counting_semaphore<MAX>(0)

    // Create vectors to store producer and consumer threads
    producers = new vector<thread>()
    consumers = new vector<thread>()

    // Create 3 producers and 3 consumers
    for i in 0 to 2
        producers.emplace_back(Producer, ref(sem_empty), ref(sem_full), i)
        consumers.emplace_back(Consumer, ref(sem_empty), ref(sem_full), i)

    // Join producer and consumer threads
    for each producer in producers
        producer.join()

    for each consumer in consumers
        consumer.join()

    // Deallocate memory
    delete[] buffer
    exit(0)
```

### Run the program

In the `launch.json` file, you should be able to change the args:

```json
"args": ["Number of iterations", "Buffer size"]
```

To compile and run the program:

```bash
gcc boundedBuffer.cpp -o boundedBuffer
./boundedBuffer <Number of Iterations> <Buffer Size>
```

Sample outputs:

```bash
$ ./boundedBuffer 100 100
the total is 4950
the total is 4950
the total is 4950

$ ./boundedBuffer 100 10
the total is 2978
the total is 4159
the total is 7713

$ ./boundedBuffer 1000 10
the total is 440711
the total is 415469
the total is 642320
```

## Question 2

### Implementation
This is the pseudocode provided in this Assignment.
```plaintext
void OneVehicle() {
int direc = rand() % 2;
ArriveBridge(direc);
CrossBridge(direc);
ExitBridge(direc);
}
void CrossBridge (int direc) {
//crossing the bridge;
}
monitor Bridge {
Condition safe;
int currentDirec;
int currentNumber;
void ArriveBridge(int direc) {
while (!isSafe(direc) )
safe.Wait();
currentNumber++;
currentDirec = direc;
}
void ExitBridge(Bridge::Direction direc) {
currentNumber--;
safe.Broadcast();
}
bool isSafe(int direc) {
if ( currentNumber == 0 )
return TRUE; // always safe when bridge is empty
else if ((currentNumber < 3) && (currentDirec == direc))
return TRUE; // room for us to follow others in direc
else
return FALSE; // bridge is full or has oncoming traffic.
}
}

```
Implement the pseudocode in C++ and create the main function to initialize all the values of the monitor and create 50 child threads.

### Run the program

```plaintext
// Instructions to run the program for Question 2
```

## Question 3 Part A

### Implementation

```plaintext
Question 3 Part A implements a monitor that represents the savings account that is shared by numerous customers. Each customer can deposit and withdraw, only if there is enough in the balance for withdrawal to occur. A mutex lock is used to ensure mutual exclusion of threads.

```

### Pseudocode (given by professor)
```plaintext
    

```

### Run the program: Sample Outputs

In the `launch.json` file, you should be able to change the args:

```json
"args": ["Number of customers"]
```

To compile and run the program:

```bash
gcc bank.cpp -o bank
./bank <Number of Customers>
```

Sample outputs:

```bash
$ ./bank 3
Customer 1 deposited: 99
Customer 2 withdrew: 7
Customer 3 withdrew: 31
Final Balance: 61

$ ./bank 2
Customer 1 deposited: 89
Customer 2 withdrew: 14
Final Balance: 75

$ ./bank 5
Customer 1 deposited: 89
Customer 2 withdrew: 72
Customer 5 deposited: 43
Customer 3 withdrew: 1
Customer 4 withdrew: 21
Final Balance: 38
```

## Question 3 Part B

### Implementation

```plaintext
Question 3 Part B implements the monitor but adds the First Come First Serve to all withdrawals. This is a FIFO structure implemented. 
```

### Pseudocode (given by professor)
```plaintext
    Monitor FCFSBank {
        int balance = 0;
        int numOfWithdraw = 0;
        cond okToWithdraw;
        cond okToBank;
        void Deposit(int amount) {
            balance = balance + amount;
            okToWithdraw->signal(); //it is also fine,
            okToWithdraw->broadcast();
            }
        void Withdraw(int amount) {
            numOfWithdraw++;
        //Somebody is ahead, wait for turn
            if (numOfWithdraw > 1) { //it cannot be a while
        loop
            okToBank->wait();
            }
        //wait for funds
            while (amount > balance) {
                okToWithdraw->wait();
            }
            balance = balance â amount;
            numOfWithdraw--;
            okToBank->signal(); // signal the next withdraw
            }
        }
```

### Run the program: Sample Outputs