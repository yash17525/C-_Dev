#include <iostream>
#include <stdlib.h>

using namespace std;

struct Buffer
{
    Buffer()
    {
        referenceCount = 1;
        ptr = (Buffer*) malloc(sizeof(Buffer));
    }

    ~Buffer()
    {
        cout << "Inside Destructor ! \n";
        decreaseReferenceCount();
        if(referenceCount <= 0)
        {
            free(ptr);
            cout << "Object released ! \n";
        }
    }

    void increaseReferenceCount()
    {
        referenceCount++;
    }

    void decreaseReferenceCount()
    {
        referenceCount --;
    }

    int referenceCount;
    Buffer* ptr;
};

void function1(Buffer* obj)
{
    obj->increaseReferenceCount();
    cout << "reference count: " << obj->referenceCount << endl;

    // do some stuff

    obj->decreaseReferenceCount();
    cout << "reference count: " << obj->referenceCount << endl;
}

void function2(Buffer *obj)
{
    obj->increaseReferenceCount();
    cout << "reference count: " << obj->referenceCount << endl;

    // do some stuff

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
   Buffer b = Buffer();
   cout << "reference count: " << b.referenceCount << endl;
   function1(&b);
   function2(&b);
   function3(&b);
}