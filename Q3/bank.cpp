#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
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

void customerAction(int customerID, Bank& acct) {
    int depositAmt = rand() % 100 + 1;
    int withdrawAmt = rand() % 100 + 1;
    int lunchtime = rand() % 5 + 1;

    if(rand() % 2 == 0) {
        acct.deposit(depositAmt);
        cout << "Customer" << customerID << " Deposited: " << depositAmt << endl;
    }
    else{
        acct.withdraw(withdrawAmt);
        cout << "Customer" << customerID << " Withdrew: " << withdrawAmt << endl;

    }
    this_thread::sleep_for(chrono::milliseconds(lunchtime));
}


int main(int argc, char* argv[]) { //change to use command line args
    if(argc != 2) {
        cout << "Usage: bank <# of customers>" << endl;
        return 1;
    }
    int numCustomers = atoi(argv[1]);
    srand(static_cast<unsigned int>(time(0)));
    Bank myBank;
    vector<thread> customers;
    
    for(int i = 0; i < numCustomers; i++) {
        customers.emplace_back(customerAction, i, ref(myBank));
    }
    for (auto& thread:customers) {
        thread.join();
    }
    cout << "Final Balance: " << myBank.getBalance() << endl;
    
    return 0;
}