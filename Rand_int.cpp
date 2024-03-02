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
		// https://stackoverflow.com/questions/12469171/code-to-generate-random-numbers-in-c
		std::chrono::time_point<std::chrono::system_clock>now {std::chrono::system_clock::now()};
		std::chrono::system_clock::duration epoch {now.time_since_epoch()};
		typedef std::chrono::duration<unsigned long long int, std::milli> ms;

		auto seed = std::chrono::duration_cast<ms>(epoch).count() + SDL_GetTicks();

		SDL_Log("Setting random generator from rd() to SDL_GetTicks()");
		gen.seed(seed);
		SDL_Log("std::chrono::duration_cast<ms>(epoch).count() + SDL_GetTicks() value: %d", seed);
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