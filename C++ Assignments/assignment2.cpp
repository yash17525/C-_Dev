/*
    Demonstrate how to use functions inside struct in C.
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

void setSpeedValue(void* self, int speed);

struct Car
{
    Car()
    {
        this->setSpeed = &setSpeedValue;
        speed = 0;
    }

    ~Car(){}

    void (*setSpeed) (void* self, int speed);
    int speed;
};

void setSpeedValue(void* self, int speed)
{
    ((Car*)self)->speed += speed;
}

int main()
{
    Car c = Car();
    cout << "Initial Spped: " << c.speed << endl;
    c.setSpeed(&c, 5);
    cout << "Speed After Increment: " << c.speed << endl;
}