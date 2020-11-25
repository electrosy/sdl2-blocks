/*
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Nov/11/2020
*/

#include <chrono>
#include "Rand_int.h"
/* RAII */
ley::Rand_int::Rand_int (int low, int high) :
gen(clock()),
dis{low,high} {

}

ley::Rand_int::~Rand_int() {
	
}

/* Accessors */

/* Functions */

/* Operators */

int ley::Rand_int::operator()() {

	int num = dis(gen);
	return num;
}