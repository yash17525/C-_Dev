/*
    Write structure with reference counting mechanism.
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

struct Buffer;

void function1(Buffer *obj);
void function2(Buffer *obj);
void function3(Buffer *obj);

struct RefHelper
{
    RefHelper(Buffer* b)
    {
        buffer = b;
        b->increaseReferenceCount();
    }

    ~ RefHelper()
    {
        buffer->decreaseReferenceCount();
    }

    Buffer* buffer;
};

struct Buffer
{
    Buffer()
    {
        referenceCount = 0;
        ptr = (Buffer *)malloc(sizeof(Buffer));
    }

    ~Buffer()
    {
        cout << "Inside Destructor ! \n";
        decreaseReferenceCount();
    }

    void increaseReferenceCount()
    {
        referenceCount++;
        cout << "reference count: " << referenceCount << endl;
    }

    void decreaseReferenceCount()
    {
        referenceCount--;
        cout << "reference count: " << referenceCount << endl;
        if (referenceCount <= 0)
        {
            free(this);
            cout << "Object released ! \n";
        }
    }

    int referenceCount;
    Buffer *ptr;
};

void function1(Buffer *obj)
{
    RefHelper ref(obj);
    // do some stuff
    function2(obj);
}

void function2(Buffer *obj)
{
    RefHelper ref(obj);
    // do some stuff
    function3(obj);
}

void function3(Buffer *obj)
{
    RefHelper ref(obj);
    // do some stuff
}

int main()
{
    Buffer *b = new Buffer();
    cout << "reference count: " << b->referenceCount << endl;
    function1(b);
}