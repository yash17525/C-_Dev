/*
    Write structure with reference counting mechanism.
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

struct Buffer;

void function1(Buffer *obj);
void function2(Buffer *obj);
void function3(Buffer* obj);

    struct Buffer
{
    Buffer()
    {
        referenceCount = 0;
        ptr = (Buffer*) malloc(sizeof(Buffer));
    }

    ~Buffer()
    {
        cout << "Inside Destructor ! \n";
        decreaseReferenceCount();
    }

    void increaseReferenceCount()
    {
        referenceCount++;
    }

    void decreaseReferenceCount()
    {
        referenceCount --;
        if (referenceCount <= 0)
        {
            free(this);
            cout << "Object released ! \n";
        }
    }

    int referenceCount;
    Buffer* ptr;
};

void function1(Buffer* obj)
{
    obj->increaseReferenceCount();
    cout << "reference count: " << obj->referenceCount << endl;

    // do some stuff
    function2(obj);

    obj->decreaseReferenceCount();
    cout << "reference count: " << obj->referenceCount << endl;
}

void function2(Buffer *obj)
{
    obj->increaseReferenceCount();
    cout << "reference count: " << obj->referenceCount << endl;

    // do some stuff
    function3(obj);

    obj->decreaseReferenceCount();
    cout << "reference count: " << obj->referenceCount << endl;
}

void function3(Buffer *obj)
{
    obj->increaseReferenceCount();
    cout << "reference count: " << obj->referenceCount << endl;

    // do some stuff

    obj->decreaseReferenceCount();
    cout << "reference count: " << obj->referenceCount << endl;
}

int main()
{
   Buffer *b = new Buffer();
   cout << "reference count: " << b->referenceCount << endl;
   function1(b);
}