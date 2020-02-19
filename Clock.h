/* 
Author: Steven Philley
Purpose: A system to deal with timers and retrieving the system time.
Date: Feb/16/2020
*/
#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <string>

#include <SDL2/SDL.h>


namespace ley {

class Clock {

private:
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

public:
    Clock();
    ~Clock();

    size_t secondsFromStart();
    void reset();
};

}
#endif
