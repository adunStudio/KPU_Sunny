#include <iostream>

#include "app/Window.h"


int main()
{
    std::cout << "Hello, Sunny Project!" << std::endl;

    sunny::app::Window window("Test", {800, 600, false, false});

    window.SetTitle("Test Window");

    window.Run();

    return 0;
}

