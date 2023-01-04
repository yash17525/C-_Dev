#include<iostream>

using namespace std;

class Animal
{   
    public:
    int getNL() {return -1;}
    int getNL_A() {return -1;}
};

class Mammal: public Animal
{
    public:
    int getNL() {return 4;}
    int getNL_M() {return 4;}
};

int main()
{
    Animal* a = new Animal();
    Mammal* m = new Mammal();

    cout << "Number of legs NL for animal: " << a->getNL() << endl;
    cout << "Number of legs NL_A for animal: " << a->getNL_A() << endl;

    cout << "Number of legs NL for mammal: " << m->getNL() << endl;
    cout << "Number of legs NL_M for mammal: " << m->getNL_M() << endl;

    Animal* na = m;
    cout << "Number of legs NL for animal (casted from mammal): " << na->getNL() << endl;

    try
    {
        cout << "Number of legs NL_M for animal (casted from mammal): " << na->getNL_M() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}