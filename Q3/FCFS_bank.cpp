#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>
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
        }
        int getBalance() {
            return balance;
        }
};

void customerDeposit(int amount, FCFS_Bank& acct) {
    //while(!end) {
    int lunchtime = rand() % 5 + 1;
    acct.deposit(amount);
    this_thread::sleep_for(chrono::seconds(lunchtime));
    //}
}

void customerWithdraw(int amount, FCFS_Bank& acct) {
   // while(!end) {
    int lunchtime = rand() % 5 + 1;
    acct.withdraw(amount);
    this_thread::sleep_for(chrono::seconds(lunchtime));
   // }
}

int main() {
    FCFS_Bank myBank;
    thread customer1(customerDeposit, 100, ref(myBank));
    thread customer2(customerWithdraw, 51, ref(myBank));
    //this_thread::sleep_for(chrono::seconds(10));
    customer1.join();
    customer2.join();
    cout << "Balance: " << myBank.getBalance() << endl;
    
    return 0;
}