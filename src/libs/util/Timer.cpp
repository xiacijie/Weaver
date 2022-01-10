#include "Timer.h"

using namespace util;


void Timer::start() {
    _lastTime = clock::now();
}

double Timer::stop() {
    second duration = clock::now() - _lastTime;
    return duration.count();
}