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

struct RefClass
{
    RefClass()
    {
        cout << "Inside Constructor of RefClass ! \n";
        referenceCount = 0;
        ptr = (char *)malloc(sizeof("yashwant")); // allocating a resource + memory
    }

    RefClass(const RefClass& rt)
    {
        cout << "-------------------------------------------Inside copy constructor of RefClass ! \n";
        cout << "size of ptr ----------\n" << sizeof(*ptr);
        this->ptr = (char *)malloc(sizeof(*ptr));
        strcpy(this->ptr, rt.ptr);
    }

    ~RefClass()
    {
        cout << "Inside Destructor of RefClass ! \n";
        Release();
    }

    void AddRef()
    {
        referenceCount++;
        cout << "increased reference count: " << referenceCount << endl;
    }

    void Release()
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
    char *ptr;
};

struct RefHelper
{
    RefHelper(RefClass *rc)
    {
        cout << "Inside Constructor of RefHelper ! \n";
        this->rc = rc;
        this->rc->AddRef();
    }

    RefHelper(const RefHelper& rh)
    {
        cout << "****** inside copy constructor of RefHelper ******\n";
        this->rc = rh.rc; // don't copy pointers directly, this will be problematic as both copied and orignal object point to the same buffer object. if one object delete the buffer object then other won't be able to use it.
        this->rc->AddRef();
    }

    ~RefHelper()
    {
        cout << "Inside Destructor of RefHelper ! \n";
        this->rc->Release();
    }

    RefClass *rc;
};

void function1(RefHelper h)
{
    // RefRefRefRefHelperref(obj);
    // do some stuff, to acccess the buffer object: h.myRef->buffer
    cout << "Inside function 1 \n";

    
}

void function2(RefHelper h)
{
    // do some stuff
    cout << "Inside function 2 \n";
}

int main()
{
    RefClass *rc = new RefClass();
    RefHelper rh(rc);
    function1(rh);
    function2(rh);

    cout << "about to exit main() \n";
}