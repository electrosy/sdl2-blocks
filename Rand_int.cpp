/*
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Nov/11/2020
*/

#include <SDL2/SDL.h>
#include <chrono>
#include "Rand_int.h"
/* RAII */
ley::Rand_int::Rand_int (int low, int high) :
gen(SDL_GetTicks()),
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