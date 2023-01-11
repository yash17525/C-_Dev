/*
    Write structure with reference counting mechanism.

    For implementing reference counting mechanism, use another structure (say RefHelper) which will call increseReference() and 
    decreaseReference().
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

struct Buffer;

void function1(Buffer *obj);
void function2(Buffer *obj);
void function3(Buffer *obj);

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

struct RefHelper
{
    RefHelper(Buffer *b)
    {
        buffer = b;
        b->increaseReferenceCount();
    }

    ~RefHelper()
    {
        buffer->decreaseReferenceCount();
    }

    Buffer *buffer;
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