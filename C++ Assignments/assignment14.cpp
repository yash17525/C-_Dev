// Observe the output of the below code
#include<iostream>
#include <pthread.h>

using namespace std;

void* thread_function(void* arr)
{
    int* new_arr = reinterpret_cast<int*>(arr);
    cout << "thread id: " << pthread_self() << ", array: " << new_arr << endl;

    for(int i = 0; i < 3; i++)
    {
        cout << "thread id: " << pthread_self() << ", array[" <<i <<"]: " << new_arr[i] << endl;
    }
}

int main()
{
    int* arr = new int[1024];
    arr[0] = 0;
    arr[1] = 1;
    arr[2] = 2;

    pthread_t my_thread;
    int ret = pthread_create(&my_thread, NULL, &thread_function, arr);
    cout << "thread id: " << pthread_self() << ", array: " << arr << endl;

    for (int i = 0; i < 3; i++)
    {
        cout << "thread id: " << pthread_self() << ", array[" << i << "]: " << arr[i] << endl;
    }

    return 0;
}

/*
First Run Output:
        yashwantkumar@Yashwants-MacBook-Pro C++ Assignments % ./a.out
        thread id: 0x102408580, array: 0x146808800
        thread id: 0x102408580, array[0]: 0
        thread id: 0x102408580, array[1]: 1
        thread id: 0x102408580, array[2]: 2

Second Run Output:
        yashwantkumar@Yashwants-MacBook-Pro C++ Assignments % ./a.out
        thread id: 0x100e60580, array: 0x136008800
        thread id: 0x100e60580, array[0]: 0
        thread id: 0x100e60580, array[1]: 1
        thread id: 0x100e60580, array[2]: thread id: 0x16f3af000, array: 0x136008800
        thread id: 0x16f3af000, array[2
        0]: 0
        thread id: 0x16f3af000, array[1]: 1
        thread id: 0x16f3af000, array[2]: 2

Third Run Output:
        yashwantkumar@Yashwants-MacBook-Pro C++ Assignments % ./a.out
        thread id: 0x103130580, array: 0x14e008800
        thread id: 0x103130580, array[0]: 0
        thread id: thread id: 0x16d087000, array: 0x14e008800
        thread id: 0x16d087000, array[0]: 0
        thread id: 0x16d087000, array[1]: 1
        thread id: 0x16d087000, array[2]: 2
        0x103130580, array[1]: 1
        thread id: 0x103130580, array[2]: 2
*/