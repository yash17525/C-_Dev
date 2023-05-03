#include <iostream>
#include <pthread.h>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

class Ref
{
    public:
    Ref(): mRefCount(0)
    {}

    ~ Ref()
    {}

    void addRef()
    {
        ++mRefCount;
    }

    void releaseRefCount()
    {
        -- mRefCount;
        if(!mRefCount)
        {
            delete this;
        }
    }

    private:
    int mRefCount;
};

class MutexRef: public Ref
{
    public: 
    MutexRef(pthread_mutex_t* mutex): mMutex(mutex)
    {}

    ~ MutexRef()
    {
        delete mMutex;
    }

    void lock()
    {
        pthread_mutex_lock(mMutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(mMutex);
    }

    private:
    pthread_mutex_t* mMutex;
};

class MutexLockHelper
{
    public:
    MutexLockHelper(MutexRef* mutexRef): mMutexRef(mutexRef)
    {
        mMutexRef->addRef();
        mMutexRef->lock();
    }

    MutexLockHelper(const MutexLockHelper &lockHelper)
    {
        mMutexRef = lockHelper.mMutexRef; // don't copy pointers directly, this will be problematic as both copied and orignal object point to the same buffer object. if one object delete the buffer object then other won't be able to use it.
        mMutexRef->addRef();
    }

    ~ MutexLockHelper()
    {
        mMutexRef->unlock();
        mMutexRef->releaseRefCount();
    }

    private:
    MutexRef* mMutexRef;
};

char ReadCharacter(pthread_mutex_t* mutex, char* buffer, int position)
{
    char ch;
    {
        MutexRef* mutexRef = new MutexRef(mutex);
        MutexLockHelper lock(mutexRef);
        ch = buffer[position];
    }

    return ch;
}

void writeCharacter(pthread_mutex_t* mutex, char* buffer, int position, char ch)
{
    {
        MutexRef* mutexRef = new MutexRef(mutex);
        MutexLockHelper lock(mutexRef);
        buffer[position] = ch;
    }
}

void writeCharacterToBuffer(pthread_mutex_t* mutex, char * buffer, pthread_cond_t* condVariables)
{
    char arr[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
    for(int i = 0; i < 10; i ++)
    {
        writeCharacter(mutex, buffer, i, arr[i]);
        cout << "wrote char to buffer: " << arr[i] << endl;
        pthread_cond_signal(&condVariables[i]);
    }
}

int waitAnyOfTheCharactersToBeWritten(pthread_cond_t* condVariables, int size)
{
    return 1;
}

char readCharactersToBuffer(pthread_mutex_t* mutex, char* buffer, pthread_cond_t* condVariables, int size)
{
    int pos = waitAnyOfTheCharactersToBeWritten(condVariables, size);
    char ch = ReadCharacter(mutex, buffer, pos);
    return ch;
}

struct Params
{
    pthread_mutex_t* mMutex;
    char* mBuffer;
    pthread_cond_t* mConditionVariables;
    pthread_cond_t* mChildThreadCompleted;
    int mSize;
};

void *readFromBufferThread(void *args)
{
    Params *params = reinterpret_cast<Params *>(args);
    cout << "This is thread 2, thread id: " << pthread_self() << endl;

    int i = 0;
    while(i < params->mSize)
    {
        char c = readCharactersToBuffer(params->mMutex, params->mBuffer, params->mConditionVariables, params->mSize);
        cout << "read char from buffer: " << c << endl;
    }
    
    pthread_cond_signal(params->mChildThreadCompleted);
}


int main()
{
    pthread_mutex_t *mLock;
    pthread_cond_t *childThreadCompleted; // conditionalVariable

    pthread_cond_t* conditionVariables = new pthread_cond_t[10];
    for(int i = 0; i < 10; i++)
    {
        int status = pthread_cond_init(&conditionVariables[i], NULL);
        if (status != 0)
            return 1;
    }

    int status = pthread_cond_init(childThreadCompleted, NULL);
    if (status != 0)
        return 1;

    if (pthread_mutex_init(mLock, NULL) != 0)
    {
        cout << "\n mutex init has failed\n";
        return 1;
    }

    char* buffer = new char[10];

    pthread_t childThread;
    Params params = Params();
    params.mMutex = mLock;
    params.mBuffer = buffer;
    params.mConditionVariables = conditionVariables;
    params.mChildThreadCompleted = childThreadCompleted;

    int ret = pthread_create(&childThread, NULL, &readFromBufferThread, &params);
    cout << "This is thread 1, thread id: " << pthread_self() << endl;

    writeCharacterToBuffer(mLock, buffer, conditionVariables);


    cout << "waiting for completion of child thread ... \n";
    pthread_cond_wait(childThreadCompleted, mLock);
    cout << "exit main ...";
    return 0;
}