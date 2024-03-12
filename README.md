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

gcc bridge.cpp -o bridge
./bridge

Sample outputs:
```plaintext
Car 25 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 1.
Car 25 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 1.
Car 20 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 2.
Car 20 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 2.
Car 34 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 34 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 34 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 20 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 1.
Car 25 traveling in direction 1 has exited from the bridge. CurrDir: 0. #Cars: 0.
Car 41 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 1.
Car 41 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 2.
Car 7 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 2.
Car 7 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 18 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 18 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 7 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 18 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 1.
Car 36 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 2.
Car 36 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 2.
Car 41 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 1.
Car 29 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 2.
Car 29 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 2.
Car 36 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 1.
Car 29 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 0.
Car 44 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 1.
Car 44 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 2.
Car 8 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 2.
Car 8 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 13 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 13 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 13 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 16 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 16 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 8 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 37 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 37 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 44 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 45 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 45 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 2.
Car 16 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 37 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 1.
Car 10 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 2.
Car 10 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 2.
Car 3 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 3 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 45 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 3 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 1.
Car 10 traveling in direction 1 has exited from the bridge. CurrDir: 0. #Cars: 0.
Car 33 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 1.
Car 33 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 2.
Car 5 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 2.
Car 5 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 6 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 6 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 5 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 21 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 21 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 2.
Car 33 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 19 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 19 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 6 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 46 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 46 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 46 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 21 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 1.
Car 19 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 0.
Car 24 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 1.
Car 24 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 2.
Car 4 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 2.
Car 4 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 26 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 26 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 26 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 22 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 22 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 4 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 24 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 1.
Car 22 traveling in direction 1 has exited from the bridge. CurrDir: 0. #Cars: 0.
Car 30 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 1.
Car 30 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 2.
Car 12 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 2.
Car 12 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 39 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 39 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 39 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 28 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 28 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 12 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 30 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 1.
Car 49 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 2.
Car 49 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 2.
Car 48 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 48 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 28 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 11 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 11 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 48 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 27 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 27 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 49 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 15 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 15 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 27 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 32 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 32 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 11 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 15 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 1.
Car 38 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 2.
Car 38 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 2.
Car 47 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 3.
Car 47 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 3.
Car 32 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 2.
Car 38 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 1.
Car 2 traveling in direction 0 has arrived at the bridge. CurrDir: 0. #Cars: 2.
Car 2 traveling in direction 0 is crossing the bridge. CurrDir: 0. #Cars: 1.
Car 47 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 1.
Car 2 traveling in direction 0 has exited from the bridge. CurrDir: 0. #Cars: 0.
Car 31 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 1.
Car 31 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 2.
Car 0 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 2.
Car 0 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 1 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 1 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 1 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 43 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 43 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 31 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 23 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 23 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 43 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 35 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 35 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 0 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 40 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 40 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 23 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 17 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 17 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 40 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 35 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 1.
Car 9 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 2.
Car 9 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 14 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 14 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 17 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 42 traveling in direction 1 has arrived at the bridge. CurrDir: 1. #Cars: 3.
Car 42 traveling in direction 1 is crossing the bridge. CurrDir: 1. #Cars: 3.
Car 9 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 2.
Car 14 traveling in direction 1 has exited from the bridge. CurrDir: 1. #Cars: 1.
Car 42 traveling in direction 1 has exited from the bridge. CurrDir: 0. #Cars: 0.
```

## Question 3 Part A

### Implementation

```plaintext
Question 3 Part A implements a monitor that represents the savings account that is shared by numerous customers. Each customer can deposit and withdraw, only if there is enough in the balance for withdrawal to occur. A mutex lock is used to ensure mutual exclusion of threads.

```

