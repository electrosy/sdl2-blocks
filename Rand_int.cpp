#include "Rand_int.h"


/* RAII */

ley::Rand_int::Rand_int (int low, int high) :gen(rd()), dis{low,high} { 

}

ley::Rand_int::~Rand_int() {
	
}

/* Accessors */

/* Functions */

/* Operators */

int ley::Rand_int::operator()() {

	int num = dis(gen);

	// prevent repeated numbers, if we have the same number as the last one, generate a new one.
	while(last == num) {
		num = dis(gen);
	}
	last = num;

	return num;
}

/*
std::ostream& ley::operator<<(std::ostream& os, const ley::Rand_int& ri) {
	os << "Rand_int_value: " << std::to_chars(ri());
	return os;
	*/