/*
    Use C++ 98 and use structure & class to determine the default scope (public, proctected or void) of both.
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

struct Buffer
{
    Buffer(int size)
    {
        cout << "Constructor called, memory allocated ! \n";
        ptr = (int*) malloc(size);
    }

    ~ Buffer()
    {
        cout << "Going out of scope, Destructor called, memory released ! \n";
        free(ptr);
    }

    int * ptr;
};

int main()
{
    {
        Buffer b = Buffer(sizeof(int));

        // doing some stuff
    }
}