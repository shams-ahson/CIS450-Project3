/*
 * Author: Eejoy Lim
 * Creation Date: 1/26/2023
 * Last Modification Date: 2/3/2023
 * Purpose: This program represents a saving account problem which a saving account is shared by several people.
 *  Each person can either deposit or withdraw from the shared account. However, a person is not allowed to withdraw
 *  if the balance of the account is less than the amount that the person wish to withdraw and the person has to wait.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_AMOUNT 500
#define MAX_LUNCH_TIME 10
#define MAX_TRANSACTION 50

// monitor Account
typedef struct Account {
    pthread_cond_t withdraw_cv;
    pthread_mutex_t mutex;
    int balance;
} Account;

// Args to be passed to the routine as parameter
typedef struct Args {
    Account* account;
    int id;
} Args;

void deposit(Account*, int, int, int);
void withdraw(Account*, int, int, int);
void* customer(void*);

// create threads
int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t* cus;

    if (argc < 2) {
        printf("Usage: savingAccount <Number of Customers>\n");
        exit(0);
    }
    int numOfCus = atoi(argv[1]);
    cus = (pthread_t*)malloc(numOfCus * sizeof(pthread_t));

    // intialization
    Account account;
    account.balance = 0;
    pthread_cond_init(&account.withdraw_cv, NULL);
    pthread_mutex_init(&account.mutex, NULL);

    Args* args = (Args*)malloc(numOfCus * sizeof(Args));
    for(int i = 0; i < numOfCus; i++) {
        args[i].account = &account;
        args[i].id = i;
        pthread_create(&cus[i], NULL, customer, (void*) &args[i]);
    }
    
    for(int i = 0; i < numOfCus; i++) {
        pthread_join(cus[i], NULL);
    }

    pthread_cond_destroy(&account.withdraw_cv);
    pthread_mutex_destroy(&account.mutex);
}

// deposit money into account
void deposit(Account* account, int amount, int id, int i) {
    pthread_mutex_lock(&account->mutex);
    account->balance += amount;
    printf("Thread %d Transaction %d: Deposit %d - Current Balance %d\n", id, i, amount, account->balance);
    pthread_cond_broadcast(&account->withdraw_cv);
    pthread_mutex_unlock(&account->mutex);
    sched_yield();
}

// withdraw money from account
void withdraw(Account* account, int amount, int id, int i) {
    pthread_mutex_lock(&account->mutex);

    // wait if the balance is insufficient
    while(account->balance < amount) {
        printf("Thread %d Transaction %d fails to withdraw %d\n", id, i, amount);
        pthread_cond_wait(&account->withdraw_cv, &account->mutex);
    }

    account->balance -= amount;
    printf("Thread %d Transaction %d: Withdraw %d - Current Balance %d\n", id, i, amount, account->balance);
    pthread_mutex_unlock(&account->mutex);
    sched_yield();
}

// allow customer to perform transaction
void* customer(void* args) {
    Account* account = ((Args*)args)->account;
    int id = ((Args*)args)->id;
    int numOfTrans = rand() % MAX_TRANSACTION + 1;
    int i = 0;
    int action = rand() % 2;
    while(i < numOfTrans) {
        int amount = rand() % MAX_AMOUNT + 1;
        int lunchTime = rand() % MAX_LUNCH_TIME;
        if (action == 0) {
            deposit((Account*) account, amount, id, i);
        }
        else {
            withdraw((Account*) account, amount, id, i);
        }
        sleep(lunchTime);
        i++;
    }
}