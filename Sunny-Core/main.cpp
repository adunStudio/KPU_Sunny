#include <iostream>

#include "app/Application.h"


int main()
{
    std::cout << "Hello, Sunny Project!" << std::endl;

    sunny::Application game("Test", {800, 600, false, false});

    game.Start();

    return 0;
}

