/*
    Write structure with reference counting mechanism.
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

struct Buffer;
struct RefHelper;

void function1(RefHelper h);
void function2(RefHelper h);
void function3(RefHelper h);

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
        cout << "increased reference count: " << referenceCount << endl;
    }

    void decreaseReferenceCount()
    {
        referenceCount--;
        cout << "decreased reference count: " << referenceCount << endl;
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

    RefHelper(const RefHelper& rh)
    {
        cout << "****** inside copy constructor ******";
        this->buffer = rh.buffer; // don't copy pointers directly, this will be problematic as both copied and orignal object point to the same buffer object. if one object delete the buffer object then other won't be able to use it.
        this->buffer->increaseReferenceCount();
    }

    ~RefHelper()
    {
        buffer->decreaseReferenceCount();
    }

    Buffer *buffer;
};

void function1(RefHelper h)
{
    // RefRefRefRefHelperref(obj);
    // do some stuff, to acccess the buffer object: h.myRef->buffer
    cout << "Inside function 1 \n";
    delete(h.buffer); 
    /* currently both the objects created using the copy constructor points to the same buffer object. so that is a problem.
    here we are deleting h.buffer for copied object, so the original object won't be able to use it*/
}

void function2(RefHelper h)
{
    // do some stuff
    cout << "Inside function 2 \n";
}

int main()
{
    Buffer *b = new Buffer();
    RefHelper h(b);
    function1(h);
    function2(h);

    cout << "about to exit main() \n";
}