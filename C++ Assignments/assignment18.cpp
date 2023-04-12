// Use two threads, where 1st thread writes a character to a particular file and the second thread reads the character from the same file.
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

pthread_mutex_t mLock;
pthread_cond_t waitForWriteOperation; // conditionalVariable
pthread_cond_t childThreadCompleted;  // conditionalVariable

struct RefClass
{
    RefClass(int numCharacters)
    {
        referenceCount = 0;
        buffer = new char[numCharacters];
        writeStatus = new bool[numCharacters];
    }

    virtual ~RefClass()
    {
        delete buffer;
        delete writeStatus;
    }

    void AddRef()
    {
        referenceCount++;
    }

    void Release()
    {
        referenceCount--;
        if (referenceCount <= 0)
        {
            delete (this);
        }
    }

    int referenceCount;
    bool* writeStatus;
    char* buffer;
};

struct RefHelper
{
    RefHelper(RefClass *rc)
    {
        this->rc = rc;
        this->rc->AddRef();
    }

    RefHelper(const RefHelper &rh)
    {
        this->rc = rh.rc;
        this->rc->AddRef();
    }

    ~RefHelper()
    {
        this->rc->Release();
    }

    RefClass *rc;
};

void readFromBuffer(pthread_mutex_t lock, RefHelper &refHelper, char &character, int charNumber)
{
    while (refHelper.rc->writeStatus[charNumber] == false)
        pthread_cond_wait(&waitForWriteOperation, &lock);

    character = refHelper.rc->buffer[charNumber];
    cout << "thread (2) reading from buffer, CHAR:  " << character << endl;

}

void writeToBuffer(pthread_mutex_t lock, RefHelper &refHelper, char &character, int charNumber)
{
    pthread_mutex_lock(&lock);
    refHelper.rc->buffer[charNumber] = character;
    cout << "thread (1) writing to buffer, CHAR:   " << character << endl;

    refHelper.rc->writeStatus[charNumber] = true;
    pthread_cond_signal(&waitForWriteOperation);
    pthread_mutex_unlock(&lock);
}

void *thread_function(void *args)
{
    RefHelper* refHelper = reinterpret_cast<RefHelper*>(args);
    cout << "This is thread 2, thread id: " << pthread_self() << endl;
    char c;
    int i = 1;
    while (i <= 10)
    {
        readFromBuffer(mLock, *refHelper, c, i);
        i++;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    pthread_cond_signal(&childThreadCompleted);
}

int main()
{
    int status = pthread_cond_init(&waitForWriteOperation, NULL);
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

    RefClass *refClass = new RefClass(10);
    RefHelper refHelper(refClass);

    char arr[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

    pthread_t my_thread;
    int ret = pthread_create(&my_thread, NULL, &thread_function, &refHelper);
    cout << "This is thread 1, thread id: " << pthread_self() << endl;
    int i = 1;
    while (i <= 10)
    {
        writeToBuffer(mLock, refHelper, arr[i - 1], i);
        i++;
        if(i % 3 == 0)
            this_thread::sleep_for(chrono::milliseconds(1000));
     }

    cout << "waiting for completion of child thread ... \n";
    pthread_cond_wait(&childThreadCompleted, &mLock);
    cout << "exit main ...";
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