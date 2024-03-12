#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
using namespace std;

class Bank { 
    //class Bank serves as the savings account
    private:
        int balance = 0; //initialize balance
        condition_variable okToWithdraw;
        mutex mtx; //create mutex lock
    
    public:
        void deposit(int amount) {
            //function for customer to deposit one thread at a time and signals withdrawal 
            unique_lock<mutex> lock(mtx);
            balance = balance + amount;
            okToWithdraw.notify_one(); //signal withdrawals
        }
        void withdraw(int amount) {
            //function for customer to withdraw, waits for mutex and sufficient funds
            unique_lock<mutex> lock(mtx);
            while (amount > balance) { //if not enough funds
                okToWithdraw.wait(lock); //wait for mutex
            }
            balance = balance - amount;
        }
        int getBalance() {
            return balance; //simple getter
        }
};

void customerAction(int customerID, Bank& acct) {
    int depositAmt = rand() % 100 + 1; //randomize deposit amt
    int withdrawAmt = rand() % 100 + 1; //randomize withdraw amt
    int lunchtime = rand() % 5 + 1; //randomize "lunchtime" off

    if(rand() % 2 == 0) {
        acct.deposit(depositAmt); //call deposit if turn to deposit
        cout << "Customer" << customerID << " Deposited: " << depositAmt << endl;
    }
    else{
        acct.withdraw(withdrawAmt); //call withdraw if turn to withdraw
        cout << "Customer" << customerID << " Withdrew: " << withdrawAmt << endl;

    }
    this_thread::sleep_for(chrono::milliseconds(lunchtime)); //sleep for lunchtime
}


int main(int argc, char* argv[]) { 
    if(argc != 2) { //check for appropriate command line args
        cout << "Usage: bank <# of customers>" << endl;
        return 1;
    }
    int numCustomers = atoi(argv[1]); //get # of customers
    srand(static_cast<unsigned int>(time(0))); //add seed to randomize
    Bank myBank; //create Bank object
    vector<thread> customers; //create vector of all customer threads to be populated
    
    for(int i = 0; i < numCustomers; i++) { //for # of customers
        customers.emplace_back(customerAction, i, ref(myBank)); //call action function for thread execution
    }
    for (auto& thread:customers) {
        thread.join(); //join threads
    }
    cout << "Final Balance: " << myBank.getBalance() << endl; //display final balance
    
    return 0;
}