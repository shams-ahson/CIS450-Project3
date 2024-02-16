/**
 * File: dining-philosophers.cpp
 * ---------------------------------------------
 * This program implements the classic dining philosophers
 * simulation using mutexes and condition variables (Monitor).
 */
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <random>
using namespace std;

/**
 * Defines a collection of timing constants used for
 * the dining philosopher simulation.
 */

static const unsigned int kLowThinkTime = 100;
static const unsigned int kHighThinkTime = 2000;
static const unsigned int kLowEatTime = 250;
static const unsigned int kHighEatTime = 500;

random_device rd;  // a seed source for the random number engine
mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
uniform_int_distribution<> thinkTime(kLowThinkTime, kHighThinkTime);
uniform_int_distribution<> eatTime(kLowEatTime, kHighEatTime);

mutex mtx; // monitor lock

static const size_t kNumMeals = 3; // number of times each philosopher eats

enum
{
  THINKING,
  HUNGRY,
  EATING
} state[5];
condition_variable self[5];

void test(int i)
{
  if ((state[(i + 4) % 5] != EATING) &&
      (state[i] == HUNGRY) &&
      (state[(i + 1) % 5] != EATING))
  {
    state[i] = EATING;
    self[i].notify_one();
  }
}

void pickup(int i)
{
  unique_lock<mutex> lck(mtx);
  state[i] = HUNGRY;
  test(i);
  if (state[i] != EATING)
    self[i].wait(lck);

  self[i].wait(lck, [&i]
               { return state[i] == EATING; });
}

void putdown(int i)
{
  unique_lock<mutex> lck(mtx);
  state[i] = THINKING;

  // test left and right neighbors
  test((i + 4) % 5);
  test((i + 1) % 5);
}

static void think(size_t id)
{
  printf("%zu starts thinking.\n", id);
  this_thread::sleep_for(chrono::milliseconds(thinkTime(gen))); // simulate "thinking"
  printf("%zu done thinking. \n", id);
}

void eat(size_t id)
{
  pickup(id);
  printf("%zu starts eating om nom nom nom.\n", id);
  this_thread::sleep_for(chrono::milliseconds(eatTime(gen))); // simulate "eating"
  printf("%zu done eating.\n", id);
  putdown(id);
}

static void philosopher(size_t id)
{
  state[id] = THINKING;
  for (size_t i = 0; i < kNumMeals; i++)
  {
    think(id);
    eat(id);
  }
}

int main(int argc, const char *argv[])
{
  thread philosophers[5];

  for (size_t i = 0; i < 5; i++)
  {
    philosophers[i] = thread(philosopher, i);
  }

  for (thread &p : philosophers)
    p.join();
  return 0;
}