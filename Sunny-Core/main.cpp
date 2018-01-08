#include <iostream>
#include "utils/Timer.h"

int main()
{
    std::cout << "Hello, Sunny Project!" << std::endl;

    sunny::utils::Timer timer;

    for(int i = 0; i < 1000000; ++i) { /* 필요한 작업 수행 */ }

    float elapsed_s  = timer.Elapsed();
    float elapsed_ms = timer.ElapsedMillis();

    std::cout << elapsed_s  << std::endl;
    std::cout << elapsed_ms << std::endl;

    return 0;
}
