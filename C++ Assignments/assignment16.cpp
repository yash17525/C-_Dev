// Use two threads, where 1st thread writes to a particular file and the second thread reads from the same file.
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

pthread_mutex_t mLock;
pthread_cond_t waitForReadOperation; // conditionalVariable
pthread_cond_t waitForWriteOperation; // conditionalVariable
pthread_cond_t childThreadCompleted;  // conditionalVariable
bool writeOperationDone, readOperationDone;

void readWriteOperation(int type, string &line, pthread_mutex_t lock, int lineNumber)
{
    pthread_mutex_lock(&lock);
    if (type == 0) // write to file operation
    {
        fstream writePtr;

        writePtr.open("sample.txt");
        for (int i = 0; i < lineNumber - 1; ++i)
        {
            string temp;
            getline(writePtr, temp);
        }
        cout << "thread (1) writing to file, line:   " << line << endl;
        writePtr << line << endl;
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

        fstream readPtr;
        readPtr.open("sample.txt");

        for (int i = 1; i < lineNumber; i++)
            getline(readPtr, line);

        getline(readPtr, line);
        cout << "thread (2) reading from file, line:  " << line << endl;
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
    string line;
    int i = 1;
    while (i <= 10)
    {
        readWriteOperation(1, line, mLock, i);
        i++;
    }

    pthread_cond_signal(&childThreadCompleted);
}

int main()
{
    writeOperationDone = false;
    readOperationDone = false;
    int status = pthread_cond_init(&waitForReadOperation, NULL);
    if(status != 0)
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

    string lines[] = {
        "this is line number 1: gjeg",
        "this is line number 2: gdg",
        "this is line number 3: dfl",
        "this is line number 4: fef",
        "this is line number 5: efljwf",
        "this is line number 6: fefk",
        "this is line number 7: dfs",
        "this is line number 8: dfesfw",
        "this is line number 9: fwefw",
        "this is line number 10: ljlj"};

    pthread_t my_thread;
    int ret = pthread_create(&my_thread, NULL, &thread_function, NULL);
    cout << "This is thread 1, thread id: " << pthread_self() << endl;
    int i = 1;
    while (i <= 10)
    {
        readWriteOperation(0, lines[i-1], mLock, i);
        i++;
    }

    pthread_cond_wait(&childThreadCompleted, &mLock);
    return 0;
}

/*
OUTPUTS:

yashwantkumar@Yashwants-MacBook-Pro C++ Assignments % ./a.out
This is thread 1, thread id: 0x100abc580
This is thread 2, thread id: 0x16f63f000
thread (1) writing to file, line:   this is line number 1: gjeg
thread (2) reading from file, line:  this is line number 1: gjeg
thread (1) writing to file, line:   this is line number 2: gdg
thread (2) reading from file, line:  this is line number 2: gdg
thread (1) writing to file, line:   this is line number 3: dfl
thread (2) reading from file, line:  this is line number 3: dfl
thread (1) writing to file, line:   this is line number 4: fef
thread (2) reading from file, line:  this is line number 4: fef
thread (1) writing to file, line:   this is line number 5: efljwf
thread (2) reading from file, line:  this is line number 5: efljwf
thread (1) writing to file, line:   this is line number 6: fefk
thread (2) reading from file, line:  this is line number 6: fefk
thread (1) writing to file, line:   this is line number 7: dfs
thread (2) reading from file, line:  this is line number 7: dfs
thread (1) writing to file, line:   this is line number 8: dfesfw
thread (2) reading from file, line:  this is line number 8: dfesfw
thread (1) writing to file, line:   this is line number 9: fwefw
thread (2) reading from file, line:  this is line number 9: fwefw
thread (1) writing to file, line:   this is line number 10: ljlj
thread (2) reading from file, line:  this is line number 10: ljlj

*/