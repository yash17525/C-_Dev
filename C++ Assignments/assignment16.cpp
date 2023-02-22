// Use two threads, where 1st thread writes to a particular file and the second thread reads from the same file.
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

int readWriteSemaphore = 0;
int waitSemaphore = 0;

void signal()
{
    readWriteSemaphore == 0 ? 1 : 0;
}

int readWriteOperation(int type, string &line, pthread_mutex_t lock, int lineNumber)
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
        return 1;
    }
    else if (type == 1) // read from file operation
    {
        fstream readPtr;
        readPtr.open("sample.txt");

        for (int i = 1; i < lineNumber; i++)
            getline(readPtr, line);

        getline(readPtr, line);
        cout << "thread (2) reading from file, line:  " << line << endl;
        readPtr.close();

    }
    pthread_mutex_unlock(&lock);
}

void *thread_function(void *args)
{
    pthread_mutex_t lock;
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        exit(0);
    }
    
    cout << "This is thread 2, thread id: " << pthread_self() << endl;
    string line;
    int i = 1;
    while (i <= 10)
    {
        while (readWriteSemaphore != 1);
        readWriteOperation(1, line, lock, i);
        i++;
        readWriteSemaphore = 0; // put current thread on hold and signal parent thread to write
    }

    this_thread::sleep_for(chrono::milliseconds(5000));
    waitSemaphore = 1; //child thread completed it's execution, signal main thread 
}

int main()
{
    pthread_mutex_t lock;
    if (pthread_mutex_init(&lock, NULL) != 0)
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
        while(readWriteSemaphore != 0);
        readWriteOperation(0, lines[i-1], lock, i);
        i++;
        readWriteSemaphore = 1; // put current thread on hold and signal child thread to read 
    }

    auto start = std::chrono::high_resolution_clock::now();

    while(waitSemaphore == 0); // wait for child thread to complete

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "main thread waited for child thread for " << duration.count() << " milliseconds ...";
    return 0;
}

/*
OUTPUTS:

yashwantkumar@Yashwants-MacBook-Pro C++ Assignments % ./a.out
This is thread 1, thread id: This is thread 2, thread id: 0x16f623000
0x100cc4580
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
main thread waited for child thread for 5005 milliseconds ...%

*/