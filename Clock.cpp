/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/16/2020
*/
#include "Clock.h"

/* RAII */
ley::Clock::Clock() {
    
}

ley::Clock::~Clock() {

}

/* Functions */
size_t ley::Clock::secondsFromStart() {
    auto end = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
}
