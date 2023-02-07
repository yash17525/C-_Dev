// Create a program with multiple processes using fork()

#include<iostream>
#include <unistd.h>

using namespace std;

int main()
{
    int process_id = fork();

    /*
        If int value returned from fork() call is:
        1. 0, then child process is returned
        2. +ve, Returned to parent or caller. The value contains process ID of newly created child process.
        3. -ve, creation of a child process was unsuccessful.
    */

   if(process_id == 0)
   {
        cout << "hello from child" << endl;
   }
   else if(process_id > 0)
   {
        cout << "hello from parent, parent_process_id is: " << process_id << endl;
   }
   else
   {
        cout << "child process creation failed !";
   }

   return 0;
}