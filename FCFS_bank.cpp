#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>
#include<vector>
using namespace std;

class FCFS_Bank {
    private:
        int balance = 0;
        int numWithdraw = 0;
        condition_variable okToWithdraw;
        condition_variable okToBank;
        mutex mtx;
    
    public:
        void deposit(int amount) {
            unique_lock<mutex> lock(mtx);
            balance = balance + amount;
            okToWithdraw.notify_all();
        }
        void withdraw(int amount) {
            unique_lock<mutex> lock(mtx);
            numWithdraw++;
            
            if (numWithdraw > 1) {
                okToBank.wait(lock);
            }
            while (amount > balance) {
                okToWithdraw.wait(lock);
            }
            balance = balance - amount;
            numWithdraw--;
            okToBank.notify_one();
            okToWithdraw.notify_all();
        }
        int getBalance() {
            return balance;
        }
};

void customerDeposit(int amount, FCFS_Bank& acct, int customerID) {
    int lunchtime = rand() % 5 + 1; //randomize time away

    cout << "Customer " << customerID << " wants to deposit: " << amount << ", Current Balance: " << acct.getBalance() << endl; //prior to transaction
    acct.deposit(amount); //deposit
    cout << "Customer " << customerID<< " deposited: " << amount << ", Current Balance: " << acct.getBalance() << endl; //after transaction

    this_thread::sleep_for(chrono::seconds(lunchtime)); //sleep for lunchtime
}

void customerWithdraw(int amount, FCFS_Bank& acct, int customerID) {
    int lunchtime = rand() % 5 + 1; //randomize time away
    cout << "Customer " << customerID << " wants to withdraw: " << amount << ", Current Balance: " << acct.getBalance() << endl; //prior to transaction
    acct.withdraw(amount); //withdraw
    cout << "Customer " << customerID << " withdrew: " << amount << ", Current Balance: " << acct.getBalance() << endl; //after transaction

    this_thread::sleep_for(chrono::seconds(lunchtime)); //sleep for lunchtime
}

int main(int argc, char* argv[]) {
    if(argc != 2) { //check for appropriate command line args
        cout << "Usage: bank <# of customers>" << endl;
        return 1;
    }
    int numCustomers = atoi(argv[1]); //get # of customers
    srand(static_cast<unsigned int>(time(0))); //add seed to randomize
    FCFS_Bank myBank; //create Bank object
    vector<thread> customers; //create vector of all customer threads to be populated

    for (int i = 1; i <= numCustomers; ++i) {
        int depositAmt = rand() % 101;  //rand deposit amount
        int withdrawAmt = rand() % 101; //rand withdraw amount
        if (rand() % 2 == 0) { //if deposit's turn, call deposit
            customers.emplace_back(customerDeposit, depositAmt, ref(myBank), i);
        } else { //if withdraws's turn, call withdraw
            customers.emplace_back(customerWithdraw, withdrawAmt, ref(myBank), i);
        }
    }

    for (auto& thread : customers) {
        thread.join(); //join all customer threads
    }

    cout << "Final Balance: " << myBank.getBalance() << endl; //display final balance
    
    return 0;
}