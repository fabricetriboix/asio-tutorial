#include "rtimer-impl.hpp"
#include <iostream>
#include <thread>

int main()
{
    rtimer::rtimer_impl rtimer;
    rtimer.enable(std::chrono::milliseconds(500), [](){ std::cout << "tick" << std::endl; });
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
