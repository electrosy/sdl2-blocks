/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Generate random int number.
Date: Nov/11/2020
*/
#ifndef RAND_INT_H
#define RAND_INT_H

#include <iostream>
#include <iomanip>
#include <random>

namespace ley {

class Rand_int {

private:
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<> dis;
	int last;
public:
	/* RAII */
	Rand_int(int low, int high);
	/* Operators */
	int operator()();
};
}
#endif