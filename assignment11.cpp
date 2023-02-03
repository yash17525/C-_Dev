/*
Using RefClass and RefHelper structs to handle lifetime of class objects using reference counting.
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

class Animal;
class Mammal;

enum ObjectType
{
    ANIMAL,
    MAMMAL
};

struct RefClass
{
    RefClass()
    {
        cout << "Inside Constructor of RefClass ! \n";
        referenceCount = 0;
        //animal = new (); // allocating a resource + memory
    }

    RefClass(const RefClass &rt)
    {
        cout << "-------------------------------------------Inside copy constructor of RefClass ! \n";
    }

    virtual ~RefClass()
    {
        cout << "Inside Destructor of RefClass ! \n";
        // delete (animal);
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
            cout << "Object released ! \n";
            delete (this);
        }
    }

    int referenceCount;
    // RefClass *animal;
};

struct RefHelper
{
    RefHelper(RefClass *rc)
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

    RefClass *rc;
};

class Animal : public RefClass
{
    public:

    virtual int getNL()
    {
        return -1;
    }

    Animal()
    {
        cout << "animal const()\n";
    }

    ~Animal()
    {
        cout << "animal dest()\n";
    }
};

class Mammal : public Animal
{
    public:

    int getNL() override
    {
        return 4;
    }

    Mammal()
    {
        cout << "mammal const()\n";
    }

    ~Mammal()
    {
        cout << "mammal dest()\n";
    }
};


int main()
{
    Animal *a = new Animal();
    RefHelper rh_animal(a);

    Animal *casted_animal = dynamic_cast<Animal*>(rh_animal.rc);
    cout << "Value of getNL() for animal: " << casted_animal->getNL() << endl;

    Mammal *m = new Mammal();
    RefHelper rh_mammal(m);

    Animal *casted_mammal = dynamic_cast<Mammal*>(rh_mammal.rc);
    cout << "Value of getNL() for mammal: " << casted_mammal->getNL() << endl;

    cout << "about to exit main() \n";
    return 0;
}