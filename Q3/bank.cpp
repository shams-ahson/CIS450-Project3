#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

class Bank {
    private:
        int balance = 0;
        condition_variable okToWithdraw;
        mutex mtx;
    
    public:
        void deposit(int amount) {
            unique_lock<mutex> lock(mtx);
            balance = balance + amount;
            okToWithdraw.notify_all();
        }
        void withdraw(int amount) {
            unique_lock<mutex> lock(mtx);
            while (amount > balance) {
                okToWithdraw.wait(lock);
            }
            balance = balance - amount;
        }
        int getBalance() {
            return balance;
        }
};

void customerDeposit(int amount, Bank& acct) {
    //while(!end) {
    int lunchtime = rand() % 5 + 1;
    acct.deposit(amount);
    this_thread::sleep_for(chrono::seconds(lunchtime));
    //}
}

void customerWithdraw(int amount, Bank& acct) {
   // while(!end) {
    int lunchtime = rand() % 5 + 1;
    acct.withdraw(amount);
    this_thread::sleep_for(chrono::seconds(lunchtime));
   // }
}

int main() { //change to use command line args
    Bank myBank;
    thread customer1(customerDeposit, 100, ref(myBank));
    thread customer2(customerWithdraw, 51, ref(myBank));
    //this_thread::sleep_for(chrono::seconds(10));
    customer1.join();
    customer2.join();
    cout << "Balance: " << myBank.getBalance() << endl;
    
    return 0;
}