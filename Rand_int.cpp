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
gen(rd()),
dis{low,high} {

	SDL_Log("Random device entropy %f", rd.entropy());

	if(rd.entropy() == 0) {
		SDL_Log("Setting random generator from rd() to SDL_GetTicks()");
		gen.seed(SDL_GetTicks());
	}
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