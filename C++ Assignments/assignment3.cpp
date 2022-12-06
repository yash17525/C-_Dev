/*
    Write a structure to allocate memory in C++
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

struct Buffer
{
    Buffer(int size)
    {
        cout << "Inside constructor ! \n";
        ptr = (Buffer *)malloc(size);
    }

    ~Buffer()
    {
        cout << "Inside Destructor ! \n";
        free(ptr);
    }

    Buffer *ptr;
};

int main()
{
    Buffer b = Buffer(4);   
}