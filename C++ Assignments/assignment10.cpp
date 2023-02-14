/*
Using RefClass and RefHelper structs to handle lifetime of class objects using reference counting.
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

class Animal
{
public:
    virtual int getNL()
    {
        return -1;
    }
};

class Mammal : public Animal
{
public:
    int getNL() override
    {
        return 4;
    }
};

class Bird
{
};

struct RefClass
{
    RefClass()
    {
        cout << "Inside Constructor of RefClass ! \n";
        referenceCount = 0;
        animal = new Animal(); // allocating a resource + memory
    }

    RefClass(const RefClass &rt)
    {
        cout << "-------------------------------------------Inside copy constructor of RefClass ! \n";
    }

    ~RefClass()
    {
        cout << "Inside Destructor of RefClass ! \n";
        delete(animal);
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
            delete (this);
            cout << "Object released ! \n";
        }
    }

    int referenceCount;
    Animal *animal;
};

struct RefClassMammal
{
    RefClassMammal()
    {
        cout << "Inside Constructor of RefClass ! \n";
        referenceCount = 0;
        mammal = new Mammal(); // allocating a resource + memory
    }

    RefClassMammal(const RefClassMammal &rt)
    {
        cout << "-------------------------------------------Inside copy constructor of RefClass ! \n";
    }

    ~RefClassMammal()
    {
        cout << "Inside Destructor of RefClass ! \n";
        delete(mammal);
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
            delete (this);
            cout << "Object released ! \n";
        }
    }

    int referenceCount;
    Mammal *mammal;
};

struct RefClassBird
{
    RefClassBird()
    {
        cout << "Inside Constructor of RefClass ! \n";
        referenceCount = 0;
        bird = new Bird(); // allocating a resource + memory
    }

    RefClassBird(const RefClassBird &rt)
    {
        cout << "-------------------------------------------Inside copy constructor of RefClass ! \n";
    }

    ~RefClassBird()
    {
        cout << "Inside Destructor of RefClass ! \n";
        delete(bird);
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
            delete (this);
            cout << "Object released ! \n";
        }
    }

    int referenceCount;
    Bird *bird;
};

struct RefHelperAnimal
{
    RefHelperAnimal(RefClass *rc)
    {
        cout << "Inside Constructor of RefHelper ! \n";
        this->rc = rc;
        this->rc->AddRef();
    }

    RefHelperAnimal(const RefHelperAnimal &rh)
    {
        cout << "****** inside copy constructor of RefHelper ******\n";
        this->rc = rh.rc; // don't copy pointers directly, this will be problematic as both copied and orignal object point to the same buffer object. if one object delete the buffer object then other won't be able to use it.
        this->rc->AddRef();
    }

    ~RefHelperAnimal()
    {
        cout << "Inside Destructor of RefHelper ! \n";
        this->rc->Release();
    }

    RefClass *rc;
};

struct RefHelper
{
    RefHelper(RefClassMammal *rc)
    {
        cout << "Inside Constructor of RefHelper ! \n";
        this->rc = rc;
        this->rc->AddRef();
    }

    RefHelper(const RefHelper &rh)
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

    RefClassMammal *rc;
};

struct RefHelperBird
{
    RefHelperBird(RefClassBird *rc)
    {
        cout << "Inside Constructor of RefHelper ! \n";
        this->rc = rc;
        this->rc->AddRef();
    }

    RefHelperBird(const RefHelperBird &rh)
    {
        cout << "****** inside copy constructor of RefHelper ******\n";
        this->rc = rh.rc; // don't copy pointers directly, this will be problematic as both copied and orignal object point to the same buffer object. if one object delete the buffer object then other won't be able to use it.
        this->rc->AddRef();
    }

    ~RefHelperBird()
    {
        cout << "Inside Destructor of RefHelper ! \n";
        this->rc->Release();
    }

    RefClassBird *rc;
};

int main()
{
    RefClassMammal *rc_mammal = new RefClassMammal();
    RefHelper rh_mammal(rc_mammal);

    RefClass *rc_animal = new RefClass();
    RefHelperAnimal rh_animal(rc_animal);

    RefClassBird *rc_bird = new RefClassBird();
    RefHelperBird rh_bird(rc_bird);

    // Mammal *m = new Mammal();
    Animal *a = dynamic_cast<Animal*>(rh_mammal.rc->mammal);
    Mammal *m1 = dynamic_cast<Mammal*>(a);
    
    Bird *b = dynamic_cast<Bird*>(a); // we are trying to convert Animal Object to Bird Object which is not possible. So dynamic_cast will return null while reinterpret_cast won't return null
    if(b == nullptr)
        cout << "b is null \n"; 
    Bird *b1 = reinterpret_cast<Bird*>(a);
    if(b1)
    {
        cout << "b1 is not null \n";
        // b1->getNL(); we can't access getNL()
    }

    cout << "about to exit main() \n";
    return 0;
}