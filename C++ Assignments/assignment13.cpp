// Create a program for multithreading using posix threads (pthreads) 

// thread0.c
#include <pthread.h>
#include <iostream>

using namespace std;

void *worker_thread(void *arg)
{
    cout << "This is worker_thread() \n";
    pthread_exit(NULL);
}

int main()
{
    pthread_t my_thread;
    int ret;

    cout << "In main: creating thread \n";

    // pthread_create(pointer for thread, attribute object for thread, pointer to function which thread will execute, argument to the function)
    ret = pthread_create(&my_thread, NULL, &worker_thread, NULL);
    if (ret != 0)
    {
        cout << "Error: pthread_create() failed \n";
        exit(0);
    }

    pthread_exit(NULL);
}