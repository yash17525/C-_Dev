#include<iostream>
#include<stdlib.h>

using namespace std;

int GetAnimalLegs();
int GetMammalLegs();
int GetReptileLegs();

struct Animal
{
    Animal()
    {
        getNumberOfLegs = &GetAnimalLegs;
    }

    int (*getNumberOfLegs)();
};

struct Mammal: public Animal
{
    Mammal()
    {
        getNumberOfLegs = &GetMammalLegs;
    }
};

struct Reptile: public Animal
{
    Reptile()
    {
        getNumberOfLegs = &GetReptileLegs;
    }
};

int GetAnimalLegs()
{
    return -1;
}

int GetMammalLegs()
{
    return 4;
}

int GetReptileLegs()
{
    return 100;
}

int main()
{
    Animal a = Animal();
    cout << "Number of legs for unkonwn animal: " << a.getNumberOfLegs() << endl;;

    Mammal m = Mammal();
    cout << "Number of legs for mammal: " << m.getNumberOfLegs() << endl;

    Reptile r = Reptile();
    cout << "Number of legs for reptile: " << r.getNumberOfLegs() << endl;
}