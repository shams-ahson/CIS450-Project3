#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

// Constants
const int MAX_ON_BRIDGE = 3;
const int NUM_CARS = 50;

// Mutexes and condition variables for synchronization
mutex output_mtx;
mutex bridge_mtx;
condition_variable bridge_cv;

// Global variables to track bridge state
int onBridge = 0;
int currentDirection = -1;

// Function to print car status
void PrintStatus(int id, int dir, const string &action) {
    lock_guard<mutex> lock(output_mtx);
    cout << "Car " << id << " traveling in direction " << dir
         << " has " << action << " the bridge. "
         << "CurrDir: " << (currentDirection != -1 ? currentDirection : 0)
         << ". #Cars: " << onBridge << ".\n";
}

// Function for a car arriving at the bridge
void ArriveBridge(int id, int dir) {
    unique_lock<mutex> lock(bridge_mtx);

    // Wait until it's safe to arrive on the bridge
    while ((currentDirection != -1 && currentDirection != dir) || onBridge >= MAX_ON_BRIDGE) {
        bridge_cv.wait(lock);
    }

    // Update bridge state and print status
    currentDirection = dir;
    ++onBridge;
    PrintStatus(id, dir, "arrived at");
}

// Function for a car exiting the bridge
void ExitBridge(int id, int dir) {
    lock_guard<mutex> lock(bridge_mtx);

    // Update bridge state and notify waiting threads
    --onBridge;
    if (onBridge == 0) {
        currentDirection = -1;
    }

    bridge_cv.notify_all();

    // Print exit status
    PrintStatus(id, dir, "exited from");
}

// Function to simulate a car crossing the bridge
void CrossBridge(int id, int dir) {
    {
        lock_guard<mutex> lock(output_mtx);
        cout << "Car " << id << " traveling in direction " << dir
             << " is crossing the bridge. CurrDir: " << currentDirection
             << ". #Cars: " << onBridge << ".\n";
    }

    // Simulate crossing time with random delay
    mt19937 rng(random_device{}());
    uniform_int_distribution<mt19937::result_type> dist(100, 400);
    this_thread::sleep_for(chrono::milliseconds(dist(rng)));
}

// Function representing behavior of each car
void OneVehicle(int id) {
    mt19937 gen(random_device{}());
    uniform_int_distribution<> dir_dist(0, 1);
    uniform_int_distribution<> wait_dist(100, 5000);

    // Randomly determine the direction of the vehicle
    int dir = dir_dist(gen);

    // Simulate the vehicle waiting for a random amount of time before arrival
    this_thread::sleep_for(chrono::milliseconds(wait_dist(gen)));

    // Perform actions of arriving, crossing, and exiting the bridge
    ArriveBridge(id, dir);
    CrossBridge(id, dir);
    ExitBridge(id, dir);
}

// Main function
int main() {
    // Vector to store threads representing cars
    vector<thread> cars(NUM_CARS);

    try {
        // Create and start one thread for each car
        for (int i = 0; i < NUM_CARS; ++i) {
            cars[i] = thread(OneVehicle, i);
        }

        // Wait for all threads to finish
        for (auto &car : cars) {
            car.join();
        }
    } catch (const system_error& e) {
        // Handle system errors
        cerr << "System error: " << e.what() << '\n';
    }

    return 0;
}
