#include <iostream>
#include <pthread.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <queue>

using namespace std;

class Ref
{
public:
    Ref() : mRefCount(0)
    {
    }

    ~Ref()
    {
    }

    void addRef()
    {
        ++mRefCount;
    }

    void releaseRefCount()
    {
        --mRefCount;
        if (!mRefCount)
        {
            delete this;
        }
    }

private:
    int mRefCount;
};

class MutexRef : public Ref
{
public:
    MutexRef(pthread_mutex_t *mutex) : mMutex(mutex)
    {
        if (pthread_mutex_init(mMutex, NULL) != 0)
        {
            cout << "\n mutex init has failed\n";
            // exit(0);
        }
    }

    ~MutexRef()
    {
        pthread_mutex_destroy(mMutex);
        // delete mMutex;
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
    pthread_mutex_t *mMutex;
};

class MutexRefHelper
{
public:
    MutexRefHelper(){}

    MutexRefHelper(MutexRef *mutexRef) : mMutexRef(mutexRef)
    {
        mMutexRef->addRef();
    }

    MutexRefHelper(const MutexRefHelper &refHelper)
    {
        mMutexRef = refHelper.mMutexRef; 
        mMutexRef->addRef();
    }

    ~MutexRefHelper()
    {
        mMutexRef->releaseRefCount();
    }

    void lock()
    {
        mMutexRef->lock();
    }

    void unlock()
    {
        mMutexRef->unlock();
    }

private:
    MutexRef *mMutexRef;
};

class MutexLockHelper
{
public:
    MutexLockHelper(MutexRefHelper mutexRefHelper) : mMutexRefHelper(mutexRefHelper)
    {
        mMutexRefHelper.lock();
    }

    ~MutexLockHelper()
    {
        mMutexRefHelper.unlock();
    }

private:
    MutexRefHelper mMutexRefHelper;
};

class ConditionalVariableRef : public Ref
{
public:
    ConditionalVariableRef(pthread_cond_t* condtVar) : mCondtVar(condtVar)
    {
        if (pthread_cond_init(mCondtVar, NULL) != 0)
        {
            cout << "\n conditional varaible init has failed\n";
            // exit(0);
        }

        MutexRef *mMutexRef = new MutexRef(mMutex);
        mMutexRefHelper = MutexRefHelper(mMutexRef);
    }

    ~ConditionalVariableRef()
    {
        pthread_cond_destroy(mCondtVar);
        // delete mCondtVar
    }

    void wait()
    {
        MutexLockHelper lockHelper(mMutexRefHelper);
        pthread_cond_wait(mCondtVar, mMutex);
    }

    void signal()
    {
        MutexLockHelper lockHelper(mMutexRefHelper);
        pthread_cond_signal(mCondtVar);
    }

private:
    pthread_cond_t* mCondtVar;
    pthread_mutex_t *mMutex;
    MutexRefHelper mMutexRefHelper;

};

class ConditionalVariableHelper
{
public:
    ConditionalVariableHelper() {}

    ConditionalVariableHelper(ConditionalVariableRef *condVarRef) : mCondVarRef(condVarRef)
    {
        mCondVarRef->addRef();
    }

    ConditionalVariableHelper(const ConditionalVariableHelper &condVarRefHelper)
    {
        mCondVarRef = condVarRefHelper.mCondVarRef;
        mCondVarRef->addRef();
    }

    ~ConditionalVariableHelper()
    {
        mCondVarRef->releaseRefCount();
    }

    void wait()
    {
       mCondVarRef->wait();
    }

    void signal()
    {
        mCondVarRef->signal();
    }

private:
    ConditionalVariableRef *mCondVarRef;
};

class QueueElement
{
    public: 
    QueueElement()
    {
        pthread_mutex_t *mutex;
        MutexRef *mutexRef = new MutexRef(mutex);
        mMutexRefHelper = MutexRefHelper(mutexRef);
    }

    void setValue(char c, char (*execute)(void * self))
    {
        {
            MutexLockHelper lock(mMutexRefHelper);
            ch = c;
        }

        mExecute = execute;
    }

    char getValue()
    {
        char c;
        {
            MutexLockHelper lock(mMutexRefHelper);
            c = ch;
        }

        return c;
    }

