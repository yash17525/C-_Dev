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
    int arraySize;
    cout << "Enter array size: ";
    cin >> arraySize;
    cout << "\n Enter array elements: ";

    Buffer intArray = Buffer(arraySize * sizeof(int));   
    for(int i = 0; i < arraySize; i++)
    {   
        int x;
        cin >> x;
        (intArray.ptr)[i] = x;
    }
    
    cout << endl;
}