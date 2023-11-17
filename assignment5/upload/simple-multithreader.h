#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <time.h>
int user_main(int argc, char **argv);

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> &&lambda)
{
  lambda();
}

int main(int argc, char **argv)
{
  /*
   * Declaration of a sample C++ lambda function
   * that captures variable 'x' by value and 'y'
   * by reference. Global variables are by default
   * captured by reference and are not to be supplied
   * in the capture list. Only local variables must be
   * explicity captured if they are used inside lambda.
   */
  int x = 5, y = 1;
  // Declaring a lambda expression that accepts void type parameter
  auto /*name*/ lambda1 = /*capture list*/ [/*by value*/ x, /*by reference*/ &y](void)
  {
    /* Any changes to 'x' will throw compilation error as x is captured by value */
    y = 5;
    std::cout << "====== Welcome to Assignment-" << y << " of the CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  // Executing the lambda function
  demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

  int rc = user_main(argc, argv);

  auto /*name*/ lambda2 = [/*nothing captured*/]()
  {
    std::cout << "====== Hope you enjoyed CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  demonstration(lambda2);
  return rc;
}

#define main user_main

// parallel_for accepts a C++11 lambda function and runs the loop body (lambda) in
// parallel by using ‘numThreads’ number of Pthreads to be created by the simple-multithreader
typedef struct
{
  int low;
  int high;
  std::function<void(int)> lambda;
} thread_args;

void *thread_func(void *ptr)
{
  thread_args *t = ((thread_args *)ptr);
  for (int i = t->low; i < t->high; i++)
  {
    t->lambda(i);
  }
  return NULL;
}
void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads)
{
  // code here
  clock_t start_time = clock();
  pthread_t tid[numThreads];
  thread_args args[numThreads];
  int chunk = ceil((high - low) / numThreads);
  for (int i = low; i < numThreads + low; i++)
  {
    args[i].low = i * chunk;
    args[i].high = (i + 1) * chunk > (high) ? high : (i + 1) * chunk;
    args[i].lambda = lambda;
    pthread_create(&tid[i], NULL, thread_func, (void *)&args[i]);
  }
  for (int i = 0; i < numThreads; i++)
  {
    pthread_join(tid[i], NULL);
  }
  clock_t end_time = clock();

  clock_t duration = end_time - start_time;

  printf("Runtime of this threadding: %f seconds\n", ((double)duration) / CLOCKS_PER_SEC);
}

// This version of parallel_for is for parallelizing two-dimensional for-loops, i.e., an outter for-i loop and
// an inner for-j loop. Loop properties, i.e. low, high are mentioned below for both outter
// and inner for-loops. The suffixes “1” and “2” represents outter and inner loop properties respectively.
typedef struct
{
  int low1;
  int high1;
  int low2;
  int high2;
  std::function<void(int, int)> lambda;
} thread_arg2;

void *thread_func2(void *ptr)
{
  thread_arg2 *t = ((thread_arg2 *)ptr);
  for (int i = t->low1; i < t->high1; i++)
  {
    for (int j = t->low2; j < t->high2; j++)
    {
      t->lambda(i, j);
    }
  }
  return NULL;
}

void parallel_for(int low1, int high1, int low2, int high2,
                  std::function<void(int, int)> &&lambda, int numThreads)
{
  clock_t start_time = clock();
  pthread_t tid[numThreads];
  thread_arg2 args[numThreads];
  int chunk = ceil((high1 - low1) / numThreads);
  for (int i = 0; i < numThreads; i++)
  {
    args[i].high2 = high2;
    args[i].low2 = low2;
    args[i].low1 = i * chunk;
    args[i].high1 = (i + 1) * chunk > (high1) ? high1 : (i + 1) * chunk;
    args[i].lambda=lambda;
    pthread_create(&tid[i], NULL, thread_func2, (void *)&args[i]);
  }

  for (int i = 0; i < numThreads; i++)
  {
    pthread_join(tid[i], NULL);
  }
  clock_t end_time = clock();

  clock_t duration = end_time - start_time;

  printf("Runtime of this threadding: %f seconds\n", ((double)duration) / CLOCKS_PER_SEC);
}
