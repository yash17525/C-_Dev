/*
    Use C++ 98
    Use Structure
    Use malloc to 'allocate' memory and 'free' to release memory
    Use malloc inside structure
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