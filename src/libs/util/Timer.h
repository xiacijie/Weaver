#pragma once
#include <chrono>

using namespace std;

namespace util
{
    using clock = chrono::system_clock;
    using second = chrono::duration<double>;

    class Timer {
    public:
        Timer() {}
        void start();
        double stop();
    private:
        clock::time_point _lastTime;
    };
} 