### Pseudocode (given by professor)
```plaintext
    Monitor Bank {
        int balance = 0
        cond okToWithdraw;
        void Deposit(int amount) {
            balance = balance + amount;
            okToWithdraw->broadcast();
        }
        void Withdraw(int amount) {
            //wait for funds
            while (amount > balance) {
                okToWithdraw->wait();
            }
            balance = balance – amount;
        }
    }
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
Question 3 Part B implements the monitor but adds the First Come First Serve to all withdrawals. This is a FIFO structure implemented, as withdrawls must wait until sufficent balance is encountered, but waiting withdrawals are prioritized to complete prior to new ones.
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
In the `launch.json` file, you should be able to change the args:

```json
"args": ["Number of customers"]
```

To compile and run the program:

```bash
gcc FCFS_bank.cpp -o FCFS_bank
./FCFS_bank <Number of Customers>

```bash

##TEST CASE 1: Basic Functionality with 4 customers
$ ./FCFS_bank -o FCFS_bank 4
Customer 1 wants to deposit: 54, Current Balance: 0
Customer 1 deposited: 54, Current Balance: 54
Customer 3 wants to withdraw: 73, Current Balance: 54
Customer 2 wants to deposit: 21, Current Balance: 54
Customer 2 deposited: 21, Current Balance: 75
Customer 3 withdrew: 73, Current Balance: 2
Customer 4 wants to deposit: 8, Current Balance: 2
Customer 4 deposited: 8, Current Balance: 10
Final Balance: 10
##TEST CASE VERDICT: PASS

##TEST CASE 2: Partial withdrawal with 3 customers
$ ./FCFS_bank -o FCFS_bank 3
Customer 1 wants to deposit: 74, Current Balance: 0
Customer 2 wants to withdraw: 173, Current Balance: 74
Customer 3 wants to deposit: 4, Current Balance: 74
Customer 3 deposited: 4, Current Balance: 78
Customer 2 withdrew: 73, Current Balance: 5
Final Balance: 5
##TEST CASE VERDICT: PASS

##TEST CASE 3: Large number of customers (10)
$ ./FCFS_bank -o FCFS_bank 10
Customer 2 wants to deposit: 96, Current Balance: 0
Customer 2 deposited: 96, Current Balance: 96
Customer 7 wants to withdraw: 85, Current Balance: 96
Customer 7 withdrew: 85, Current Balance: 11
Customer 4 wants to withdraw: 27, Current Balance: 11
Customer 6 wants to withdraw: 66, Current Balance: 11
Customer 9 wants to deposit: 57, Current Balance: 11
Customer 9 deposited: 57, Current Balance: 68
Customer 4 withdrew: 27, Current Balance: 41
Customer 10 wants to withdraw: 54, Current Balance: 41
Customer 8 wants to deposit: 90, Current Balance: 41
Customer 8 deposited: 90, Current Balance: 131
Customer 6 withdrew: 66, Current Balance: 65
Customer 10 withdrew: 54, Current Balance: 11
Customer 1 wants to deposit: 26, Current Balance: 11
Customer 1 deposited: 26, Current Balance: 37
Customer 3 wants to withdraw: 2, Current Balance: 37
Customer 3 withdrew: 2, Current Balance: 35
Customer 5 wants to deposit: 62, Current Balance: 35
Customer 5 deposited: 62, Current Balance: 97
Final Balance: 97
##TEST CASE VERDICT: PASS

##TEST CASE 4: Single Customer
$ ./FCFS_bank -o FCFS_bank 1
Customer 1 wants to deposit: 32, Current Balance: 0
Customer 1 deposited: 32, Current Balance: 32
Final Balance: 32
##TEST CASE VERDICT: PASS

##TEST CASE 5: Deposit after Request for Withdrawal with 2 customers
$ ./FCFS_bank -o FCFS_bank 2
Customer 2 wants to withdraw: 80, Current Balance: 0
Customer 1 wants to deposit: 85, Current Balance: 0
Customer 1 deposited: 85, Current Balance: 85
Customer 2 withdrew: 80, Current Balance: 5
Final Balance: 5
##TEST CASE VERDICT: PASS
```