/*
    Write structure with reference counting mechanism.
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

struct Buffer;
struct Helper;

void function1(Helper h);
void function2(Helper h);
void function3(Helper h);

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

struct Helper
{
    Helper(RefHelper *p)
    {
        myRef = p;
    }

    RefHelper *myRef;
};

void function1(Helper h)
{
    // RefHelper ref(obj);
    // do some stuff, to acccess the buffer object: h.myRef->buffer
    function2(h);
}

void function2(Helper h)
{
    // do some stuff
    function3(h);
}

void function3(Helper h)
{
    // do some stuff
}

int main()
{
    Buffer *b = new Buffer();
    RefHelper *p = new RefHelper(b);
    Helper h(p);
    // cout << "reference count: " << b->referenceCount << endl;
    function1(h);
}