    char (*mExecute)(void *self);

private:
    char ch;
    MutexRefHelper mMutexRefHelper;
};

char ReadCharacterOutsourced(void* self)
{
    QueueElement* queueElement = (QueueElement*)(self);
    char ch = queueElement->getValue();
    return ch;
}

class CustomQueue
{
public:
    CustomQueue()
    {
        pthread_mutex_t *mutex;
        MutexRef *mutexRef = new MutexRef(mutex);
        mMutexRefHelper = MutexRefHelper(mutexRef);

        pthread_cond_t* waitForwriteOperation;
        ConditionalVariableRef *condVarRef = new ConditionalVariableRef(waitForwriteOperation);
        mWaitForWriteOperationCondVarRef = ConditionalVariableHelper(condVarRef);
    }

    void push(char ch)
    {
        QueueElement* element = new QueueElement();
        {
            MutexLockHelper lock(mMutexRefHelper);
            cout << "push into queue \n";
            element->setValue(ch, &ReadCharacterOutsourced);
            buffer.push(element);
        }

        mWaitForWriteOperationCondVarRef.signal();
    }

    char pop()
    {
        if (buffer.empty())
        {
            // wait for the write operation
            mWaitForWriteOperationCondVarRef.wait();
        }

        char ch;
        {
            MutexLockHelper lock(mMutexRefHelper);
            QueueElement* queueElement = buffer.front();
            buffer.pop();
            ch = queueElement->mExecute(queueElement);
        }

        return ch;
    }

private:
    // queue<char> buffer;
    queue<QueueElement*> buffer;
    MutexRefHelper mMutexRefHelper;
    ConditionalVariableHelper mWaitForWriteOperationCondVarRef;
};

class CustomQueueRef : public Ref
{
public:
    CustomQueueRef(CustomQueue* customQueue) : mBuffer(customQueue)
    {
    }

    ~CustomQueueRef()
    {
        delete mBuffer;
    }

    void push(char ch)
    {
        mBuffer->push(ch);
    }

    char pop()
    {
        return mBuffer->pop();
    }

    CustomQueue* mBuffer;
};

class CustomQueueRefHelper
{
public:
    CustomQueueRefHelper(){}

    CustomQueueRefHelper(CustomQueueRef *ref) : rf(ref)
    {
        rf->addRef();
    }

    CustomQueueRefHelper(const CustomQueueRefHelper &queueRefHelper)
    {
        rf = queueRefHelper.rf;
        rf->addRef();
    }

    ~CustomQueueRefHelper()
    {
        rf->releaseRefCount();
    }

    void push(char ch)
    {
        rf->push(ch);
    }

    char pop()
    {
        rf->pop();
    }

private:
    CustomQueueRef *rf;
};

void writeCharacterToBuffer(CustomQueueRefHelper customQueueRefHelper)
{
    char arr[11] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '$'}; // '$' used as special character
    for (int i = 0; i < 11; i++)
    {
        customQueueRefHelper.push(arr[i]);
        cout << "wrote char to buffer: " << arr[i] << endl;
    }
}

struct Params
{
    Params(CustomQueueRefHelper customQueueRefHelper, pthread_cond_t childThreadCompleted)
    {
        mCustomQueueRefHelper = customQueueRefHelper;
        mChildThreadCompleted = childThreadCompleted;
    }

    CustomQueueRefHelper mCustomQueueRefHelper;
    pthread_cond_t mChildThreadCompleted;
};

void *childThreadFunction(void *args)
{
    Params *params = reinterpret_cast<Params *>(args);
    cout << "This is thread 2, thread id: " << pthread_self() << endl;

    int i = 0;
    char ch;
    do
    {
        ch = params->mCustomQueueRefHelper.pop();
        cout << "read char from buffer: " << ch << endl;

    } while(ch != '$');

    pthread_cond_signal(&(params->mChildThreadCompleted));
}

int main()
{
    pthread_mutex_t *mLock;
    pthread_cond_t childThreadCompleted; // conditionalVariable

    int status = pthread_cond_init(&childThreadCompleted, NULL);
    if (status != 0)
        return 1;

    if (pthread_mutex_init(mLock, NULL) != 0)
    {
        cout << "\n mutex init has failed\n";
        return 1;
    }

    CustomQueueRef* ref = new CustomQueueRef(new CustomQueue());
    CustomQueueRefHelper customQueueRefHelper(ref);

    pthread_t childThread;
    Params params = Params(customQueueRefHelper, childThreadCompleted);

    int ret = pthread_create(&childThread, NULL, &childThreadFunction, &params);
    cout << "This is thread 1, thread id: " << pthread_self() << endl;

    writeCharacterToBuffer(customQueueRefHelper);

    cout << "waiting for completion of child thread ... \n";
    pthread_cond_wait(&childThreadCompleted, mLock);
    cout << "exit main ...";
    return 0;
}