// Use two threads, where 1st thread writes a character to a particular file and the second thread reads the character from the same file.
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

pthread_mutex_t mLock;
pthread_cond_t waitForReadOperation;  // conditionalVariable
pthread_cond_t waitForWriteOperation; // conditionalVariable
pthread_cond_t childThreadCompleted;  // conditionalVariable
bool writeOperationDone, readOperationDone;

void readWriteOperation(int type, char& c, pthread_mutex_t lock, int charNumber)
{
    pthread_mutex_lock(&lock);
    if (type == 0) // write to file operation
    {
        ofstream writePtr;
        writePtr.open("characterReadWrite.txt");
        writePtr.seekp(0, ios::end);
        writePtr.seekp(1, ios::cur);
        writePtr.put(c);
        cout << "thread (1) writing to file, CHAR:   " << c << endl;
        writePtr.close();

        readOperationDone = false;
        writeOperationDone = true;
        pthread_cond_signal(&waitForWriteOperation);
        // while(readOperationDone == false)
            pthread_cond_wait(&waitForReadOperation, &lock);
    }
    else if (type == 1) // read from file operation
    {
        // while(writeOperationDone == false)
            pthread_cond_wait(&waitForWriteOperation, &lock);

        ifstream readPtr;
        readPtr.open("characterReadWrite.txt");
        readPtr.seekg(-1, ios::end);
        readPtr.get(c);
        cout << "thread (2) reading from file, CHAR:  " << c << endl;
        readPtr.close();

        readOperationDone = true; // in some cases this predicate might be related to memory and it might take time for this condition to reflect in memory,
                                  // but we would have already signalled pthread_cond_signal(&waitForReadOperation); So, we need to put a while loop on pthread_cond_wait(&waitForReadOperation, &lock); waiting for readWriteOperation to be true.
        writeOperationDone = false;
        pthread_cond_signal(&waitForReadOperation);
    }
    pthread_mutex_unlock(&lock);
}

void *thread_function(void *args)
{
    cout << "This is thread 2, thread id: " << pthread_self() << endl;
    char c;
    int i = 1;
    while (i <= 10)
    {
        readWriteOperation(1, c, mLock, i);
        i++;
    }

    pthread_cond_signal(&childThreadCompleted);
}

int main()
{
    writeOperationDone = false;
    readOperationDone = false;
    int status = pthread_cond_init(&waitForReadOperation, NULL);
    if (status != 0)
        return 1;

    status = pthread_cond_init(&waitForWriteOperation, NULL);
    if (status != 0)
        return 1;

    status = pthread_cond_init(&childThreadCompleted, NULL);
    if (status != 0)
        return 1;

    if (pthread_mutex_init(&mLock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }

    char arr[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

    pthread_t my_thread;
    int ret = pthread_create(&my_thread, NULL, &thread_function, NULL);
    cout << "This is thread 1, thread id: " << pthread_self() << endl;
    int i = 1;
    while (i <= 10)
    {
        readWriteOperation(0, arr[i - 1], mLock, i);
        i++;
    }

    pthread_cond_wait(&childThreadCompleted, &mLock);
    return 0;
}

/*
OUTPUTS:

yashwantkumar@Yashwants-MacBook-Pro C++ Assignments % ./a.out
This is thread 1, thread id: 0x206574140
This is thread 2, thread id: 0x16b6b7000
thread (1) writing to file, CHAR:   1
thread (2) reading from file, CHAR:  1
thread (1) writing to file, CHAR:   2
thread (2) reading from file, CHAR:  2
thread (1) writing to file, CHAR:   3
thread (2) reading from file, CHAR:  3
thread (1) writing to file, CHAR:   4
thread (2) reading from file, CHAR:  4
thread (1) writing to file, CHAR:   5
thread (2) reading from file, CHAR:  5
thread (1) writing to file, CHAR:   6
thread (2) reading from file, CHAR:  6
thread (1) writing to file, CHAR:   7
thread (2) reading from file, CHAR:  7
thread (1) writing to file, CHAR:   8
thread (2) reading from file, CHAR:  8
thread (1) writing to file, CHAR:   9
thread (2) reading from file, CHAR:  9
thread (1) writing to file, CHAR:   0
thread (2) reading from file, CHAR:  0

*/